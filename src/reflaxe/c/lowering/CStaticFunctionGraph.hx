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
import reflaxe.c.lowering.CBodyConstructor.CBodyConstructorInput;
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
	public final constructors:Array<CBodyConstructorInput>;
	public final specializations:Array<CGenericFunctionSpecialization>;

	public function new(entryFunctionId:String, functions:Array<CBodyFunctionInput>, globals:Array<CBodyGlobalInput>,
			constructors:Array<CBodyConstructorInput>, specializations:Array<CGenericFunctionSpecialization>) {
		this.entryFunctionId = entryFunctionId;
		this.functions = functions.copy();
		this.globals = globals.copy();
		this.constructors = constructors.copy();
		this.specializations = specializations.copy();
	}
}

private typedef CConstructorDependency = {
	final targetId:String;
	final position:haxe.macro.Expr.Position;
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
		final availableConstructors = constructorInputs(program);
		final byId:Map<String, CBodyFunctionInput> = [];
		final pending:Array<CBodyFunctionInput> = [];
		final constructorsById:Map<String, CBodyConstructorInput> = [];
		final pendingConstructors:Array<CBodyConstructorInput> = [];
		final constructorDependencies:Map<String, Array<CConstructorDependency>> = [];
		if (typeParameters(entry).length != 0) {
			unsupportedAt(entry.expression.pos, entry.sourcePath, 'entry-point-generic-parameters:${typeParameters(entry).length}');
		}
		add(entry, byId, pending);
		available.set(CBodyLowering.functionId(entry.declarationPath, entry.fieldName), entry);
		var functionIndex = 0;
		var constructorIndex = 0;
		drainPending(available, availableConstructors, byId, pending, constructorsById, pendingConstructors, constructorDependencies, functionIndex,
			constructorIndex);
		functionIndex = pending.length;
		constructorIndex = pendingConstructors.length;
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
				collectExpression(initializer.expression, initializerCaller, null, available, availableConstructors, byId, pending, constructorsById,
					pendingConstructors, constructorDependencies);
				while (functionIndex < pending.length || constructorIndex < pendingConstructors.length) {
					if (functionIndex < pending.length) {
						final caller = pending[functionIndex++];
						collectExpression(caller.expression, caller, null, available, availableConstructors, byId, pending, constructorsById,
							pendingConstructors, constructorDependencies);
					}
					if (constructorIndex < pendingConstructors.length) {
						final constructor = pendingConstructors[constructorIndex++];
						collectExpression(constructor.expression, constructorCaller(constructor), constructor, available, availableConstructors, byId,
							pending, constructorsById, pendingConstructors, constructorDependencies);
					}
				}
			}
		}
		final functions = [for (fn in byId) fn];
		functions.sort(compareInputs);
		final specializations = [for (specialization in specializationsByKey) specialization];
		specializations.sort((left, right) -> CGenericTypeCanonicalizer.compareUtf8(left.key, right.key));
		final constructors = finalizeConstructors(constructorsById, constructorDependencies);
		return new CStaticFunctionGraph(CBodyLowering.functionInputId(entry), functions, staticGlobalInputs(program), constructors, specializations);
	}

	function collectExpression(expression:TypedExpr, caller:CBodyFunctionInput, currentConstructor:Null<CBodyConstructorInput>,
			available:Map<String, CBodyFunctionInput>, availableConstructors:Map<String, CBodyConstructorInput>, byId:Map<String, CBodyFunctionInput>,
			pending:Array<CBodyFunctionInput>, constructorsById:Map<String, CBodyConstructorInput>, pendingConstructors:Array<CBodyConstructorInput>,
			constructorDependencies:Map<String, Array<CConstructorDependency>>):Void {
		switch expression.expr {
			case TNew(classReference, _, _):
				final target = constructorForGraph(classReference, availableConstructors);
				if (target != null) {
					addConstructor(target, constructorsById, pendingConstructors);
				}
				if (currentConstructor != null && target != null) {
					addConstructorDependency(currentConstructor.id, target.id, expression.pos, constructorDependencies);
				}
			case TCall(callee, _) if (currentConstructor != null && isSuperCall(callee)):
				final baseId = currentConstructor.baseConstructorId;
				if (baseId == null) {
					unsupportedAt(expression.pos, caller.sourcePath, 'TCall(super:constructor-without-base:${currentConstructor.declarationPath})');
				}
				final target = availableConstructors.get(baseId);
				if (target != null) {
					addConstructor(target, constructorsById, pendingConstructors);
					addConstructorDependency(currentConstructor.id, target.id, expression.pos, constructorDependencies);
				}
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
		TypedExprTools.iter(expression,
			child -> collectExpression(child, caller, currentConstructor, available, availableConstructors, byId, pending, constructorsById,
				pendingConstructors, constructorDependencies));
	}

	function drainPending(available:Map<String, CBodyFunctionInput>, availableConstructors:Map<String, CBodyConstructorInput>,
			byId:Map<String, CBodyFunctionInput>, pending:Array<CBodyFunctionInput>, constructorsById:Map<String, CBodyConstructorInput>,
			pendingConstructors:Array<CBodyConstructorInput>, constructorDependencies:Map<String, Array<CConstructorDependency>>, functionStart:Int,
			constructorStart:Int):Void {
		var functionIndex = functionStart;
		var constructorIndex = constructorStart;
		while (functionIndex < pending.length || constructorIndex < pendingConstructors.length) {
			if (functionIndex < pending.length) {
				final caller = pending[functionIndex++];
				collectExpression(caller.expression, caller, null, available, availableConstructors, byId, pending, constructorsById, pendingConstructors,
					constructorDependencies);
			}
			if (constructorIndex < pendingConstructors.length) {
				final constructor = pendingConstructors[constructorIndex++];
				collectExpression(constructor.expression, constructorCaller(constructor), constructor, available, availableConstructors, byId, pending,
					constructorsById, pendingConstructors, constructorDependencies);
			}
		}
	}

	static function constructorForGraph(reference:Ref<ClassType>, available:Map<String, CBodyConstructorInput>):Null<CBodyConstructorInput> {
		final definition = reference.get();
		if (definition.isExtern || definition.meta.has(":c.layout") || definition.isInterface || definition.params.length != 0)
			return null;
		return available.get(CBodyConstructor.id(CBodyConstructor.classPath(reference)));
	}

	static function addConstructor(input:CBodyConstructorInput, byId:Map<String, CBodyConstructorInput>, pending:Array<CBodyConstructorInput>):Void {
		if (!byId.exists(input.id)) {
			byId.set(input.id, input);
			pending.push(input);
		}
	}

	static function addConstructorDependency(ownerId:String, targetId:String, position:haxe.macro.Expr.Position,
			dependencies:Map<String, Array<CConstructorDependency>>):Void {
		var values = dependencies.get(ownerId);
		if (values == null) {
			values = [];
			dependencies.set(ownerId, values);
		}
		for (value in values) {
			if (value.targetId == targetId)
				return;
		}
		values.push({targetId: targetId, position: position});
		values.sort((left, right) -> compareUtf8(left.targetId, right.targetId));
	}

	static function constructorCaller(input:CBodyConstructorInput):CBodyFunctionInput
		return {
			modulePath: input.modulePath,
			declarationPath: input.declarationPath,
			sourcePath: input.sourcePath,
			fieldName: "new",
			sourceOrder: input.sourceOrder,
			fieldType: input.fieldType,
			expression: input.expression,
			typeParameters: [],
			specialization: null
		};

	static function isSuperCall(callee:TypedExpr):Bool {
		return switch callee.expr {
			case TConst(TSuper): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isSuperCall(inner);
			case _: false;
		};
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

	static function constructorInputs(program:TypedProgramInput):Map<String, CBodyConstructorInput> {
		final result:Map<String, CBodyConstructorInput> = [];
		for (declaration in program.declarations) {
			final classReference = switch declaration.raw {
				case TClassDecl(reference): reference;
				case _: continue;
			};
			for (field in declaration.fields) {
				if (field.role != "constructor" || field.expression == null || field.rawClassField == null)
					continue;
				final id = CBodyConstructor.id(declaration.path);
				if (result.exists(id))
					throw new CBodyEmissionError('constructor discovery received duplicate typed input `$id`');
				final definition = classReference.get();
				final baseConstructorId = if (definition.superClass == null) {
					null;
				} else {
					final base = definition.superClass.t.get();
					base.constructor == null ? null : CBodyConstructor.id(base.pack.concat([base.name]).join("."));
				};
				result.set(id, {
					id: id,
					modulePath: declaration.ownerModulePath,
					declarationPath: declaration.path,
					sourcePath: declaration.sourcePath,
					sourceOrder: field.sourceOrder,
					fieldType: field.rawClassField.type,
					expression: field.expression,
					classReference: classReference,
					baseConstructorId: baseConstructorId,
					elided: false,
					canFail: false
				});
			}
		}
		return result;
	}

	function finalizeConstructors(selected:Map<String, CBodyConstructorInput>,
			dependencies:Map<String, Array<CConstructorDependency>>):Array<CBodyConstructorInput> {
		final state:Map<String, Int> = [];
		final ids = [for (id in selected.keys()) id];
		ids.sort(compareUtf8);
		for (id in ids)
			validateConstructorAcyclic(id, selected, dependencies, state, []);

		final trivial:Map<String, Bool> = [];
		final failing:Map<String, Bool> = [];
		final result:Array<CBodyConstructorInput> = [];
		for (id in ids) {
			final input = selected.get(id);
			if (input == null)
				throw new CBodyEmissionError('constructor analysis lost selected input `$id`');
			result.push({
				id: input.id,
				modulePath: input.modulePath,
				declarationPath: input.declarationPath,
				sourcePath: input.sourcePath,
				sourceOrder: input.sourceOrder,
				fieldType: input.fieldType,
				expression: input.expression,
				classReference: input.classReference,
				baseConstructorId: input.baseConstructorId,
				elided: constructorIsTrivial(id, selected, trivial),
				canFail: constructorCanFail(id, selected, dependencies, failing)
			});
		}
		return result;
	}

	function validateConstructorAcyclic(id:String, selected:Map<String, CBodyConstructorInput>, dependencies:Map<String, Array<CConstructorDependency>>,
			state:Map<String, Int>, stack:Array<String>):Void {
		final existing = state.get(id);
		if (existing == 2)
			return;
		state.set(id, 1);
		final nextStack = stack.concat([id]);
		final edges = dependencies.get(id);
		if (edges != null) {
			for (edge in edges) {
				if (!selected.exists(edge.targetId))
					continue;
				if (state.get(edge.targetId) == 1) {
					final cycleStart = nextStack.indexOf(edge.targetId);
					final cycle = (cycleStart == -1 ? nextStack : nextStack.slice(cycleStart)).concat([edge.targetId]);
					final owner = selected.get(id);
					if (owner == null)
						throw new CBodyEmissionError('constructor cycle analysis lost `$id`');
					unsupportedAt(edge.position, owner.sourcePath, 'TNew(constructor-cycle:${cycle.map(constructorPath).join(" -> ")})');
				}
				validateConstructorAcyclic(edge.targetId, selected, dependencies, state, nextStack);
			}
		}
		state.set(id, 2);
	}

	function constructorIsTrivial(id:String, selected:Map<String, CBodyConstructorInput>, memo:Map<String, Bool>):Bool {
		if (memo.exists(id))
			return memo.get(id) == true;
		final input = selected.get(id);
		if (input == null)
			return false;
		// Reserve before following a base edge; cycle validation has already run.
		memo.set(id, false);
		final value = trivialConstructorExpression(input.expression, input, selected, memo);
		memo.set(id, value);
		return value;
	}

	function trivialConstructorExpression(expression:TypedExpr, owner:CBodyConstructorInput, selected:Map<String, CBodyConstructorInput>,
			memo:Map<String, Bool>):Bool {
		return switch expression.expr {
			case TFunction(value): trivialConstructorExpression(value.expr, owner, selected, memo);
			case TBlock(expressions): expressions.length == 0 || expressions.length == 1 && trivialConstructorExpression(expressions[0], owner, selected, memo);
			case TCall(callee, arguments) if (isSuperCall(callee) && arguments.length == 0): owner.baseConstructorId != null && constructorIsTrivial(owner.baseConstructorId,
					selected, memo);
			case TReturn(null): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): trivialConstructorExpression(inner, owner, selected, memo);
			case _: false;
		};
	}

	function constructorCanFail(id:String, selected:Map<String, CBodyConstructorInput>, dependencies:Map<String, Array<CConstructorDependency>>,
			memo:Map<String, Bool>):Bool {
		if (memo.exists(id))
			return memo.get(id) == true;
		final input = selected.get(id);
		if (input == null)
			return false;
		memo.set(id, false);
		var value = expressionContainsThrow(input.expression);
		final edges = dependencies.get(id);
		if (!value && edges != null) {
			for (edge in edges) {
				if (constructorCanFail(edge.targetId, selected, dependencies, memo)) {
					value = true;
					break;
				}
			}
		}
		memo.set(id, value);
		return value;
	}

	static function expressionContainsThrow(expression:TypedExpr):Bool {
		switch expression.expr {
			case TThrow(_):
				return true;
			case _:
		}
		var found = false;
		TypedExprTools.iter(expression, child -> {
			if (!found && expressionContainsThrow(child))
				found = true;
		});
		return found;
	}

	static function constructorPath(id:String):String
		return StringTools.startsWith(id, "constructor.") ? id.substr("constructor.".length) : id;

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
		throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile), node,
			"reachable function and constructor graph discovery", source), position);
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
