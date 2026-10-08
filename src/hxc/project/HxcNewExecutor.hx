package hxc.project;

import haxe.Exception;
import haxe.io.Path;
import hxc.cli.HxcCliCommand;
import hxc.cli.HxcCliDiagnostic;
import hxc.cli.HxcCliExecution;
import hxc.cli.HxcCliExecutor;
import hxc.cli.HxcCliExitCategory;
import hxc.cli.HxcCliRequest;
import hxc.config.HxcProjectConfig;
import sys.FileSystem;
import sys.io.File;

/**
	Create one deterministic starter project without surprising overwrites.

	Parsing and preflight finish before the first write. Default mode requires a
	new directory, merge creates only missing template files, and force replaces
	only manifest-owned files while preserving unrelated user content.
**/
class HxcNewExecutor implements HxcCliExecutor {
	static final PROJECT_PATTERN = ~/^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$/;
	static final MODULE_PATTERN = ~/^[A-Z][A-Za-z0-9_]*$/;
	static final RESERVED_MODULES = [
		"Any",
		"Array",
		"Bool",
		"Class",
		"Date",
		"Dynamic",
		"EReg",
		"Enum",
		"EnumValue",
		"Float",
		"Int",
		"IntIterator",
		"Iterable",
		"Iterator",
		"KeyValue",
		"KeyValueIterator",
		"Lambda",
		"List",
		"Map",
		"Math",
		"Never",
		"Null",
		"Reflect",
		"Single",
		"Std",
		"String",
		"StringBuf",
		"StringTools",
		"Sys",
		"Type",
		"UInt",
		"UnicodeString",
		"Vector",
		"Void",
		"Xml"
	];
	static final LICENSES = ["UNLICENSED", "Apache-2.0", "BSD-3-Clause", "GPL-3.0-only", "MIT", "MPL-2.0"];

	final templates:Array<HxcNewTemplateFile>;

	/** Embed the reviewed data catalog once when the CLI is compiled. */
	public function new() {
		templates = HxcNewTemplateCatalog.embedded();
	}

	/** Report only `new`; other command owners remain independent. */
	public function isAvailable(command:HxcCliCommand):Bool
		return command == HxcCliCommand.New;

	/** Parse, preflight, and materialize one project request. */
	public function execute(request:HxcCliRequest):HxcCliExecution {
		try {
			final options = parse(request.arguments);
			return create(options);
		} catch (failure:HxcNewFailure) {
			return failed(failure.category, failure.code, failure.message, failure.remediation);
		} catch (failure:Exception) {
			return failed(HxcCliExitCategory.Internal, "HXC-CLI-0299", "project creation failed unexpectedly",
				"Check filesystem access and retry. If the failure repeats, report the command and diagnostic.");
		}
	}

	function parse(arguments:Array<String>):HxcNewOptions {
		if (arguments.length == 0) {
			usage("project name is required", "Run `hxc new <name>`.");
		}
		var name:Null<String> = null;
		var kind:HxcNewKind = HxcNewKind.App;
		var module:Null<String> = null;
		var license = "UNLICENSED";
		var mode:HxcNewWriteMode = HxcNewWriteMode.NewDirectory;
		var kindSeen = false;
		var moduleSeen = false;
		var licenseSeen = false;
		var index = 0;
		while (index < arguments.length) {
			final argument = arguments[index];
			switch argument {
				case "--kind":
					if (kindSeen)
						usage("`--kind` may appear only once", "Keep one project kind.");
					final value = optionValue(arguments, index, "--kind");
					final parsed = HxcNewKind.parse(value);
					if (parsed == null)
						usage('invalid project kind `$value`', "Use app, library, or embedded.");
					kind = parsed;
					kindSeen = true;
					index += 2;
				case "--module":
					if (moduleSeen)
						usage("`--module` may appear only once", "Keep one Haxe module name.");
					module = optionValue(arguments, index, "--module");
					moduleSeen = true;
					index += 2;
				case "--license":
					if (licenseSeen)
						usage("`--license` may appear only once", "Keep one SPDX license identifier.");
					license = optionValue(arguments, index, "--license");
					licenseSeen = true;
					index += 2;
				case "--merge":
					if (mode != HxcNewWriteMode.NewDirectory)
						usage("`--merge` and `--force` are mutually exclusive", "Choose one write mode.");
					mode = HxcNewWriteMode.Merge;
					index++;
				case "--force":
					if (mode != HxcNewWriteMode.NewDirectory)
						usage("`--merge` and `--force` are mutually exclusive", "Choose one write mode.");
					mode = HxcNewWriteMode.Force;
					index++;
				case value if (StringTools.startsWith(value, "-")):
					usage('unknown hxc new option `$value`', "Use `hxc help new` to list admitted options.");
				case value:
					if (name != null)
						usage('unexpected project argument `$value`', "Provide exactly one project name.");
					name = value;
					index++;
			}
		}

		if (name == null)
			usage("project name is required", "Run `hxc new <name>`.");
		if (name.length > 64 || !PROJECT_PATTERN.match(name) || isPortableDeviceName(name)) {
			usage('invalid project name `$name`',
				"Use at most 64 lowercase letters, digits, and single hyphens, starting with a letter; avoid reserved device names.");
		}
		final selectedModule = module == null ? defaultModule(name) : module;
		if (selectedModule.length > 128
			|| !MODULE_PATTERN.match(selectedModule)
			|| RESERVED_MODULES.indexOf(selectedModule) >= 0
			|| isPortableDeviceName(selectedModule)) {
			usage('invalid Haxe module name `$selectedModule`',
				"Use an uppercase Haxe identifier of at most 128 characters that is not a built-in type or reserved device name.");
		}
		if (LICENSES.indexOf(license) < 0) {
			usage('unsupported license identifier `$license`', 'Use one of: ${LICENSES.join(", ")}');
		}
		return new HxcNewOptions(name, kind, selectedModule, license, mode);
	}

	function create(options:HxcNewOptions):HxcCliExecution {
		final target = Path.join([Sys.getCwd(), options.name]);
		final targetExists = FileSystem.exists(target);
		if (targetExists)
			rejectLink(target, options.name);
		if (targetExists && !FileSystem.isDirectory(target)) {
			conflict('target `${options.name}` exists and is not a directory', "Choose another project name or move the existing file.");
		}
		if (targetExists && options.mode == HxcNewWriteMode.NewDirectory) {
			conflict('target directory `${options.name}` already exists', "Use --merge to preserve existing files or --force to replace template-owned files.");
		}

		final rendered:Array<HxcNewRenderedFile> = [];
		for (template in templates) {
			if (template.kind != options.kind)
				continue;
			final relative = substitute(template.path, options);
			if (!safeTemplatePath(relative)) {
				throw new HxcNewFailure(HxcCliExitCategory.Internal, "HXC-CLI-0299", 'embedded template produced unsafe path `$relative`',
					"Report the installed hxc version and template kind.");
			}
			rendered.push(new HxcNewRenderedFile(relative, substitute(template.contents, options)));
		}
		rendered.sort(function(left, right) return HxcProjectConfig.compareUtf8(left.path, right.path));
		if (rendered.length == 0) {
			throw new HxcNewFailure(HxcCliExitCategory.Internal, "HXC-CLI-0299", 'no embedded files exist for `${options.kind}`',
				"Reinstall hxc from a complete package.");
		}

		var preserved = 0;
		for (entry in rendered) {
			final output = Path.join([target, entry.path]);
			if (FileSystem.exists(output)) {
				rejectLink(output, entry.path);
				if (FileSystem.isDirectory(output)) {
					conflict('template file `${entry.path}` is an existing directory', "Move it, or choose another project name.");
				}
				if (options.mode == HxcNewWriteMode.Merge)
					preserved++;
			}
			preflightParents(target, entry.path);
		}

		if (!targetExists)
			FileSystem.createDirectory(target);
		for (entry in rendered) {
			final output = Path.join([target, entry.path]);
			if (options.mode == HxcNewWriteMode.Merge && FileSystem.exists(output))
				continue;
			createParents(target, entry.path);
			File.saveContent(output, entry.contents);
		}
		final suffix = preserved == 0 ? "" : '; preserved $preserved existing template file' + (preserved == 1 ? "" : "s");
		return new HxcCliExecution(HxcCliExitCategory.Success, 0,
			'Created ${options.kind} project `${options.name}` in ${options.name}/ (${rendered.length} template files$suffix).\n', "");
	}

	static function optionValue(arguments:Array<String>, index:Int, option:String):String {
		if (index + 1 >= arguments.length || StringTools.startsWith(arguments[index + 1], "-")) {
			usage('option `$option` requires a value', "Provide the value immediately after the option.");
		}
		return arguments[index + 1];
	}

	static function defaultModule(name:String):String {
		final output = new StringBuf();
		for (part in name.split("-")) {
			output.add(part.substring(0, 1).toUpperCase());
			output.add(part.substring(1));
		}
		return output.toString();
	}

	static function isPortableDeviceName(value:String):Bool {
		final upper = value.toUpperCase();
		if (upper == "CON" || upper == "PRN" || upper == "AUX" || upper == "NUL")
			return true;
		return ~/^(?:COM|LPT)[1-9]$/.match(upper);
	}

	static function substitute(value:String, options:HxcNewOptions):String {
		var result = StringTools.replace(value, "${PROJECT_NAME}", options.name);
		result = StringTools.replace(result, "${MODULE_NAME}", options.module);
		return StringTools.replace(result, "${LICENSE}", options.license);
	}

	static function safeTemplatePath(value:String):Bool {
		if (value == "" || value.indexOf("\\") >= 0 || StringTools.startsWith(value, "/") || Path.normalize(value) != value)
			return false;
		for (part in value.split("/"))
			if (part == "" || part == "." || part == "..")
				return false;
		return true;
	}

	static function preflightParents(target:String, relative:String):Void {
		final parts = relative.split("/");
		var current = target;
		for (index in 0...parts.length - 1) {
			current = Path.join([current, parts[index]]);
			if (FileSystem.exists(current)) {
				final logical = parts.slice(0, index + 1).join("/");
				rejectLink(current, logical);
				if (!FileSystem.isDirectory(current)) {
					conflict('template directory `$logical` is an existing file', "Move it, or choose another project name.");
				}
			}
		}
	}

	static function rejectLink(path:String, logical:String):Void {
		final lexical = Path.normalize(FileSystem.absolutePath(path));
		final resolved = Path.normalize(FileSystem.fullPath(path));
		if (lexical != resolved) {
			conflict('existing path `$logical` resolves through a symbolic link',
				"Replace the link with a real file or directory before using --merge or --force.");
		}
	}

	static function createParents(target:String, relative:String):Void {
		final parts = relative.split("/");
		var current = target;
		for (index in 0...parts.length - 1) {
			current = Path.join([current, parts[index]]);
			if (!FileSystem.exists(current))
				FileSystem.createDirectory(current);
		}
	}

	static function usage<T>(message:String, remediation:String):T
		throw new HxcNewFailure(HxcCliExitCategory.Usage, "HXC-CLI-0201", message, remediation);

	static function conflict<T>(message:String, remediation:String):T
		throw new HxcNewFailure(HxcCliExitCategory.Command, "HXC-CLI-0202", message, remediation);

	static function failed(category:HxcCliExitCategory, code:String, message:String, remediation:String):HxcCliExecution
		return new HxcCliExecution(category, category.code(), "", "", null, null, [new HxcCliDiagnostic(code, message, remediation)]);
}

/** Parsed and validated request used after command-line input is no longer raw. */
private class HxcNewOptions {
	public final name:String;
	public final kind:HxcNewKind;
	public final module:String;
	public final license:String;
	public final mode:HxcNewWriteMode;

	public function new(name:String, kind:HxcNewKind, module:String, license:String, mode:HxcNewWriteMode) {
		this.name = name;
		this.kind = kind;
		this.module = module;
		this.license = license;
		this.mode = mode;
	}
}

/** Explicit policy for an already-existing target directory. */
private enum HxcNewWriteMode {
	NewDirectory;
	Merge;
	Force;
}

/** One fully substituted output file ready for deterministic preflight. */
private class HxcNewRenderedFile {
	public final path:String;
	public final contents:String;

	public function new(path:String, contents:String) {
		this.path = path;
		this.contents = contents;
	}
}

/** Typed command failure that preserves a stable category and remediation. */
private class HxcNewFailure extends Exception {
	public final category:HxcCliExitCategory;
	public final code:String;
	public final remediation:String;

	public function new(category:HxcCliExitCategory, code:String, message:String, remediation:String) {
		super(message);
		this.category = category;
		this.code = code;
		this.remediation = remediation;
	}
}
