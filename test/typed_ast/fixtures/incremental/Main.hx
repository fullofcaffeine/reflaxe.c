/**
	Connects two tiny class modules for the successful Haxe-server cache probe.

	A real class is intentional here: Haxe exposes class rebuilds through build
	macros, so this fixture can distinguish one changed implementation from one
	unchanged dependency without inventing a target-specific cache mechanism.
**/
class Main {
	/**
		Keep one entry-class static initializer in the invalidation program.

		Changing this value must rebuild its initializer function and keep the
		initializer's destination in the shared global plan. That combination
		guards the planning-order regression found while this catalog was added.
	**/
	public static var startupOffset:Int = 0;

	/** Runs both workers so neither module can disappear through dead-code elimination. */
	static function main():Void {
		final result = ChangedWorker.snapshot();
		if (result.changed + result.stable + ReplayFeatures.score() + startupOffset == 23) {
			Sys.println("incremental fixture");
		} else {
			Sys.println("incremental fixture changed");
		}
	}
}
