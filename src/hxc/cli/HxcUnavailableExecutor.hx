package hxc.cli;

/**
	Fail-closed bootstrap executor used until each command's owning task lands.

	Recognizing a command is not a claim that its behavior exists. This executor
	makes the missing owner visible and gives later implementations one typed seam.
**/
class HxcUnavailableExecutor implements HxcCliExecutor {
	public function new() {}

	/** Keep product commands visibly unavailable in the router and help output. */
	public function isAvailable(command:HxcCliCommand):Bool
		return false;

	/** Fail if a caller bypasses the router's availability check. */
	public function execute(request:HxcCliRequest):HxcCliExecution {
		throw "the unavailable CLI executor cannot execute a command";
	}
}
