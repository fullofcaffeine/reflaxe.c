import hxc.cli.HxcCli;
import hxc.cli.HxcProductExecutor;
import hxc.config.HxcConfigLoader;

/**
	Exercise project creation from a temporary working directory through one compiled probe.

	The test runner starts compilation from the repository's pinned scope, then
	this probe changes only the product working directory. Compiling it once keeps
	the suite fast while preserving the same typed router and filesystem behavior.
**/
class NewProjectProbe {
	/** Route one CLI request or validate one generated project configuration. */
	static function main():Void {
		final arguments = Sys.args();
		if (arguments.length < 2)
			throw "NewProjectProbe requires a mode and path";
		switch arguments[0] {
			case "cli":
				Sys.setCwd(arguments[1]);
				final response = new HxcCli(new HxcProductExecutor()).route(arguments.slice(2));
				Sys.stdout().writeString(response.renderStdout());
				Sys.stderr().writeString(response.renderStderr());
				Sys.stdout().flush();
				Sys.stderr().flush();
				Sys.exit(response.exitCode);
			case "config":
				HxcConfigLoader.load(arguments[1]);
				Sys.println("hxc-new-config: OK");
			case unknown:
				throw 'unknown NewProjectProbe mode `$unknown`';
		}
	}
}
