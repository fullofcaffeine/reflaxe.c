package c;

import haxe.io.Bytes;

/**
	A mutable NUL-terminated byte buffer borrowed for one direct C call.

	Use this only for a C parameter declared as `char *` when the C function edits
	caller-owned text in place. `CStringBufferRef.to(bytes)` keeps the ordinary
	Haxe `Bytes` value as the owner and lends its storage until that one imported
	call returns. haxe.c rejects storing, returning, or forwarding the pointer,
	and hxrt checks that the buffer is non-empty and already contains a NUL byte.

	The higher-level wrapper still owns the text contract: it must reserve space
	for the final NUL, pass the same byte capacity to C, and decode the edited
	bytes as UTF-8 after the call. This type deliberately does not expose pointer
	arithmetic or a general `Bytes`-to-pointer conversion.
**/
@:coreType
extern abstract CStringBufferRef {
	/**
		Lend one existing `Bytes` owner's mutable storage to a direct C import.

		This compiler intrinsic creates no Haxe object. In generated C it becomes a
		checked hxrt borrow followed immediately by the native call. A temporary
		such as `Bytes.alloc(64)` is rejected because no caller-owned variable would
		keep the buffer alive for the full call.
	**/
	public static function to(bytes:Bytes):CStringBufferRef;
}
