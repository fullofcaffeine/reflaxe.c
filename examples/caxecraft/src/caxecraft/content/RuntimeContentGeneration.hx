package caxecraft.content;

import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageProvenance;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.RuntimeContentPack.RuntimeContentPackResult;
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelCandidate;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadError;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadResult;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelSource;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import caxecraft.content.RuntimeContentDigest.runtimeSha256;
import caxecraft.domain.EntityId;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.RuntimeUiCatalog.RuntimeUiCatalogResult;
import haxe.io.Bytes;

/**
 * Builds one all-or-nothing runtime owner from the staged content package.
 *
 * The receipt is read first and names exactly one pack, UI catalog, and map.
 * Each file is then read into owned bytes, checked against its SHA-256 and byte
 * count, decoded at its lowest faithful layer, and finally resolved into a
 * fresh level. Nothing escapes until all four inputs agree. The renderer and
 * application may borrow the resulting registry/catalog only from this owner.
 */
/** Application-selected player facts; the pack owns the aquatic profile. */
typedef RuntimeContentPlayerOptions = {
	/** Stable entity ID for the locally controlled character. */
	final entityId:EntityId;

	/** Initial health selected by the game or deterministic pilot. */
	final initialHealth:Int;
}

/** Cryptographic receipt for one exact runtime source file. */
typedef RuntimeContentFileReceipt = {
	/** Closed role assigned by the package receipt. */
	final kind:String;

	/** Validated path below the selected package root. */
	final logicalPath:String;

	/** Exact bytes consumed by the matching decoder. */
	final byteLength:Int;

	/** Lowercase SHA-256 over those exact bytes. */
	final sha256:String;
}

/** Complete identity bound to one runtime content candidate. */
typedef RuntimeContentReceipt = {
	/** SHA-256 over the ordered manifest, pack, UI, and map identities. */
	final generationSha256:String;

	/** Reviewed asset-manifest schema version. */
	final assetManifestSchemaVersion:Int;

	/** Reviewed asset package identity. */
	final assetManifestId:String;

	/** SHA-256 of the complete reviewed asset manifest. */
	final assetManifestSha256:String;

	/** Exact base-pack source receipt. */
	final content:RuntimeContentFileReceipt;

	/** Exact UI-catalog source receipt. */
	final ui:RuntimeContentFileReceipt;

	/** Exact first-playable map source receipt. */
	final map:RuntimeContentFileReceipt;
}

/** Exact stage that rejected the complete runtime package candidate. */
enum RuntimeContentLoadError {
	/** The receipt or one named source could not be read safely. */
	RuntimeContentSourceRejected(logicalPath:String, error:ContentPackageError);

	/** The receipt JSON did not match the one admitted closed package shape. */
	RuntimeContentReceiptRejected(diagnostic:RuntimeSchemaDiagnostic);

	/** One source no longer matches the byte count recorded beside its hash. */
	RuntimeContentLengthMismatch(logicalPath:String, expected:Int, actual:Int);

	/** One source or the generation identity no longer matches its SHA-256. */
	RuntimeContentHashMismatch(logicalPath:String, expected:String);

	/** The base pack failed its exact runtime schema. */
	RuntimeContentPackSchemaRejected(diagnostic:RuntimeSchemaDiagnostic);

	/** The UI catalog failed its exact runtime schema. */
	RuntimeContentUiSchemaRejected(diagnostic:RuntimeSchemaDiagnostic);

	/** The map failed source, syntax, semantic, resolution, or construction checks. */
	RuntimeContentLevelRejected(error:RuntimeLevelLoadError);
}

/** One complete unpublished content generation, or one fail-closed reason. */
enum RuntimeContentLoadResult {
	/** Registry, catalog, map, session, and receipts agree and may be published. */
	RuntimeContentReady(candidate:RuntimeContentGeneration);

	/** No complete owner escaped the failed request. */
	RuntimeContentRejected(error:RuntimeContentLoadError);
}

/** Parsed receipt facts kept private until every source has been verified. */
private typedef ParsedRuntimeContentReceipt = {
	final generationSha256:String;
	final assetManifestSchemaVersion:Int;
	final assetManifestId:String;
	final assetManifestSha256:String;
	final content:ParsedRuntimeContentFile;
	final ui:ParsedRuntimeContentFile;
	final map:ParsedRuntimeContentFile;
}

/** One parsed file record before real bytes prove its claims. */
private typedef ParsedRuntimeContentFile = {
	final kind:String;
	final logicalPath:String;
	final byteLength:Int;
	final sha256:String;
}

/** Receipt parsing either returns all closed facts or one located diagnostic. */
private enum ParsedRuntimeContentReceiptResult {
	ParsedRuntimeContentReceiptReady(receipt:ParsedRuntimeContentReceipt);
	ParsedRuntimeContentReceiptRejected(diagnostic:RuntimeSchemaDiagnostic);
}

/** A verified file retains its owned bytes until the relevant decoder finishes. */
private typedef VerifiedRuntimeContentFile = {
	final loaded:LoadedPackageBytes;
	final receipt:RuntimeContentFileReceipt;
}

/** Small read capability shared by the real store and exact focused fixtures. */
private interface RuntimeContentSource {
	/** Read one logical package path into independently owned bytes. */
	function read(logicalPath:String):ContentPackageReadResult;
}

/** Keep filesystem authority behind the same narrow source used by verification. */
private final class StoredRuntimeContentSource implements RuntimeContentSource {
	final store:ContentPackageStore;

	/** Borrow one already-open package capability for this load attempt. */
	public function new(store:ContentPackageStore)
		this.store = store;

	/** Delegate one exact logical read without exposing the store elsewhere. */
	public function read(logicalPath:String):ContentPackageReadResult
		return store.read(logicalPath);
}

#if caxecraft_runtime_content_testing
/** Exact byte owners used to challenge receipt verification without disk mutation. */
typedef RuntimeContentTestSources = {
	/** Candidate receipt bytes, valid or deliberately malformed. */
	final receipt:Bytes;

	/** Candidate base-pack bytes. */
	final content:Bytes;

	/** Candidate UI-catalog bytes. */
	final ui:Bytes;

	/** Candidate first-playable map bytes. */
	final map:Bytes;

	/** Exact listed path to reject as missing, or empty when every source exists. */
	final missingLogicalPath:String;
}

/**
	Serve four exact in-memory files to focused receipt-rejection checks.

	The adapter is compiled only for the runtime-content test define. It preserves
	the production verifier and decoder path while avoiding checkout mutations or
	timing-sensitive filesystem races in focused negative cases.
**/
private final class ExactRuntimeContentSource implements RuntimeContentSource {
	final sources:RuntimeContentTestSources;

	/** Retain the exact test-owned byte bundle for one complete attempt. */
	public function new(sources:RuntimeContentTestSources)
		this.sources = sources;

	/** Return the listed fixture bytes, or the one deliberately missing source. */
	public function read(logicalPath:String):ContentPackageReadResult {
		if (logicalPath == sources.missingLogicalPath)
			return PackageBytesRejected(EntryMissing);
		if (logicalPath == "packs/caxecraft/base/runtime-content.json")
			return loaded(logicalPath, sources.receipt);
		if (logicalPath == "packs/caxecraft/base/content.json")
			return loaded(logicalPath, sources.content);
		if (logicalPath == "locales/ui.json")
			return loaded(logicalPath, sources.ui);
		if (logicalPath == "scenarios/first-playable/map.caxemap")
			return loaded(logicalPath, sources.map);
		return PackageBytesRejected(EntryMissing);
	}

	/** Attach valid path provenance to one exact fixture byte owner. */
	function loaded(logicalPath:String, bytes:Bytes):ContentPackageReadResult {
		return switch ContentPackagePath.parse(logicalPath) {
			case PathAccepted(path): PackageBytesRead(new LoadedPackageBytes(bytes,
					new ContentPackageProvenance("runtime-content-focused", path, bytes.length, 1)));
			case PathRejected(error): PackageBytesRejected(InvalidLogicalPath(error));
		};
	}
}
#end

/**
 * Owns one coherent runtime registry, catalog, and loaded level.
 *
 * A class gives the mutable session and its immutable interpretation facts one
 * shared lifetime. Its constructor is private so a caller cannot pair a map
 * produced by one registry with another registry or UI/hash receipt.
 */
@:allow(caxecraft.content.RuntimeContentGeneration)
final class RuntimeContentGeneration {
	final contentRegistry:RuntimeContentRegistry;
	final uiCatalog:RuntimeUiCatalog;
	final loadedLevel:RuntimeLevelCandidate;
	final sourceReceipt:RuntimeContentReceipt;

	/** Construct only after every source, schema, reference, and level stage passed. */
	private function new(registry:RuntimeContentRegistry, catalog:RuntimeUiCatalog, level:RuntimeLevelCandidate, receipt:RuntimeContentReceipt) {
		contentRegistry = registry;
		uiCatalog = catalog;
		loadedLevel = level;
		sourceReceipt = receipt;
	}

	/** Monotonic identity shared with the enclosed playable generation. */
	public inline function generationId():ContentGenerationId
		return loadedLevel.generation().generationId();

	/** Immutable mechanics and presentation facts decoded from the base pack. */
	public inline function registry():RuntimeContentRegistry
		return contentRegistry;

	/** Immutable typed text decoded from the UI catalog. */
	public inline function catalog():RuntimeUiCatalog
		return uiCatalog;

	/** Complete map/session candidate resolved through `registry()`. */
	public inline function level():RuntimeLevelCandidate
		return loadedLevel;

	/** Return copy-owned scalar/string provenance for this complete generation. */
	public function receipt():RuntimeContentReceipt
		return {
			generationSha256: sourceReceipt.generationSha256,
			assetManifestSchemaVersion: sourceReceipt.assetManifestSchemaVersion,
			assetManifestId: sourceReceipt.assetManifestId,
			assetManifestSha256: sourceReceipt.assetManifestSha256,
			content: copyFileReceipt(sourceReceipt.content),
			ui: copyFileReceipt(sourceReceipt.ui),
			map: copyFileReceipt(sourceReceipt.map)
		};

	/** Copy one nested receipt so callers cannot alias owner records. */
	static function copyFileReceipt(receipt:RuntimeContentFileReceipt):RuntimeContentFileReceipt
		return {
			kind: receipt.kind,
			logicalPath: receipt.logicalPath,
			byteLength: receipt.byteLength,
			sha256: receipt.sha256
		};
}

/**
 * Read and build one complete candidate without receiving active game state.
 *
 * Expected values originate in the staged runtime receipt. SHA-256 is computed
 * again from the owned bytes, and the receipt's own generation digest is also
 * recomputed before either schema decoder runs.
 */
function loadRuntimeContent(store:ContentPackageStore, generationId:ContentGenerationId, player:RuntimeContentPlayerOptions):RuntimeContentLoadResult {
	return loadRuntimeContentFromSource(new StoredRuntimeContentSource(store), generationId, player);
}

#if caxecraft_runtime_content_testing
/** Exercise the production verifier with exact in-memory sources owned by a focused test. */
function loadRuntimeContentForTesting(sources:RuntimeContentTestSources, generationId:ContentGenerationId,
		player:RuntimeContentPlayerOptions):RuntimeContentLoadResult {
	return loadRuntimeContentFromSource(new ExactRuntimeContentSource(sources), generationId, player);
}
#end

/** Verify, decode, resolve, and construct one candidate from a narrow read source. */
private function loadRuntimeContentFromSource(source:RuntimeContentSource, generationId:ContentGenerationId,
		player:RuntimeContentPlayerOptions):RuntimeContentLoadResult {
	final manifestPath = "packs/caxecraft/base/runtime-content.json";
	final manifestBytes = switch source.read(manifestPath) {
		case PackageBytesRead(value): value;
		case PackageBytesRejected(error): return RuntimeContentRejected(RuntimeContentSourceRejected(manifestPath, error));
	};
	final parsed = switch parseReceipt(manifestBytes.bytes) {
		case ParsedRuntimeContentReceiptReady(value): value;
		case ParsedRuntimeContentReceiptRejected(diagnostic): return RuntimeContentRejected(RuntimeContentReceiptRejected(diagnostic));
	};
	if (!generationMatches(parsed))
		return RuntimeContentRejected(RuntimeContentHashMismatch(manifestPath, parsed.generationSha256));

	final content = switch readVerified(source, parsed.content) {
		case VerifiedRuntimeContentReady(value): value;
		case VerifiedRuntimeContentRejected(error): return RuntimeContentRejected(error);
	};
	final ui = switch readVerified(source, parsed.ui) {
		case VerifiedRuntimeContentReady(value): value;
		case VerifiedRuntimeContentRejected(error): return RuntimeContentRejected(error);
	};
	final map = switch readVerified(source, parsed.map) {
		case VerifiedRuntimeContentReady(value): value;
		case VerifiedRuntimeContentRejected(error): return RuntimeContentRejected(error);
	};

	final assets = RuntimeAssetInventory.reviewedBase();
	final registry = switch RuntimeContentPack.decode(content.loaded.bytes, assets) {
		case RuntimeContentPackReady(value): value;
		case RuntimeContentPackRejected(diagnostic): return RuntimeContentRejected(RuntimeContentPackSchemaRejected(diagnostic));
	};
	final catalog = switch RuntimeUiCatalog.decode(ui.loaded.bytes) {
		case RuntimeUiCatalogReady(value): value;
		case RuntimeUiCatalogRejected(diagnostic): return RuntimeContentRejected(RuntimeContentUiSchemaRejected(diagnostic));
	};
	final level = switch RuntimeLevelLoader.loadRuntimeLevel(AdmittedPackageBytes(map.loaded), generationId, registry, registry, {
		entityId: player.entityId,
		initialHealth: player.initialHealth,
		aquaticProfile: registry.defaultAquaticProfile()
	}) {
		case RuntimeLevelReady(value): value;
		case RuntimeLevelRejected(error): return RuntimeContentRejected(RuntimeContentLevelRejected(error));
	};
	return RuntimeContentReady(new RuntimeContentGeneration(registry, catalog, level, {
		generationSha256: parsed.generationSha256,
		assetManifestSchemaVersion: parsed.assetManifestSchemaVersion,
		assetManifestId: parsed.assetManifestId,
		assetManifestSha256: parsed.assetManifestSha256,
		content: content.receipt,
		ui: ui.receipt,
		map: map.receipt
	}));
}

/** Reading one source either verifies its complete receipt or returns no bytes. */
private enum VerifiedRuntimeContentResult {
	VerifiedRuntimeContentReady(value:VerifiedRuntimeContentFile);
	VerifiedRuntimeContentRejected(error:RuntimeContentLoadError);
}

/** Read one named source and compare both size and SHA-256 before decoding it. */
private function readVerified(source:RuntimeContentSource, expected:ParsedRuntimeContentFile):VerifiedRuntimeContentResult {
	final loaded = switch source.read(expected.logicalPath) {
		case PackageBytesRead(value): value;
		case PackageBytesRejected(error): return VerifiedRuntimeContentRejected(RuntimeContentSourceRejected(expected.logicalPath, error));
	};
	if (loaded.bytes.length != expected.byteLength)
		return VerifiedRuntimeContentRejected(RuntimeContentLengthMismatch(expected.logicalPath, expected.byteLength, loaded.bytes.length));
	if (!sha256Matches(loaded.bytes, expected.sha256))
		return VerifiedRuntimeContentRejected(RuntimeContentHashMismatch(expected.logicalPath, expected.sha256));
	return VerifiedRuntimeContentReady({
		loaded: loaded,
		receipt: {
			kind: expected.kind,
			logicalPath: expected.logicalPath,
			byteLength: expected.byteLength,
			sha256: expected.sha256
		}
	});
}

/** Decode the exact checked package-receipt shape and canonical file order. */
private function parseReceipt(input:Bytes):ParsedRuntimeContentReceiptResult {
	final reader = new RuntimeSchemaReader();
	final root = reader.parse(input);
	if (root == null)
		return rejectedReceipt(reader);
	final fields = reader.object(root, "runtimeContent", ["schemaVersion", "generationSha256", "assetManifest", "files"]);
	if (fields == null)
		return rejectedReceipt(reader);
	final versionNode = reader.field(fields, "schemaVersion");
	final version = reader.integer(versionNode, "schemaVersion", 0, 2147483647);
	if (version == null)
		return rejectedReceipt(reader);
	if (version != 1) {
		reader.reject(versionNode, SchemaUnsupportedVersion("schemaVersion", 1));
		return rejectedReceipt(reader);
	}
	final generationNode = reader.field(fields, "generationSha256");
	final generationSha256 = readSha256(reader, generationNode, "generationSha256");
	if (generationSha256 == null)
		return rejectedReceipt(reader);

	final assetNode = reader.field(fields, "assetManifest");
	final assetFields = reader.object(assetNode, "assetManifest", ["schemaVersion", "packId", "sha256"]);
	if (assetFields == null)
		return rejectedReceipt(reader);
	final assetVersionNode = reader.field(assetFields, "schemaVersion");
	final assetVersion = reader.integer(assetVersionNode, "assetManifest.schemaVersion", 0, 2147483647);
	final assetIdNode = reader.field(assetFields, "packId");
	final assetId = reader.string(assetIdNode, "assetManifest.packId", 128);
	final assetHashNode = reader.field(assetFields, "sha256");
	final assetHash = readSha256(reader, assetHashNode, "assetManifest.sha256");
	if (assetVersion == null || assetId == null || assetHash == null)
		return rejectedReceipt(reader);
	if (assetVersion != 1
		|| assetId != RuntimeAssetInventory.reviewedBase().manifestId()
		|| assetHash != RuntimeAssetInventory.REVIEWED_MANIFEST_SHA256) {
		reader.reject(assetNode, SchemaInvalidInvariant("assetManifest"));
		return rejectedReceipt(reader);
	}

	final filesNode = reader.field(fields, "files");
	final fileNodes = reader.array(filesNode, "files", 3, 3);
	if (fileNodes == null)
		return rejectedReceipt(reader);
	final expectedKinds = ["content-pack", "ui-catalog", "level-map"];
	final expectedPaths = [
		"packs/caxecraft/base/content.json",
		"locales/ui.json",
		"scenarios/first-playable/map.caxemap"
	];
	final parsedFiles:Array<ParsedRuntimeContentFile> = [];
	for (index in 0...fileNodes.length) {
		final path = "files[" + index + "]";
		final fileFields = reader.object(fileNodes[index], path, ["kind", "path", "byteLength", "sha256"]);
		if (fileFields == null)
			return rejectedReceipt(reader);
		final kindNode = reader.field(fileFields, "kind");
		final logicalNode = reader.field(fileFields, "path");
		final lengthNode = reader.field(fileFields, "byteLength");
		final hashNode = reader.field(fileFields, "sha256");
		final kind = reader.string(kindNode, path + ".kind", 32);
		final logicalPath = reader.string(logicalNode, path + ".path", 256);
		final byteLength = reader.integer(lengthNode, path + ".byteLength", 1, ContentPackageStore.MAXIMUM_PACKAGE_BYTES);
		final sha256 = readSha256(reader, hashNode, path + ".sha256");
		if (kind == null || logicalPath == null || byteLength == null || sha256 == null)
			return rejectedReceipt(reader);
		if (kind != expectedKinds[index] || logicalPath != expectedPaths[index]) {
			reader.reject(fileNodes[index], SchemaInvalidInvariant(path));
			return rejectedReceipt(reader);
		}
		parsedFiles.push({
			kind: kind,
			logicalPath: logicalPath,
			byteLength: byteLength,
			sha256: sha256
		});
	}
	return ParsedRuntimeContentReceiptReady({
		generationSha256: generationSha256,
		assetManifestSchemaVersion: assetVersion,
		assetManifestId: assetId,
		assetManifestSha256: assetHash,
		content: parsedFiles[0],
		ui: parsedFiles[1],
		map: parsedFiles[2]
	});
}

/** Parse one lowercase 64-digit SHA-256 without accepting another alphabet. */
private function readSha256(reader:RuntimeSchemaReader, node:ContentJson.ContentJsonNode, path:String):Null<String> {
	final value = reader.string(node, path, 64);
	if (value == null)
		return null;
	if (value.length != 64) {
		reader.reject(node, SchemaInvalidInvariant(path));
		return null;
	}
	for (index in 0...value.length) {
		final code = value.charCodeAt(index);
		if (!((code >= 0x30 && code <= 0x39) || (code >= 0x61 && code <= 0x66))) {
			reader.reject(node, SchemaInvalidInvariant(path));
			return null;
		}
	}
	return value;
}

/** Recompute the ordered package generation identity from parsed receipts. */
private function generationMatches(receipt:ParsedRuntimeContentReceipt):Bool {
	final value = "asset-manifest\x00" + receipt.assetManifestId + "\x00" + receipt.assetManifestSha256 + "\n" + receipt.content.kind + "\x00"
		+ receipt.content.logicalPath + "\x00" + receipt.content.sha256 + "\n" + receipt.ui.kind + "\x00" + receipt.ui.logicalPath + "\x00"
		+ receipt.ui.sha256 + "\n" + receipt.map.kind + "\x00" + receipt.map.logicalPath + "\x00" + receipt.map.sha256 + "\n";
	return sha256Matches(Bytes.ofString(value), receipt.generationSha256);
}

/** Compare SHA-256 bytes directly so validation does not require hex output APIs. */
private function sha256Matches(input:Bytes, expected:String):Bool {
	if (expected.length != 64)
		return false;
	final digest = runtimeSha256(input);
	if (digest.length != 32)
		return false;
	for (index in 0...digest.length) {
		final high = hexDigit(expected.charCodeAt(index * 2));
		final low = hexDigit(expected.charCodeAt(index * 2 + 1));
		if (high < 0 || low < 0 || digest.get(index) != high * 16 + low)
			return false;
	}
	return true;
}

/** Translate one already-bounded lowercase hexadecimal digit. */
private inline function hexDigit(code:Int):Int
	return code >= 0x30 && code <= 0x39 ? code - 0x30 : code >= 0x61 && code <= 0x66 ? code - 0x61 + 10 : -1;

/** Return the reader's first diagnostic with an unreachable defensive fallback. */
private function rejectedReceipt(reader:RuntimeSchemaReader):ParsedRuntimeContentReceiptResult {
	final diagnostic = reader.failure;
	if (diagnostic != null)
		return ParsedRuntimeContentReceiptRejected(diagnostic);
	return ParsedRuntimeContentReceiptRejected({
		line: 1,
		column: 1,
		kind: SchemaInvalidInvariant("runtimeContent")
	});
}
