/** A by-value record cannot represent Haxe's null Array growth default. */
typedef Entry = {
	final value:Int;
}

/** Proves resize growth stays closed when the element has no exact null carrier. */
final class Main {
	static function main():Void {
		final values:Array<Entry> = [{value: 1}];
		values.resize(2);
	}
}
