/** Keeps Date.fromString's remaining gap explicit at the ordinary API call. */
class Main {
	/** Request the unsupported parser so lowering must fail before C emission. */
	static function main():Void {
		final value = Date.fromString("2024-03-10 01:30:00");
		Sys.println(value.getTime());
	}
}
