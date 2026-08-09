package caxecraft.tools;

import caxecraft.pilot.PilotCatalog.PilotExecutionKind;
import caxecraft.pilot.PilotCatalog.PilotMetadata;
import caxecraft.pilot.PilotCatalog.allPilotMetadata;
import haxe.crypto.Sha256;
import sys.FileSystem;
import sys.io.File;

/**
 * Checks or refreshes the host manifest derived from typed Haxe pilot metadata.
 *
 * The command does not build or run Caxecraft. It renders deterministic JSON
 * from `PilotCatalog`, then either compares the committed bytes or replaces
 * them on an explicit `--write` request.
 */
final class PilotCatalogMain {
	static inline final OUTPUT_PATH = "pilot-catalog.json";
	static inline final AUTHORITY_PATH = "src/caxecraft/pilot/PilotCatalog.hx";

	/** Parse the closed command surface and return a process status. */
	public static function main():Void {
		final arguments = Sys.args();
		if (arguments.length > 1 || (arguments.length == 1 && arguments[0] != "--check" && arguments[0] != "--write")) {
			Sys.stderr().writeString("usage: PilotCatalogMain [--check|--write]\n");
			Sys.exit(2);
		}
		final write = arguments.length == 1 && arguments[0] == "--write";
		final metadata = allPilotMetadata();
		final validation = validate(metadata);
		if (validation != null) {
			Sys.stderr().writeString('caxecraft-pilot-catalog: $validation\n');
			Sys.exit(2);
		}
		final authoritySha256 = Sha256.make(File.getBytes(AUTHORITY_PATH)).toHex();
		final expected = render(metadata, authoritySha256);
		if (write) {
			if (!FileSystem.exists(OUTPUT_PATH) || File.getContent(OUTPUT_PATH) != expected)
				File.saveContent(OUTPUT_PATH, expected);
			Sys.println('caxecraft-pilot-catalog: wrote ${metadata.length} Haxe-owned pilot records');
			return;
		}
		if (!FileSystem.exists(OUTPUT_PATH)) {
			Sys.stderr().writeString("caxecraft-pilot-catalog: pilot-catalog.json is missing; run npm run caxecraft:pilot-catalog:refresh\n");
			Sys.exit(1);
		}
		if (File.getContent(OUTPUT_PATH) != expected) {
			Sys.stderr().writeString("caxecraft-pilot-catalog: pilot-catalog.json is stale; run npm run caxecraft:pilot-catalog:refresh\n");
			Sys.exit(1);
		}
		Sys.println('caxecraft-pilot-catalog: OK: ${metadata.length} Haxe-owned pilot records');
	}

	/** Reject duplicate or unsafe host records before publishing bytes. */
	static function validate(metadata:Array<PilotMetadata>):Null<String> {
		if (metadata.length == 0)
			return "catalog is empty";
		final ids:Map<String, Bool> = [];
		final defines:Map<String, Bool> = [];
		final screenshots:Map<String, Bool> = [];
		for (pilot in metadata) {
			if (!safeToken(pilot.id, false))
				return 'pilot ID is unsafe: ${pilot.id}';
			if (!safeToken(pilot.haxeDefine, true))
				return 'pilot define is unsafe: ${pilot.haxeDefine}';
			if (!safeScreenshot(pilot.screenshot))
				return 'pilot screenshot is unsafe: ${pilot.screenshot}';
			if (pilot.scriptCode < 0)
				return 'pilot ${pilot.id} has a negative script code';
			if (pilot.frameLimit < 2)
				return 'pilot ${pilot.id} has an unsafe frame limit';
			if (ids.exists(pilot.id))
				return 'duplicate pilot ID: ${pilot.id}';
			if (defines.exists(pilot.haxeDefine))
				return 'duplicate pilot define: ${pilot.haxeDefine}';
			if (screenshots.exists(pilot.screenshot))
				return 'duplicate pilot screenshot: ${pilot.screenshot}';
			ids.set(pilot.id, true);
			defines.set(pilot.haxeDefine, true);
			screenshots.set(pilot.screenshot, true);
		}
		return null;
	}

	/** Render stable, reviewable JSON without an untyped serialization boundary. */
	static function render(metadata:Array<PilotMetadata>, authoritySha256:String):String {
		final output = new StringBuf();
		output.add('{\n  "schemaVersion": 1,\n  "authoritySha256": "$authoritySha256",\n  "pilots": [\n');
		for (index in 0...metadata.length) {
			final pilot = metadata[index];
			output.add("    {\n");
			output.add('      "id": "${pilot.id}",\n');
			output.add('      "scriptCode": ${pilot.scriptCode},\n');
			output.add('      "frameLimit": ${pilot.frameLimit},\n');
			output.add('      "haxeDefine": "${pilot.haxeDefine}",\n');
			output.add('      "screenshot": "${pilot.screenshot}",\n');
			output.add('      "execution": "${executionName(pilot.execution)}"\n');
			output.add(index + 1 == metadata.length ? "    }\n" : "    },\n");
		}
		output.add("  ]\n}\n");
		return output.toString();
	}

	/** Convert the closed execution kind to its stable manifest spelling. */
	static function executionName(kind:PilotExecutionKind):String
		return switch kind {
			case Compiled: "compiled";
			case RuntimeContent: "runtime-content";
		};

	/** Accept lowercase command IDs or underscore-separated Haxe defines. */
	static function safeToken(value:String, underscore:Bool):Bool {
		if (value.length == 0 || value.length > 80)
			return false;
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			final letter = code >= "a".code && code <= "z".code;
			final digit = code >= "0".code && code <= "9".code;
			if (!letter && !digit && code != (underscore ? "_".code : "-".code))
				return false;
		}
		return true;
	}

	/** Keep every review image inside the executable directory. */
	static function safeScreenshot(value:String):Bool {
		if (!StringTools.endsWith(value, ".png") || value.length > 96)
			return false;
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			final letter = code >= "a".code && code <= "z".code;
			final digit = code >= "0".code && code <= "9".code;
			if (!letter && !digit && code != "-".code && code != ".".code)
				return false;
		}
		return true;
	}
}
