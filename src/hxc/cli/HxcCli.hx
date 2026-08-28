package hxc.cli;

/**
	Parse global CLI syntax and route one command to its typed owner.

	This object owns only command selection, help/version, JSON framing, and
	stable exit categories. Build and product behavior stays in injected command
	executors so later tasks extend one router instead of replacing it.
**/
class HxcCli {
	public static inline final CLI_PROTOCOL_VERSION = 1;

	final executor:HxcCliExecutor;

	public function new(executor:HxcCliExecutor) {
		this.executor = executor;
	}

	/** Parse one argument vector without reading or writing process-global streams. */
	public function route(arguments:Array<String>):HxcCliResponse {
		final parsed = parseGlobalJson(arguments);
		if (parsed.duplicate) {
			return usage(parsed.json, "hxc", "HXC-CLI-0003", "`--json` may appear only once", "Remove the duplicate option.");
		}
		final values = parsed.arguments;
		if (values.length == 0) {
			return success(parsed.json, "help", helpText());
		}

		final first = values[0];
		if (first == "--help" || first == "-h") {
			return values.length == 1 ? success(parsed.json, "help",
				helpText()) : usage(parsed.json, "help", "HXC-CLI-0002", 'unknown help option `${values[1]}`', "Run `hxc help` without extra options.");
		}
		if (first == "--version") {
			return values.length == 1 ? success(parsed.json, "version",
				versionText()) : usage(parsed.json, "version", "HXC-CLI-0002", 'unknown version option `${values[1]}`',
					"Run `hxc version` without extra options.");
		}
		if (StringTools.startsWith(first, "-")) {
			return usage(parsed.json, "hxc", "HXC-CLI-0002", 'unknown global option `$first`', "Use `hxc help` to list admitted options.");
		}

		final command = HxcCliCommand.parse(first);
		if (command == null) {
			return usage(parsed.json, first, "HXC-CLI-0001", 'unknown command `$first`', "Use `hxc help` to list commands.");
		}
		final commandArguments = values.slice(1);
		if (command == HxcCliCommand.Help) {
			return routeHelp(parsed.json, commandArguments);
		}
		if (command == HxcCliCommand.Version) {
			return commandArguments.length == 0 ? success(parsed.json, command,
				versionText()) : usage(parsed.json, command, "HXC-CLI-0002", 'unknown version option `${commandArguments[0]}`',
					"Run `hxc version` without extra options.");
		}
		if (commandArguments.length == 1 && (commandArguments[0] == "--help" || commandArguments[0] == "-h")) {
			return success(parsed.json, command, commandHelp(command));
		}
		var forwards = false;
		for (argument in commandArguments) {
			if (argument == "--") {
				forwards = true;
			} else if (!forwards && StringTools.startsWith(argument, "--experimental-")) {
				return usage(parsed.json, command, "HXC-CLI-0002", 'experimental option `$argument` is not available',
					"Remove the option or use a build that explicitly documents it.");
			}
		}
		if (!executor.isAvailable(command)) {
			return unavailable(parsed.json, command);
		}

		final execution = executor.execute(new HxcCliRequest(command, commandArguments, parsed.json));
		return new HxcCliResponse(parsed.json, command, execution.exitCategory, execution.exitCode, execution.stdout, execution.stderr, execution.signal,
			execution.logs, execution.diagnostics);
	}

	function routeHelp(json:Bool, arguments:Array<String>):HxcCliResponse {
		if (arguments.length == 0) {
			return success(json, "help", helpText());
		}
		if (arguments.length > 1 || StringTools.startsWith(arguments[0], "-")) {
			return usage(json, "help", "HXC-CLI-0002", "help accepts at most one command name", "Run `hxc help <command>`.");
		}
		final command = HxcCliCommand.parse(arguments[0]);
		return command == null ? usage(json, "help", "HXC-CLI-0001", 'unknown command `${arguments[0]}`',
			"Use `hxc help` to list commands.") : success(json, command, commandHelp(command));
	}

	function success(json:Bool, command:String, stdout:String):HxcCliResponse
		return new HxcCliResponse(json, command, HxcCliExitCategory.Success, 0, stdout, "");

	function usage(json:Bool, command:String, code:String, message:String, remediation:String):HxcCliResponse
		return new HxcCliResponse(json, command, HxcCliExitCategory.Usage, HxcCliExitCategory.Usage.code(), "", "", null, null,
			[new HxcCliDiagnostic(code, message, remediation)]);

	function unavailable(json:Bool, command:HxcCliCommand):HxcCliResponse
		return new HxcCliResponse(json, command, HxcCliExitCategory.Unavailable, HxcCliExitCategory.Unavailable.code(), "", "", null, null, [
			new HxcCliDiagnostic("HXC-CLI-0004", 'command `$command` is recognized but not implemented in this build',
				"Use `hxc help` to inspect the current surface and check the owning roadmap task.")
		]);

	function versionText():String
		return 'hxc ${HxcBuildInfo.version()} (CLI schema $CLI_PROTOCOL_VERSION)\n';

	function helpText():String {
		final output = new StringBuf();
		output.add("Usage: hxc [--json] <command> [arguments]\n\nCommands:\n");
		for (command in HxcCliCommand.all()) {
			output.add("  ");
			output.add(StringTools.rpad(command, " ", 15));
			output.add(command.summary());
			if (!isAvailable(command)) {
				output.add(" [unavailable]");
			}
			output.add("\n");
		}
		output.add("\nRun `hxc help <command>` for a command synopsis.\n");
		return output.toString();
	}

	function commandHelp(command:HxcCliCommand):String {
		final synopsis = switch command {
			case HxcCliCommand.New: "hxc new <name> [--kind app|library|embedded]";
			case HxcCliCommand.Build: "hxc build [project.hxml] [options]";
			case HxcCliCommand.Run: "hxc run [project.hxml] [-- arguments...]";
			case HxcCliCommand.Test: "hxc test [selector]";
			case HxcCliCommand.Clean: "hxc clean";
			case HxcCliCommand.Doctor: "hxc doctor";
			case HxcCliCommand.Inspect: "hxc inspect <report> [--manifest <path>] [--show-sensitive]";
			case HxcCliCommand.Bindgen: "hxc bindgen <header> [options]";
			case HxcCliCommand.Export: "hxc export [project.hxml] [options]";
			case HxcCliCommand.FmtGenerated: "hxc fmt-generated";
			case HxcCliCommand.Version: "hxc version";
			case HxcCliCommand.Help: "hxc help [command]";
			case _: "hxc help";
		};
		final status = isAvailable(command) ? "" : "\nStatus: unavailable in this build.\n";
		return 'Usage: $synopsis\n\n${command.summary()}\n$status';
	}

	function isAvailable(command:HxcCliCommand):Bool
		return command == HxcCliCommand.Help || command == HxcCliCommand.Version || executor.isAvailable(command);

	function parseGlobalJson(arguments:Array<String>):{arguments:Array<String>, json:Bool, duplicate:Bool} {
		final filtered:Array<String> = [];
		var json = false;
		var duplicate = false;
		var forwards = false;
		for (argument in arguments) {
			if (forwards) {
				filtered.push(argument);
			} else if (argument == "--") {
				forwards = true;
				filtered.push(argument);
			} else if (argument == "--json") {
				if (json) {
					duplicate = true;
				}
				json = true;
			} else {
				filtered.push(argument);
			}
		}
		return {arguments: filtered, json: json, duplicate: duplicate};
	}
}
