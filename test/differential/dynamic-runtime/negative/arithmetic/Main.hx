/**
	Protect runtime-typed arithmetic from the proven signed-Int division path.

	The numerator has no static primitive type, so haxe.c must reject the division
	without emitting a direct C operator. A future Dynamic arithmetic operation can
	replace this negative only when it preserves the runtime carrier's semantics.
**/
class Main {
	/** Exercise the natural source expression at the unsupported boundary. */
	static function main():Void {
		final numerator:Dynamic = 47;
		final result:Int = Std.int(numerator / 8);
	}
}
