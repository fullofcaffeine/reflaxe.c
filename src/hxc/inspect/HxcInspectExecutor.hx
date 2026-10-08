package hxc.inspect;

import hxc.cli.HxcCliCommand;
import hxc.cli.HxcCliDiagnostic;
import hxc.cli.HxcCliExecution;
import hxc.cli.HxcCliExecutor;
import hxc.cli.HxcCliExitCategory;
import hxc.cli.HxcCliRequest;
import hxc.config.HxcJsonCodec.renderJson;

/**
	Own the read-only `hxc inspect` command while later product commands remain unavailable.

	The executor accepts a compiler manifest, validates its complete addressed
	artifact set, and returns either human-readable JSON or a compact versioned
	report inside the shared CLI response envelope.
**/
class HxcInspectExecutor implements HxcCliExecutor {
	public function new() {}

	/** Only inspection has a product implementation at this task boundary. */
	public function isAvailable(command:HxcCliCommand):Bool
		return command == HxcCliCommand.Inspect;

	/** Parse inspection-only options and return one bounded CLI result. */
	public function execute(request:HxcCliRequest):HxcCliExecution {
		try {
			final parsed = parseArguments(request.arguments);
			final report = new HxcInspectManifest(parsed.manifest).report(parsed.report, parsed.showSensitive);
			final body = renderJson(report, !request.json) + "\n";
			return new HxcCliExecution(HxcCliExitCategory.Success, 0, body, "");
		} catch (error:HxcInspectError) {
			return failure(error.category, error.code, error.message, error.remediation);
		} catch (error:haxe.Exception) {
			return failure(HxcCliExitCategory.Internal, "HXC-CLI-0799", "inspection failed before it could produce a trusted report",
				"Check the manifest and filesystem permissions, then retry.");
		}
	}

	function parseArguments(arguments:Array<String>):{report:HxcInspectReport, manifest:String, showSensitive:Bool} {
		if (arguments.length == 0)
			return usage("inspect requires one report name", reportRemediation());
		final report = HxcInspectReport.parse(arguments[0]);
		if (report == null)
			return usage('unknown inspection report `${arguments[0]}`', reportRemediation());
		var manifest = "hxc.manifest.json";
		var manifestSeen = false;
		var showSensitive = false;
		var index = 1;
		while (index < arguments.length) {
			final argument = arguments[index];
			if (argument == "--manifest") {
				if (manifestSeen || index + 1 >= arguments.length || StringTools.startsWith(arguments[index + 1], "-"))
					return usage("`--manifest` requires one path and may appear only once", "Pass `--manifest <path>` once.");
				manifest = arguments[index + 1];
				manifestSeen = true;
				index += 2;
			} else if (argument == "--show-sensitive") {
				if (showSensitive)
					return usage("`--show-sensitive` may appear only once", "Remove the duplicate option.");
				showSensitive = true;
				index++;
			} else {
				return usage('unknown inspect option `$argument`', "Use only `--manifest <path>` or `--show-sensitive`.");
			}
		}
		return {report: report, manifest: manifest, showSensitive: showSensitive};
	}

	function usage<T>(message:String, remediation:String):T
		throw new HxcInspectError(HxcCliExitCategory.Usage, "HXC-CLI-0701", message, remediation);

	function failure(category:HxcCliExitCategory, code:String, message:String, remediation:String):HxcCliExecution
		return new HxcCliExecution(category, category.code(), "", "", null, null, [new HxcCliDiagnostic(code, message, remediation)]);

	function reportRemediation():String
		return 'Choose one of: ${HxcInspectReport.all().join(", ")}.';
}
