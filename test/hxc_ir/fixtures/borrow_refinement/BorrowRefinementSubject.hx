/** A concrete reference gives forwarding tests ordinary typed parameter identities. */
class RefinementValue {
	public var value:Int = 42;

	public function new() {}
}

/** Exact instance forwarding reserves the receiver before its explicit argument. */
final class RefinementReceiver {
	public function new() {}

	public function forward(a:RefinementValue):Void
		BorrowRefinementSubject.sink(a);
}

/** Typed call graphs; only the scalar main runs, so recursive contract probes do not recurse at runtime. */
class BorrowRefinementSubject {
	static var retained:Null<RefinementValue>;

	public static function sink(a:RefinementValue):Void
		retained = a;

	public static function safe(a:RefinementValue):Int
		return a.value;

	public static function unknown(a:RefinementValue):Void {}

	public static function sentinel(a:RefinementValue):Void {}

	public static function early(a:RefinementValue):Void {
		unknown(a);
		sentinel(a);
	}

	public static function cycleA(a:RefinementValue):Void
		cycleB(a);

	public static function cycleB(a:RefinementValue):Void
		cycleA(a);

	public static function badCycleA(a:RefinementValue):Void
		badCycleB(a);

	public static function badCycleB(a:RefinementValue):Void {
		badCycleA(a);
		sink(a);
	}

	public static function pair(a:RefinementValue, b:RefinementValue):Void
		sink(b);

	public static function duplicate(a:RefinementValue):Void
		pair(a, a);

	public static function wrapper(a:RefinementValue, b:RefinementValue, choose:Bool):Void
		sink(choose ? a : b);

	public static function condition(a:RefinementValue, b:RefinementValue):Void
		sink(a.value > 0 ? b : b);

	public static function blockLast(a:RefinementValue, b:RefinementValue):Void
		sink({a.value; b;});

	public static function generic<T>(a:T):Void {}

	public static function genericCaller(a:RefinementValue):Void
		generic(a);

	public static function instanceCaller(a:RefinementValue, receiver:RefinementReceiver):Void
		receiver.forward(a);

	public static function chain0(a:RefinementValue):Void
		chain1(a);

	public static function chain1(a:RefinementValue):Void
		chain2(a);

	public static function chain2(a:RefinementValue):Void
		chain3(a);

	public static function chain3(a:RefinementValue):Void
		chain4(a);

	public static function chain4(a:RefinementValue):Void
		chain5(a);

	public static function chain5(a:RefinementValue):Void
		chain6(a);

	public static function chain6(a:RefinementValue):Void
		chain7(a);

	public static function chain7(a:RefinementValue):Void
		chain8(a);

	public static function chain8(a:RefinementValue):Void
		chain9(a);

	public static function chain9(a:RefinementValue):Void
		chain10(a);

	public static function chain10(a:RefinementValue):Void
		chain11(a);

	public static function chain11(a:RefinementValue):Void
		chain12(a);

	public static function chain12(a:RefinementValue):Void
		chain13(a);

	public static function chain13(a:RefinementValue):Void
		chain14(a);

	public static function chain14(a:RefinementValue):Void
		chain15(a);

	public static function chain15(a:RefinementValue):Void
		chain16(a);

	public static function chain16(a:RefinementValue):Void
		chain17(a);

	public static function chain17(a:RefinementValue):Void
		chain18(a);

	public static function chain18(a:RefinementValue):Void
		chain19(a);

	public static function chain19(a:RefinementValue):Void
		chain20(a);

	public static function chain20(a:RefinementValue):Void
		chain21(a);

	public static function chain21(a:RefinementValue):Void
		chain22(a);

	public static function chain22(a:RefinementValue):Void
		chain23(a);

	public static function chain23(a:RefinementValue):Void
		chain24(a);

	public static function chain24(a:RefinementValue):Void
		chain25(a);

	public static function chain25(a:RefinementValue):Void
		chain26(a);

	public static function chain26(a:RefinementValue):Void
		chain27(a);

	public static function chain27(a:RefinementValue):Void
		chain28(a);

	public static function chain28(a:RefinementValue):Void
		chain29(a);

	public static function chain29(a:RefinementValue):Void
		chain30(a);

	public static function chain30(a:RefinementValue):Void
		chain31(a);

	public static function chain31(a:RefinementValue):Void
		sink(a);

	public static function main():Void {}
}
