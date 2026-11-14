import hxc.bindgen.HxcBindgenProcess.runBindgenProcess;
import hxc.bindgen.HxcBindgenProcess.runBindgenDiagnosticProcess;

/** Prove each constrained Clang pass drains its selected large child stream. */
class HxcBindgenProcessProbe {
	static function main():Void {
		final arguments = Sys.args();
		if (arguments.length != 2)
			throw "expected child executable and script";
		final stdoutResult = runBindgenProcess(arguments[0], [arguments[1], "stdout"]);
		final stderrResult = runBindgenDiagnosticProcess(arguments[0], [arguments[1], "stderr"]);
		if (stdoutResult.exitCode != 0 || stdoutResult.stdout.length != 262144 || stdoutResult.stderr != "")
			throw "large child stdout capture was incomplete";
		if (stderrResult.exitCode != 0 || stderrResult.stdout != "" || stderrResult.stderr.length != 262144)
			throw "child stream capture was incomplete";
		Sys.println("hxc-bindgen-process: OK");
	}
}
