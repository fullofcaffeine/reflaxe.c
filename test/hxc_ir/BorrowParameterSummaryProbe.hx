#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.lowering.CBodyParameterBorrowSummary;
#end

/** Compare authored escape expectations and bound one-pass work against the old per-parameter traversal. */
class BorrowParameterSummaryProbe {
	#if macro
	/** Verify the production summary against both authored expectations and the retained old algorithm. */
	public static function install():Void {
		Context.onAfterInitMacros(() -> {
			final definition = switch Context.getType("BorrowSummarySubject") {
				case TInst(reference, _): reference.get();
				case _: throw "expected ordinary subject class";
			};
			final fields = definition.statics.get();
			function body(name:String):TFunc {
				for (field in fields)
					if (field.name == name) {
						final expression = field.expr();
						if (expression == null)
							throw 'missing typed body $name';
						return switch expression.expr {
							case TFunction(value): value;
							case _: throw 'expected typed function $name';
						};
					}
				throw 'missing field $name';
			}
			final expected:Array<{name:String, safe:Array<Bool>}> = [
				{name: "condition", safe: [true, false]},
				{name: "blockLast", safe: [true, false]},
				{name: "switchValue", safe: [true, false]},
				{name: "bothArms", safe: [false, false]},
				{name: "alias", safe: [false]},
				{name: "store", safe: [false, true]},
				{name: "construct", safe: [false]},
				{name: "thrown", safe: [false]},
				{name: "captured", safe: [false]},
				{name: "deepCapture", safe: [false]},
				{name: "receiver", safe: [true]},
				{name: "forward", safe: [true]},
				{name: "wrapped", safe: [false]}
			];
			for (item in expected) {
				final fn = body(item.name);
				final old = new OldBorrowTraversal();
				final summary = new CBodyParameterBorrowSummary(fn);
				if (summary.visitedNodes != 0)
					throw "unused summary eagerly traversed its body";
				for (index in 0...item.safe.length)
					if (old.canBorrow(fn.expr, fn.args[index].v.id) != item.safe[index]
						|| summary.canBorrow(fn.args[index].v.id) != item.safe[index])
						throw 'authored verdict mismatch ${item.name} argument $index';
			}
			final counts:Array<{
				parameters:Int,
				nodes:Int,
				visits:Int,
				oldVisits:Int,
				bound:Int
			}> = [];
			var exceeded = false;
			for (parameterCount in [1, 8, 32]) {
				final fn = body('safe$parameterCount');
				if (fn.args.length != parameterCount)
					throw "typed parameter count changed";
				var nodes = 0;
				function count(expression:TypedExpr):Void {
					nodes++;
					TypedExprTools.iter(expression, count);
				}
				count(fn.expr);
				final old = new OldBorrowTraversal();
				final summary = new CBodyParameterBorrowSummary(fn);
				if (summary.visitedNodes != 0)
					throw "zero eligible queries must leave analysis unbuilt";
				for (argument in fn.args)
					if (!old.canBorrow(fn.expr, argument.v.id) || !summary.canBorrow(argument.v.id))
						throw "safe body unexpectedly retains a parameter";
				final bound = 2 * nodes + parameterCount;
				counts.push({
					parameters: parameterCount,
					nodes: nodes,
					visits: summary.visitedNodes,
					oldVisits: old.bodyVisits,
					bound: bound
				});
				if (summary.visitedNodes != nodes)
					throw "summary must visit every structural node exactly once";
				for (argument in fn.args)
					summary.canBorrow(argument.v.id);
				if (summary.visitedNodes != nodes)
					throw "repeated query rebuilt the summary";
				if (summary.visitedNodes > bound)
					exceeded = true;
			}
			Sys.println("BORROW_SUMMARY_COUNTS=" + haxe.Json.stringify(counts));
			if (exceeded)
				Context.fatalError("BORROW_SUMMARY_VISIT_BOUND: initial borrowing repeats the typed body", Context.currentPos());
			Sys.println("borrow parameter summary contracts passed");
		});
	}
	#end
}

#if macro
/** Frozen old production algorithm, counted only at its body visitor; independent of the proposed summary. */
private class OldBorrowTraversal {
	public var bodyVisits(default, null):Int = 0;

	public function new() {}

	public function canBorrow(body:TypedExpr, compilerId:Int):Bool {
		var safe = true;
		function visit(expression:TypedExpr):Void {
			bodyVisits++;
			if (!safe)
				return;
			switch expression.expr {
				case TVar(_, initializer) if (initializer != null && isDirectParameterValue(initializer, compilerId)):
					safe = false;
				case TBinop(OpAssign, _, right) if (isDirectParameterValue(right, compilerId)):
					safe = false;
				case TReturn(value) if (value != null && isDirectParameterValue(value, compilerId)):
					safe = false;
				case TThrow(value) if (isDirectParameterValue(value, compilerId)):
					safe = false;
				case TNew(_, _, arguments):
					for (argument in arguments)
						if (isDirectParameterValue(argument, compilerId)) {
							safe = false;
							break;
						}
				case TFunction(_) if (referencesParameter(expression, compilerId)):
					// A nested function can run after its enclosing call returns.
					safe = false;
				case _:
			}
			if (safe)
				TypedExprTools.iter(expression, visit);
		}
		visit(body);
		return safe;
	}

	public static function isDirectParameterValue(expression:TypedExpr, compilerId:Int):Bool {
		return switch expression.expr {
			case TLocal(variable): variable.id == compilerId;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): isDirectParameterValue(inner, compilerId);
			case TIf(_, whenTrue, whenFalse): isDirectParameterValue(whenTrue,
					compilerId) || whenFalse != null && isDirectParameterValue(whenFalse, compilerId);
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

	static function referencesParameter(expression:TypedExpr, compilerId:Int):Bool {
		var found = false;
		function visit(value:TypedExpr):Void {
			if (found)
				return;
			switch value.expr {
				case TLocal(variable) if (variable.id == compilerId):
					found = true;
				case _:
					TypedExprTools.iter(value, visit);
			}
		}
		visit(expression);
		return found;
	}
}
#end
