package c;

/**
	Typed exact-width integer conversion operations.

	The C target validates both carriers before emission. `exact` requires the
	source range to fit wholly in the inferred target. `modulo` requires an
	unsigned inferred target and applies modulo 2^N. These are compiler
	operations: no callable wrapper or runtime symbol is emitted.
**/
extern class IntConvert {
	/**
		Convert only when every possible `From` value is representable by the
		context-inferred `To` integer carrier.
	**/
	public static function exact<From, To>(value:From):To;

	/**
		Convert to a context-inferred unsigned `To` carrier using modulo 2^N,
		where N is the target width.
	**/
	public static function modulo<From, To>(value:From):To;
}
