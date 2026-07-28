/**
	Connects two tiny class modules for the successful Haxe-server cache probe.

	A real class is intentional here: Haxe exposes class rebuilds through build
	macros, so this fixture can distinguish one changed implementation from one
	unchanged dependency without inventing a target-specific cache mechanism.
**/
class Main {
	/** Runs both workers so neither module can disappear through dead-code elimination. */
	static function main():Void {
		final result = ChangedWorker.snapshot();
		if (result.changed + result.stable == 10) {
			Sys.println("incremental fixture");
		} else {
			Sys.println("incremental fixture changed");
		}
	}
}
