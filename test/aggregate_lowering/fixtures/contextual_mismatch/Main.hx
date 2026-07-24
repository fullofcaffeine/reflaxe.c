/**
	Proves that Haxe rejects an object literal missing a declared record field
	before haxe.c can construct a contextual aggregate.
**/
private typedef ParsedPoint = {
	final x:Int;
	final y:Int;
}

/** Deliberately malformed source used by the aggregate negative test. */
class Main {
	/** Returns an incomplete record so the Haxe typer must reject this source. */
	static function incomplete():ParsedPoint
		return {x: 1};

	/** Keeps the malformed function reachable by the compiler front end. */
	static function main():Void {
		final value = incomplete();
		while (value.x == 1) {}
	}
}
