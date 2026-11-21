import Reading.readingIf;

/** Exercise a delayed field read with both outcomes of each part of its saved decision. */
final class Main {
	static var visits:Int = 0;

	/** This observable call has no access to the local that holds the reading. */
	static function unrelated():Void {
		visits++;
	}

	/** A write before the comparison is safe. A later true decision refers to that current value. */
	static function read(input:Null<Reading>, allow:Bool, hide:Bool):Int {
		var selected = input;
		if (hide)
			selected = null;
		final minimum = allow ? 0 : 17;
		final visible = selected != null && selected.above(minimum);
		unrelated();
		return visible ? selected.value : -1;
	}

	/** Emit the same five observations under the interpreter and the generated executable. */
	static function main():Void {
		final present = readingIf(true);
		Sys.println(read(present, true, false));
		Sys.println(read(present, false, false));
		Sys.println(read(readingIf(false), true, false));
		Sys.println(read(present, true, true));
		Sys.println(visits);
	}
}
