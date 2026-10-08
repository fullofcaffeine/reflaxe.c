import hxc.cli.HxcCli;
import hxc.cli.HxcCliDiagnostic;
import hxc.cli.HxcCliExecution;
import hxc.cli.HxcCliExecutor;
import hxc.cli.HxcCliExitCategory;
import hxc.cli.HxcCliRequest;

/** Eval probe that gives the router a deterministic child-process observer. */
class CliProbe {
	static function main():Void {
		final response = new HxcCli(new ProbeExecutor()).route(Sys.args());
		Sys.stdout().writeString(response.renderStdout());
		Sys.stderr().writeString(response.renderStderr());
		Sys.exit(response.exitCode);
	}
}

/** Synthetic command owner used only to prove exact output and exit propagation. */
private class ProbeExecutor implements HxcCliExecutor {
	public function new() {}

	/** Admit every routed product command for exact execution propagation tests. */
	public function isAvailable(command:hxc.cli.HxcCliCommand):Bool
		return true;

	public function execute(request:HxcCliRequest):HxcCliExecution {
		final command:String = request.command;
		return new HxcCliExecution(HxcCliExitCategory.Command, 23, 'child-out:$command:${request.arguments.join("|")}\n', "child-err", "SIGTERM",
			["probe-log"], [
				new HxcCliDiagnostic("HXC-CLI-0099", "synthetic child failure", "Inspect the propagated child result.")
			]);
	}
}
