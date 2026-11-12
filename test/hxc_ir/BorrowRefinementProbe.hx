#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import haxe.macro.TypedExprTools;
import reflaxe.c.lowering.CBodyBorrowRefinement.CBodyBorrowFunction;
import reflaxe.c.lowering.CBodyBorrowRefinement.CBodyBorrowParameter;
import reflaxe.c.lowering.CBodyBorrowRefinement.CBodyBorrowTarget;
import reflaxe.c.lowering.CBodyBorrowRefinement.CBodyBorrowRefinementResult;
import reflaxe.c.lowering.CBodyBorrowRefinement.refineBorrowing;
#end

/** Check final borrowing decisions independently and reject repeated body scans after the first sweep. */
class BorrowRefinementProbe {
	#if macro
	/** Use typed authored bodies, a retained old algorithm, and explicit expected flags. */
	public static function install():Void {
		Context.onAfterInitMacros(() -> {
			final subject = switch Context.getType("BorrowRefinementSubject") {
				case TInst(reference, _): reference.get();
				case _: throw "expected typed subject";
			};
			final receiver = switch Context.getType("BorrowRefinementSubject.RefinementReceiver") {
				case TInst(reference, _): reference.get();
				case _: throw "expected typed receiver";
			};
			function input(name:String, borrowed:Array<Bool>, refinable:Bool = true, instance:Bool = false):CBodyBorrowFunction {
				final fields = instance ? receiver.fields.get() : subject.statics.get();
				for (field in fields)
					if (field.name == name) {
						final expression = field.expr();
						if (expression == null)
							throw 'missing body $name';
						final fn = switch expression.expr {
							case TFunction(value): value;
							case _: throw 'missing function $name';
						};
						final parameters:Array<CBodyBorrowParameter> = [];
						if (instance)
							parameters.push({compilerId: -1, borrowed: false});
						if (fn.args.length != borrowed.length)
							throw 'argument count $name';
						for (i in 0...fn.args.length)
							parameters.push({compilerId: fn.args[i].v.id, borrowed: borrowed[i]});
						return {
							id: name,
							body: fn.expr,
							parameters: parameters,
							refinable: refinable
						};
					}
				throw 'missing field $name';
			}
			function resolver(log:Array<String>):TypedExpr->Null<CBodyBorrowTarget> {
				function resolve(callee:TypedExpr):Null<CBodyBorrowTarget> {
					return switch callee.expr {
						case TField(_, FStatic(_, field)): final name = field.get()
								.name; log.push(name); if (name == "sentinel") throw "resolved after a failing forwarding use"; name == "unknown" || field.get()
								.params.length > 0 ? null : {functionId: name, parameterOffset: 0};
						case TField(_, FInstance(_, _, field)):
							log.push(field.get().name);
							{functionId: field.get().name, parameterOffset: 1};
						case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): resolve(inner);
						case _: null;
					};
				}
				return resolve;
			}
			function check(functions:Array<CBodyBorrowFunction>, expected:Map<String, Array<Bool>>):CBodyBorrowRefinementResult {
				final oldLog:Array<String> = [];
				final newLog:Array<String> = [];
				final old = new OldBorrowRefinement(functions, resolver(oldLog));
				final current = refineBorrowing(functions, resolver(newLog));
				if (newLog.join("|") != old.firstSweepLog.join("|"))
					throw "first sweep resolution order changed";
				if (current.visitedNodes != old.firstSweepVisits || current.resolvedTargets != newLog.length)
					throw "typed body traversal repeated after first sweep";
				for (fn in functions) {
					final wanted = expected.get(fn.id);
					final before = old.flags.get(fn.id);
					final after = current.borrowedByFunction.get(fn.id);
					if (wanted == null
						|| before == null
						|| after == null
						|| before.join(",") != wanted.join(",")
						|| after.join(",") != wanted.join(","))
						throw 'borrow verdict mismatch ${fn.id}';
				}
				return current;
			}
			final cases = [
				input("sink", [false]),
				input("safe", [true]),
				input("cycleA", [true]),
				input("cycleB", [true]),
				input("badCycleA", [true]),
				input("badCycleB", [true]),
				input("pair", [true, true]),
				input("duplicate", [true]),
				input("wrapper", [true, true, false]),
				input("condition", [true, true]),
				input("blockLast", [true, true]),
				input("genericCaller", [true]),
				input("early", [true]),
				input("forward", [true], true, true),
				input("instanceCaller", [true, false])
			];
			check(cases, [
				"sink" => [false],
				"safe" => [true],
				"cycleA" => [true],
				"cycleB" => [true],
				"badCycleA" => [false],
				"badCycleB" => [false],
				"pair" => [true, false],
				"duplicate" => [false],
				"wrapper" => [false, false, false],
				"condition" => [true, false],
				"blockLast" => [true, false],
				"genericCaller" => [false],
				"early" => [false],
				"forward" => [false, false],
				"instanceCaller" => [false, false]
			]);
			final untouched = check([input("early", [false]), input("cycleA", [true], false)], ["early" => [false], "cycleA" => [true]]);
			if (untouched.bodyScans != 0 || untouched.resolvedTargets != 0)
				throw "owning or non-refined body scanned";
			final results:Array<String> = [];
			for (length in [1, 8, 32]) {
				final functions:Array<CBodyBorrowFunction> = [input("sink", [false]), input("safe", [true])];
				final expected:Map<String, Array<Bool>> = ["sink" => [false], "safe" => [true]];
				for (i in (32 - length)...32) {
					functions.push(input('chain$i', [true]));
					expected.set('chain$i', [false]);
				}
				for (reverse in [false, true]) {
					if (reverse)
						functions.reverse();
					final current = check(functions, expected);
					final old = new OldBorrowRefinement(functions, resolver([]));
					// The safe unrelated body is scanned once per old round, including the final unchanged round.
					final expectedOld = Std.int(length * (length + 1) / 2) + length + 1;
					final observedScans = #if borrow_refinement_old_control old.bodyScans #else current.bodyScans #end;
					if (old.bodyScans != expectedOld || observedScans != length + 1 || current.propagatedEdges != length - 1)
						throw "BORROW_REFINEMENT_VISIT_BOUND";
					results.push('$length:$reverse:${old.bodyScans}:${current.bodyScans}');
				}
			}
			Sys.println("borrow-refinement: OK " + results.join(" "));
		});
	}
	#end
}

#if macro
/** Retained synchronous algorithm: deliberately repeats body traversal until no parameter changes. */
private class OldBorrowRefinement {
	public final flags:Map<String, Array<Bool>> = [];
	public final firstSweepLog:Array<String> = [];
	public var bodyScans(default, null):Int = 0;
	public var firstSweepVisits(default, null):Int = 0;

	public function new(functions:Array<CBodyBorrowFunction>, resolve:TypedExpr->Null<CBodyBorrowTarget>) {
		for (fn in functions)
			flags.set(fn.id, [for (p in fn.parameters) p.borrowed]);
		var changed = true;
		var first = true;
		var remaining = 1;
		for (fn in functions)
			remaining += fn.parameters.length;
		while (changed) {
			if (remaining-- <= 0)
				throw "old refinement did not converge";
			changed = false;
			final replacements:Map<String, Array<Bool>> = [];
			for (fn in functions) {
				if (!fn.refinable)
					continue;
				final before = flags.get(fn.id);
				if (before == null)
					throw "missing old function";
				final after:Array<Bool> = [];
				for (i in 0...fn.parameters.length) {
					var safe = before[i];
					if (safe) {
						bodyScans++;
						final id = fn.parameters[i].compilerId;
						function visit(expression:TypedExpr):Void {
							if (!safe)
								return;
							if (first)
								firstSweepVisits++;
							switch expression.expr {
								case TCall(callee, arguments):
									for (index in 0...arguments.length)
										if (oldDirect(arguments[index], id)) {
											if (first)
												firstSweepLog.push(calleeName(callee));
											final forwarding = resolve(callee);
											final target = forwarding == null ? null : flags.get(forwarding.functionId);
											final offset = forwarding == null ? -1 : index + forwarding.parameterOffset;
											if (target == null || offset < 0 || offset >= target.length || !target[offset]) {
												safe = false;
												return;
											}
										}
								case _:
							}
							if (safe)
								TypedExprTools.iter(expression, visit);
						}
						visit(fn.body);
					}
					after.push(safe);
					if (safe != before[i])
						changed = true;
				}
				replacements.set(fn.id, after);
			}
			for (id => values in replacements)
				flags.set(id, values);
			first = false;
		}
	}

	static function calleeName(expression:TypedExpr):String
		return switch expression.expr {
			case TField(_, FStatic(_, field)) | TField(_, FInstance(_, _, field)): field.get().name;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): calleeName(inner);
			case _: "unknown";
		};

	/** Independent copy of the original direct-value predicate, not a call to the candidate. */
	static function oldDirect(expression:TypedExpr, compilerId:Int):Bool
		return switch expression.expr {
			case TLocal(variable): variable.id == compilerId;
			case TParenthesis(inner) | TMeta(_, inner) | TCast(inner, _): oldDirect(inner, compilerId);
			case TIf(_, whenTrue, whenFalse): oldDirect(whenTrue, compilerId) || whenFalse != null && oldDirect(whenFalse, compilerId);
			case TBlock(expressions): expressions.length > 0 && oldDirect(expressions[expressions.length - 1], compilerId);
			case TSwitch(_, cases, defaultExpression):
				var direct = defaultExpression != null && oldDirect(defaultExpression, compilerId);
				for (item in cases)
					if (oldDirect(item.expr, compilerId)) {
						direct = true;
						break;
					}
				direct;
			case _: false;
		};
}
#end
