/** Proves that an inline facade retains its module-private direct-C helper. */
final class Main {
	static function main():Void {
		final runtimeText = "Ha" + "xe";
		while (!RuntimeTextFacade.matchesStatic()) {}
		while (!RuntimeTextFacade.matches(runtimeText)) {}
	}
}
