package hxc.project;

#if macro
import haxe.io.Path;
import haxe.macro.Context;
import haxe.macro.Expr;
import sys.FileSystem;
import sys.io.File;
#end

/**
	Embeds the reviewed project-template catalog for installation-independent use.

	The tab-separated manifest and its text files are the only authored template
	facts. The macro validates and embeds them so `haxelib run` can create a
	project from any working directory without a runtime repository-path lookup.
**/
class HxcNewTemplateCatalog {
	/** Return every manifest-owned template file in deterministic manifest order. */
	public static macro function embedded():ExprOf<Array<HxcNewTemplateFile>> {
		final manifestPath = Context.resolvePath("hxc/project/template-data/manifest.tsv");
		register(manifestPath);
		final text = File.getContent(manifestPath);
		if (!StringTools.endsWith(text, "\n")) {
			Context.error("hxc new template manifest must end with one line terminator", Context.currentPos());
		}
		final lines = text.split("\n");
		if (lines.shift() != "hxc-new-manifest-v1") {
			Context.error("hxc new template manifest requires hxc-new-manifest-v1", Context.currentPos());
		}

		final root = Path.directory(manifestPath);
		final seen:Map<String, Bool> = [];
		final counts:Map<String, Int> = ["app" => 0, "library" => 0, "embedded" => 0];
		final expressions:Array<Expr> = [];
		for (lineIndex in 0...lines.length) {
			final lineNumber = lineIndex + 2;
			final line = lines[lineIndex];
			if (line == "") {
				continue;
			}
			final fields = line.split("\t");
			if (fields.length != 3) {
				Context.error('hxc new template manifest line $lineNumber must have kind, output path, and source path', Context.currentPos());
			}
			final kind = fields[0];
			if (kind != "app" && kind != "library" && kind != "embedded") {
				Context.error('hxc new template manifest line $lineNumber has unknown kind `$kind`', Context.currentPos());
			}
			final outputPath = fields[1];
			validateRelative(outputPath, "output", lineNumber);
			final sourcePath = fields[2];
			validateRelative(sourcePath, "source", lineNumber);
			final key = kind + "\u0000" + outputPath;
			if (seen.exists(key)) {
				Context.error('hxc new template manifest repeats `$kind` path `$outputPath`', Context.currentPos());
			}
			seen.set(key, true);
			final priorCount = counts.get(kind);
			if (priorCount == null)
				Context.error('hxc new template manifest cannot count unknown kind `$kind`', Context.currentPos());
			counts.set(kind, priorCount + 1);

			final absoluteSource = Path.join([root, sourcePath]);
			if (!FileSystem.exists(absoluteSource) || FileSystem.isDirectory(absoluteSource)) {
				Context.error('hxc new template source does not exist: `$sourcePath`', Context.currentPos());
			}
			register(absoluteSource);
			final contents = File.getContent(absoluteSource);
			validatePlaceholders(outputPath, lineNumber);
			validatePlaceholders(contents, lineNumber);
			final kindExpression = switch kind {
				case "app": macro HxcNewKind.App;
				case "library": macro HxcNewKind.Library;
				case "embedded": macro HxcNewKind.Embedded;
				case _: macro HxcNewKind.App;
			};
			expressions.push(macro new HxcNewTemplateFile($kindExpression, $v{outputPath}, $v{contents}));
		}
		for (kind in ["app", "library", "embedded"]) {
			final count = counts.get(kind);
			if (count == null || count == 0)
				Context.error('hxc new template manifest has no `$kind` files', Context.currentPos());
		}
		return macro $a{expressions};
	}

	#if macro
	static function register(path:String):Void
		Context.registerModuleDependency(Context.getLocalModule(), path);

	static function validateRelative(value:String, label:String, lineNumber:Int):Void {
		if (value == "" || value.indexOf("\\") >= 0 || StringTools.startsWith(value, "/") || Path.normalize(value) != value) {
			Context.error('hxc new template $label path on line $lineNumber must be a normalized POSIX relative path', Context.currentPos());
		}
		for (part in value.split("/")) {
			if (part == "" || part == "." || part == "..") {
				Context.error('hxc new template $label path on line $lineNumber contains an unsafe segment', Context.currentPos());
			}
		}
	}

	static function validatePlaceholders(value:String, lineNumber:Int):Void {
		var start = value.indexOf("${");
		while (start >= 0) {
			final end = value.indexOf("}", start + 2);
			if (end < 0) {
				Context.error('hxc new template line $lineNumber contains an unterminated placeholder', Context.currentPos());
			}
			final name = value.substring(start + 2, end);
			if (name != "PROJECT_NAME" && name != "MODULE_NAME" && name != "LICENSE") {
				Context.error('hxc new template line $lineNumber contains unknown placeholder `$name`', Context.currentPos());
			}
			start = value.indexOf("${", end + 1);
		}
	}
	#end
}
