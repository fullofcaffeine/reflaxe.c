/** Proves that the neighboring uppercase API remains fail-closed. */
final class Main {
	/** Keep the unsupported result observable if lowering ever admits it. */
	static function main():Void
		Sys.println("Haxe".toUpperCase());
}
