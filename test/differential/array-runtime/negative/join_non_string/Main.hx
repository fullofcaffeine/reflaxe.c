/**
	Proves that the bounded `Array<String>.join` lowering does not silently
	format other element types. Converting `Int`, class, enum, or record values
	needs a separately owned Haxe formatting contract before haxe.c may admit it.
**/
final class Main {
	static function main():Void {
		final values = [1, 2];
		final joined = values.join(",");
		while (joined != "1,2") {}
	}
}
