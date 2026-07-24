/**
	Proves that runtime String parameters do not silently enable unrelated String work.

	`Bytes.ofString` can copy any already-admitted immutable UTF-8 view. Producing
	a new String with `toUpperCase` has separate Unicode, allocation, and ownership
	semantics, so that neighboring operation must remain an exact fail-closed edge.
**/
final class Main {
	static function convert(value:String):haxe.io.Bytes
		return haxe.io.Bytes.ofString(value.toUpperCase());

	static function main():Void {
		final bytes = convert("Haxe");
		while (bytes.length != 4) {}
	}
}
