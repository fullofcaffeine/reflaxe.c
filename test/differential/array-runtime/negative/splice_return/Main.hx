/**
	Proves that the bounded splice slice cannot masquerade as full Array parity.

	The compiler may remove one element without allocating when the returned
	Array is discarded. Reading that returned Array needs a separate ownership
	plan and therefore remains fail-closed.
**/
final class Main {
	static function main():Void {
		final values = [1, 2, 3];
		final removed = values.splice(1, 1);
		while (removed.length != 1 || removed[0] != 2) {}
	}
}
