package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.ds.IntMap;
import haxe.macro.Expr;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;

/**
	Summarize which parameter references leave one already-typed function body.

	Signature preparation asks the same question for several parameters. This
	request-local holder builds one answer on its first eligible query, rather
	than traversing the body once per parameter. It never follows types, forces
	declarations, emits diagnostics, or retains compiler objects across requests.
**/
class CBodyParameterBorrowSummary {
	final functionValue:TFunc;
	var summary:Null<{escaped:IntMap<Bool>, visits:Int}>;

	/** Retain the current typed body without examining its arguments or expressions. */
	public function new(functionValue:TFunc) {
		this.functionValue = functionValue;
		this.summary = null;
	}

	/** Actual structural visits; zero means no eligible parameter requested analysis. */
	public var visitedNodes(get, never):Int;

	function get_visitedNodes():Int {
		final current = summary;
		return current == null ? 0 : current.visits;
	}

	/**
		Answer the initial escape question, preserving exact compiler-variable IDs.

		Callers retain eligibility and diagnostic ordering. Explicit call arguments
		still belong to the separate forwarding refiner; constructor self-field
		retention and synchronous-callback rules are not part of this summary.
	**/
	public function canBorrow(compilerId:Int):Bool {
		final current = summary;
		if (current != null)
			return !current.escaped.exists(compilerId);
		final built = analyze();
		summary = built;
		return !built.escaped.exists(compilerId);
	}

	/**
		Visit every structural child once, carrying only two contextual facts.

		A direct escape follows result positions, not arbitrary references inside
		a condition, field receiver, or call. A nested closure instead captures
		any parameter it mentions. Keeping these contexts as Booleans avoids
		copying sets of referenced IDs at every parent node. The walk may inspect
		more nodes than an early-failing single-parameter query, so it must remain
		pure: no type resolution, source display, diagnostics, or user callbacks.
	**/
	function analyze():{escaped:IntMap<Bool>, visits:Int} {
		final parameters = new IntMap<Bool>();
		for (argument in functionValue.args)
			parameters.set(argument.v.id, true);
		final escaped = new IntMap<Bool>();
		var visits = 0;
		function visit(expression:TypedExpr, directEscape:Bool, insideClosure:Bool):Void {
			visits++;
			switch expression.expr {
				case TLocal(variable):
					if ((directEscape || insideClosure) && parameters.exists(variable.id))
						escaped.set(variable.id, true);
				case TVar(_, initializer):
					if (initializer != null)
						visit(initializer, true, insideClosure);
				case TBinop(OpAssign, left, right):
					visit(left, false, insideClosure);
					visit(right, true, insideClosure);
				case TReturn(value):
					if (value != null)
						visit(value, true, insideClosure);
				case TThrow(value):
					visit(value, true, insideClosure);
				case TNew(_, _, arguments):
					for (argument in arguments)
						visit(argument, true, insideClosure);
				case TFunction(nested):
					// TypedExprTools.iter visits only the body, not constant defaults.
					visit(nested.expr, false, true);
				case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _):
					visit(inner, directEscape, insideClosure);
				case TIf(condition, whenTrue, whenFalse):
					visit(condition, false, insideClosure);
					visit(whenTrue, directEscape, insideClosure);
					if (whenFalse != null)
						visit(whenFalse, directEscape, insideClosure);
				case TBlock(expressions):
					for (index in 0...expressions.length)
						visit(expressions[index], directEscape && index == expressions.length - 1, insideClosure);
				case TSwitch(subject, cases, defaultExpression):
					visit(subject, false, insideClosure);
					for (item in cases) {
						for (value in item.values)
							visit(value, false, insideClosure);
						visit(item.expr, directEscape, insideClosure);
					}
					if (defaultExpression != null)
						visit(defaultExpression, directEscape, insideClosure);
				case _:
					TypedExprTools.iter(expression, child -> visit(child, false, insideClosure));
			}
		}
		visit(functionValue.expr, false, false);
		return {escaped: escaped, visits: visits};
	}
}
#end
