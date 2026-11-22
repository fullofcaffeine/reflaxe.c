package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Expr.Binop;
import haxe.macro.Expr.Unop;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;

/**
	Owns the source-level definite-assignment proof used before body lowering.

	One analysis instance follows one compiler local through typed Haxe control
	flow. It admits only paths whose reachable reads follow a write, while the
	independent HxcIR validator repeats that property on the finished graph.
**/
/** One control-flow result for a local's two-state assignment proof. */
private typedef CBodyAssignmentFlow = {
	final normal:Int;
	final breaks:Int;
	final continues:Int;
}

/** The typed switch-arm shape needed by the assignment proof. */
typedef CBodyDefiniteAssignmentSwitchArm = {
	final values:Array<TypedExpr>;
	final expr:TypedExpr;
}

/**
	Proves that an empty primitive local is assigned before every reachable read.

	The analysis tracks both possible states of one local through source-ordered
	expressions. Branches merge those states, loops iterate them to a fixed point,
	and terminating paths do not contaminate later reads. Unsupported control-flow
	that mentions the local fails closed. HxcIR validation repeats the proof on the
	finished graph, so this source pass is the user-facing admission boundary, not
	the final safety authority.
**/
class CBodyDefiniteAssignment {
	static inline final UNASSIGNED = 1;
	static inline final ASSIGNED = 2;

	final compilerId:Int;
	final exhaustiveSwitch:(TypedExpr, Array<CBodyDefiniteAssignmentSwitchArm>) -> Bool;
	var sawAssignment = false;
	var sawRead = false;
	var unsafeRead = false;

	/** Create a proof for one compiler local and the owner's closed-switch rule. */
	public function new(compilerId:Int, exhaustiveSwitch:(TypedExpr, Array<CBodyDefiniteAssignmentSwitchArm>) -> Bool) {
		this.compilerId = compilerId;
		this.exhaustiveSwitch = exhaustiveSwitch;
	}

	/** Return true only when the selected sequence contains a safe store and read. */
	public function proves(expressions:Array<TypedExpr>, start:Int, endExclusive:Int):Bool {
		if (start < 0 || endExclusive < start || endExclusive > expressions.length)
			return false;
		analyzeSequence(expressions, start, endExclusive, UNASSIGNED);
		return sawAssignment && sawRead && !unsafeRead;
	}

	/** Follow one source-ordered slice while preserving normal, break, and continue states. */
	function analyzeSequence(expressions:Array<TypedExpr>, start:Int, endExclusive:Int, incoming:Int):CBodyAssignmentFlow {
		var normal = incoming;
		var breaks = 0;
		var continues = 0;
		var index = start;
		while (index < endExclusive && normal != 0) {
			final next = analyze(expressions[index], normal);
			normal = next.normal;
			breaks |= next.breaks;
			continues |= next.continues;
			index++;
		}
		return flow(normal, breaks, continues);
	}

	/** Transfer one typed expression without evaluating unreachable successors. */
	function analyze(expression:TypedExpr, incoming:Int):CBodyAssignmentFlow {
		if (incoming == 0)
			return flow(0);
		return switch expression.expr {
			case TConst(_) | TTypeExpr(_) | TIdent(_):
				flow(incoming);
			case TLocal(variable):
				if (variable.id == compilerId) {
					sawRead = true;
					if ((incoming & UNASSIGNED) != 0)
						unsafeRead = true;
				}
				flow(incoming);
			case TArray(collection, index):
				chain(collection, index, incoming);
			case TBinop(OpAssign, left, right) if (isTargetLocal(left)):
				final evaluated = analyze(right, incoming);
				if (evaluated.normal != 0)
					sawAssignment = true;
				flow(evaluated.normal == 0 ? 0 : ASSIGNED, evaluated.breaks, evaluated.continues);
			case TBinop(OpAssign, left, right):
				analyzeNonTargetAssignment(left, right, incoming);
			case TBinop(OpAssignOp(_), left, right) if (isTargetLocal(left)):
				final loaded = analyze(left, incoming);
				final evaluated = analyze(right, loaded.normal);
				if (evaluated.normal != 0)
					sawAssignment = true;
				flow(evaluated.normal == 0 ? 0 : ASSIGNED, loaded.breaks | evaluated.breaks, loaded.continues | evaluated.continues);
			case TBinop(OpBoolAnd, left, right) | TBinop(OpBoolOr, left, right):
				final leftFlow = analyze(left, incoming);
				final rightFlow = analyze(right, leftFlow.normal);
				flow(leftFlow.normal | rightFlow.normal, leftFlow.breaks | rightFlow.breaks, leftFlow.continues | rightFlow.continues);
			case TBinop(_, left, right):
				chain(left, right, incoming);
			case TField(receiver, _):
				analyze(receiver, incoming);
			case TUnop(operation, _, operand):
				final evaluated = analyze(operand, incoming);
				if (isTargetLocal(operand) && switch operation {
						case OpIncrement | OpDecrement: true;
						case _: false;
					}) {
					sawAssignment = true;
					flow(evaluated.normal == 0 ? 0 : ASSIGNED, evaluated.breaks, evaluated.continues);
					} else {
					evaluated;
				}
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _):
				analyze(inner, incoming);
			case TArrayDecl(values):
				analyzeExpressions(values, incoming);
			case TCall(callee, arguments):
				final calleeFlow = analyze(callee, incoming);
				mergeSequential(calleeFlow, analyzeExpressions(arguments, calleeFlow.normal));
			case TNew(_, _, arguments):
				analyzeExpressions(arguments, incoming);
			case TObjectDecl(fields):
				analyzeExpressions([for (field in fields) field.expr], incoming);
			case TVar(_, initializer):
				initializer == null ? flow(incoming) : analyze(initializer, incoming);
			case TFunction(_):
				conservativeBoundary(expression, incoming);
			case TBlock(expressions):
				analyzeSequence(expressions, 0, expressions.length, incoming);
			case TIf(condition, whenTrue, whenFalse):
				analyzeConditional(condition, whenTrue, whenFalse, incoming);
			case TWhile(condition, body, normalWhile):
				normalWhile ? analyzePreTestLoop(condition, body, incoming) : analyzePostTestLoop(condition, body, incoming);
			case TSwitch(subject, cases, defaultExpression):
				analyzeSwitch(subject, cases, defaultExpression, incoming);
			case TTry(_, _) | TFor(_, _, _):
				conservativeBoundary(expression, incoming);
			case TReturn(value):
				final evaluated = value == null ? flow(incoming) : analyze(value, incoming);
				flow(0, evaluated.breaks, evaluated.continues);
			case TThrow(value):
				final evaluated = analyze(value, incoming);
				flow(0, evaluated.breaks, evaluated.continues);
			case TBreak:
				flow(0, incoming, 0);
			case TContinue:
				flow(0, 0, incoming);
			case TEnumParameter(value, _, _) | TEnumIndex(value):
				analyze(value, incoming);
		};
	}

	/** Preserve target reads in complex places while treating another local's left side as a place. */
	function analyzeNonTargetAssignment(left:TypedExpr, right:TypedExpr, incoming:Int):CBodyAssignmentFlow {
		final leftFlow = isAnyLocal(left) ? flow(incoming) : analyze(left, incoming);
		return mergeSequential(leftFlow, analyze(right, leftFlow.normal));
	}

	/** Merge both continuing branch outcomes after evaluating their shared condition once. */
	function analyzeConditional(condition:TypedExpr, whenTrue:TypedExpr, whenFalse:Null<TypedExpr>, incoming:Int):CBodyAssignmentFlow {
		final conditionFlow = analyze(condition, incoming);
		final trueFlow = analyze(whenTrue, conditionFlow.normal);
		final falseFlow = whenFalse == null ? flow(conditionFlow.normal) : analyze(whenFalse, conditionFlow.normal);
		return flow(trueFlow.normal | falseFlow.normal, conditionFlow.breaks | trueFlow.breaks | falseFlow.breaks,
			conditionFlow.continues | trueFlow.continues | falseFlow.continues);
	}

	/** Merge every reachable switch arm and retain an open switch's unmatched path. */
	function analyzeSwitch(subject:TypedExpr, cases:Array<CBodyDefiniteAssignmentSwitchArm>, defaultExpression:Null<TypedExpr>,
			incoming:Int):CBodyAssignmentFlow {
		final subjectFlow = analyze(subject, incoming);
		var normal = defaultExpression == null && !exhaustiveSwitch(subject, cases) ? subjectFlow.normal : 0;
		var breaks = subjectFlow.breaks;
		var continues = subjectFlow.continues;
		for (item in cases) {
			// Case labels are patterns rather than ordinary branch bodies. A
			// reference to the empty local there has no preceding arm assignment,
			// so reject it conservatively instead of treating it as inert syntax.
			for (value in item.values)
				if (referencesTarget(value))
					unsafeRead = true;
			final arm = analyze(item.expr, subjectFlow.normal);
			normal |= arm.normal;
			breaks |= arm.breaks;
			continues |= arm.continues;
		}
		if (defaultExpression != null) {
			final fallback = analyze(defaultExpression, subjectFlow.normal);
			normal |= fallback.normal;
			breaks |= fallback.breaks;
			continues |= fallback.continues;
		}
		return flow(normal, breaks, continues);
	}

	/** Iterate a pre-test loop to the two-state fixed point and expose zero-iteration exits. */
	function analyzePreTestLoop(condition:TypedExpr, body:TypedExpr, incoming:Int):CBodyAssignmentFlow {
		var head = incoming;
		var conditionFlow = flow(head);
		var bodyFlow = flow(0);
		var changed = true;
		while (changed) {
			conditionFlow = analyze(condition, head);
			bodyFlow = analyze(body, conditionFlow.normal);
			final next = incoming | bodyFlow.normal | bodyFlow.continues;
			changed = next != head;
			head = next;
		}
		return flow(conditionFlow.normal | bodyFlow.breaks, conditionFlow.breaks, conditionFlow.continues);
	}

	/** Iterate a post-test loop after its mandatory first body execution. */
	function analyzePostTestLoop(condition:TypedExpr, body:TypedExpr, incoming:Int):CBodyAssignmentFlow {
		var bodyInput = incoming;
		var bodyFlow = flow(bodyInput);
		var conditionFlow = flow(0);
		var changed = true;
		while (changed) {
			bodyFlow = analyze(body, bodyInput);
			conditionFlow = analyze(condition, bodyFlow.normal | bodyFlow.continues);
			final next = incoming | conditionFlow.normal;
			changed = next != bodyInput;
			bodyInput = next;
		}
		return flow(bodyFlow.breaks | conditionFlow.normal, conditionFlow.breaks, conditionFlow.continues);
	}

	function analyzeExpressions(expressions:Array<TypedExpr>, incoming:Int):CBodyAssignmentFlow
		return analyzeSequence(expressions, 0, expressions.length, incoming);

	function chain(left:TypedExpr, right:TypedExpr, incoming:Int):CBodyAssignmentFlow {
		final leftFlow = analyze(left, incoming);
		return mergeSequential(leftFlow, analyze(right, leftFlow.normal));
	}

	static function mergeSequential(first:CBodyAssignmentFlow, second:CBodyAssignmentFlow):CBodyAssignmentFlow
		return flow(second.normal, first.breaks | second.breaks, first.continues | second.continues);

	/** Reject an unsupported construct only when it can observe the selected local. */
	function conservativeBoundary(expression:TypedExpr, incoming:Int):CBodyAssignmentFlow {
		if (referencesTarget(expression))
			unsafeRead = true;
		return flow(incoming);
	}

	/** Find the exact compiler-local identity without confusing a shadowed name. */
	function referencesTarget(expression:TypedExpr):Bool {
		var found = false;
		function visit(current:TypedExpr):Void {
			if (found)
				return;
			switch current.expr {
				case TLocal(variable) if (variable.id == compilerId):
					found = true;
				case _:
					TypedExprTools.iter(current, visit);
			}
		}
		visit(expression);
		return found;
	}

	function isTargetLocal(expression:TypedExpr):Bool
		return switch expression.expr {
			case TLocal(variable): variable.id == compilerId;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isTargetLocal(inner);
			case _: false;
		};

	static function isAnyLocal(expression:TypedExpr):Bool
		return switch expression.expr {
			case TLocal(_): true;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isAnyLocal(inner);
			case _: false;
		};

	static inline function flow(normal:Int, breaks:Int = 0, continues:Int = 0):CBodyAssignmentFlow
		return {normal: normal, breaks: breaks, continues: continues};
}
#end
