package hxc.bindgen;

import hxc.cli.HxcCliCommand;
import hxc.cli.HxcCliDiagnostic;
import hxc.cli.HxcCliExecution;
import hxc.cli.HxcCliExecutor;
import hxc.cli.HxcCliExitCategory;
import hxc.cli.HxcCliRequest;

/** Parse and execute the semantic-lock stage of `hxc bindgen`. */
class HxcBindgenExecutor implements HxcCliExecutor {
	/** Construct the stateless bindgen command owner. */
	public function new() {}

	/** Bindgen is the only command owned by this executor. */
	public function isAvailable(command:HxcCliCommand):Bool
		return command == HxcCliCommand.Bindgen;

	/** Return a deterministic lock or one stable CLI failure. */
	public function execute(request:HxcCliRequest):HxcCliExecution {
		try {
			final options = parseArguments(request.arguments);
			final result = HxcBindgenDriver.generate(options);
			final stdout = request.json || options.dryRun ? result.lockText : 'hxc bindgen: wrote ${result.outputPath}\n';
			return new HxcCliExecution(HxcCliExitCategory.Success, 0, stdout, result.childStderr);
		} catch (error:HxcBindgenError) {
			return new HxcCliExecution(error.category, error.category.code(), "", error.childStderr, null, null,
				[new HxcCliDiagnostic(error.code, error.message, error.remediation)]);
		} catch (_:haxe.Exception) {
			return new HxcCliExecution(HxcCliExitCategory.Internal, HxcCliExitCategory.Internal.code(), "", "", null, null, [
				new HxcCliDiagnostic("HXC-CLI-0899", "bindgen failed before it could publish a trusted semantic lock",
					"Check filesystem permissions and the selected Clang installation, then retry.")
			]);
		}
	}

	function parseArguments(arguments:Array<String>):HxcBindgenOptions {
		if (arguments.length == 0 || StringTools.startsWith(arguments[0], "-"))
			return usage("bindgen requires one C header path", "Run `hxc help bindgen` and pass the header first.");
		final header = arguments[0];
		var clang = "clang";
		var target:Null<String> = null;
		final includeDirectories:Array<String> = [];
		final defines:Array<String> = [];
		var output = "bindings";
		var dryRun = false;
		var clangSeen = false;
		var outputSeen = false;
		var index = 1;
		while (index < arguments.length) {
			final argument = arguments[index];
			if (argument == "--dry-run") {
				if (dryRun)
					return usage("`--dry-run` may appear only once", "Remove the duplicate option.");
				dryRun = true;
				index++;
				continue;
			}
			final value = optionValue(arguments, index, argument);
			switch argument {
				case "--clang":
					if (clangSeen)
						return usage("`--clang` may appear only once", "Pass one exact Clang executable.");
					clang = value;
					clangSeen = true;
				case "--target":
					if (target != null)
						return usage("`--target` may appear only once", "Pass one exact Clang target triple.");
					target = value;
				case "--include-dir":
					includeDirectories.push(value);
				case "--define":
					if (!validDefine(value))
						return usage('invalid preprocessor definition `$value`', "Use NAME or NAME=value with a C identifier name.");
					defines.push(value);
				case "--output":
					if (outputSeen)
						return usage("`--output` may appear only once", "Pass one output directory.");
					output = value;
					outputSeen = true;
				case _:
					return usage('unknown bindgen option `$argument`',
						"Use only `--clang`, `--target`, `--include-dir`, `--define`, `--output`, or `--dry-run`.");
			}
			index += 2;
		}
		return new HxcBindgenOptions(header, clang, target, includeDirectories, defines, output, dryRun);
	}

	function optionValue(arguments:Array<String>, index:Int, option:String):String {
		if (!StringTools.startsWith(option, "--") || index + 1 >= arguments.length || StringTools.startsWith(arguments[index + 1], "--"))
			return usage('option `$option` requires one value', "Pass the value as the next argument.");
		final value = arguments[index + 1];
		if (StringTools.trim(value) == "" || value.indexOf("\x00") >= 0)
			return usage('option `$option` has an empty or invalid value', "Pass one non-empty value.");
		return value;
	}

	function validDefine(value:String):Bool {
		final equals = value.indexOf("=");
		final name = equals < 0 ? value : value.substr(0, equals);
		if (name == "" || !identifierStart(name.charCodeAt(0)))
			return false;
		for (index in 1...name.length)
			if (!identifierPart(name.charCodeAt(index)))
				return false;
		return value.indexOf("\n") < 0 && value.indexOf("\r") < 0;
	}

	function identifierStart(code:Null<Int>):Bool
		return code != null && (code == 0x5F || code >= 0x41 && code <= 0x5A || code >= 0x61 && code <= 0x7A);

	function identifierPart(code:Null<Int>):Bool
		return identifierStart(code) || code != null && code >= 0x30 && code <= 0x39;

	function usage<T>(message:String, remediation:String):T
		throw new HxcBindgenError(HxcCliExitCategory.Usage, "HXC-CLI-0801", message, remediation);
}
