package reflaxe.c.interop;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.ast.CAST.CInclude;
import reflaxe.c.ast.CAST.CIncludeKind;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.contract.TypedCContract.TypedCContractField;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.contract.TypedCContract.TypedCDeclaration;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.frontend.TypedProgramInput.TypedAstDeclaration;
import reflaxe.c.frontend.TypedProgramInput.TypedAstField;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRDiagnostic;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyLoweringError;
import reflaxe.c.lowering.HaxeSourceSpan;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

typedef CImportValueTypeResolver = (Type, Position, String, String, (Position, String) -> Void, String) -> CBodyValueType;

/** The exact C spelling category of one header-owned nominal type. */
enum CImportTypeKind {
	CITStruct;
	CITEnum;
	CITTypedef;
}

/** One source field in an imported struct before C names are finalized. */
class CPreparedImportField {
	public final name:String;
	public final type:CBodyValueType;
	public final mutable:Bool;
	public final source:HxcSourceSpan;
	public final request:CSymbolRequest;

	public function new(name:String, type:CBodyValueType, mutable:Bool, source:HxcSourceSpan, request:CSymbolRequest) {
		this.name = name;
		this.type = type;
		this.mutable = mutable;
		this.source = source;
		this.request = request;
	}
}

/** One precise header-owned type retained as a nominal direct HxcIR value. */
class CPreparedImportType {
	public final haxePath:String;
	public final ownerModule:String;
	public final declarationId:String;
	public final instanceId:String;
	public final kind:CImportTypeKind;
	public final source:HxcSourceSpan;
	public final request:CSymbolRequest;
	public final fields:Array<CPreparedImportField> = [];
	public var aliasTarget(default, null):Null<CBodyValueType> = null;

	public function new(haxePath:String, ownerModule:String, kind:CImportTypeKind, source:HxcSourceSpan, request:CSymbolRequest) {
		this.haxePath = haxePath;
		this.ownerModule = ownerModule;
		this.declarationId = 'type.c-import.$haxePath';
		this.instanceId = 'instance.c-import.$haxePath';
		this.kind = kind;
		this.source = source;
		this.request = request;
	}

	public function declaration():HxcIRTypeDeclaration {
		return {
			id: declarationId,
			displayName: haxePath,
			kind: switch kind {
				case CITStruct:
					IRTKAggregate(fields.map(field -> {
						name: field.name,
						type: field.type.irType,
						mutable: field.mutable,
						source: field.source
					}));
				case CITEnum | CITTypedef: IRTKExtern;
			},
			source: source
		};
	}

	public function instance():HxcIRTypeInstance {
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [],
			representation: IRRDirect,
			source: source
		};
	}

	public function field(name:String):Null<CPreparedImportField> {
		for (field in fields) {
			if (field.name == name)
				return field;
		}
		return null;
	}

	public function setAliasTarget(target:CBodyValueType):Void {
		if (aliasTarget != null)
			throw new haxe.Exception('import typedef `$haxePath` was prepared more than once');
		aliasTarget = target;
	}
}

/** One exact, non-variadic imported C function. */
class CPreparedImportFunction {
	public final id:String;
	public final ownerModule:String;
	public final haxePath:String;
	public final parameters:Array<CBodyValueType>;
	public final returnType:CBodyValueType;
	public final source:HxcSourceSpan;
	public final request:CSymbolRequest;

	public function new(id:String, ownerModule:String, haxePath:String, parameters:Array<CBodyValueType>, returnType:CBodyValueType, source:HxcSourceSpan,
			request:CSymbolRequest) {
		this.id = id;
		this.ownerModule = ownerModule;
		this.haxePath = haxePath;
		this.parameters = parameters.copy();
		this.returnType = returnType;
		this.source = source;
		this.request = request;
	}
}

/** One header-owned enum value or object-like scalar macro. */
class CPreparedImportConstant {
	public final id:String;
	public final ownerModule:String;
	public final haxePath:String;
	public final type:CBodyValueType;
	public final source:HxcSourceSpan;
	public final request:CSymbolRequest;

	public function new(id:String, ownerModule:String, haxePath:String, type:CBodyValueType, source:HxcSourceSpan, request:CSymbolRequest) {
		this.id = id;
		this.ownerModule = ownerModule;
		this.haxePath = haxePath;
		this.type = type;
		this.source = source;
		this.request = request;
	}
}

class CLoweredImportField {
	public final prepared:CPreparedImportField;
	public final cName:CIdentifier;

	public function new(prepared:CPreparedImportField, cName:CIdentifier) {
		this.prepared = prepared;
		this.cName = cName;
	}
}

class CLoweredImportType {
	public final prepared:CPreparedImportType;
	public final cName:CIdentifier;
	public final fields:Array<CLoweredImportField>;

	public function new(prepared:CPreparedImportType, cName:CIdentifier, fields:Array<CLoweredImportField>) {
		this.prepared = prepared;
		this.cName = cName;
		this.fields = fields.copy();
	}

	public function field(name:String):Null<CLoweredImportField> {
		for (field in fields) {
			if (field.prepared.name == name)
				return field;
		}
		return null;
	}
}

class CLoweredImportFunction {
	public final prepared:CPreparedImportFunction;
	public final cName:CIdentifier;

	public function new(prepared:CPreparedImportFunction, cName:CIdentifier) {
		this.prepared = prepared;
		this.cName = cName;
	}
}

class CLoweredImportConstant {
	public final prepared:CPreparedImportConstant;
	public final cName:CIdentifier;

	public function new(prepared:CPreparedImportConstant, cName:CIdentifier) {
		this.prepared = prepared;
		this.cName = cName;
	}
}

/** Finalized imports consumed only by structural C AST emission. */
class CLoweredImports {
	public final types:Array<CLoweredImportType>;
	public final functions:Array<CLoweredImportFunction>;
	public final constants:Array<CLoweredImportConstant>;
	public final includes:Array<CInclude>;
	public final buildFacts:Array<TypedCBuildFact>;

	final typesByInstance:Map<String, CLoweredImportType> = [];
	final functionsById:Map<String, CLoweredImportFunction> = [];
	final constantsById:Map<String, CLoweredImportConstant> = [];

	public function new(types:Array<CLoweredImportType>, functions:Array<CLoweredImportFunction>, constants:Array<CLoweredImportConstant>,
			includes:Array<CInclude>, buildFacts:Array<TypedCBuildFact>) {
		this.types = types.copy();
		this.functions = functions.copy();
		this.constants = constants.copy();
		this.includes = includes.copy();
		this.buildFacts = buildFacts.copy();
		for (type in this.types)
			typesByInstance.set(type.prepared.instanceId, type);
		for (fn in this.functions)
			functionsById.set(fn.prepared.id, fn);
		for (constant in this.constants)
			constantsById.set(constant.prepared.id, constant);
	}

	public function typeByInstance(instanceId:String):Null<CLoweredImportType>
		return typesByInstance.get(instanceId);

	public function functionById(id:String):Null<CLoweredImportFunction>
		return functionsById.get(id);

	public function constantById(id:String):Null<CLoweredImportConstant>
		return constantsById.get(id);

	public static function empty():CLoweredImports
		return new CLoweredImports([], [], [], [], []);
}

/**
	Request-local admission and reachability registry for hand-authored imports.

	The authoritative C header owns spelling and layout. Haxe declarations supply
	the typed use-site model, while the generated program includes the header and
	never recreates an imported definition.
**/
class CImportRegistry {
	final context:CompilationContext;
	final snapshot:TypedCContractSnapshot;
	final resolveValueType:CImportValueTypeResolver;
	final declarationsByPath:Map<String, TypedAstDeclaration> = [];
	final contractsByPath:Map<String, TypedCDeclaration> = [];
	final includeFactsByOwner:Map<String, Array<TypedCBuildFact>> = [];
	final preparedTypes:Map<String, CPreparedImportType> = [];
	final preparedFunctions:Map<String, CPreparedImportFunction> = [];
	final preparedConstants:Map<String, CPreparedImportConstant> = [];
	final reachedOwners:Map<String, Bool> = [];

	public function new(context:CompilationContext, program:TypedProgramInput, snapshot:TypedCContractSnapshot, resolveValueType:CImportValueTypeResolver) {
		this.context = context;
		this.snapshot = snapshot;
		this.resolveValueType = resolveValueType;
		if (snapshot.schemaVersion != 2)
			throw new CBodyLoweringError(HxcIRDiagnostic.invalidAbiBoundary(Std.string(context.profile), "C import contract",
				'Unsupported typed C contract schema ${snapshot.schemaVersion}; expected schema 2.', fallbackSource(program)),
				fallbackPosition(program));
		for (declaration in program.declarations)
			declarationsByPath.set(declaration.path, declaration);
		for (contract in snapshot.declarations)
			contractsByPath.set(contract.modulePath, contract);
		for (fact in snapshot.buildFacts) {
			if (fact.kind != "include")
				continue;
			for (owner in fact.ownerModulePaths) {
				var facts = includeFactsByOwner.get(owner);
				if (facts == null) {
					facts = [];
					includeFactsByOwner.set(owner, facts);
				}
				facts.push(fact);
			}
		}
	}

	/** Recognize a nominal import or the literal-only CString boundary. */
	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CBodyValueType> {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : valueType(resolved, position, ownerModule, sourcePath, fail, node);
			case TLazy(resolve): valueType(resolve(), position, ownerModule, sourcePath, fail, node);
			case TType(reference, parameters):
				final path = typePath(reference.get().module, reference.get().pack, reference.get().name);
				isImportedType(path) ? CBodyValueType.imported(prepareType(path, parameters, position, sourcePath)) : null;
			case TInst(reference, parameters):
				final value = reference.get();
				final path = typePath(value.module, value.pack, value.name);
				isImportedType(path) ? CBodyValueType.imported(prepareType(path, parameters, position, sourcePath)) : null;
			case TEnum(reference, parameters):
				final value = reference.get();
				final path = typePath(value.module, value.pack, value.name);
				isImportedType(path) ? CBodyValueType.imported(prepareType(path, parameters, position, sourcePath)) : null;
			case TAbstract(reference, parameters): final value = reference.get(); final path = typePath(value.module, value.pack,
					value.name); path == "c.CString" && parameters.length == 0 ? CBodyValueType.cString() : null;
			case _: null;
		};
	}

	public function functionFor(callee:TypedExpr, callPosition:Position, callerSourcePath:String):Null<CPreparedImportFunction> {
		return switch unwrap(callee).expr {
			case TField(_, FStatic(classReference, fieldReference)):
				final owner = classReference.get();
				final ownerPath = typePath(owner.module, owner.pack, owner.name);
				final contract = contractsByPath.get(ownerPath);
				final fieldContract = contract == null ? null : contractField(contract, fieldReference.get().name, "function");
				fieldContract == null ? null : prepareFunction(ownerPath, fieldReference.get(), fieldContract, callPosition, callerSourcePath);
			case _: null;
		};
	}

	public function staticConstantFor(classReference:Ref<ClassType>, fieldReference:Ref<ClassField>, position:Position,
			callerSourcePath:String):Null<CPreparedImportConstant> {
		final owner = classReference.get();
		final ownerPath = typePath(owner.module, owner.pack, owner.name);
		final contract = contractsByPath.get(ownerPath);
		final field = fieldReference.get();
		final fieldContract = contract == null ? null : contractField(contract, field.name, "constant");
		return fieldContract == null ? null : prepareConstant(ownerPath, field, fieldContract, position, callerSourcePath, null);
	}

	public function enumConstantFor(reference:Ref<EnumType>, field:EnumField, position:Position, callerSourcePath:String):Null<CPreparedImportConstant> {
		final owner = reference.get();
		final ownerPath = typePath(owner.module, owner.pack, owner.name);
		if (!isImportedType(ownerPath))
			return null;
		final contract = contractsByPath.get(ownerPath);
		final fieldContract = contract == null ? null : contractField(contract, field.name, "constant");
		if (fieldContract == null)
			abiFailure(field.pos, callerSourcePath, 'imported enum `$ownerPath`', 'Constructor `${field.name}` requires an exact `@:c.name`.');
		final importedType = CBodyValueType.imported(prepareType(ownerPath, [], position, callerSourcePath));
		return prepareEnumConstant(ownerPath, field, fieldContract, importedType, callerSourcePath);
	}

	public function canonicalTypes():Array<CPreparedImportType> {
		final values = [for (value in preparedTypes) value];
		values.sort((left, right) -> compareUtf8(left.instanceId, right.instanceId));
		return values;
	}

	public function finalize(symbols:CSymbolRegistry):CLoweredImports {
		final types:Array<CLoweredImportType> = [];
		for (prepared in canonicalTypes()) {
			types.push(new CLoweredImportType(prepared, symbols.identifierFor(prepared.request),
				prepared.fields.map(field -> new CLoweredImportField(field, symbols.identifierFor(field.request)))));
		}
		final functions = [for (value in preparedFunctions) value];
		functions.sort((left, right) -> compareUtf8(left.id, right.id));
		final loweredFunctions = functions.map(value -> new CLoweredImportFunction(value, symbols.identifierFor(value.request)));
		final constants = [for (value in preparedConstants) value];
		constants.sort((left, right) -> compareUtf8(left.id, right.id));
		final loweredConstants = constants.map(value -> new CLoweredImportConstant(value, symbols.identifierFor(value.request)));
		return new CLoweredImports(types, loweredFunctions, loweredConstants, reachedIncludes(), reachedBuildFacts());
	}

	function prepareType(path:String, parameters:Array<Type>, position:Position, callerSourcePath:String):CPreparedImportType {
		final existing = preparedTypes.get(path);
		if (existing != null)
			return existing;
		if (parameters.length != 0)
			abiFailure(position, callerSourcePath, 'imported type `$path`', "Generic C import types require a closed generated specialization.");
		final contract = requireContract(path, position, callerSourcePath);
		final declaration = requireDeclaration(path, position, callerSourcePath);
		final cName = requireExactName(contract.cName, position, callerSourcePath, 'imported type `$path`');
		final kind = switch contract.sourceKind {
			case "class" if (contract.layout == "struct"): CITStruct;
			case "enum" if (contract.layout == "enum"): CITEnum;
			case "typedef" if (contract.layout == null): CITTypedef;
			case _:
				abiFailure(position, callerSourcePath, 'imported type `$path`',
					"This slice admits header-owned structs, closed enums, and scalar typedefs only.");
		};
		requireAuthoritativeInclude(path, position, callerSourcePath);
		if (contract.pack != null || contract.align != null)
			abiFailure(position, callerSourcePath, 'imported type `$path`', "Packing/alignment claims require the later Clang-derived ABI model.");
		if (kind == CITStruct && !declaration.isExtern)
			abiFailure(position, callerSourcePath, 'imported struct `$path`', "A header-owned struct must be declared as an extern class.");
		final source = declarationSource(declaration);
		final namespace = switch kind {
			case CITStruct: CNSTag("translation-unit");
			case CITEnum | CITTypedef: CNSOrdinary("translation-unit");
		};
		final request = new CSymbolRequest(CSKType, path.split("."), namespace, CSVExternal, cName);
		context.symbols.register(request);
		final prepared = new CPreparedImportType(path, declaration.ownerModulePath, kind, source, request);
		preparedTypes.set(path, prepared);
		reach(path);
		switch kind {
			case CITStruct:
				prepareStructFields(prepared, contract, declaration);
			case CITEnum:
				prepareEnumFields(prepared, contract, declaration);
			case CITTypedef:
				prepareTypedefTarget(prepared, declaration);
		}
		return prepared;
	}

	function prepareStructFields(prepared:CPreparedImportType, contract:TypedCDeclaration, declaration:TypedAstDeclaration):Void {
		final fields = contract.fields.filter(field -> field.kind == "field");
		if (fields.length == 0)
			abiFailure(rawPosition(declaration), declaration.sourcePath, 'imported struct `${prepared.haxePath}`',
				"A by-value struct must expose at least one field.");
		for (index in 0...fields.length) {
			final contractField = fields[index];
			if (contractField.bitWidth != null || contractField.align != null)
				abiFailure(rawPosition(declaration), declaration.sourcePath, 'imported field `${prepared.haxePath}.${contractField.name}`',
					"Bit-fields and field alignment require the later Clang-derived ABI model.");
			final raw = requireClassField(declaration, contractField.name);
			if (raw.rawClassField == null)
				abiFailure(rawPosition(declaration), declaration.sourcePath, 'imported field `${prepared.haxePath}.${contractField.name}`',
					"Typed field storage was not retained.");
			final field = raw.rawClassField;
			final type = resolveValueType(field.type, field.pos, declaration.ownerModulePath, declaration.sourcePath, abiRejectFor(declaration.sourcePath),
				'imported-field:${prepared.haxePath}.${field.name}');
			if (type.irType == IRTVoid || type.isCString())
				abiFailure(field.pos, declaration.sourcePath, 'imported field `${prepared.haxePath}.${field.name}`',
					"Void and CString are not by-value struct fields.");
			final cName = requireExactName(contractField.cName, field.pos, declaration.sourcePath, 'imported field `${prepared.haxePath}.${field.name}`');
			final request = new CSymbolRequest(CSKField, prepared.haxePath.split(".").concat([field.name]), CNSMember(prepared.declarationId), CSVExternal,
				cName, [], [], index);
			context.symbols.register(request);
			prepared.fields.push(new CPreparedImportField(field.name, type, fieldWritable(field),
				HaxeSourceSpan.fromPosition(field.pos, declaration.sourcePath), request));
		}
	}

	function prepareEnumFields(prepared:CPreparedImportType, contract:TypedCDeclaration, declaration:TypedAstDeclaration):Void {
		final constants = contract.fields.filter(field -> field.kind == "constant");
		if (constants.length == 0)
			abiFailure(rawPosition(declaration), declaration.sourcePath, 'imported enum `${prepared.haxePath}`',
				"A closed imported enum must expose at least one named value.");
		for (field in declaration.fields) {
			if (field.role != "enum-constructor" || field.rawEnumField == null)
				continue;
			final contractField = contractField(contract, field.name, "constant");
			if (contractField == null)
				abiFailure(field.rawEnumField.pos, declaration.sourcePath, 'imported enum `${prepared.haxePath}`',
					'Constructor `${field.name}` requires an exact `@:c.name`.');
			if (enumHasPayload(field.rawEnumField.type))
				abiFailure(field.rawEnumField.pos, declaration.sourcePath, 'imported enum `${prepared.haxePath}.${field.name}`',
					"Payload constructors are not C enum constants.");
			prepareEnumConstant(prepared.haxePath, field.rawEnumField, contractField, CBodyValueType.imported(prepared), declaration.sourcePath);
		}
	}

	function prepareTypedefTarget(prepared:CPreparedImportType, declaration:TypedAstDeclaration):Void {
		final raw = switch declaration.raw {
			case TTypeDecl(reference): reference.get();
			case _: abiFailure(rawPosition(declaration), declaration.sourcePath, 'imported typedef `${prepared.haxePath}`',
					"The retained declaration is not a typedef.");
		};
		if (raw.params.length != 0)
			abiFailure(raw.pos, declaration.sourcePath, 'imported typedef `${prepared.haxePath}`', "Generic C typedefs are outside this direct slice.");
		final target = resolveValueType(raw.type, raw.pos, declaration.ownerModulePath, declaration.sourcePath, abiRejectFor(declaration.sourcePath),
			'imported-typedef:${prepared.haxePath}');
		if (target.irType == IRTVoid || target.isCString() || target.importedValue() != null)
			abiFailure(raw.pos, declaration.sourcePath, 'imported typedef `${prepared.haxePath}`', "This slice admits a scalar typedef target only.");
		prepared.setAliasTarget(target);
	}

	function prepareFunction(ownerPath:String, field:ClassField, contractField:TypedCContractField, callPosition:Position,
			callerSourcePath:String):CPreparedImportFunction {
		final id = 'native.function.$ownerPath.${field.name}';
		final existing = preparedFunctions.get(id);
		if (existing != null)
			return existing;
		final declaration = requireDeclaration(ownerPath, callPosition, callerSourcePath);
		final contract = requireContract(ownerPath, callPosition, callerSourcePath);
		if (!declaration.isExtern)
			abiFailure(field.pos, declaration.sourcePath, 'imported function `$ownerPath.${field.name}`', "Imported functions must belong to an extern class.");
		requireAuthoritativeInclude(ownerPath, field.pos, declaration.sourcePath);
		validateFunctionPolicy(contract, contractField, field, declaration.sourcePath);
		final signature = functionType(field.type, field.pos, declaration.sourcePath, '$ownerPath.${field.name}');
		final parameters:Array<CBodyValueType> = [];
		for (index in 0...signature.arguments.length) {
			final argument = signature.arguments[index];
			if (argument.opt)
				abiFailure(field.pos, declaration.sourcePath, 'imported function `$ownerPath.${field.name}`',
					'Optional parameter `${argument.name}` has no exact C ABI.');
			if (isRestType(argument.t))
				abiFailure(field.pos, declaration.sourcePath, 'imported function `$ownerPath.${field.name}`',
					"Variadic functions are outside the admitted direct slice.");
			validateBoundaryType(argument.t, field.pos, declaration.sourcePath, '$ownerPath.${field.name} parameter $index', true);
			final mapping = resolveValueType(argument.t, field.pos, declaration.ownerModulePath, declaration.sourcePath, abiRejectFor(declaration.sourcePath),
				'imported-function:$ownerPath.${field.name}:argument:$index');
			if (mapping.irType == IRTVoid)
				abiFailure(field.pos, declaration.sourcePath, 'imported function `$ownerPath.${field.name}`', "A C parameter cannot have semantic Void type.");
			parameters.push(mapping);
		}
		validateBoundaryType(signature.result, field.pos, declaration.sourcePath, '$ownerPath.${field.name} result', false);
		final returnType = resolveValueType(signature.result, field.pos, declaration.ownerModulePath, declaration.sourcePath,
			abiRejectFor(declaration.sourcePath), 'imported-function:$ownerPath.${field.name}:return');
		if (returnType.isCString())
			abiFailure(field.pos, declaration.sourcePath, 'imported function `$ownerPath.${field.name}`',
				"Borrowed CString returns need an explicit lifetime owner.");
		final cName = requireExactName(contractField.cName, field.pos, declaration.sourcePath, 'imported function `$ownerPath.${field.name}`');
		final request = new CSymbolRequest(CSKMethod, ownerPath.split(".").concat([field.name]), CNSOrdinary("translation-unit"), CSVExternal, cName);
		context.symbols.register(request);
		final prepared = new CPreparedImportFunction(id, declaration.ownerModulePath, '$ownerPath.${field.name}', parameters, returnType,
			HaxeSourceSpan.fromPosition(field.pos, declaration.sourcePath), request);
		preparedFunctions.set(id, prepared);
		reach(ownerPath);
		return prepared;
	}

	function prepareConstant(ownerPath:String, field:ClassField, contractField:TypedCContractField, position:Position, callerSourcePath:String,
			expected:Null<CBodyValueType>):CPreparedImportConstant {
		final id = 'native.constant.$ownerPath.${field.name}';
		final existing = preparedConstants.get(id);
		if (existing != null)
			return existing;
		final declaration = requireDeclaration(ownerPath, position, callerSourcePath);
		requireAuthoritativeInclude(ownerPath, field.pos, declaration.sourcePath);
		validateBoundaryType(field.type, field.pos, declaration.sourcePath, '$ownerPath.${field.name} constant', false);
		final type = expected == null ? resolveValueType(field.type, field.pos, declaration.ownerModulePath, declaration.sourcePath,
			abiRejectFor(declaration.sourcePath), 'imported-constant:$ownerPath.${field.name}') : expected;
		if (type.irType == IRTVoid || type.isCString() || type.importedStructValue() != null)
			abiFailure(field.pos, declaration.sourcePath, 'imported constant `$ownerPath.${field.name}`',
				"Constants must be scalar, typedef, or closed-enum values.");
		final cName = requireExactName(contractField.cName, field.pos, declaration.sourcePath, 'imported constant `$ownerPath.${field.name}`');
		final request = new CSymbolRequest(CSKField, ownerPath.split(".").concat([field.name]), CNSOrdinary("translation-unit"), CSVExternal, cName);
		context.symbols.register(request);
		final prepared = new CPreparedImportConstant(id, declaration.ownerModulePath, '$ownerPath.${field.name}', type,
			HaxeSourceSpan.fromPosition(field.pos, declaration.sourcePath), request);
		preparedConstants.set(id, prepared);
		reach(ownerPath);
		return prepared;
	}

	function prepareEnumConstant(ownerPath:String, field:EnumField, contractField:TypedCContractField, type:CBodyValueType,
			sourcePath:String):CPreparedImportConstant {
		final id = 'native.constant.$ownerPath.${field.name}';
		final existing = preparedConstants.get(id);
		if (existing != null)
			return existing;
		final cName = requireExactName(contractField.cName, field.pos, sourcePath, 'imported enum constant `$ownerPath.${field.name}`');
		final request = new CSymbolRequest(CSKField, ownerPath.split(".").concat([field.name]), CNSOrdinary("translation-unit"), CSVExternal, cName);
		context.symbols.register(request);
		final declaration = requireDeclaration(ownerPath, field.pos, sourcePath);
		final prepared = new CPreparedImportConstant(id, declaration.ownerModulePath, '$ownerPath.${field.name}', type,
			HaxeSourceSpan.fromPosition(field.pos, sourcePath), request);
		preparedConstants.set(id, prepared);
		reach(ownerPath);
		return prepared;
	}

	function validateFunctionPolicy(owner:TypedCDeclaration, field:TypedCContractField, raw:ClassField, sourcePath:String):Void {
		for (convention in [owner.callingConvention, field.callingConvention]) {
			if (convention != null && convention != "c")
				abiFailure(raw.pos, sourcePath, 'imported function `${owner.modulePath}.${raw.name}`',
					'Calling convention `$convention` is not strict portable C in this slice.');
		}
		for (linkage in [owner.linkage, field.linkage]) {
			if (linkage != null && linkage != "external")
				abiFailure(raw.pos, sourcePath, 'imported function `${owner.modulePath}.${raw.name}`', 'Linkage `$linkage` is not an external C import.');
		}
		if (owner.visibility != null || field.visibility != null || owner.section != null || field.section != null || field.exported)
			abiFailure(raw.pos, sourcePath, 'imported function `${owner.modulePath}.${raw.name}`',
				"Export, visibility, and section controls describe generated declarations, not a header-owned import.");
	}

	function validateBoundaryType(type:Type, position:Position, sourcePath:String, label:String, parameter:Bool):Void {
		switch type {
			case TMono(reference):
				final resolved = reference.get();
				if (resolved != null)
					validateBoundaryType(resolved, position, sourcePath, label, parameter);
			case TLazy(resolve):
				validateBoundaryType(resolve(), position, sourcePath, label, parameter);
			case TType(reference, parameters):
				final path = typePath(reference.get().module, reference.get().pack, reference.get().name);
				if (!isImportedType(path))
					validateBoundaryType(TypeTools.applyTypeParameters(reference.get().type, reference.get().params, parameters), position, sourcePath, label,
						parameter);
			case TFun(_, _):
				abiFailure(position, sourcePath, label, "Callbacks require the later typed function-pointer and context-lifetime contract.");
			case TDynamic(_):
				abiFailure(position, sourcePath, label, "Dynamic cannot cross a C ABI boundary.");
			case TAnonymous(_):
				abiFailure(position, sourcePath, label, "Anonymous structural values are not nominal C ABI types.");
			case TAbstract(reference, _):
				final value = reference.get();
				final path = typePath(value.module, value.pack, value.name);
				if (path == "c.FunctionPtr")
					abiFailure(position, sourcePath, label, "Callbacks require the later typed function-pointer and context-lifetime contract.");
				if (pointerLike(path) && !(parameter && path == "c.CString"))
					abiFailure(position, sourcePath, label, "Pointer and retained-borrow lifetimes are outside this direct by-value slice.");
			case TInst(reference, _):
				final value = reference.get();
				final path = typePath(value.module, value.pack, value.name);
				if (!isImportedType(path))
					abiFailure(position, sourcePath, label, 'Class `$path` is not a header-owned by-value C type.');
			case TEnum(reference, _):
				final value = reference.get();
				final path = typePath(value.module, value.pack, value.name);
				if (!isImportedType(path))
					abiFailure(position, sourcePath, label, 'Enum `$path` is not a header-owned closed C enum.');
		}
	}

	function reachedBuildFacts():Array<TypedCBuildFact> {
		final result:Array<TypedCBuildFact> = [];
		for (fact in snapshot.buildFacts) {
			final owners = fact.ownerModulePaths.filter(owner -> reachedOwners.exists(owner));
			if (owners.length == 0)
				continue;
			owners.sort(compareUtf8);
			if (fact.kind == "define")
				abiFailure(fallbackPositionForOwner(owners[0]), sourcePathForOwner(owners[0]), 'C import build fact `${fact.name}`',
					"Preprocessor definitions require an exact configuration/ABI identity and are outside this direct slice.");
			result.push({
				kind: fact.kind,
				name: fact.name,
				value: fact.value,
				valueKind: fact.valueKind,
				ownerModulePaths: owners
			});
		}
		result.sort((left, right) -> compareUtf8(factKey(left), factKey(right)));
		return result;
	}

	function reachedIncludes():Array<CInclude> {
		final byPath:Map<String, CIncludeKind> = [];
		for (fact in reachedBuildFacts()) {
			if (fact.kind != "include")
				continue;
			final kind = fact.value == "local" ? Local : System;
			final prior = byPath.get(fact.name);
			if (prior != null && prior != kind)
				abiFailure(fallbackPositionForOwner(fact.ownerModulePaths[0]), sourcePathForOwner(fact.ownerModulePaths[0]), 'import include `${fact.name}`',
					"The same header cannot be both local and system.");
			byPath.set(fact.name, kind);
		}
		final paths = [for (path in byPath.keys()) path];
		paths.sort(compareUtf8);
		return paths.map(path -> {path: path, kind: byPath.get(path)});
	}

	function isImportedType(path:String):Bool {
		final contract = contractsByPath.get(path);
		return contract != null && contract.headerPath == null && includeFactsByOwner.exists(path) && switch contract.sourceKind {
			case "class": contract.layout == "struct";
			case "enum": contract.layout == "enum";
			case "typedef": contract.layout == null;
			case _: false;
		};
	}

	function requireAuthoritativeInclude(owner:String, position:Position, sourcePath:String):Void {
		if (!includeFactsByOwner.exists(owner))
			abiFailure(position, sourcePath, 'C import `$owner`', "An authoritative `@:c.include` is required.");
	}

	function requireContract(path:String, position:Position, sourcePath:String):TypedCDeclaration {
		final value = contractsByPath.get(path);
		return value == null ? abiFailure(position, sourcePath, 'C import `$path`', "The typed C contract declaration is missing.") : value;
	}

	function requireDeclaration(path:String, position:Position, sourcePath:String):TypedAstDeclaration {
		final value = declarationsByPath.get(path);
		return value == null ? abiFailure(position, sourcePath, 'C import `$path`', "The retained typed declaration is missing.") : value;
	}

	function requireClassField(declaration:TypedAstDeclaration, name:String):TypedAstField {
		for (field in declaration.fields) {
			if (field.name == name && field.rawClassField != null)
				return field;
		}
		return abiFailure(rawPosition(declaration), declaration.sourcePath, 'C import `${declaration.path}.$name`', "The retained typed field is missing.");
	}

	static function contractField(contract:TypedCDeclaration, name:String, kind:String):Null<TypedCContractField> {
		for (field in contract.fields) {
			if (field.name == name && field.kind == kind)
				return field;
		}
		return null;
	}

	function functionType(type:Type, position:Position, sourcePath:String, path:String):{arguments:Array<{name:String, opt:Bool, t:Type}>, result:Type} {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? abiFailure(position, sourcePath, 'imported function `$path`',
					"Its function type is unresolved.") : functionType(resolved, position, sourcePath, path);
			case TLazy(resolve): functionType(resolve(), position, sourcePath, path);
			case TFun(arguments, result): {arguments: arguments, result: result};
			case _: abiFailure(position, sourcePath, 'imported function `$path`', "The retained field type is not a function prototype.");
		};
	}

	static function isRestType(type:Type):Bool {
		return switch type {
			case TAbstract(reference, _): final value = reference.get(); value.name == "Rest" && (value.pack.join(".") == "haxe"
					|| value.pack.join(".") == "haxe.extern");
			case TType(reference, parameters):
				final value = reference.get();
				isRestType(TypeTools.applyTypeParameters(value.type, value.params, parameters));
			case TMono(reference): final resolved = reference.get(); resolved != null && isRestType(resolved);
			case TLazy(resolve): isRestType(resolve());
			case _: false;
		};
	}

	static function enumHasPayload(type:Type):Bool {
		return switch type {
			case TMono(reference): final resolved = reference.get(); resolved != null && enumHasPayload(resolved);
			case TLazy(resolve): enumHasPayload(resolve());
			case TFun(arguments, _): arguments.length != 0;
			case _: false;
		};
	}

	static function pointerLike(path:String):Bool {
		return switch path {
			case "c.Ptr" | "c.ConstPtr" | "c.NullablePtr" | "c.Ref" | "c.ConstRef" | "c.RestrictPtr" | "c.VolatilePtr" | "c.FunctionPtr" | "c.Span" |
				"c.ConstSpan" | "c.CString" | "c.StringView" | "c.Owned" | "c.Borrowed" | "c.Allocator" | "c.Arena": true;
			case _: false;
		};
	}

	static function fieldWritable(field:ClassField):Bool {
		return switch field.kind {
			case FVar(_, write): switch write {
					case AccNo | AccNever: false;
					case _: true;
				};
			case FMethod(_): false;
		};
	}

	function reach(owner:String):Void
		reachedOwners.set(owner, true);

	function fallbackPositionForOwner(owner:String):Position {
		final declaration = declarationsByPath.get(owner);
		return declaration == null ? fallbackPositionFromContracts() : rawPosition(declaration);
	}

	function sourcePathForOwner(owner:String):String {
		final declaration = declarationsByPath.get(owner);
		return declaration == null ? "<compiler>" : declaration.sourcePath;
	}

	function fallbackPositionFromContracts():Position {
		for (declaration in declarationsByPath)
			return rawPosition(declaration);
		throw new haxe.Exception("C import registry has no typed source position");
	}

	static function rawPosition(declaration:TypedAstDeclaration):Position {
		return switch declaration.raw {
			case TClassDecl(reference): reference.get().pos;
			case TEnumDecl(reference): reference.get().pos;
			case TTypeDecl(reference): reference.get().pos;
			case TAbstract(reference): reference.get().pos;
		};
	}

	static function declarationSource(declaration:TypedAstDeclaration):HxcSourceSpan
		return HaxeSourceSpan.fromPosition(rawPosition(declaration), declaration.sourcePath);

	static function fallbackSource(program:TypedProgramInput):HxcSourceSpan {
		for (declaration in program.declarations)
			return declarationSource(declaration);
		return new HxcSourceSpan("compiler/input", 1, 1, 1, 1);
	}

	static function fallbackPosition(program:TypedProgramInput):Position {
		for (declaration in program.declarations)
			return rawPosition(declaration);
		throw new haxe.Exception("C import registry requires at least one typed declaration");
	}

	function requireExactName(value:Null<String>, position:Position, sourcePath:String, context:String):String
		return value == null ? abiFailure(position, sourcePath, context, "An exact `@:c.name` is required for a header-owned declaration.") : value;

	static function unwrap(expression:TypedExpr):TypedExpr {
		return switch expression.expr {
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): unwrap(inner);
			case _: expression;
		};
	}

	static function typePath(module:String, pack:Array<String>, name:String):String {
		final expected = pack.length == 0 ? name : pack.join(".") + "." + name;
		return module == expected ? module : module + "." + name;
	}

	static function factKey(fact:TypedCBuildFact):String
		return '${fact.kind}\x00${fact.name}\x00${fact.valueKind == null ? "" : fact.valueKind}\x00${fact.value == null ? "" : fact.value}\x00${fact.ownerModulePaths.join("\x00")}';

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}

	function abiRejectFor(sourcePath:String):(Position, String) -> Void
		return (position, node) -> abiFailure(position, sourcePath, "C import type", 'Unsupported ABI type `$node`.');

	function abiFailure<T>(position:Position, sourcePath:String, boundary:String, detail:String):T {
		throw new CBodyLoweringError(HxcIRDiagnostic.invalidAbiBoundary(Std.string(context.profile), boundary, detail,
			HaxeSourceSpan.fromPosition(position, sourcePath)), position);
	}
}
#else
class CImportRegistry {
	public function new() {}
}
#end
