/** Proves that distinct Array function signatures cannot share one specialization. */
final class Main {
	static function add(left:Int, right:Int):Int
		return left + right;

	static function main():Void {
		final operations:Array<Int->Int> = [add];
		while (operations[0](3) != 3) {}
	}
}
