package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Type;
import haxe.macro.TypedExprTools;

/**
	Settle call-bounded borrowing without revisiting typed bodies after the first sweep.

	A borrowed parameter stays borrowed only if every exact helper receiving it also
	borrows. The first sweep preserves the caller's resolution order and early exits.
	Later revocations follow recorded dependencies instead of forcing types again.
	All inputs and intermediate state belong to this compiler request.
**/
/** The signature facts available before forwarding refinement starts. */
typedef CBodyBorrowParameter = {
	final compilerId:Int;
	final borrowed:Bool;
}

/** One prepared function; constructors supply target facts but are not refined callers. */
typedef CBodyBorrowFunction = {
	final id:String;
	final body:TypedExpr;
	final parameters:Array<CBodyBorrowParameter>;
	final refinable:Bool;
}

/** An exact known callee; instance signatures reserve parameter zero for the receiver. */
typedef CBodyBorrowTarget = {
	final functionId:String;
	final parameterOffset:Int;
}

/** Final flags and actual work counts, independent of wall time or host scheduling. */
typedef CBodyBorrowRefinementResult = {
	final borrowedByFunction:Map<String, Array<Bool>>;
	final bodyScans:Int;
	final visitedNodes:Int;
	final resolvedTargets:Int;
	final propagatedEdges:Int;
}

/**
	A parameter has request-local identity so reverse edges share its settled flag.
	Only false propagation mutates it; no typed expression escapes into a cache.
**/
private class BorrowParameterState {
	public final initiallyBorrowed:Bool;
	public var borrowed:Bool;
	public final dependents:Array<BorrowParameterState> = [];

	public function new(borrowed:Bool) {
		this.initiallyBorrowed = borrowed;
		this.borrowed = borrowed;
	}
}

/**
	Preserve one synchronous scan, then propagate owning requirements to callers.

	The resolver is invoked only where the old first sweep invoked it, including
	repeated arguments and early failures. A successful scan has visited every
	forwarding use; its stable target IDs/indexes are sufficient for later rounds.
	Safe cycles stay borrowed. A cycle reaching an owning target becomes owning.
	The caller must keep target membership, parameter order and dispatch facts fixed
	for this call; only borrowed flags are being refined.
**/
function refineBorrowing(functions:Array<CBodyBorrowFunction>, resolve:TypedExpr->Null<CBodyBorrowTarget>):CBodyBorrowRefinementResult {
	final states:Map<String, Array<BorrowParameterState>> = [];
	for (fn in functions)
		states.set(fn.id, [for (parameter in fn.parameters) new BorrowParameterState(parameter.borrowed)]);
	final revoked:Array<BorrowParameterState> = [];
	var bodyScans = 0;
	var visitedNodes = 0;
	var resolvedTargets = 0;
	for (fn in functions) {
		if (!fn.refinable)
			continue;
		final parameters = states.get(fn.id);
		if (parameters == null)
			throw "borrow refinement lost a prepared function";
		for (index in 0...fn.parameters.length) {
			final parameter = fn.parameters[index];
			if (!parameter.borrowed)
				continue;
			bodyScans++;
			final dependencies:Array<BorrowParameterState> = [];
			var safe = true;
			function visit(expression:TypedExpr):Void {
				if (!safe)
					return;
				visitedNodes++;
				switch expression.expr {
					case TCall(callee, arguments):
						for (argumentIndex in 0...arguments.length)
							if (isDirectParameterValue(arguments[argumentIndex], parameter.compilerId)) {
								resolvedTargets++;
								final target = resolve(callee);
								final targetParameters = target == null ? null : states.get(target.functionId);
								final targetIndex = target == null ? -1 : argumentIndex + target.parameterOffset;
								if (targetParameters == null
									|| targetIndex < 0
									|| targetIndex >= targetParameters.length
									|| !targetParameters[targetIndex].initiallyBorrowed) {
									safe = false;
									return;
								}
								dependencies.push(targetParameters[targetIndex]);
							}
					case _:
				}
				if (safe)
					TypedExprTools.iter(expression, visit);
			}
			visit(fn.body);
			final state = parameters[index];
			if (safe) {
				for (dependency in dependencies)
					dependency.dependents.push(state);
			} else
				revoked.push(state);
		}
	}
	// Publish the first sweep together: its earlier failures must not shorten
	// later scans and thereby change which lazy declarations are first forced.
	for (state in revoked)
		state.borrowed = false;
	var cursor = 0;
	var propagatedEdges = 0;
	while (cursor < revoked.length) {
		final state = revoked[cursor++];
		for (dependent in state.dependents) {
			propagatedEdges++;
			if (dependent.borrowed) {
				dependent.borrowed = false;
				revoked.push(dependent);
			}
		}
	}
	final result:Map<String, Array<Bool>> = [];
	for (fn in functions) {
		final parameters = states.get(fn.id);
		if (parameters == null)
			throw "borrow refinement lost a settled function";
		result.set(fn.id, [for (parameter in parameters) parameter.borrowed]);
	}
	return {
		borrowedByFunction: result,
		bodyScans: bodyScans,
		visitedNodes: visitedNodes,
		resolvedTargets: resolvedTargets,
		propagatedEdges: propagatedEdges
	};
}

/** True only when an expression returns this parameter itself, rather than inspecting it. */
function isDirectParameterValue(expression:TypedExpr, compilerId:Int):Bool {
	return switch expression.expr {
		case TLocal(variable): variable.id == compilerId;
		case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isDirectParameterValue(inner, compilerId);
		case TIf(_, whenTrue, whenFalse): isDirectParameterValue(whenTrue, compilerId) || whenFalse != null && isDirectParameterValue(whenFalse, compilerId);
		case TBlock(expressions): expressions.length > 0 && isDirectParameterValue(expressions[expressions.length - 1], compilerId);
		case TSwitch(_, cases, defaultExpression):
			var direct = defaultExpression != null && isDirectParameterValue(defaultExpression, compilerId);
			for (item in cases)
				if (isDirectParameterValue(item.expr, compilerId)) {
					direct = true;
					break;
				}
			direct;
		case _: false;
	};
}
#end
