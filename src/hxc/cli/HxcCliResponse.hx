package hxc.cli;

import haxe.Json;
import hxc.cli.HxcCliDiagnostic.HxcCliDiagnosticJson;

/** Schema-1 JSON shape emitted as the only stdout value in JSON mode. */
typedef HxcCliResponseJson = {
	final schemaVersion:Int;
	final command:String;
	final status:String;
	final exitCategory:String;
	final exitCode:Int;
	final stdout:String;
	final stderr:String;
	final signal:Null<String>;
	final diagnostics:Array<HxcCliDiagnosticJson>;
}

/**
	One complete router result with separate machine and human renderings.

	The response owns framing. Command handlers return data and cannot write an
	extra value to JSON stdout accidentally.
**/
class HxcCliResponse {
	public static inline final SCHEMA_VERSION = 1;

	public final json:Bool;
	public final command:String;
	public final exitCategory:HxcCliExitCategory;
	public final exitCode:Int;
	public final stdout:String;
	public final stderr:String;
	public final signal:Null<String>;
	public final logs:Array<String>;
	public final diagnostics:Array<HxcCliDiagnostic>;

	public function new(json:Bool, command:String, exitCategory:HxcCliExitCategory, exitCode:Int, stdout:String, stderr:String, ?signal:String,
			?logs:Array<String>, ?diagnostics:Array<HxcCliDiagnostic>) {
		if (StringTools.trim(command) == ""
			|| exitCode < 0
			|| exitCode > 255
			|| (exitCode == 0) != (exitCategory == HxcCliExitCategory.Success)) {
			throw "CLI response command, category, or exit code is invalid";
		}
		if (signal != null && StringTools.trim(signal) == "") {
			throw "CLI response signal must be absent or non-empty";
		}
		this.json = json;
		this.command = command;
		this.exitCategory = exitCategory;
		this.exitCode = exitCode;
		this.stdout = stdout;
		this.stderr = stderr;
		this.signal = signal;
		this.logs = logs == null ? [] : logs.copy();
		this.diagnostics = diagnostics == null ? [] : diagnostics.copy();
	}

	/** Serialize exactly one documented JSON object plus its line terminator. */
	public function renderJson():String
		return Json.stringify(toJsonValue()) + "\n";

	/** Return normal stdout; JSON mode replaces it with the response object. */
	public function renderStdout():String
		return json ? renderJson() : stdout;

	/**
		Render logs, child stderr, and diagnostics on stderr in stable order.

		JSON mode still uses stderr for human progress while retaining child stream,
		exit, signal, and diagnostic facts inside the machine response on stdout.
	**/
	public function renderStderr():String {
		final output = new StringBuf();
		for (log in logs) {
			output.add(log);
			if (!StringTools.endsWith(log, "\n")) {
				output.add("\n");
			}
		}
		output.add(stderr);
		if (stderr != "" && diagnostics.length > 0 && !StringTools.endsWith(stderr, "\n")) {
			output.add("\n");
		}
		for (diagnostic in diagnostics) {
			output.add(diagnostic.render());
			output.add("\n");
		}
		return output.toString();
	}

	/** Return the typed schema value used by tests, agents, and CI. */
	public function toJsonValue():HxcCliResponseJson {
		return {
			schemaVersion: SCHEMA_VERSION,
			command: command,
			status: exitCode == 0 ? "ok" : "error",
			exitCategory: exitCategory,
			exitCode: exitCode,
			stdout: stdout,
			stderr: stderr,
			signal: signal,
			diagnostics: diagnostics.map(diagnostic -> diagnostic.toJsonValue())
		};
	}
}
