package hxc.bindgen;

import sys.io.Process;

/** Select the only stream allowed to be large for one constrained Clang pass. */
private enum HxcBindgenPrimaryStream {
	Stdout;
	Stderr;
}

/** Captured child-process result; bindgen never delegates argument parsing to a shell. */
class HxcBindgenProcessResult {
	/** Exact child exit code. */
	public final exitCode:Int;

	/** Complete captured standard output. */
	public final stdout:String;

	/** Complete captured standard error. */
	public final stderr:String;

	/** Retain one completed child result without reading process-global streams. */
	public function new(exitCode:Int, stdout:String, stderr:String) {
		this.exitCode = exitCode;
		this.stdout = stdout;
		this.stderr = stderr;
	}
}

/**
	Run one exact executable and argument array while preserving both child streams.

	An optional bounded input lets semantic probes use Clang's standard-input
	translation unit without publishing temporary C files. The caller still owns
	the source text and Clang remains the only process that interprets it.
**/
function runBindgenProcess(executable:String, arguments:Array<String>, input:Null<String> = null):HxcBindgenProcessResult {
	return runConstrainedProcess(executable, arguments, Stdout, input);
}

/** Run a diagnostics-only Clang pass whose standard output is constrained to be empty. */
function runBindgenDiagnosticProcess(executable:String, arguments:Array<String>):HxcBindgenProcessResult {
	return runConstrainedProcess(executable, arguments, Stderr, null);
}

/** Read the pass's only potentially large stream first, then preserve its bounded companion. */
function runConstrainedProcess(executable:String, arguments:Array<String>, primary:HxcBindgenPrimaryStream, input:Null<String>):HxcBindgenProcessResult {
	final process = try {
		new Process(executable, arguments);
	} catch (_:haxe.Exception) {
		throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Unavailable, "HXC-CLI-0802", 'cannot start Clang executable `$executable`',
			"Install Clang or pass its exact executable with `--clang <path>`.");
	};
	if (input != null)
		process.stdin.writeString(input);
	process.stdin.close();
	final primaryText = (primary == Stdout ? process.stdout : process.stderr).readAll().toString();
	final secondaryText = (primary == Stdout ? process.stderr : process.stdout).readAll().toString();
	final exitCode = process.exitCode();
	process.close();
	return primary == Stdout ? new HxcBindgenProcessResult(exitCode, primaryText,
		secondaryText) : new HxcBindgenProcessResult(exitCode, secondaryText, primaryText);
}
