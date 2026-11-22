package c;

/**
	Immutable NUL-terminated text prepared for one non-retaining C call.

	`CStringArg.to(text)` borrows a String whose view already ends at owned NUL
	storage. An interior view instead receives temporary terminated storage that
	is released immediately after the direct imported function returns. Embedded
	NUL fails before C can observe truncated text, and the pointer cannot escape
	the call.

	Use `CStringRef` when allocation must be forbidden. This separate carrier
	makes the borrow-or-copy policy explicit at the source boundary.
**/
@:coreType
extern abstract CStringArg {
	/** Prepare one immutable String for the direct C import containing this call. */
	public static function to(text:String):CStringArg;
}
