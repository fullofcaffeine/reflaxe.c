/** Provides one typed C boundary used only to report a failed semantic check. */
@:c.include("stdlib.h", c.IncludeKind.System)
extern class AtomicExit {
	/** Use the C implementation's portable nonzero failure value. */
	@:c.constant
	@:c.name("EXIT_FAILURE")
	public static var failure(default, never):c.Int32;

	/** End the native probe with the supplied nonzero status. */
	@:c.name("exit")
	public static function withCode(code:c.Int32):Void;

	/** End the probe after any failed semantic assertion. */
	public static inline function fail():Void {
		withCode(failure);
	}
}
