/**
	Provides an unchanged class dependency beside the edited worker.

	Keeping this module in the successful program gives the probe a concrete
	class that Haxe should restore from its frontend cache after the edit.
**/
class StableWorker {
	/** Returns a stable integer so the generated program uses this module. */
	public static function value():Int {
		return 6;
	}
}
