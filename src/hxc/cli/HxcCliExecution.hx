package hxc.cli;

/**
	A command handler's complete observable result.

	Raw output and signals remain data so `run` and `test` can preserve native
	behavior in both human and JSON modes instead of converting failures to a
	generic success or swallowing a child stream.
**/
class HxcCliExecution {
	public final exitCategory:HxcCliExitCategory;
	public final exitCode:Int;
	public final stdout:String;
	public final stderr:String;
	public final signal:Null<String>;
	public final logs:Array<String>;
	public final diagnostics:Array<HxcCliDiagnostic>;

	public function new(exitCategory:HxcCliExitCategory, exitCode:Int, stdout:String, stderr:String, ?signal:String, ?logs:Array<String>,
			?diagnostics:Array<HxcCliDiagnostic>) {
		if (exitCode < 0 || exitCode > 255) {
			throw 'CLI command exit code must be between 0 and 255, got $exitCode';
		}
		if ((exitCode == 0) != (exitCategory == HxcCliExitCategory.Success)) {
			throw "CLI success category and process exit code disagree";
		}
		this.exitCategory = exitCategory;
		this.exitCode = exitCode;
		this.stdout = stdout;
		this.stderr = stderr;
		this.signal = signal;
		this.logs = logs == null ? [] : logs.copy();
		this.diagnostics = diagnostics == null ? [] : diagnostics.copy();
	}
}
