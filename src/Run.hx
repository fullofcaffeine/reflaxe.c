import hxc.cli.HxcCli;
import hxc.cli.HxcProductExecutor;

/**
	Eval bootstrap and future native composition root for the `hxc` command.

	All parsing and rendering lives in the target-neutral CLI core. This entry
	point only connects process arguments and streams, which keeps Eval and the
	future haxe.c-built executable on the same observable contract.
**/
class Run {
	static function main():Void {
		final response = new HxcCli(new HxcProductExecutor()).route(Sys.args());
		Sys.stdout().writeString(response.renderStdout());
		Sys.stdout().flush();
		Sys.stderr().writeString(response.renderStderr());
		Sys.stderr().flush();
		Sys.exit(response.exitCode);
	}
}
