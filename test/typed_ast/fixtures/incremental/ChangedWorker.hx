import WorkerResult.snapshot as workerSnapshot;

/**
	Provides a class caller for the edited module-level record function.

	The implementation-only edit lives beside `WorkerResult`'s typedef. This
	class ensures Haxe's dependency closure includes an ordinary class caller.
**/
class ChangedWorker {
	/** Delegates to the module function whose record literal is edited. */
	public static function snapshot():WorkerResult {
		return workerSnapshot();
	}
}
