package hxc.cli;

import hxc.bindgen.HxcBindgenExecutor;
import hxc.inspect.HxcInspectExecutor;

/**
	Compose the implemented `hxc` product commands at one process boundary.

	Each command keeps its own parser and behavior owner. This dispatcher only
	selects that owner, so adding bindgen does not couple Clang policy to the
	read-only inspection path.
**/
class HxcProductExecutor implements HxcCliExecutor {
	final inspect:HxcInspectExecutor;
	final bindgen:HxcBindgenExecutor;

	/** Construct the fixed set of command owners available in this build. */
	public function new() {
		inspect = new HxcInspectExecutor();
		bindgen = new HxcBindgenExecutor();
	}

	/** Report whether one composed command has an implementation. */
	public function isAvailable(command:HxcCliCommand):Bool
		return inspect.isAvailable(command) || bindgen.isAvailable(command);

	/** Route one request to its exact command owner. */
	public function execute(request:HxcCliRequest):HxcCliExecution {
		if (inspect.isAvailable(request.command))
			return inspect.execute(request);
		if (bindgen.isAvailable(request.command))
			return bindgen.execute(request);
		throw 'HxcProductExecutor received unavailable command `${request.command}`';
	}
}
