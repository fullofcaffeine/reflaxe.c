/**
	Names the record returned by the edited worker.

	The typedef gives the two values one shared structural contract. The
	incremental-input test uses that contract to connect its changed and stable
	class dependencies. The same test requires every cold and warm HxcIR field
	to keep these declaration lines, because a reused Haxe anonymous type can
	otherwise expose the later object-literal positions below.
**/
typedef WorkerResult = {
	/** Value changed by the probe's implementation-only edit. */
	final changed:Int;

	/** Value supplied by the unchanged worker dependency. */
	final stable:Int;
}

/**
	Construct the named record through an ordinary Haxe module-level function.

	The test edits only the first literal, which lets Haxe report the rebuilt
	class dependency closure while leaving `StableWorker` unchanged. A
	module-level function is the smaller source shape because this operation has
	no instance state or identity.
**/
function snapshot():WorkerResult {
	return {
		changed: 4,
		stable: StableWorker.value()
	};
}
