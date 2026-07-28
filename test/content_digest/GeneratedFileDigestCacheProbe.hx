import haxe.Json;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.emit.GeneratedFileDigestCache;
import sys.io.File;

/**
	Exercises warm generated-file digest reuse inside Haxe's macro server.

	Each invocation is a separate compiler request, while `@:persistent` state
	in the production cache may survive between requests. The Python owner
	chooses a small mutation sequence and independently checks every returned
	SHA-256 digest, cache count, disabled-cache result, and aborted-request
	recovery.
**/
class GeneratedFileDigestCacheProbe {
	/**
		Run one cache request and save its path-free evidence for the test owner.

		The `abort-baseline` mode deliberately withholds `completeRequest`; the
		next request must still hit the generation completed before it.
	**/
	public static function run(mode:String, reportPath:String):Void {
		final enabled = mode != "disabled";
		GeneratedFileDigestCache.beginRequest(enabled);
		final files = fixtureFiles(mode);
		if (mode == "abort-baseline") {
			GeneratedFileDigestCache.abortRequest();
			File.saveContent(reportPath, Json.stringify({
				mode: mode,
				aborted: true,
				files: fileEvidence(files)
			}));
			return;
		}

		final stats = GeneratedFileDigestCache.completeRequest(files);
		File.saveContent(reportPath, Json.stringify({
			mode: mode,
			aborted: false,
			stats: stats,
			files: fileEvidence(files)
		}));
	}

	static function fixtureFiles(mode:String):Array<GeneratedFile> {
		final contentText = switch mode {
			case "baseline" | "abort-baseline": "alpha-content\n";
			case "changed-content" | "changed-kind" | "changed-path" | "disabled": "beta-content\n";
			case _: throw 'unknown generated-file digest cache probe mode `$mode`';
		};
		final kind = switch mode {
			case "changed-kind" | "changed-path" | "disabled": GeneratedFileKind.PrivateHeader;
			case _: GeneratedFileKind.Source;
		};
		final firstPath = switch mode {
			case "changed-path" | "disabled": "src/renamed.c";
			case _: "src/stable.c";
		};
		return [
			new GeneratedFile(firstPath, "stable\n", GeneratedFileKind.Source),
			new GeneratedFile("src/content.c", contentText, GeneratedFileKind.Source),
			new GeneratedFile("src/kind.c", "kind\n", kind)
		];
	}

	static function fileEvidence(files:Array<GeneratedFile>):Array<{
		relativePath:String,
		contents:String,
		kind:String,
		sha256:String,
		byteLength:Int
	}> {
		return files.map(file -> {
			relativePath: file.relativePath,
			contents: file.contents,
			kind: Std.string(file.kind),
			sha256: file.contentSha256,
			byteLength: file.contentByteLength
		});
	}
}
