package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import haxe.macro.ExprTools;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.frontend.TypedProgramInput.TypedAstDeclaration;
import reflaxe.c.frontend.TypedProgramInput.TypedAstEntryFunction;
import reflaxe.c.frontend.TypedProgramInput.TypedAstEntryPoint;
import reflaxe.c.frontend.TypedProgramInput.TypedAstExpressionRoot;
import reflaxe.c.frontend.TypedProgramInput.TypedAstField;
import reflaxe.c.frontend.TypedProgramInput.TypedAstMetadata;
import reflaxe.c.frontend.TypedProgramInput.TypedAstModule;

/** Converts Haxe compiler objects into a stable whole-program input model. */
class TypedAstNormalizer {
	public static function normalize(moduleTypes:Array<ModuleType>, mainModule:Null<ModuleType>, mainExpr:Null<TypedExpr>):TypedProgramInput {
		final rawModules = moduleTypes.copy();
		rawModules.sort(compareModuleTypes);

		final declarations:Array<TypedAstDeclaration> = [];
		final expressionRoots:Array<TypedAstExpressionRoot> = [];
		for (moduleType in rawModules) {
			final declaration = normalizeDeclaration(moduleType, expressionRoots);
			declarations.push(declaration);
		}
		declarations.sort((left, right) -> compareStrings(declarationKey(left), declarationKey(right)));
		expressionRoots.sort(compareExpressionRoots);

		final grouped:Map<String, Array<TypedAstDeclaration>> = [];
		for (declaration in declarations) {
			final existing = grouped.get(declaration.ownerModulePath);
			if (existing == null) {
				grouped.set(declaration.ownerModulePath, [declaration]);
			} else {
				existing.push(declaration);
			}
		}

		final modulePaths = [for (path in grouped.keys()) path];
		modulePaths.sort(compareStrings);
		final modules:Array<TypedAstModule> = [];
		for (path in modulePaths) {
			modules.push({
				path: path,
				sourcePath: sourcePath(path),
				declarations: grouped.get(path)
			});
		}

		var entryPoint:Null<TypedAstEntryPoint> = null;
		if (mainExpr != null) {
			final mainModulePath = mainModule == null ? null : baseType(mainModule).module;
			final mainDeclarationPath = mainModule == null ? null : declarationPath(baseType(mainModule));
			entryPoint = {
				modulePath: mainModulePath,
				declarationPath: mainDeclarationPath,
				expression: mainExpr,
				target: captureEntryFunction(mainExpr)
			};
			expressionRoots.push({
				sourceOrder: 0,
				ownerModulePath: mainModulePath == null ? "<entry-point>" : mainModulePath,
				ownerDeclarationPath: mainDeclarationPath,
				fieldName: null,
				role: "entry-point",
				expression: mainExpr
			});
			expressionRoots.sort(compareExpressionRoots);
		}

		return new TypedProgramInput(modules, declarations, expressionRoots, entryPoint, rawModules);
	}

	static function captureEntryFunction(expression:TypedExpr):Null<TypedAstEntryFunction> {
		return switch expression.expr {
			case TCall({expr: TField(_, FStatic(classReference, fieldReference))}, arguments) if (arguments.length == 0):
				final owner = classReference.get();
				final field = fieldReference.get();
				final fieldExpression = field.expr();
				if (field.name != "main" || fieldExpression == null) {
					null;
				} else {
					final staticFields = owner.statics.get();
					var sourceOrder = 0;
					for (index in 0...staticFields.length) {
						if (staticFields[index].name == field.name) {
							sourceOrder = index;
							break;
						}
					}
					{
						modulePath: owner.module,
						declarationPath: declarationPath(owner),
						readableDeclarationPath: switch owner.kind {
							case KModuleFields(_): owner.module;
							case _: null;
						},
						sourcePath: sourcePath(owner.module),
						fieldName: field.name,
						sourceOrder: sourceOrder,
						fieldType: field.type,
						expression: fieldExpression
					};
				}
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): captureEntryFunction(inner);
			case _: null;
		};
	}

	static function normalizeDeclaration(moduleType:ModuleType, expressionRoots:Array<TypedAstExpressionRoot>):TypedAstDeclaration {
		final base = baseType(moduleType);
		final path = declarationPath(base);
		final fields:Array<TypedAstField> = [];
		var classKind:Null<String> = null;
		var isInterface = false;
		var isFinal = false;
		var isAbstract = false;
		var representedTypeKind:Null<String> = null;
		var representedTypeDisplay:Null<String> = null;

		switch moduleType {
			case TClassDecl(classRef):
				final classType = classRef.get();
				classKind = classKindName(classType.kind);
				isInterface = classType.isInterface;
				isFinal = classType.isFinal;
				isAbstract = classType.isAbstract;
				final memberFields = classType.fields.get();
				for (index in 0...memberFields.length) {
					fields.push(normalizeClassField(memberFields[index], "member", index, base.module, path, expressionRoots));
				}
				final staticFields = classType.statics.get();
				for (index in 0...staticFields.length) {
					fields.push(normalizeClassField(staticFields[index], "static", index, base.module, path, expressionRoots));
				}
				if (classType.constructor != null) {
					fields.push(normalizeClassField(classType.constructor.get(), "constructor", 0, base.module, path, expressionRoots));
				}
				if (classType.init != null) {
					expressionRoots.push({
						sourceOrder: 0,
						ownerModulePath: base.module,
						ownerDeclarationPath: path,
						fieldName: null,
						role: "class-init",
						expression: classType.init
					});
				}
			case TEnumDecl(enumRef):
				final enumType = enumRef.get();
				for (name in enumType.names) {
					final field = enumType.constructs.get(name);
					if (field != null) {
						fields.push(normalizeEnumField(field));
					}
				}
			case TTypeDecl(typeRef):
				final target = typeRef.get().type;
				representedTypeKind = typeKind(target);
				representedTypeDisplay = TypeTools.toString(target);
			case TAbstract(abstractRef):
				final underlying = abstractRef.get().type;
				representedTypeKind = typeKind(underlying);
				representedTypeDisplay = TypeTools.toString(underlying);
		}

		fields.sort(compareFields);
		return {
			path: path,
			name: base.name,
			kind: declarationKind(moduleType),
			ownerModulePath: base.module,
			ownership: path == base.module ? "primary" : "secondary",
			sourcePath: sourcePath(base.module),
			isPrivate: base.isPrivate == true,
			isExtern: base.isExtern == true,
			classKind: classKind,
			isInterface: isInterface,
			isFinal: isFinal,
			isAbstract: isAbstract,
			representedTypeKind: representedTypeKind,
			representedTypeDisplay: representedTypeDisplay,
			metadata: normalizeMetadata(base.meta.get()),
			fields: fields,
			raw: moduleType
		};
	}

	static function normalizeClassField(field:ClassField, role:String, sourceOrder:Int, ownerModulePath:String, ownerDeclarationPath:String,
			expressionRoots:Array<TypedAstExpressionRoot>):TypedAstField {
		final expression = field.expr();
		if (expression != null) {
			expressionRoots.push({
				sourceOrder: sourceOrder,
				ownerModulePath: ownerModulePath,
				ownerDeclarationPath: ownerDeclarationPath,
				fieldName: field.name,
				role: role,
				expression: expression
			});
		}
		return {
			sourceOrder: sourceOrder,
			name: field.name,
			role: role,
			kind: fieldKind(field.kind),
			typeKind: typeKind(field.type),
			typeDisplay: TypeTools.toString(field.type),
			isPublic: field.isPublic == true,
			isExtern: field.isExtern == true,
			metadata: normalizeMetadata(field.meta.get()),
			expression: expression,
			rawClassField: field,
			rawEnumField: null
		};
	}

	static function normalizeEnumField(field:EnumField):TypedAstField {
		return {
			sourceOrder: field.index,
			name: field.name,
			role: "enum-constructor",
			kind: "enum-constructor",
			typeKind: typeKind(field.type),
			typeDisplay: TypeTools.toString(field.type),
			isPublic: true,
			isExtern: false,
			metadata: normalizeMetadata(field.meta.get()),
			expression: null,
			rawClassField: null,
			rawEnumField: field
		};
	}

	static function normalizeMetadata(entries:Array<haxe.macro.Expr.MetadataEntry>):Array<TypedAstMetadata> {
		final normalized:Array<TypedAstMetadata> = [];
		var sourceOrder = 0;
		for (entry in entries) {
			final arguments = entry.params == null ? [] : [for (argument in entry.params) ExprTools.toString(argument)];
			// Reflaxe attaches this process-local cache hook globally. It is not
			// source metadata and appears only after a compiler-server cache hit.
			if (entry.name == ":build" && arguments.length == 1 && arguments[0] == "reflaxe.ReflectCompiler.addToBuildCache()") {
				continue;
			}
			normalized.push({
				sourceOrder: sourceOrder++,
				name: entry.name,
				arguments: arguments,
				raw: entry
			});
		}
		return normalized;
	}

	public static function baseType(moduleType:ModuleType):BaseType {
		return switch moduleType {
			case TClassDecl(reference): reference.get();
			case TEnumDecl(reference): reference.get();
			case TTypeDecl(reference): reference.get();
			case TAbstract(reference): reference.get();
		};
	}

	public static function declarationPath(base:BaseType):String {
		return base.pack.concat([base.name]).join(".");
	}

	public static function declarationKind(moduleType:ModuleType):String {
		return switch moduleType {
			case TClassDecl(_): "class";
			case TEnumDecl(_): "enum";
			case TTypeDecl(_): "typedef";
			case TAbstract(_): "abstract";
		};
	}

	public static function typeKind(type:Type):String {
		return switch type {
			case TMono(_): "monomorph";
			case TEnum(_, _): "enum";
			case TInst(_, _): "instance";
			case TType(_, _): "typedef";
			case TFun(_, _): "function";
			case TAnonymous(_): "anonymous";
			case TDynamic(_): "dynamic";
			case TLazy(_): "lazy";
			case TAbstract(_, _): "abstract";
		};
	}

	public static function expressionKind(expression:TypedExpr):String {
		return switch expression.expr {
			case TConst(_): "constant";
			case TLocal(_): "local";
			case TArray(_, _): "array-access";
			case TBinop(_, _, _): "binary-operator";
			case TField(_, _): "field-access";
			case TTypeExpr(_): "type-expression";
			case TParenthesis(_): "parenthesis";
			case TObjectDecl(_): "object-declaration";
			case TArrayDecl(_): "array-declaration";
			case TCall(_, _): "call";
			case TNew(_, _, _): "new";
			case TUnop(_, _, _): "unary-operator";
			case TFunction(_): "function";
			case TVar(_, _): "variable";
			case TBlock(_): "block";
			case TFor(_, _, _): "for";
			case TIf(_, _, _): "if";
			case TWhile(_, _, _): "while";
			case TSwitch(_, _, _): "switch";
			case TTry(_, _): "try";
			case TReturn(_): "return";
			case TBreak: "break";
			case TContinue: "continue";
			case TThrow(_): "throw";
			case TCast(_, _): "cast";
			case TMeta(_, _): "metadata";
			case TEnumParameter(_, _, _): "enum-parameter";
			case TEnumIndex(_): "enum-index";
			case TIdent(_): "identifier";
		};
	}

	static function classKindName(kind:ClassKind):String {
		return switch kind {
			case KNormal: "normal";
			case KTypeParameter(_): "type-parameter";
			case KModuleFields(_): "module-fields";
			case KExpr(_): "expression";
			case KGeneric: "generic";
			case KGenericInstance(_, _): "generic-instance";
			case KMacroType: "macro-type";
			case KAbstractImpl(_): "abstract-implementation";
			case KGenericBuild: "generic-build";
		};
	}

	static function fieldKind(kind:FieldKind):String {
		return switch kind {
			case FVar(_, _): "variable";
			case FMethod(MethNormal): "method-normal";
			case FMethod(MethInline): "method-inline";
			case FMethod(MethDynamic): "method-dynamic";
			case FMethod(MethMacro): "method-macro";
		};
	}

	static function sourcePath(modulePath:String):String {
		return modulePath.split(".").join("/") + ".hx";
	}

	static function compareModuleTypes(left:ModuleType, right:ModuleType):Int {
		final leftBase = baseType(left);
		final rightBase = baseType(right);
		return compareStrings('${leftBase.module}\u0000${declarationPath(leftBase)}\u0000${declarationKind(left)}',
			'${rightBase.module}\u0000${declarationPath(rightBase)}\u0000${declarationKind(right)}');
	}

	static function declarationKey(declaration:TypedAstDeclaration):String {
		return '${declaration.ownerModulePath}\u0000${declaration.path}\u0000${declaration.kind}';
	}

	static function compareExpressionRoots(left:TypedAstExpressionRoot, right:TypedAstExpressionRoot):Int {
		final ownership = compareStrings('${left.ownerModulePath}\u0000${left.ownerDeclarationPath}\u0000${left.role}',
			'${right.ownerModulePath}\u0000${right.ownerDeclarationPath}\u0000${right.role}');
		if (ownership != 0) {
			return ownership;
		}
		final order = compareInts(left.sourceOrder, right.sourceOrder);
		return order != 0 ? order : compareStrings('${left.fieldName}', '${right.fieldName}');
	}

	static function compareFields(left:TypedAstField, right:TypedAstField):Int {
		final role = compareInts(fieldRoleOrder(left.role), fieldRoleOrder(right.role));
		if (role != 0) {
			return role;
		}
		final order = compareInts(left.sourceOrder, right.sourceOrder);
		return order != 0 ? order : compareStrings('${left.name}\u0000${left.kind}', '${right.name}\u0000${right.kind}');
	}

	static function fieldRoleOrder(role:String):Int {
		return switch role {
			case "constructor": 0;
			case "member": 1;
			case "static": 2;
			case "enum-constructor": 3;
			case _: 4;
		};
	}

	static function compareInts(left:Int, right:Int):Int {
		return left < right ? -1 : (left > right ? 1 : 0);
	}

	static function compareStrings(left:String, right:String):Int {
		return left < right ? -1 : (left > right ? 1 : 0);
	}
}
#else
class TypedAstNormalizer {}
#end
