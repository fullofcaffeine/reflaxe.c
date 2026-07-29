/**
 * Proves that a typed carrier still needs a fresh `to(bytes)` at the native call.
 *
 * The reachable helper deliberately forwards its parameter. haxe.c must reject
 * that forwarding rather than treating the type alone as lifetime evidence.
 */
final class Main {
	static function main():Void
		forward(null);

	static function forward(buffer:c.CStringBufferRef):Void
		MutableTextApi.replace(buffer, 16);
}
