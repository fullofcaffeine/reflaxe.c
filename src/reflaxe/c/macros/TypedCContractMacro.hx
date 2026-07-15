package reflaxe.c.macros;

#if macro
import haxe.Json;
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr;
import haxe.macro.Expr.MetadataEntry;
import haxe.macro.Type;
import haxe.macro.Type.ClassField;
import haxe.macro.Type.ClassType;
import haxe.macro.Type.MetaAccess;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.contract.TypedCContract.TypedCContractField;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.contract.TypedCContract.TypedCDeclaration;
import reflaxe.c.contract.TypedCContract.TypedCDependency;
import reflaxe.c.contract.TypedCContract.TypedCTypeRef;
#end

/**
	Validates Haxe-authored C declaration facts after typing.

	This seed is a structural collector, not a code generator. Later compiler
	stages consume its snapshot and own all generated files through Reflaxe.
**/
class TypedCContractMacro {
	public static inline final REPORT_DEFINE = "reflaxe_c_contract_report";

	#if macro
	static inline final INSTALLED_DEFINE = "reflaxe_c_contract_validator_installed";

	static final TYPE_METADATA = [
		"c.layout",
		"c.header",
		"c.name",
		"c.include",
		"c.link",
		"c.define",
		"c.pkgConfig",
		"c.framework",
		"c.pack",
		"c.align",
		"c.linkage",
		"c.callingConvention",
		"c.visibility",
		"c.section",
		"c.export"
	];

	static final FIELD_METADATA = [
		"c.name",
		"c.bitField",
		"c.align",
		"c.linkage",
		"c.callingConvention",
		"c.visibility",
		"c.section",
		"c.export",
		"c.constant"
	];

	static final POINTER_LIKE_TYPES = [
		"c.Ptr",
		"c.ConstPtr",
		"c.NullablePtr",
		"c.Ref",
		"c.ConstRef",
		"c.RestrictPtr",
		"c.VolatilePtr",
		"c.FunctionPtr",
		"c.Span",
		"c.ConstSpan",
		"c.CString",
		"c.StringView",
		"c.Owned",
		"c.Borrowed",
		"c.Allocator",
		"c.Arena"
	];

	static final C_KEYWORDS = [
		"_Alignas",
		"_Alignof",
		"_Atomic",
		"_Bool",
		"_Complex",
		"_Generic",
		"_Imaginary",
		"_Noreturn",
		"_Static_assert",
		"_Thread_local",
		"auto",
		"break",
		"case",
		"char",
		"const",
		"continue",
		"default",
		"do",
		"double",
		"else",
		"enum",
		"extern",
		"float",
		"for",
		"goto",
		"if",
		"inline",
		"int",
		"long",
		"register",
		"restrict",
		"return",
		"short",
		"signed",
		"sizeof",
		"static",
		"struct",
		"switch",
		"typedef",
		"union",
		"unsigned",
		"void",
		"volatile",
		"while"
	];

	public static function install():Void {
		if (Context.defined(INSTALLED_DEFINE)) {
			return;
		}
		Compiler.define(INSTALLED_DEFINE);

		var collected = false;
		Context.onAfterTyping(moduleTypes -> {
			if (collected) {
				return;
			}
			collected = true;
			final snapshot = collect(moduleTypes);
			if (Context.defined(REPORT_DEFINE)) {
				Sys.println("HXC_TYPED_C_CONTRACT=" + Json.stringify(snapshot));
			}
		});
	}

	public static function collect(moduleTypes:Array<ModuleType>):TypedCContractSnapshot {
		final shells:Array<DeclarationShell> = [];
		for (moduleType in moduleTypes) {
			final shell = shellFor(moduleType);
			if (shell != null && (hasCMetadata(shell.meta) || classHasCMetadata(shell.classType))) {
				shells.push(shell);
			}
		}
		shells.sort((left, right) -> compareStrings(left.path, right.path));

		final layouts:Map<String, String> = [];
		for (shell in shells) {
			final layoutEntry = single(shell.meta, "c.layout", shell.path);
			if (layoutEntry != null) {
				requireArity(layoutEntry, 1);
				layouts.set(shell.path, readEnum(layoutEntry, 0, "c.Layout", ["Struct", "Union", "Opaque", "Enum"]));
			}
		}

		final declarations:Array<TypedCDeclaration> = [];
		final drafts:Array<DeclarationDraft> = [];
		final buildFacts:Array<TypedCBuildFact> = [];
		final explicitSymbols:Map<String, SymbolOrigin> = [];
		for (shell in shells) {
			validateKnownMetadata(shell.meta, TYPE_METADATA, 'declaration `${shell.path}`');
			final draft = collectDeclaration(shell, layouts, buildFacts, explicitSymbols);
			drafts.push(draft);
			declarations.push(draft.declaration);
		}

		validateByValueGraph(drafts, layouts);
		deduplicateAndSortBuildFacts(buildFacts);
		return {
			schemaVersion: 2,
			status: "contract-seed-no-lowering",
			declarations: declarations,
			buildFacts: buildFacts,
			effects: {
				allocation: "none",
				ownership: "declarative-only",
				unsafe: "none",
				portability: "c-target-only",
				runtimeFeatures: []
			}
		};
	}

	static function collectDeclaration(shell:DeclarationShell, layouts:Map<String, String>, buildFacts:Array<TypedCBuildFact>,
			explicitSymbols:Map<String, SymbolOrigin>):DeclarationDraft {
		final layout = layouts.get(shell.path);
		final cNameEntry = single(shell.meta, "c.name", shell.path);
		if (cNameEntry != null) {
			requireArity(cNameEntry, 1);
		}
		final cName = cNameEntry == null ? null : readCIdentifier(cNameEntry, 0, "C declaration name");
		if (cName != null) {
			registerSymbol(cName, shell.path, cNameEntry.pos, explicitSymbols);
		}

		final headerEntry = single(shell.meta, "c.header", shell.path);
		var headerPath:Null<String> = null;
		var headerVisibility:Null<String> = null;
		if (headerEntry != null) {
			requireArity(headerEntry, 2);
			headerPath = readString(headerEntry, 0, "generated header path");
			validateHeaderPath(headerPath, headerEntry.params[0].pos);
			headerVisibility = readEnum(headerEntry, 1, "c.Header", ["Public", "Private"]);
		}

		final pack = readOptionalPowerOfTwo(shell.meta, "c.pack", 128, shell.path);
		final align = readOptionalPowerOfTwo(shell.meta, "c.align", 4096, shell.path);
		if (pack != null && layout != "struct" && layout != "union") {
			error("c.pack is valid only on c.Layout.Struct or c.Layout.Union declarations", single(shell.meta, "c.pack", shell.path).pos);
		}
		validateLayoutOwner(shell, layout);

		final linkage = readOptionalEnum(shell.meta, "c.linkage", "c.Linkage", ["External", "Internal", "Static", "Inline"], shell.path);
		final callingConvention = readOptionalEnum(shell.meta, "c.callingConvention", "c.CallingConvention",
			["C", "Cdecl", "Stdcall", "Fastcall", "Vectorcall"], shell.path);
		final visibility = readOptionalEnum(shell.meta, "c.visibility", "c.Visibility", ["Default", "Hidden"], shell.path);
		final section = readOptionalSection(shell.meta, shell.path);
		final exported = marker(shell.meta, "c.export", shell.path);
		collectBuildFacts(shell, buildFacts);

		final fields:Array<TypedCContractField> = [];
		final dependencies:Array<DependencyEdge> = [];
		if (shell.classType != null) {
			collectClassFields(shell, layout, layouts, fields, dependencies, explicitSymbols);
		}

		final dependencyRequirements:Map<String, String> = [];
		for (edge in dependencies) {
			final existing = dependencyRequirements.get(edge.target);
			if (existing == null || edge.byValue) {
				dependencyRequirements.set(edge.target, edge.byValue ? "complete" : "forward");
			}
		}
		final dependencyRecords:Array<TypedCDependency> = [];
		for (modulePath in dependencyRequirements.keys()) {
			dependencyRecords.push({
				modulePath: modulePath,
				requirement: dependencyRequirements.get(modulePath)
			});
		}
		dependencyRecords.sort((left, right) -> compareStrings(left.modulePath, right.modulePath));

		return {
			shell: shell,
			dependencies: dependencies,
			declaration: {
				modulePath: shell.path,
				sourceKind: shell.sourceKind,
				layout: layout,
				cName: cName,
				headerPath: headerPath,
				headerVisibility: headerVisibility,
				pack: pack,
				align: align,
				exported: exported,
				linkage: linkage,
				callingConvention: callingConvention,
				visibility: visibility,
				section: section,
				dependencies: dependencyRecords,
				fields: fields
			}
		};
	}

	static function validateLayoutOwner(shell:DeclarationShell, layout:Null<String>):Void {
		if (layout == null) {
			return;
		}
		final entry = single(shell.meta, "c.layout", shell.path);
		if (layout == "enum" && shell.sourceKind != "enum") {
			error("c.Layout.Enum must annotate a Haxe enum", entry.pos);
		}
		if ((layout == "struct" || layout == "union" || layout == "opaque") && shell.sourceKind != "class") {
			error('c.Layout.${capitalize(layout)} must annotate a Haxe class', entry.pos);
		}
		if (layout == "opaque" && shell.classType != null && !shell.classType.isExtern) {
			error("c.Layout.Opaque declarations must be extern classes", entry.pos);
		}
	}

	static function collectClassFields(shell:DeclarationShell, layout:Null<String>, layouts:Map<String, String>, fields:Array<TypedCContractField>,
			dependencies:Array<DependencyEdge>, explicitSymbols:Map<String, SymbolOrigin>):Void {
		final classType = shell.classType;
		if (classType == null) {
			return;
		}
		final instanceFields = classType.fields.get().copy();
		final staticFields = classType.statics.get().copy();
		instanceFields.sort(compareFieldsByPosition);
		staticFields.sort(compareFieldsByPosition);
		for (field in instanceFields) {
			collectField(shell, field, false, layout, layouts, fields, dependencies, explicitSymbols);
		}
		for (field in staticFields) {
			collectField(shell, field, true, layout, layouts, fields, dependencies, explicitSymbols);
		}
	}

	static function collectField(shell:DeclarationShell, field:ClassField, isStatic:Bool, layout:Null<String>, layouts:Map<String, String>,
			fields:Array<TypedCContractField>, dependencies:Array<DependencyEdge>, explicitSymbols:Map<String, SymbolOrigin>):Void {
		validateKnownMetadata(field.meta, FIELD_METADATA, 'field `${shell.path}.${field.name}`');
		final hasMetadata = hasCMetadata(field.meta);
		final isVariable = switch field.kind {
			case FVar(_, _): true;
			case FMethod(_): false;
		};
		final layoutField = !isStatic && isVariable && (layout == "struct" || layout == "union");
		if (!layoutField && !hasMetadata) {
			return;
		}

		final bitFieldEntry = single(field.meta, "c.bitField", '${shell.path}.${field.name}');
		var bitWidth:Null<Int> = null;
		if (bitFieldEntry != null) {
			requireArity(bitFieldEntry, 1);
			if (!layoutField) {
				error("c.bitField is valid only on instance fields of C structs or unions", bitFieldEntry.pos);
			}
			bitWidth = readPositiveInt(bitFieldEntry, 0, "bit-field width");
		}
		final fieldAlign = single(field.meta, "c.align", '${shell.path}.${field.name}');
		var align:Null<Int> = null;
		if (fieldAlign != null) {
			requireArity(fieldAlign, 1);
			align = readPowerOfTwo(fieldAlign, 0, 4096, "field alignment");
		}

		final cNameEntry = single(field.meta, "c.name", '${shell.path}.${field.name}');
		if (cNameEntry != null) {
			requireArity(cNameEntry, 1);
		}
		final cName = cNameEntry == null ? null : readCIdentifier(cNameEntry, 0, "C field or symbol name");
		final exported = marker(field.meta, "c.export", '${shell.path}.${field.name}');
		final constant = marker(field.meta, "c.constant", '${shell.path}.${field.name}');
		if (constant && (!isStatic || !isVariable)) {
			error("c.constant is valid only on a static value field", single(field.meta, "c.constant", '${shell.path}.${field.name}').pos);
		}
		if (cName != null && (exported || isStatic || !isVariable)) {
			registerSymbol(cName, '${shell.path}.${field.name}', cNameEntry.pos, explicitSymbols);
		}

		final linkage = readOptionalEnum(field.meta, "c.linkage", "c.Linkage", ["External", "Internal", "Static", "Inline"], '${shell.path}.${field.name}');
		final callingConvention = readOptionalEnum(field.meta, "c.callingConvention", "c.CallingConvention",
			["C", "Cdecl", "Stdcall", "Fastcall", "Vectorcall"], '${shell.path}.${field.name}');
		final visibility = readOptionalEnum(field.meta, "c.visibility", "c.Visibility", ["Default", "Hidden"], '${shell.path}.${field.name}');
		final section = readOptionalSection(field.meta, '${shell.path}.${field.name}');

		final byValueDependency = layoutField ? directByValueDependency(field.type, layouts) : null;
		final referencedDependencies:Array<String> = [];
		collectReferencedDependencies(field.type, layouts, referencedDependencies);
		for (dependency in referencedDependencies) {
			dependencies.push({
				target: dependency,
				field: field.name,
				pos: field.pos,
				byValue: dependency == byValueDependency
			});
		}
		fields.push({
			name: field.name,
			cName: cName,
			kind: constant ? "constant" : (isVariable ? (isStatic ? "static-field" : "field") : "function"),
			type: typeRef(field.type),
			bitWidth: bitWidth,
			align: align,
			exported: exported,
			linkage: linkage,
			callingConvention: callingConvention,
			visibility: visibility,
			section: section
		});
	}

	static function collectBuildFacts(shell:DeclarationShell, buildFacts:Array<TypedCBuildFact>):Void {
		for (entry in entries(shell.meta, "c.include")) {
			requireArity(entry, 2);
			final path = readString(entry, 0, "include path");
			validateIncludePath(path, entry.params[0].pos);
			final includeKind = readEnum(entry, 1, "c.IncludeKind", ["System", "Local"]);
			buildFacts.push({
				kind: "include",
				name: path,
				value: includeKind,
				valueKind: "enum",
				ownerModulePaths: [shell.path]
			});
		}
		for (entry in entries(shell.meta, "c.link")) {
			addNamedBuildFact(shell.path, entry, "link", "logical library", buildFacts);
		}
		for (entry in entries(shell.meta, "c.pkgConfig")) {
			addNamedBuildFact(shell.path, entry, "pkg-config", "pkg-config package", buildFacts);
		}
		for (entry in entries(shell.meta, "c.framework")) {
			addNamedBuildFact(shell.path, entry, "framework", "framework", buildFacts);
		}
		for (entry in entries(shell.meta, "c.define")) {
			requireArity(entry, 2);
			final name = readCIdentifier(entry, 0, "preprocessor define name");
			final value = readLiteral(entry.params[1], "preprocessor define value");
			buildFacts.push({
				kind: "define",
				name: name,
				value: value.value,
				valueKind: value.kind,
				ownerModulePaths: [shell.path]
			});
		}
	}

	static function addNamedBuildFact(ownerModulePath:String, entry:MetadataEntry, kind:String, label:String, buildFacts:Array<TypedCBuildFact>):Void {
		requireArity(entry, 1);
		final name = readString(entry, 0, '$label name');
		validateBuildName(name, label, entry.params[0].pos);
		buildFacts.push({
			kind: kind,
			name: name,
			value: null,
			valueKind: null,
			ownerModulePaths: [ownerModulePath]
		});
	}

	static function validateByValueGraph(drafts:Array<DeclarationDraft>, layouts:Map<String, String>):Void {
		final byPath:Map<String, DeclarationDraft> = [];
		for (draft in drafts) {
			byPath.set(draft.shell.path, draft);
			for (edge in draft.dependencies) {
				if (edge.byValue && layouts.get(edge.target) == "opaque") {
					error('opaque C declaration `${edge.target}` cannot be embedded by value in `${draft.shell.path}.${edge.field}`; use a typed pointer',
						edge.pos);
				}
			}
		}

		final visiting:Map<String, Bool> = [];
		final visited:Map<String, Bool> = [];
		final stack:Array<String> = [];
		function visit(path:String):Void {
			if (visited.exists(path)) {
				return;
			}
			visiting.set(path, true);
			stack.push(path);
			final draft = byPath.get(path);
			if (draft != null) {
				for (edge in draft.dependencies) {
					if (!edge.byValue) {
						continue;
					}
					final targetLayout = layouts.get(edge.target);
					if (targetLayout != "struct" && targetLayout != "union") {
						continue;
					}
					if (visiting.exists(edge.target)) {
						final cycle = stack.slice(stack.indexOf(edge.target)).concat([edge.target]);
						error('impossible by-value C declaration cycle: ${cycle.join(" -> ")}; use a typed pointer or opaque handle', edge.pos);
					} else {
						visit(edge.target);
					}
				}
			}
			stack.pop();
			visiting.remove(path);
			visited.set(path, true);
		}
		for (draft in drafts) {
			visit(draft.shell.path);
		}
	}

	static function directByValueDependency(type:Type, layouts:Map<String, String>):Null<String> {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : directByValueDependency(resolved, layouts);
			case TLazy(resolve): directByValueDependency(resolve(), layouts);
			case TType(typeRef, parameters): directByValueDependency(Context.follow(TType(typeRef, parameters)), layouts);
			case TAbstract(abstractRef, parameters):
				final path = abstractPath(abstractRef.get());
				if (path == "Null" && parameters.length == 1) {
					directByValueDependency(parameters[0], layouts);
				} else if (path == "c.CArray" && parameters.length > 0) {
					directByValueDependency(parameters[0], layouts);
				} else {
					null;
				}
			case TInst(classRef, _):
				final path = classPath(classRef.get());
				layouts.exists(path) ? path : null;
			case TEnum(enumRef, _):
				final path = enumPath(enumRef.get());
				layouts.exists(path) ? path : null;
			case _: null;
		};
	}

	static function collectReferencedDependencies(type:Type, layouts:Map<String, String>, output:Array<String>):Void {
		switch type {
			case TMono(reference):
				final resolved = reference.get();
				if (resolved != null) {
					collectReferencedDependencies(resolved, layouts, output);
				}
			case TLazy(resolve):
				collectReferencedDependencies(resolve(), layouts, output);
			case TType(typeRef, parameters):
				collectReferencedDependencies(Context.follow(TType(typeRef, parameters)), layouts, output);
			case TAbstract(_, parameters):
				for (parameter in parameters) {
					collectReferencedDependencies(parameter, layouts, output);
				}
			case TInst(classRef, parameters):
				final path = classPath(classRef.get());
				if (layouts.exists(path) && output.indexOf(path) == -1) {
					output.push(path);
				}
				for (parameter in parameters) {
					collectReferencedDependencies(parameter, layouts, output);
				}
			case TEnum(enumRef, parameters):
				final path = enumPath(enumRef.get());
				if (layouts.exists(path) && output.indexOf(path) == -1) {
					output.push(path);
				}
				for (parameter in parameters) {
					collectReferencedDependencies(parameter, layouts, output);
				}
			case TFun(arguments, result):
				for (argument in arguments) {
					collectReferencedDependencies(argument.t, layouts, output);
				}
				collectReferencedDependencies(result, layouts, output);
			case TAnonymous(anonymousRef):
				for (field in anonymousRef.get().fields) {
					collectReferencedDependencies(field.type, layouts, output);
				}
			case TDynamic(inner):
				if (inner != null) {
					collectReferencedDependencies(inner, layouts, output);
				}
		}
	}

	static function typeRef(type:Type):TypedCTypeRef {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? makeTypeRef("unknown", null, []) : typeRef(resolved);
			case TEnum(enumRef, parameters): makeTypeRef("enum", enumPath(enumRef.get()), parameters.map(typeRef));
			case TInst(classRef, parameters): makeTypeRef("class", classPath(classRef.get()), parameters.map(typeRef));
			case TType(typeDefinition, parameters):
				final definition = typeDefinition.get();
				makeTypeRef("typedef", definitionPath(definition.module, definition.pack, definition.name), parameters.map(typeRef));
			case TFun(arguments, result):
				final nodes = arguments.map(argument -> typeRef(argument.t));
				nodes.push(typeRef(result));
				makeTypeRef("function", null, nodes);
			case TAnonymous(anonymousRef):
				final fields = anonymousRef.get().fields.copy();
				fields.sort((left, right) -> compareStrings(left.name, right.name));
				makeTypeRef("anonymous", null, fields.map(field -> typeRef(field.type)));
			case TDynamic(inner): makeTypeRef("dynamic", null, inner == null ? [] : [typeRef(inner)]);
			case TLazy(resolve): typeRef(resolve());
			case TAbstract(abstractRef, parameters):
				final path = abstractPath(abstractRef.get());
				makeTypeRef(POINTER_LIKE_TYPES.indexOf(path) == -1 ? "abstract" : "c-contract", path, parameters.map(typeRef));
		};
	}

	static function makeTypeRef(kind:String, name:Null<String>, arguments:Array<TypedCTypeRef>):TypedCTypeRef {
		return {kind: kind, name: name, arguments: arguments};
	}

	static function shellFor(moduleType:ModuleType):Null<DeclarationShell> {
		return switch moduleType {
			case TClassDecl(classRef):
				final value = classRef.get();
				{
					path: classPath(value),
					sourceKind: "class",
					meta: value.meta,
					pos: value.pos,
					classType: value
				};
			case TEnumDecl(enumRef):
				final value = enumRef.get();
				{
					path: enumPath(value),
					sourceKind: "enum",
					meta: value.meta,
					pos: value.pos,
					classType: null
				};
			case TTypeDecl(typeRef):
				final value = typeRef.get();
				{
					path: definitionPath(value.module, value.pack, value.name),
					sourceKind: "typedef",
					meta: value.meta,
					pos: value.pos,
					classType: null
				};
			case TAbstract(abstractRef):
				final value = abstractRef.get();
				{
					path: abstractPath(value),
					sourceKind: "abstract",
					meta: value.meta,
					pos: value.pos,
					classType: null
				};
		};
	}

	static function classHasCMetadata(classType:Null<ClassType>):Bool {
		if (classType == null) {
			return false;
		}
		for (field in classType.fields.get().concat(classType.statics.get())) {
			if (hasCMetadata(field.meta)) {
				return true;
			}
		}
		return false;
	}

	static function hasCMetadata(meta:MetaAccess):Bool {
		for (entry in meta.get()) {
			if (StringTools.startsWith(metadataName(entry), "c.")) {
				return true;
			}
		}
		return false;
	}

	static function validateKnownMetadata(meta:MetaAccess, allowed:Array<String>, owner:String):Void {
		for (entry in meta.get()) {
			final name = metadataName(entry);
			if (StringTools.startsWith(name, "c.") && allowed.indexOf(name) == -1) {
				error('unknown typed C metadata `@:$name` on $owner', entry.pos);
			}
		}
	}

	static function metadataName(entry:MetadataEntry):String {
		return StringTools.startsWith(entry.name, ":") ? entry.name.substr(1) : entry.name;
	}

	static function entries(meta:MetaAccess, name:String):Array<MetadataEntry> {
		return meta.get().filter(entry -> metadataName(entry) == name);
	}

	static function single(meta:MetaAccess, name:String, owner:String):Null<MetadataEntry> {
		final matches = entries(meta, name);
		if (matches.length > 1) {
			error('duplicate `@:$name` metadata on `$owner`', matches[1].pos);
		}
		return matches.length == 0 ? null : matches[0];
	}

	static function marker(meta:MetaAccess, name:String, owner:String):Bool {
		final entry = single(meta, name, owner);
		if (entry == null) {
			return false;
		}
		requireArity(entry, 0);
		return true;
	}

	static function readOptionalEnum(meta:MetaAccess, name:String, enumPath:String, members:Array<String>, owner:String):Null<String> {
		final entry = single(meta, name, owner);
		if (entry != null) {
			requireArity(entry, 1);
		}
		return entry == null ? null : readEnum(entry, 0, enumPath, members);
	}

	static function readEnum(entry:MetadataEntry, index:Int, enumPath:String, members:Array<String>):String {
		requireParameter(entry, index);
		final path = expressionPath(entry.params[index]);
		if (path == null || !StringTools.startsWith(path, enumPath + ".")) {
			error('`@:${metadataName(entry)}` parameter ${index + 1} must use a typed `$enumPath` value', entry.params[index].pos);
			return members[0].toLowerCase();
		}
		final member = path.substr(enumPath.length + 1);
		if (members.indexOf(member) == -1) {
			error('unknown `$enumPath.$member` value', entry.params[index].pos);
			return members[0].toLowerCase();
		}
		return member.toLowerCase();
	}

	static function expressionPath(expression:Expr):Null<String> {
		return switch expression.expr {
			case EConst(CIdent(name)): name;
			case EField(owner, field):
				final prefix = expressionPath(owner);
				prefix == null ? null : prefix + "." + field;
			case EParenthesis(inner): expressionPath(inner);
			case _: null;
		};
	}

	static function readOptionalPowerOfTwo(meta:MetaAccess, name:String, maximum:Int, owner:String):Null<Int> {
		final entry = single(meta, name, owner);
		if (entry != null) {
			requireArity(entry, 1);
		}
		return entry == null ? null : readPowerOfTwo(entry, 0, maximum, name);
	}

	static function readPowerOfTwo(entry:MetadataEntry, index:Int, maximum:Int, label:String):Int {
		final value = readPositiveInt(entry, index, label);
		if ((value & (value - 1)) != 0 || value > maximum) {
			error('`$label` must be a power of two no greater than $maximum', entry.params[index].pos);
		}
		return value;
	}

	static function readPositiveInt(entry:MetadataEntry, index:Int, label:String):Int {
		requireParameter(entry, index);
		final value = switch entry.params[index].expr {
			case EConst(CInt(text, _)): Std.parseInt(text);
			case _: null;
		};
		if (value == null || value <= 0) {
			error('`$label` must be a positive integer literal', entry.params[index].pos);
			return 1;
		}
		return value;
	}

	static function readString(entry:MetadataEntry, index:Int, label:String):String {
		requireParameter(entry, index);
		return switch entry.params[index].expr {
			case EConst(CString(value, _)): value;
			case _:
				error('`$label` must be a string literal', entry.params[index].pos);
				"";
		};
	}

	static function readCIdentifier(entry:MetadataEntry, index:Int, label:String):String {
		final value = readString(entry, index, label);
		validateCIdentifier(value, label, entry.params[index].pos);
		return value;
	}

	static function readLiteral(expression:Expr, label:String):LiteralValue {
		return switch expression.expr {
			case EConst(CString(value, _)): {kind: "string", value: value};
			case EConst(CInt(value, _)): {kind: "integer", value: value};
			case EConst(CFloat(value)): {kind: "float", value: value};
			case EConst(CIdent("true")): {kind: "boolean", value: "true"};
			case EConst(CIdent("false")): {kind: "boolean", value: "false"};
			case _:
				error('`$label` must be a string, numeric, or Bool literal', expression.pos);
				{kind: "invalid", value: ""};
		};
	}

	static function readOptionalSection(meta:MetaAccess, owner:String):Null<String> {
		final entry = single(meta, "c.section", owner);
		if (entry == null) {
			return null;
		}
		requireArity(entry, 1);
		final section = readString(entry, 0, "section name");
		if (section == "" || containsControlOrDelimiter(section)) {
			error("section name must be non-empty and contain no control characters or C delimiters", entry.params[0].pos);
		}
		return section;
	}

	static function validateHeaderPath(path:String, pos:Position):Void {
		validateRelativePath(path, "generated header", pos);
		if (!StringTools.endsWith(path, ".h")) {
			error("generated header path must end in `.h`", pos);
		}
	}

	static function validateIncludePath(path:String, pos:Position):Void {
		validateRelativePath(path, "include", pos);
	}

	static function validateRelativePath(path:String, label:String, pos:Position):Void {
		if (path == "" || StringTools.startsWith(path, "/") || path.indexOf("\\") != -1 || path.indexOf(":") != -1 || containsControlOrDelimiter(path)) {
			error('$label path must be a normalized, relative forward-slash path', pos);
			return;
		}
		for (part in path.split("/")) {
			if (part == "" || part == "." || part == "..") {
				error('$label path contains a forbidden empty, `.` or `..` component', pos);
				return;
			}
		}
	}

	static function containsControlOrDelimiter(value:String):Bool {
		return value.indexOf("\n") != -1 || value.indexOf("\r") != -1 || value.indexOf("\x00") != -1 || value.indexOf('"') != -1
			|| value.indexOf("<") != -1 || value.indexOf(">") != -1;
	}

	static function validateBuildName(value:String, label:String, pos:Position):Void {
		if (value == "") {
			error('$label name must not be empty', pos);
			return;
		}
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			final accepted = code != null
				&& ((code >= 48 && code <= 57) || (code >= 65 && code <= 90) || (code >= 97 && code <= 122) || code == 43 || code == 45 || code == 46
					|| code == 95);
			if (!accepted) {
				error('$label name contains a character outside `[A-Za-z0-9_+.-]`', pos);
				return;
			}
		}
	}

	static function validateCIdentifier(value:String, label:String, pos:Position):Void {
		if (value == "" || !isIdentifierStart(value.charCodeAt(0))) {
			error('$label must be a valid C identifier', pos);
			return;
		}
		for (index in 1...value.length) {
			if (!isIdentifierPart(value.charCodeAt(index))) {
				error('$label must be a valid C identifier', pos);
				return;
			}
		}
		final second = value.length > 1 ? value.charCodeAt(1) : null;
		if (C_KEYWORDS.indexOf(value) != -1
			|| StringTools.startsWith(value, "__")
			|| (StringTools.startsWith(value, "_") && second != null && second >= 65 && second <= 90)
			|| StringTools.startsWith(value, "hxc_")
			|| StringTools.startsWith(value, "hxrt_")) {
			error('C identifier `$value` is reserved by C or reflaxe.c', pos);
		}
	}

	static function isIdentifierStart(code:Null<Int>):Bool {
		return code != null && ((code >= 65 && code <= 90) || (code >= 97 && code <= 122) || code == 95);
	}

	static function isIdentifierPart(code:Null<Int>):Bool {
		return isIdentifierStart(code) || (code != null && code >= 48 && code <= 57);
	}

	static function registerSymbol(name:String, owner:String, pos:Position, symbols:Map<String, SymbolOrigin>):Void {
		final existing = symbols.get(name);
		if (existing != null) {
			error('duplicate explicit C symbol `$name` on `$owner`; first declared by `${existing.owner}`', pos);
		} else {
			symbols.set(name, {owner: owner});
		}
	}

	static function deduplicateAndSortBuildFacts(facts:Array<TypedCBuildFact>):Void {
		facts.sort((left, right) -> compareStrings(buildFactKey(left), buildFactKey(right)));
		var index = facts.length - 1;
		while (index > 0) {
			if (buildFactKey(facts[index]) == buildFactKey(facts[index - 1])) {
				for (owner in facts[index].ownerModulePaths) {
					if (facts[index - 1].ownerModulePaths.indexOf(owner) == -1) {
						facts[index - 1].ownerModulePaths.push(owner);
					}
				}
				facts.splice(index, 1);
			}
			index--;
		}
		for (fact in facts) {
			fact.ownerModulePaths.sort(compareStrings);
		}
	}

	static function buildFactKey(fact:TypedCBuildFact):String {
		return [
			fact.kind,
			fact.name,
			fact.valueKind == null ? "" : fact.valueKind,
			fact.value == null ? "" : fact.value
		].join("\x00");
	}

	static function requireArity(entry:MetadataEntry, expected:Int):Void {
		if (entry.params.length != expected) {
			error('`@:${metadataName(entry)}` expects $expected parameter(s), received ${entry.params.length}', entry.pos);
		}
	}

	static function requireParameter(entry:MetadataEntry, index:Int):Void {
		if (entry.params.length <= index) {
			error('`@:${metadataName(entry)}` is missing parameter ${index + 1}', entry.pos);
		}
	}

	static function capitalize(value:String):String {
		return value == "" ? value : value.substr(0, 1).toUpperCase() + value.substr(1);
	}

	static function classPath(value:ClassType):String {
		return definitionPath(value.module, value.pack, value.name);
	}

	static function enumPath(value:EnumType):String {
		return definitionPath(value.module, value.pack, value.name);
	}

	static function abstractPath(value:AbstractType):String {
		return definitionPath(value.module, value.pack, value.name);
	}

	static function definitionPath(module:String, pack:Array<String>, name:String):String {
		final expected = pack.length == 0 ? name : pack.join(".") + "." + name;
		return module == expected ? module : module + "." + name;
	}

	static function compareFieldsByPosition(left:ClassField, right:ClassField):Int {
		final leftInfo = Context.getPosInfos(left.pos);
		final rightInfo = Context.getPosInfos(right.pos);
		if (leftInfo.file != rightInfo.file) {
			return compareStrings(leftInfo.file, rightInfo.file);
		}
		final byPosition = leftInfo.min - rightInfo.min;
		return byPosition != 0 ? byPosition : compareStrings(left.name, right.name);
	}

	static function compareStrings(left:String, right:String):Int {
		return left < right ? -1 : (left > right ? 1 : 0);
	}

	static function error(message:String, pos:Position):Void {
		Context.error("HXC5002: " + message, pos);
	}
	#else
	public static function install():Void {}
	#end
}

#if macro
private typedef DeclarationShell = {
	final path:String;
	final sourceKind:String;
	final meta:MetaAccess;
	final pos:Position;
	final classType:Null<ClassType>;
}

private typedef DependencyEdge = {
	final target:String;
	final field:String;
	final pos:Position;
	final byValue:Bool;
}

private typedef DeclarationDraft = {
	final shell:DeclarationShell;
	final dependencies:Array<DependencyEdge>;
	final declaration:TypedCDeclaration;
}

private typedef LiteralValue = {
	final kind:String;
	final value:String;
}

private typedef SymbolOrigin = {
	final owner:String;
}
#end
