package hxc.bindgen;

import haxe.io.Path;
import hxc.bindgen.HxcBindgenModel.HxcBindgenPaths;
import hxc.bindgen.HxcBindgenModel.buildBindingLock;
import hxc.bindgen.HxcBindgenModel.normalizeClangAst;
import hxc.bindgen.HxcBindgenAbiModel.buildPrimitiveAbiModel;
import hxc.bindgen.HxcBindgenAbiModel.discoverScalarMacros;
import hxc.bindgen.HxcBindgenAbiModel.enumProbeSource;
import hxc.bindgen.HxcBindgenAbiModel.macroTypeProbeSource;
import hxc.bindgen.HxcBindgenAbiModel.macroValueProbeSource;
import hxc.bindgen.HxcBindgenAbiModel.primitiveProbeSource;
import hxc.bindgen.HxcBindgenOptions.HxcBindgenDefine;
import hxc.bindgen.HxcBindgenOptions.HxcBindgenLanguage;
import hxc.bindgen.HxcBindgenProcess.HxcBindgenProcessResult;
import hxc.bindgen.HxcBindgenProcess.runBindgenDiagnosticProcess;
import hxc.bindgen.HxcBindgenProcess.runBindgenProcess;
import hxc.config.HxcJsonCodec.renderJson;
import hxc.config.HxcJsonParser;
import reflaxe.c.CUtf8Order.compare as compareUtf8;
import sys.FileSystem;
import sys.io.File;

/** Result of one semantic capture, including the optional written lock path. */
class HxcBindgenResult {
	/** Canonical schema-3 lock bytes. */
	public final lockText:String;

	/** Written lock path, or absence for a dry run. */
	public final outputPath:Null<String>;

	/** Non-fatal Clang diagnostics from the semantic parse. */
	public final childStderr:String;

	/** Retain all observable output from one successful capture. */
	public function new(lockText:String, outputPath:Null<String>, childStderr:String) {
		this.lockText = lockText;
		this.outputPath = outputPath;
		this.childStderr = childStderr;
	}
}

/**
	Invoke Clang as the only declaration authority and publish its semantic lock.

	The driver uses argument arrays, captures exact diagnostics, hashes every
	Clang-reported dependency and generated probe, and writes no generated
	externs. This stage maps primitive ABI facts; aggregate layout, functions,
	and module-file emission remain owned by later E6 tasks.
**/
class HxcBindgenDriver {
	/** Capture one translation unit and optionally write its deterministic lock. */
	public static function generate(options:HxcBindgenOptions):HxcBindgenResult {
		final headers = options.entryHeaders.map(path -> requireFile(path, "entry header"));
		final includes = options.includeDirectories.map(path -> requireDirectory(path, "include directory"));
		final sysroot = options.sysroot == null ? null : requireDirectory(options.sysroot, "sysroot");
		requireUniquePaths(headers, "entry header");
		requireUniquePaths(includes, "include directory");
		final defines = normalizedDefines(options.defines);
		final versionResult = requireToolQuery(options.clang, ["--version"], "version");
		final machineResult = requireToolQuery(options.clang, ["-dumpmachine"], "target");
		final resourceResult = requireToolQuery(options.clang, ["-print-resource-dir"], "resource directory");
		final version = firstLine(versionResult.stdout);
		if (version.toLowerCase().indexOf("clang") < 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Unavailable, "HXC-CLI-0802", "selected executable does not identify as Clang",
				"Pass a Clang executable with `--clang <path>`.");
		final dumpMachine = StringTools.trim(machineResult.stdout);
		final target = options.target == null ? dumpMachine : options.target;
		final resourceDirectory = StringTools.trim(resourceResult.stdout);
		if (dumpMachine == "" || target == "" || resourceDirectory == "")
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0804", "Clang omitted required toolchain identity",
				"Check the selected Clang installation and retry.");

		final common = clangArguments(headers, options.language, target, sysroot, includes, defines);
		final diagnosticArguments = common.concat(["-fsyntax-only"]);
		final diagnosticResult = runBindgenDiagnosticProcess(options.clang, diagnosticArguments);
		if (diagnosticResult.exitCode != 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0803", "Clang rejected the bindgen translation unit",
				"Fix the source-positioned Clang diagnostics, then retry with the same target and flags.", diagnosticResult.stderr);
		final astArguments = common.concat(["-w", "-fsyntax-only", "-Xclang", "-ast-dump=json"]);
		final astResult = runBindgenProcess(options.clang, astArguments);
		if (astResult.exitCode != 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0803", "Clang rejected the bindgen translation unit",
				"Fix the source-positioned Clang diagnostics, then retry with the same target and flags.", astResult.stderr);
		final parsed = try {
			new HxcJsonParser(astResult.stdout, "clang-ast.json").parse();
		} catch (_:haxe.Exception) {
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0805", "Clang emitted malformed semantic AST JSON",
				"Check the selected Clang version and report its AST JSON shape.");
		};

		final dependencyArguments = common.concat(["-M", "-MT", "hxc-bindgen-input"]);
		final dependencyResult = runBindgenProcess(options.clang, dependencyArguments);
		if (dependencyResult.exitCode != 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0803", "Clang could not report bindgen dependencies",
				"Fix the source-positioned Clang diagnostics, then retry with the same target and flags.", dependencyResult.stderr);
		final dependencies = parseDependencies(dependencyResult.stdout);
		if (dependencies.length == 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0806", "Clang dependency output is empty",
				"Check the selected Clang version and dependency-output support.");

		final paths = new HxcBindgenPaths(headers, includes, sysroot, resourceDirectory);
		final primitiveProbe = primitiveProbeSource(options.language) + enumProbeSource(parsed, paths);
		final abiArguments = clangProbeArguments(headers, options.language, target, sysroot, includes,
			defines).concat(["-w", "-fsyntax-only", "-Xclang", "-ast-dump=json", "-"]);
		final abiResult = runBindgenProcess(options.clang, abiArguments, primitiveProbe);
		if (abiResult.exitCode != 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0810", "Clang rejected the generated primitive ABI probe",
				"Check the selected target's primitive C model and report the source-positioned Clang diagnostics.", abiResult.stderr);
		final abiAst = try {
			new HxcJsonParser(abiResult.stdout, "clang-primitive-abi.json").parse();
		} catch (_:haxe.Exception) {
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0810", "Clang emitted malformed primitive ABI JSON",
				"Check the selected Clang version and report its AST JSON shape.");
		};
		final macroBaselineArguments = clangProbeArguments([], options.language, target, sysroot, includes, defines).concat(["-dM", "-E", "-"]);
		final macroInventoryArguments = clangProbeArguments(headers, options.language, target, sysroot, includes, defines).concat(["-dM", "-E", "-"]);
		final macroBaseline = requireAbiProcess(options.clang, macroBaselineArguments, "baseline macro inventory", "\n");
		final macroInventory = requireAbiProcess(options.clang, macroInventoryArguments, "configured macro inventory", "\n");
		final macroNames = discoverScalarMacros(macroBaseline.stdout, macroInventory.stdout);
		final macroTypeProbe = macroTypeProbeSource(macroNames);
		final macroTypeArguments = abiArguments.copy();
		final macroTypeResult = requireAbiProcess(options.clang, macroTypeArguments, "macro type probe", macroTypeProbe);
		final macroTypeAst = parseAbiAst(macroTypeResult.stdout, "clang-macro-types.json");
		final macroValueProbe = macroValueProbeSource(macroNames, macroTypeAst);
		final macroValueArguments = abiArguments.copy();
		final macroValueResult = requireAbiProcess(options.clang, macroValueArguments, "macro value probe", macroValueProbe);
		final macroValueAst = parseAbiAst(macroValueResult.stdout, "clang-macro-values.json");
		final primitiveAbiModel = buildPrimitiveAbiModel({
			ast: parsed,
			probeAst: abiAst,
			paths: paths,
			macroNames: macroNames,
			macroTypeAst: macroTypeAst,
			macroValueAst: macroValueAst,
			language: options.language
		});
		final normalizedAst = normalizeClangAst(parsed, paths);
		final lock = buildBindingLock({
			ast: normalizedAst,
			paths: paths,
			clang: options.clang,
			version: version,
			dumpMachine: dumpMachine,
			resourceDirectory: resourceDirectory,
			target: target,
			diagnosticArguments: diagnosticArguments,
			semanticArguments: astArguments,
			dependencyArguments: dependencyArguments,
			abiArguments: abiArguments,
			macroBaselineArguments: macroBaselineArguments,
			macroInventoryArguments: macroInventoryArguments,
			macroTypeArguments: macroTypeArguments,
			macroValueArguments: macroValueArguments,
			primitiveProbe: primitiveProbe,
			macroTypeProbe: macroTypeProbe,
			macroValueProbe: macroValueProbe,
			dependencies: dependencies,
			primitiveAbiModel: primitiveAbiModel,
			language: options.language,
			languageExplicit: options.languageExplicit,
			targetExplicit: options.target != null,
			sysroot: sysroot,
			defines: defines
		});
		final lockText = renderJson(lock, true) + "\n";
		if (options.dryRun)
			return new HxcBindgenResult(lockText, null, diagnosticResult.stderr);
		final outputDirectory = ensureDirectory(options.outputDirectory);
		final outputPath = Path.join([outputDirectory, "hxc.bindings.lock.json"]);
		if (FileSystem.exists(outputPath) && FileSystem.isDirectory(outputPath))
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0807", "binding lock path is an existing directory",
				"Choose a different `--output` directory.");
		try {
			File.saveContent(outputPath, lockText);
		} catch (_:haxe.Exception) {
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0807", "binding lock could not be written",
				"Check the output directory permissions and retry.");
		}
		return new HxcBindgenResult(lockText, outputPath, diagnosticResult.stderr);
	}

	static function clangArguments(headers:Array<String>, language:HxcBindgenLanguage, target:String, sysroot:Null<String>, includes:Array<String>,
			defines:Array<HxcBindgenDefine>):Array<String> {
		final arguments = clangConfigurationArguments(language, target, sysroot, includes, defines);
		for (index in 0...(headers.length - 1)) {
			arguments.push("-include");
			arguments.push(headers[index]);
		}
		arguments.push(headers[headers.length - 1]);
		return arguments;
	}

	static function clangProbeArguments(headers:Array<String>, language:HxcBindgenLanguage, target:String, sysroot:Null<String>, includes:Array<String>,
			defines:Array<HxcBindgenDefine>):Array<String> {
		final arguments = clangConfigurationArguments(language, target, sysroot, includes, defines);
		for (header in headers) {
			arguments.push("-include");
			arguments.push(header);
		}
		return arguments;
	}

	static function clangConfigurationArguments(language:HxcBindgenLanguage, target:String, sysroot:Null<String>, includes:Array<String>,
			defines:Array<HxcBindgenDefine>):Array<String> {
		final arguments:Array<String> = ["-x", language, "-target", target, "-fno-color-diagnostics", "-ferror-limit=20"];
		if (sysroot != null)
			arguments.push("--sysroot=" + sysroot);
		for (includeDirectory in includes)
			arguments.push("-I" + includeDirectory);
		for (define in defines)
			arguments.push("-D" + define.spelling());
		return arguments;
	}

	static function requireAbiProcess(clang:String, arguments:Array<String>, label:String, input:String):HxcBindgenProcessResult {
		final result = runBindgenProcess(clang, arguments, input);
		if (result.exitCode != 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0810", 'Clang rejected the generated $label',
				"Check the selected target's primitive C model and report the source-positioned Clang diagnostics.", result.stderr);
		return result;
	}

	static function parseAbiAst(text:String, label:String):hxc.config.HxcJsonValue.HxcJsonNode {
		return try {
			new HxcJsonParser(text, label).parse();
		} catch (_:haxe.Exception) {
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Internal, "HXC-CLI-0810", "Clang emitted malformed primitive ABI JSON",
				"Check the selected Clang version and report its AST JSON shape.");
		};
	}

	static function requireUniquePaths(paths:Array<String>, label:String):Void {
		final seen:Map<String, Bool> = [];
		for (path in paths) {
			final key = StringTools.replace(path, "\\", "/");
			if (seen.exists(key))
				throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Usage, "HXC-CLI-0801", 'duplicate $label `$path`',
					'Remove the repeated $label; configured order remains significant.');
			seen.set(key, true);
		}
	}

	static function normalizedDefines(defines:Array<HxcBindgenDefine>):Array<HxcBindgenDefine> {
		final seen:Map<String, Bool> = [];
		final result = defines.copy();
		for (define in result) {
			if (seen.exists(define.name))
				throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Usage, "HXC-CLI-0801", 'repeated preprocessor definition `${define.name}`',
					"Pass each definition name once so its effective value is unambiguous.");
			seen.set(define.name, true);
		}
		result.sort((left, right) -> compareUtf8(left.name, right.name));
		return result;
	}

	static function requireToolQuery(clang:String, arguments:Array<String>, fact:String):HxcBindgenProcessResult {
		final result = runBindgenProcess(clang, arguments);
		if (result.exitCode != 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Unavailable, "HXC-CLI-0802", 'Clang did not report its $fact',
				"Check the selected Clang installation and retry.", result.stderr);
		return result;
	}

	static function requireFile(path:String, label:String):String {
		final full = normalizePath(path, label);
		if (!FileSystem.exists(full) || FileSystem.isDirectory(full))
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Usage, "HXC-CLI-0801", '$label `$path` is not a regular file',
				"Pass one readable C header path.");
		return full;
	}

	static function requireDirectory(path:String, label:String):String {
		final full = normalizePath(path, label);
		if (!FileSystem.exists(full) || !FileSystem.isDirectory(full))
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Usage, "HXC-CLI-0801", '$label `$path` is not a directory',
				"Pass an existing include directory.");
		return full;
	}

	static function normalizePath(path:String, label:String):String {
		if (StringTools.trim(path) == "" || path.indexOf("\x00") >= 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Usage, "HXC-CLI-0801", '$label path is empty or invalid', "Pass a non-empty filesystem path.");
		return try FileSystem.fullPath(path) catch (_:haxe.Exception) {
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Usage, "HXC-CLI-0801", '$label path cannot be normalized', "Pass a valid filesystem path.");
		};
	}

	static function ensureDirectory(path:String):String {
		if (StringTools.trim(path) == "" || path.indexOf("\x00") >= 0)
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Usage, "HXC-CLI-0801", "output path is empty or invalid", "Pass a non-empty filesystem path.");
		final full = Path.normalize(isAbsolute(path) ? path : Path.join([Sys.getCwd(), path]));
		if (FileSystem.exists(full)) {
			if (!FileSystem.isDirectory(full))
				throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0807", "bindgen output is not a directory",
					"Choose a directory path with `--output`.");
			return full;
		}
		try {
			FileSystem.createDirectory(full);
		} catch (_:haxe.Exception) {
			throw new HxcBindgenError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0807", "bindgen output directory could not be created",
				"Create its parent directory or choose another `--output` path.");
		}
		return full;
	}

	static function isAbsolute(path:String):Bool
		return StringTools.startsWith(path, "/") || ~/^[A-Za-z]:[\\\/]/.match(path);

	static function firstLine(text:String):String {
		final newline = text.indexOf("\n");
		return StringTools.trim(newline < 0 ? text : text.substr(0, newline));
	}

	/** Parse Clang's Make dependency stream; this is input inventory, never ABI authority. */
	static function parseDependencies(text:String):Array<String> {
		final marker = "hxc-bindgen-input:";
		final start = text.indexOf(marker);
		if (start < 0)
			return [];
		final values:Array<String> = [];
		final token = new StringBuf();
		var index = start + marker.length;
		var hasToken = false;
		while (index < text.length) {
			final code = text.charCodeAt(index);
			if (code == 0x5C && index + 1 < text.length) {
				final next = text.charCodeAt(index + 1);
				if (next == 0x0A) {
					index += 2;
					continue;
				}
				token.addChar(next);
				hasToken = true;
				index += 2;
				continue;
			}
			if (code == 0x20 || code == 0x09 || code == 0x0A || code == 0x0D) {
				if (hasToken) {
					values.push(token.toString());
					token.clear();
					hasToken = false;
				}
				index++;
				continue;
			}
			token.addChar(code);
			hasToken = true;
			index++;
		}
		if (hasToken)
			values.push(token.toString());
		return values;
	}
}
