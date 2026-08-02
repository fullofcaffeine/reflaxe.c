package c;

/**
	Immutable NUL-terminated text borrowed from one Haxe String for one C call.

	`CStringRef.to(text)` keeps the String's backing storage alive until the
	direct imported function returns. hxrt validates the String and rejects an
	embedded NUL or a view without a trailing NUL before C can observe it. The
	pointer cannot be stored, returned, forwarded, or consumed twice.

	Use `CString` for static literals. This explicit carrier exists for text read
	from runtime data, where silently widening every Haxe String to `const char *`
	would hide both the validation and the call-scoped lifetime.
**/
@:coreType
extern abstract CStringRef {
	/** Lend one immutable String to the direct C import containing this call. */
	public static function to(text:String):CStringRef;
}
