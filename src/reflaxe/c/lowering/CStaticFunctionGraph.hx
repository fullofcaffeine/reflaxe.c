package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.ir.HxcIRDiagnostic;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.lowering.CBodyLowering.CBodyGlobalInput;
import reflaxe.c.lowering.CBodyLowering.CBodyInitializerInput;
import reflaxe.c.lowering.CGenericSpecialization.CGenericCallResolver;
import reflaxe.c.lowering.CGenericSpecialization.CGenericFunctionSpecialization;
import reflaxe.c.lowering.CGenericSpecialization.CGenericSpecializationReason;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeArgument;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeCanonicalizer;
import reflaxe.c.lowering.CGenericSpecializationContract;

/** Reachable static-function inputs plus the semantic executable entry ID. */
class CStaticFunctionGraph {
	public final entryFunctionId:String;
	public final functions:Array<CBodyFunctionInput>;
	public final globals:Array<CBodyGlobalInput>;
	public final specializations:Array<CGenericFunctionSpecialization>;

	public function new(entryFunctionId:String, functions:Array<CBodyFunctionInput>, globals:Array<CBodyGlobalInput>,
			specializations:Array<CGenericFunctionSpecialization>) {
		this.entryFunctionId = entryFunctionId;
		this.functions = functions.copy();
		this.globals = globals.copy();
		this.specializations = specializations.copy();
	}
}

/** Collects only direct static targets reachable from the typed Haxe entry body. */
class CStaticFunctionGraphCollector {
	public static inline final MAX_FUNCTION_SPECIALIZATIONS = CGenericSpecializationContract.MAX_FUNCTION_SPECIALIZATIONS;

	final context:CompilationContext;
	final specializationsByKey:Map<String, CGenericFunctionSpecialization> = [];
	final specializationKeysByDigest:Map<String, String> = [];

	public function new(context:CompilationContext) {
		this.context = context;
	}

	public function collect(entry:CBodyFunctionInput, program:TypedProgramInput, ?initializers:Array<CBodyInitializerInput>):CStaticFunctionGraph {
		final available = staticFunctionInputs(program);
		final byId:Map<String, CBodyFunctionInput> = [];
		final pending:Array<CBodyFunctionInput> = [];
		if (typeParameters(entry).length != 0) {
			unsupportedAt(entry.expression.pos, entry.sourcePath, 'entry-point-generic-parameters:${typeParameters(entry).length}');
		}
		add(entry, byId, pending);
		available.set(CBodyLowering.functionId(entry.declarationPath, entry.fieldName), entry);
		var index = 0;
		while (index < pending.length) {
			final caller = pending[index++];
			collectExpression(caller.expression, caller, available, byId, pending);
		}
		if (initializers != null) {
			for (initializer in initializers) {
				final initializerCaller:CBodyFunctionInput = {
					modulePath: initializer.modulePath,
					declarationPath: initializer.declarationPath,
					sourcePath: initializer.sourcePath,
					fieldName: initializer.displayName,
					sourceOrder: initializer.sourceOrder,
					fieldType: initializer.expression.t,
					expression: initializer.expression,
					typeParameters: [],
					specialization: null
				};
				collectExpression(initializer.expression, initializerCaller, available, byId, pending);
				while (index < pending.length) {
					final caller = pending[index++];
					collectExpression(caller.expression, caller, available, byId, pending);
				}
			}
		}
		final functions = [for (fn in byId) fn];
		functions.sort(compareInputs);
		final specializations = [for (specialization in specializationsByKey) specialization];
		specializations.sort((left, right) -> CGenericTypeCanonicalizer.compareUtf8(left.key, right.key));
		return new CStaticFunctionGraph(CBodyLowering.functionInputId(entry), functions, staticGlobalInputs(program), specializations);
	}

	function collectExpression(expression:TypedExpr, caller:CBodyFunctionInput, available:Map<String, CBodyFunctionInput>,
			byId:Map<String, CBodyFunctionInput>, pending:Array<CBodyFunctionInput>):Void {
		switch expression.expr {
			case TCall(callee, arguments) if (!isCompilerIntrinsicCall(callee)):
				final baseTargetId = directStaticFunctionId(callee);
				final target = baseTargetId == null ? null : available.get(baseTargetId);
				if (target != null && baseTargetId != null) {
					final parameters = typeParameters(target);
					if (parameters.length == 0) {
						add(target, byId, pending);
					} else {
						final resolved = CGenericCallResolver.resolve(baseTargetId, target.fieldType, parameters, callee.t,
							arguments.map(argument -> argument.t), caller.specialization, context.profile, expression.pos,
							(position, node) -> unsupportedAt(position, caller.sourcePath, node));
						final reason = new CGenericSpecializationReason(CBodyLowering.functionInputId(caller),
							HaxeSourceSpan.fromPosition(expression.pos, caller.sourcePath), expression.pos);
						final specialization = requireSpecialization(target, resolved.arguments, reason);
						add(specializedInput(target, specialization), byId, pending);
					}
				}
			case _:
		}
		TypedExprTools.iter(expression, child -> collectExpression(child, caller, available, byId, pending));
	}

	function requireSpecialization(base:CBodyFunctionInput, arguments:Array<CGenericTypeArgument>,
			reason:CGenericSpecializationReason):CGenericFunctionSpecialization {
		final baseId = CBodyLowering.functionId(base.declarationPath, base.fieldName);
		final key = CGenericTypeCanonicalizer.functionKey(baseId, arguments);
		final existing = specializationsByKey.get(key);
		if (existing != null) {
			existing.addReason(reason);
			return existing;
		}
		if ([for (_ in specializationsByKey.keys()) 1].length >= MAX_FUNCTION_SPECIALIZATIONS) {
			throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile),
				'TCall(generic-specialization-budget:$MAX_FUNCTION_SPECIALIZATIONS:expanding-or-excessive:$baseId)', "generic specialization discovery",
				reason.source),
				reason.position);
		}
		final specialization = new CGenericFunctionSpecialization(baseId, base.fieldName, typeParameters(base), arguments, reason);
		final priorKey = specializationKeysByDigest.get(specialization.digest);
		if (priorKey != null && priorKey != specialization.key) {
			throw new CBodyEmissionError('generic specialization digest collision `${specialization.digest}` between `$priorKey` and `${specialization.key}`');
		}
		specializationKeysByDigest.set(specialization.digest, specialization.key);
		specializationsByKey.set(specialization.key, specialization);
		return specialization;
	}

	static function specializedInput(base:CBodyFunctionInput, specialization:CGenericFunctionSpecialization):CBodyFunctionInput
		return {
			modulePath: base.modulePath,
			declarationPath: base.declarationPath,
			sourcePath: base.sourcePath,
			fieldName: base.fieldName,
			sourceOrder: base.sourceOrder,
			fieldType: base.fieldType,
			expression: base.expression,
			typeParameters: typeParameters(base),
			specialization: specialization
		};

	static function typeParameters(input:CBodyFunctionInput):Array<TypeParameter>
		return input.typeParameters == null ? [] : input.typeParameters;

	static function isCompilerIntrinsicCall(callee:TypedExpr):Bool {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)): final owner = classReference.get(); owner.pack.join(".") == "haxe" && owner.name == "Log" && fieldReference.get()
					.name == "trace";
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isCompilerIntrinsicCall(inner);
			case _: false;
		};
	}

	static function directStaticFunctionId(callee:TypedExpr):Null<String> {
		return switch callee.expr {
			case TField(_, FStatic(classReference, fieldReference)):
				final owner = classReference.get();
				CBodyLowering.functionId(owner.pack.concat([owner.name]).join("."), fieldReference.get().name);
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): directStaticFunctionId(inner);
			case _: null;
		};
	}

	static function staticFunctionInputs(program:TypedProgramInput):Map<String, CBodyFunctionInput> {
		final result:Map<String, CBodyFunctionInput> = [];
		for (declaration in program.declarations) {
			for (field in declaration.fields) {
				if (declaration.isExtern || field.role != "static" || field.isExtern || field.expression == null || field.rawClassField == null) {
					continue;
				}
				switch field.rawClassField.kind {
					case FMethod(_):
						final input:CBodyFunctionInput = {
							modulePath: declaration.ownerModulePath,
							declarationPath: declaration.path,
							sourcePath: declaration.sourcePath,
							fieldName: field.name,
							sourceOrder: field.sourceOrder,
							fieldType: field.rawClassField.type,
							expression: field.expression,
							typeParameters: field.rawClassField.params,
							specialization: null
						};
						result.set(CBodyLowering.functionId(input.declarationPath, input.fieldName), input);
					case FVar(_, _):
				}
			}
		}
		return result;
	}

	static function staticGlobalInputs(program:TypedProgramInput):Array<CBodyGlobalInput> {
		final result:Array<CBodyGlobalInput> = [];
		for (declaration in program.declarations) {
			for (field in declaration.fields) {
				if (declaration.isExtern || field.role != "static" || field.isExtern || field.rawClassField == null) {
					continue;
				}
				switch field.rawClassField.kind {
					case FVar(AccInline, _):
					case FVar(_, write):
						result.push({
							modulePath: declaration.ownerModulePath,
							declarationPath: declaration.path,
							sourcePath: declaration.sourcePath,
							fieldName: field.name,
							sourceOrder: field.sourceOrder,
							fieldType: field.rawClassField.type,
							mutable: write != AccNever,
							position: field.rawClassField.pos,
							expression: field.expression
						});
					case FMethod(_):
				}
			}
		}
		result.sort((left, right) -> {
			final identity = compareUtf8('${left.declarationPath}\u0000${left.fieldName}', '${right.declarationPath}\u0000${right.fieldName}');
			return identity != 0 ? identity : left.sourceOrder - right.sourceOrder;
		});
		return result;
	}

	static function add(input:CBodyFunctionInput, byId:Map<String, CBodyFunctionInput>, pending:Array<CBodyFunctionInput>):Void {
		final id = CBodyLowering.functionInputId(input);
		if (!byId.exists(id)) {
			byId.set(id, input);
			pending.push(input);
		}
	}

	static function compareInputs(left:CBodyFunctionInput, right:CBodyFunctionInput):Int {
		final identity = compareUtf8(CBodyLowering.functionInputId(left), CBodyLowering.functionInputId(right));
		return identity != 0 ? identity : left.sourceOrder - right.sourceOrder;
	}

	function unsupportedAt<T>(position:haxe.macro.Expr.Position, sourcePath:String, node:String):T {
		final source = HaxeSourceSpan.fromPosition(position, sourcePath);
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node, "generic specialization discovery", source),
			position);
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}
}
#else
class CStaticFunctionGraphCollector {
	public function new() {}
}
#end
