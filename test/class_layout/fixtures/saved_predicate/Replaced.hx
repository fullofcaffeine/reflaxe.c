import Reading.readingIf;

/** A saved decision can outlive the contents of the local it tested. */
final class Replaced {
	/** A true result from the old reading cannot protect a read through its replacement. */
	static function read(input:Null<Reading>, replace:Bool):Int {
		var selected = input;
		final visible = selected != null && selected.enabled(true);
		if (replace)
			selected = null;
		return visible ? selected.value : -1;
	}

	/** Exercise the route on which the pointer changes after the comparison. */
	static function main():Void {
		Sys.println(read(readingIf(true), true));
	}
}
