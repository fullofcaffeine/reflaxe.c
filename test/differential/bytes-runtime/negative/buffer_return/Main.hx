/** Proves that an imported C declaration cannot return a call-scoped carrier. */
final class Main {
	static function main():Void {
		MutableTextApi.retained();
	}
}
