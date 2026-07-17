package reflaxe.c.emit;

import haxe.crypto.Sha256;

/** Stable artifact roles admitted by the project emitter. */
enum abstract GeneratedFileKind(String) to String {
	var PublicHeader = "public-header";
	var PrivateHeader = "private-header";
	var Source = "source";
	var RuntimeHeader = "runtime-header";
	var RuntimeSource = "runtime-source";
	var CompilerManifest = "compiler-manifest";
	var RuntimePlan = "runtime-plan";
	var AbiManifest = "abi-manifest";
	var SymbolTable = "symbol-table";
	var StdlibReport = "stdlib-report";
	var SpecializationReport = "specialization-report";
	var InitializationPlan = "initialization-plan";
	var CMakeAdapter = "cmake-adapter";
	var MesonAdapter = "meson-adapter";
}

/** A validated, content-addressed, output-root-relative compiler artifact. */
class GeneratedFile {
	public static inline final OWNERSHIP_MANIFEST = "_GeneratedFiles.json";

	public final relativePath:String;
	public final contents:String;
	public final kind:GeneratedFileKind;
	public final contentSha256:String;

	public function new(relativePath:String, contents:String, kind:GeneratedFileKind) {
		if (!isNormalizedRelativePath(relativePath) || relativePath == OWNERSHIP_MANIFEST) {
			fail('generated artifact path must be normalized, output-root-relative, and must not reserve `$OWNERSHIP_MANIFEST`: `$relativePath`',
				[relativePath]);
		}
		if (contents.indexOf("\x00") != -1 || contents.indexOf("\r") != -1) {
			fail('generated text must contain neither NUL bytes nor non-canonical CR line endings: `$relativePath`', [relativePath]);
		}
		if (!isKnownKind(kind)) {
			fail('generated artifact `$relativePath` has an unknown kind `${Std.string(kind)}`', [relativePath]);
		}
		this.relativePath = relativePath;
		this.contents = contents;
		this.kind = kind;
		this.contentSha256 = Sha256.encode(contents);
	}

	public function verifyIntegrity():Void {
		if (contentSha256 != Sha256.encode(contents)) {
			fail('generated artifact content changed after hashing: `$relativePath`', [relativePath]);
		}
	}

	public static function isNormalizedRelativePath(value:String):Bool {
		if (value == "" || StringTools.startsWith(value, "/") || StringTools.startsWith(value, "~") || value.indexOf("\\") != -1
			|| value.indexOf("\x00") != -1 || ~/^[A-Za-z]:/.match(value)) {
			return false;
		}
		for (part in value.split("/")) {
			if (part == "" || part == "." || part == "..") {
				return false;
			}
		}
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				return false;
			}
		}
		return true;
	}

	public static function isPayloadKind(kind:GeneratedFileKind):Bool {
		return switch kind {
			case PublicHeader | PrivateHeader | Source | RuntimeHeader | RuntimeSource: true;
			case CompilerManifest | RuntimePlan | AbiManifest | SymbolTable | StdlibReport | SpecializationReport | InitializationPlan | CMakeAdapter |
				MesonAdapter: false;
		};
	}

	static function isKnownKind(kind:GeneratedFileKind):Bool {
		return switch kind {
			case PublicHeader | PrivateHeader | Source | RuntimeHeader | RuntimeSource | CompilerManifest | RuntimePlan | AbiManifest | SymbolTable |
				StdlibReport | SpecializationReport | InitializationPlan | CMakeAdapter | MesonAdapter:
				true;
			case _: false;
		};
	}

	static function fail(detail:String, paths:Array<String>):Void
		throw new ProjectEmissionError(detail, paths);
}
