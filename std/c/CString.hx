package c;

/**
	NUL-terminated byte-string view.

	The C backend currently admits only a direct String literal at a native-call
	argument. The implicit relation exists so Haxe can type that ergonomic call;
	the lowering boundary rejects non-literals and embedded NUL bytes before any
	artifact is written. The emitted pointer borrows the C string literal's
	static storage and allocates no wrapper.
**/
@:coreType
extern abstract CString from String {}
