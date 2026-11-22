package hxc.cli;

/** The narrow seam implemented by later build, doctor, inspect, and bindgen owners. */
interface HxcCliExecutor {
	/** Report whether this build has an implementation for the command. */
	function isAvailable(command:HxcCliCommand):Bool;

	/** Execute one already-routed request and return every observable stream fact. */
	function execute(request:HxcCliRequest):HxcCliExecution;
}
