package hxc.bindgen;

import hxc.bindgen.HxcBindgenOptions.HxcBindgenDefine;
import hxc.bindgen.HxcBindgenOptions.HxcBindgenLanguage;
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
			return usage("bindgen requires at least one entry header", "Run `hxc help bindgen` and pass entry headers before options.");
		final entryHeaders:Array<String> = [];
		var index = 0;
		while (index < arguments.length && !StringTools.startsWith(arguments[index], "-")) {
			entryHeaders.push(arguments[index]);
			index++;
		}
		var clang = "clang";
		var target:Null<String> = null;
		var language = HxcBindgenLanguage.C;
		var languageExplicit = false;
		var sysroot:Null<String> = null;
		final includeDirectories:Array<String> = [];
		final defines:Array<HxcBindgenDefine> = [];
		var output = "bindings";
		var dryRun = false;
		var clangSeen = false;
		var outputSeen = false;
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
				case "--language":
					if (languageExplicit)
						return usage("`--language` may appear only once", "Pass one exact language mode.");
					final parsed = HxcBindgenLanguage.parse(value);
					if (parsed == null)
						return usage('unsupported bindgen language `$value`', "Use `c` or `c++`.");
					language = parsed;
					languageExplicit = true;
				case "--sysroot":
					if (sysroot != null)
						return usage("`--sysroot` may appear only once", "Pass one target sysroot directory.");
					sysroot = value;
				case "--include-dir":
					includeDirectories.push(value);
				case "--define":
					final define = parseDefine(value);
					if (define == null)
						return usage('invalid preprocessor definition `$value`', "Use NAME or NAME=value with a C identifier name.");
					defines.push(define);
				case "--output":
					if (outputSeen)
						return usage("`--output` may appear only once", "Pass one output directory.");
					output = value;
					outputSeen = true;
				case _:
					return usage('unknown bindgen option `$argument`',
						"Use only `--clang`, `--target`, `--language`, `--sysroot`, `--include-dir`, `--define`, `--output`, or `--dry-run`.");
			}
			index += 2;
		}
		return new HxcBindgenOptions({
			entryHeaders: entryHeaders,
			clang: clang,
			target: target,
			language: language,
			languageExplicit: languageExplicit,
			sysroot: sysroot,
			includeDirectories: includeDirectories,
			defines: defines,
			outputDirectory: output,
			dryRun: dryRun
		});
	}

	function optionValue(arguments:Array<String>, index:Int, option:String):String {
		if (!StringTools.startsWith(option, "--") || index + 1 >= arguments.length || StringTools.startsWith(arguments[index + 1], "--"))
			return usage('option `$option` requires one value', "Pass the value as the next argument.");
		final value = arguments[index + 1];
		if (StringTools.trim(value) == "" || value.indexOf("\x00") >= 0)
			return usage('option `$option` has an empty or invalid value', "Pass one non-empty value.");
		return value;
	}

	function parseDefine(value:String):Null<HxcBindgenDefine> {
		final equals = value.indexOf("=");
		final name = equals < 0 ? value : value.substr(0, equals);
		if (name == "" || !identifierStart(name.charCodeAt(0)))
			return null;
		for (index in 1...name.length)
			if (!identifierPart(name.charCodeAt(index)))
				return null;
		if (value.indexOf("\n") >= 0 || value.indexOf("\r") >= 0)
			return null;
		return new HxcBindgenDefine(name, equals < 0 ? null : value.substr(equals + 1));
	}

	function identifierStart(code:Null<Int>):Bool
		return code != null && (code == 0x5F || code >= 0x41 && code <= 0x5A || code >= 0x61 && code <= 0x7A);

	function identifierPart(code:Null<Int>):Bool
		return identifierStart(code) || code != null && code >= 0x30 && code <= 0x39;

	function usage<T>(message:String, remediation:String):T
		throw new HxcBindgenError(HxcCliExitCategory.Usage, "HXC-CLI-0801", message, remediation);
}
