/**
	Proves that bounded `Array<String>.toString` composition does not silently
	format other element types. Each new element family needs an exact
	`Std.string` contract before haxe.c can admit it without Dynamic boxing.
**/
final class Main {
	static function main():Void {
		final values = [1, 2];
		final text = values.toString();
		while (text != "[1,2]") {}
	}
}
