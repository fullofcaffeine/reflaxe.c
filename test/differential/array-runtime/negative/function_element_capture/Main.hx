/**
	Proves that an Array cannot erase one captured function environment.

	The stored value needs `offset` after the literal is created. A direct C
	function pointer cannot carry that state, so lowering must fail before output.
**/
final class Main {
	static function main():Void {
		final offset = 2;
		final operations:Array<Int->Int> = [value -> value + offset];
		while (operations[0](3) != 5) {}
	}
}
