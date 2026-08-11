package caxecraft.tools;

import caxecraft.editor.EditorAutomationProtocol.EditorAutomationReadResult;
import caxecraft.editor.EditorAutomationProtocol.editorAutomationError;
import caxecraft.editor.EditorAutomationProtocol.editorAutomationReady;
import caxecraft.editor.EditorAutomationProtocol.readEditorAutomationRequest;
import caxecraft.editor.EditorAutomationProtocol.runEditorAutomationRequest;
import caxecraft.editor.EditorPackageSession;
import caxecraft.editor.EditorPackageSession.EditorPackageOpenResult;
import haxe.io.Bytes;
import haxe.io.Eof;

/**
 * Keeps one real CAXEMAP open while standard input supplies JSON commands.
 *
 * This local process gives text tools and agents the same editor operations as
 * the visual screen. It accepts one JSON object per line and writes one JSON
 * response per line. It does not compile Caxecraft or open network access.
 */
final class EditorSessionMain {
	/** Open the requested level and serve commands until input ends or `quit`. */
	public static function main():Void {
		final options = parseOptions(Sys.args());
		if (options == null)
			Sys.exit(2);
		final editor = switch EditorPackageSession.open(options.root, options.manifest, options.level) {
			case EditorPackageOpenRejected(error):
				write(editorAutomationError(Std.string(error)));
				Sys.exit(2);
				return;
			case EditorPackageOpened(value): value;
		};
		write(editorAutomationReady(options.level, editor.revision()));
		while (true) {
			final line = try Sys.stdin().readLine() catch (_:Eof) return;
			if (line.length == 0)
				continue;
			final request = switch readEditorAutomationRequest(Bytes.ofString(line)) {
				case AutomationRequestRejected(detail):
					write(editorAutomationError(detail));
					continue;
				case AutomationRequestReady(value): value;
			};
			final result = runEditorAutomationRequest(editor, request);
			write(result.json);
			if (result.quit)
				return;
		}
	}

	/** Parse the package root, manifest, and required level path. */
	static function parseOptions(arguments:Array<String>):Null<EditorSessionOptions> {
		var root = ".";
		var manifest = "caxecraft.package.json";
		var level:Null<String> = null;
		var index = 0;
		while (index < arguments.length) {
			final name = arguments[index];
			if (name != "--package-root" && name != "--manifest" && name != "--level")
				return usage('unknown argument: $name');
			index++;
			if (index >= arguments.length)
				return usage('$name requires a value');
			if (name == "--package-root")
				root = arguments[index];
			else if (name == "--manifest")
				manifest = arguments[index];
			else
				level = arguments[index];
			index++;
		}
		return level == null ? usage("--level is required") : {root: root, manifest: manifest, level: level};
	}

	/** Show one command-line error without mixing it with JSON responses. */
	static function usage(detail:String):Null<EditorSessionOptions> {
		Sys.stderr().writeString('caxecraft-editor: $detail\n');
		Sys.stderr().writeString("usage: EditorSessionMain [--package-root DIR] [--manifest PATH] --level PATH\n");
		return null;
	}

	/** Write one complete JSON response and flush it for a persistent client. */
	static function write(value:String):Void {
		Sys.stdout().writeString(value + "\n");
		Sys.stdout().flush();
	}
}

/** Package location selected before one local editor session starts. */
private typedef EditorSessionOptions = {
	final root:String;
	final manifest:String;
	final level:String;
}
