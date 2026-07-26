/**
	Proves that Array.sort does not erase a captured Haxe value.

	The comparator needs `direction` from its enclosing function. A plain C
	function pointer cannot carry that environment, so haxe.c must reject this
	until the separately owned closure representation is available.
**/
final class Main {
	static function main():Void {
		final direction = -1;
		final values = [1, 2, 3];
		values.sort((left, right) -> direction * (left - right));
	}
}
