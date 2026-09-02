package reflaxe.c.emit;

/**
	Defines the immutable files that the compiler gives to output ownership.

	Each value validates its path, text, role, byte hash, and construction
	provenance before filesystem code can publish it.
**/
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
	var DispatchReport = "dispatch-report";
	var InitializationPlan = "initialization-plan";
	var TypedInventory = "typed-inventory";
	var HxcIR = "hxcir";
	var CAst = "c-ast";
	var DeclarationReport = "declaration-report";
	var CMakeAdapter = "cmake-adapter";
	var MesonAdapter = "meson-adapter";
	var LicenseNotice = "license-notice";
}

/** How an artifact's bytes entered the generated application package. */
enum abstract GeneratedFileProduction(String) to String {
	/** C or headers rendered from the compiler's typed program plan. */
	var CompiledProgram = "compiled-program";

	/** Reports, manifests, and build adapters rendered by compiler tooling. */
	var CompilerSidecar = "compiler-sidecar";

	/** Repository bytes copied unchanged after their registered hash was checked. */
	var RepositoryCopy = "repository-copy";
}

/**
	License-neutral provenance for one generated application artifact.

	`production` describes the executable construction path, not a license
	conclusion. Exact repository copies additionally retain their authoritative
	source path and hash so later notice tooling can derive policy without a
	second hand-maintained inventory.
**/
typedef GeneratedFileProvenance = {
	final production:GeneratedFileProduction;
	final ?sourcePath:String;
	final ?sourceSha256:String;
}

/** A validated, content-addressed, output-root-relative compiler artifact. */
class GeneratedFile {
	public static inline final OWNERSHIP_MANIFEST = "_GeneratedFiles.json";

	public final relativePath:String;
	public final contents:String;
	public final kind:GeneratedFileKind;

	/** Validated construction provenance projected into `hxc.manifest.json`. */
	public final provenance:GeneratedFileProvenance;

	public final contentSha256:String;

	/** Exact UTF-8 payload size computed or reused with the content address. */
	public final contentByteLength:Int;

	/**
		Validate and content-address one compiler-owned artifact.

		Haxe strings are immutable and every stored field is `final`, so the
		digest and byte length remain valid for this object's whole lifetime. A
		warm compiler-server request may reuse both only when the prior completed
		request has the same path, artifact role, and complete text. Otherwise the
		constructor computes SHA-256 from fresh UTF-8 bytes. Filesystem output is
		a separate trust boundary with its own ownership and path checks. Program
		and sidecar kinds have one unambiguous default provenance. Runtime copies
		must provide their validated repository path and source hash.
	**/
	public function new(relativePath:String, contents:String, kind:GeneratedFileKind, ?provenance:GeneratedFileProvenance) {
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
		this.provenance = validatedProvenance(relativePath, kind, provenance);
		final address = GeneratedFileDigestCache.address(relativePath, contents, kind);
		this.contentSha256 = address.sha256;
		this.contentByteLength = address.byteLength;
	}

	static function validatedProvenance(relativePath:String, kind:GeneratedFileKind, provenance:Null<GeneratedFileProvenance>):GeneratedFileProvenance {
		if (provenance == null) {
			return switch kind {
				case PublicHeader | PrivateHeader | Source:
					{production: GeneratedFileProduction.CompiledProgram};
				case RuntimeHeader | RuntimeSource:
					invalid('copied runtime artifact `$relativePath` requires exact repository provenance', [relativePath]);
				case CompilerManifest | RuntimePlan | AbiManifest | SymbolTable | StdlibReport | SpecializationReport | DispatchReport | InitializationPlan |
					TypedInventory | HxcIR | CAst | DeclarationReport | CMakeAdapter | MesonAdapter | LicenseNotice:
					{production: GeneratedFileProduction.CompilerSidecar};
			};
		}
		final sourcePath = provenance.sourcePath;
		final sourceSha256 = provenance.sourceSha256;
		switch provenance.production {
			case CompiledProgram:
				if ((kind != PublicHeader && kind != PrivateHeader && kind != Source) || sourcePath != null || sourceSha256 != null) {
					fail('compiled-program provenance does not match artifact `$relativePath` kind `${Std.string(kind)}`', [relativePath]);
				}
				return {production: GeneratedFileProduction.CompiledProgram};
			case CompilerSidecar:
				if (isPayloadKind(kind) || sourcePath != null || sourceSha256 != null) {
					fail('compiler-sidecar provenance does not match artifact `$relativePath` kind `${Std.string(kind)}`', [relativePath]);
				}
				return {production: GeneratedFileProduction.CompilerSidecar};
			case RepositoryCopy:
				if (kind != RuntimeHeader && kind != RuntimeSource) {
					fail('repository-copy provenance for `$relativePath` requires an exact runtime/hxrt source path and lowercase SHA-256', [relativePath]);
				}
				final exactSourcePath:String = sourcePath == null ? invalid('repository-copy provenance for `$relativePath` requires a source path',
					[relativePath]) : sourcePath;
				final exactSourceSha256:String = sourceSha256 == null ? invalid('repository-copy provenance for `$relativePath` requires a source SHA-256',
					[relativePath]) : sourceSha256;
				if (!isNormalizedRelativePath(exactSourcePath)
					|| !StringTools.startsWith(exactSourcePath, "runtime/hxrt/")
					|| !~/^[0-9a-f]{64}$/.match(exactSourceSha256)) {
					fail('repository-copy provenance for `$relativePath` requires an exact runtime/hxrt source path and lowercase SHA-256', [relativePath]);
				}
				return {
					production: GeneratedFileProduction.RepositoryCopy,
					sourcePath: exactSourcePath,
					sourceSha256: exactSourceSha256
				};
			case _:
				return invalid('generated artifact `$relativePath` has unknown production provenance `${Std.string(provenance.production)}`', [relativePath]);
		}
	}

	static function invalid<T>(detail:String, paths:Array<String>):T
		throw new ProjectEmissionError(detail, paths);

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
			case CompilerManifest | RuntimePlan | AbiManifest | SymbolTable | StdlibReport | SpecializationReport | DispatchReport | InitializationPlan |
				TypedInventory | HxcIR | CAst | DeclarationReport | CMakeAdapter | MesonAdapter | LicenseNotice: false;
		};
	}

	static function isKnownKind(kind:GeneratedFileKind):Bool {
		return switch kind {
			case PublicHeader | PrivateHeader | Source | RuntimeHeader | RuntimeSource | CompilerManifest | RuntimePlan | AbiManifest | SymbolTable |
				StdlibReport | SpecializationReport | DispatchReport | InitializationPlan | TypedInventory | HxcIR | CAst | DeclarationReport | CMakeAdapter |
				MesonAdapter | LicenseNotice:
				true;
			case _: false;
		};
	}

	static function fail(detail:String, paths:Array<String>):Void
		throw new ProjectEmissionError(detail, paths);
}
