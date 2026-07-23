/** Managed element copies need a path-scoped owner inside a branch. */
final class Main {
	static var gate:Bool = false;

	static function nestedCopy(rows:Array<Array<Int>>, enabled:Bool):Int {
		if (enabled && rows.length > 0) {
			final selected = rows[0];
			return selected.length;
		}
		return 0;
	}

	static function main():Void {
		final inner:Array<Int> = [1];
		final rows:Array<Array<Int>> = [inner];
		nestedCopy(rows, gate);
	}
}
