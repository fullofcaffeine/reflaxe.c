/**
 * Module-owned operations need no wrapper class.
 *
 * `base` also proves that a module value participates in ordinary Haxe static
 * initialization before the compiler calls this module's `main` function.
 */
final base:Int = 7;

private inline function increment(value:Int):Int
	return value + 1;

function doubled(value:Int):Int
	return value * 2;

private function answer():Int
	return increment(doubled(base));

function main():Void {
	// A broken module global, call, or inline expansion would keep the native
	// fixture in this loop until its bounded test timeout reports the defect.
	while (answer() != 15) {}
}
