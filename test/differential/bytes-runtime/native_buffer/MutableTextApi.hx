/**
 * The independent C boundary used to prove one-call mutable text borrowing.
 *
 * The declaration mirrors `mutable_text.h`; no compiler-owned test helper knows
 * its symbol. `CStringBufferRef` says that C may edit the bytes during this
 * call, while the ordinary Haxe `Bytes` value remains their owner.
 */
@:c.include("mutable_text.h", c.IncludeKind.Local)
extern class MutableTextApi {
	/**
	 * Replace the NUL-terminated text without retaining its pointer.
	 *
	 * `capacity` is the full allocation size, including room for the final NUL.
	 * This fixture checks it independently so a wrong wrapper cannot look safe.
	 */
	@:c.name("fixture_replace_mutable_text")
	public static function replace(text:c.CStringBufferRef, capacity:Int):Bool;
}
