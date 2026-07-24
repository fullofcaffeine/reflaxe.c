/**
	Proves that an unsupported String method stays with the String intrinsic owner.

	Reachability must not misclassify this call as ordinary virtual dispatch, and
	lowering must reject it before any plausible C project is written.
**/
class Main {
	/** Keep the unsupported receiver and result fully ordinary Haxe values. */
	static function main():Void {
		final value = "not yet".toUpperCase();
		Sys.println(value);
	}
}
