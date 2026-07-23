package c;

/**
	NUL-terminated byte-string view.

	The C backend admits an embedded-NUL-free String literal or a closed
	conditional/switch whose every result is such a literal. Program-local
	`c.CString` helpers may propagate that proven carrier to a native call. The
	implicit relation exists so Haxe can type this ergonomic static-text path;
	dynamic String values remain rejected before any artifact is written. The
	emitted pointer borrows immutable C literal storage and allocates no wrapper.
**/
@:coreType
extern abstract CString from String {}
