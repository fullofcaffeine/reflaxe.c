package caxecraft.qa;

import caxecraft.content.ContentPackageAssetClosure.ContentPackageAssetClosureResult;
import caxecraft.content.ContentPackageAssetClosure.verifyContentPackageAssetClosure;
import caxecraft.content.ContentPackageManifest.ContentPackageLoadResult;
import caxecraft.content.ContentPackageManifest.ContentPackageManifestReadResult;
import caxecraft.content.ContentPackageManifest.LoadedContentPackage;
import caxecraft.content.ContentPackageManifest.decodeContentPackageManifest;
import caxecraft.content.ContentPackageManifest.loadContentPackage;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageProvenance;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackageSource;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.ContentPackageZipExport.ContentPackageZipExportError;
import caxecraft.content.ContentPackageZipExport.ContentPackageZipExportResult;
import caxecraft.content.ContentPackageZipExport.exportContentPackageZip;
import caxecraft.content.ContentPackageZipSource;
import haxe.io.Bytes;

/**
 * Proves one complete authored directory becomes one reproducible shared ZIP.
 *
 * The package data selects eight real PNG assets in addition to the semantic
 * campaign, levels, packs, localization, and asset inventory. Two independent
 * exports must match byte-for-byte, and the existing ZIP reader must recover
 * the same verified package identity and receipts from those bytes.
 */
/** First failed export or round-trip invariant, or zero after success. */
var observed:Int = 0;

/** Stable package identity including all fifteen directly owned payloads. */
var traceIdentity:Int = 0;

/** Sum of all verified semantic and selected-asset payload bytes. */
var tracePayloadBytes:Int = 0;

/** Exact canonical archive byte count for the independently inspected output. */
var traceArchiveBytes:Int = 0;

/** Byte-order fingerprint compared between Eval and generated native C. */
var traceArchiveFingerprint:Int = 0;

/** Hide one declared file while preserving every other source behavior. */
private final class MissingPackageEntrySource implements ContentPackageSource {
	final source:ContentPackageSource;
	final missingPath:String;

	public function new(source:ContentPackageSource, missingPath:String) {
		this.source = source;
		this.missingPath = missingPath;
	}

	/** Return the same closed missing-entry result as a confined real source. */
	public function read(logicalPath:String):ContentPackageReadResult
		return logicalPath == missingPath ? PackageBytesRejected(EntryMissing) : source.read(logicalPath);
}

/** Replace one declared payload while retaining a coherent source receipt. */
private final class ReplacedPackageEntrySource implements ContentPackageSource {
	final source:ContentPackageSource;
	final replacedPath:String;
	final replacement:Bytes;

	public function new(source:ContentPackageSource, replacedPath:String, replacement:Bytes) {
		this.source = source;
		this.replacedPath = replacedPath;
		this.replacement = replacement;
	}

	/** Delegate other paths and report the replacement's exact byte length. */
	public function read(logicalPath:String):ContentPackageReadResult {
		final original = source.read(logicalPath);
		if (logicalPath != replacedPath)
			return original;
		return switch original {
			case PackageBytesRejected(error): PackageBytesRejected(error);
			case PackageBytesRead(content):
				PackageBytesRead(new LoadedPackageBytes(replacement,
					new ContentPackageProvenance(content.provenance.rootLabel, content.provenance.logicalPath, replacement.length,
						content.provenance.readAttempts)));
		};
	}
}

/** Vary source-only metadata that canonical archive bytes must ignore. */
private final class RelabeledPackageSource implements ContentPackageSource {
	final source:ContentPackageSource;

	public function new(source:ContentPackageSource)
		this.source = source;

	/** Preserve payloads while changing root labels and read-attempt receipts. */
	public function read(logicalPath:String):ContentPackageReadResult
		return switch source.read(logicalPath) {
			case PackageBytesRejected(error): PackageBytesRejected(error);
			case PackageBytesRead(content):
				final attempts = content.provenance.readAttempts == 1 ? 2 : 1;
				PackageBytesRead(new LoadedPackageBytes(content.bytes,
					new ContentPackageProvenance("metadata-variant", content.provenance.logicalPath, content.bytes.length, attempts)));
		};
}

/** Run the proof on Eval or expose its scalar results to native C. */
function main():Void {
	observed = selfCheck();
	#if !c
	Sys.println(observed);
	Sys.println(traceIdentity);
	Sys.println(tracePayloadBytes);
	Sys.println(traceArchiveBytes);
	Sys.println(traceArchiveFingerprint);
	#end
}

/** Return zero only after deterministic export and shared-reader parity pass. */
function selfCheck():Int {
	final directory = switch ContentPackageStore.open(".", "first-adventure-export", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	switch directory.read("handoff.zip") {
		case PackageBytesRead(content):
			return verifyCanonicalArchive(content.bytes);
		case PackageBytesRejected(EntryMissing):
		case PackageBytesRejected(_):
			return 2;
	}
	if (!missingAssetRejects(directory))
		return 3;
	if (!staleAssetRejects(directory))
		return 4;
	if (!unselectedAssetRejects(directory))
		return 5;
	if (!oversizedArchiveRejects(directory))
		return 6;
	final first = switch exportContentPackageZip(directory, "caxecraft.package.json") {
		case PackageZipExported(value): value;
		case PackageZipExportRejected(_): return 7;
	};
	final second = switch exportContentPackageZip(new RelabeledPackageSource(directory), "caxecraft.package.json") {
		case PackageZipExported(value): value;
		case PackageZipExportRejected(_): return 8;
	};
	if (!sameBytes(first, second))
		return 9;
	#if !c
	final outputPath = Sys.getEnv("CAXECRAFT_ZIP_EXPORT_PATH");
	if (outputPath != null && outputPath.length > 0)
		sys.io.File.saveBytes(outputPath, first);
	#end
	return verifyCanonicalArchive(first);
}

/** Reopen one transported archive and observe only its semantic package facts. */
function verifyCanonicalArchive(bytes:Bytes):Int {
	final zip = switch ContentPackageZipSource.open(bytes, "canonical-first-adventure", ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES) {
		case PackageZipOpened(value): value;
		case PackageZipRejected(_): return 10;
	};
	final loaded = ready(loadContentPackage(zip, "caxecraft.package.json"));
	if (loaded == null)
		return 11;
	traceIdentity = loaded.manifest.version * 100000 + loaded.manifest.roleCount() * 1000 + loaded.manifest.entryCount() * 10
		+ loaded.manifest.dependencyCount();
	tracePayloadBytes = 0;
	for (index in 0...loaded.receiptCount())
		tracePayloadBytes += loaded.receiptAt(index).byteLength;
	traceArchiveBytes = bytes.length;
	traceArchiveFingerprint = archiveFingerprint(bytes);
	return traceIdentity == 105150
		&& tracePayloadBytes == 9487237
		&& traceArchiveBytes == 9492667
		&& traceArchiveFingerprint == 1338236009 ? 0 : 12;
}

/** Prove a selected asset cannot disappear before archive construction. */
function missingAssetRejects(source:ContentPackageSource):Bool {
	final path = "assets/atlases/adventure-items.png";
	return switch exportContentPackageZip(new MissingPackageEntrySource(source, path), "caxecraft.package.json") {
		case PackageZipExportRejected(EntrySourceRejected(actual, EntryMissing)) if (actual == path): true;
		case _: false;
	};
}

/** Prove same-length stale bytes fail their authored SHA-256 receipt. */
function staleAssetRejects(source:ContentPackageSource):Bool {
	final path = "assets/atlases/adventure-items.png";
	final original = switch source.read(path) {
		case PackageBytesRejected(_): return false;
		case PackageBytesRead(content): content.bytes;
	};
	final stale = original.sub(0, original.length);
	stale.set(0, stale.get(0) ^ 1);
	return switch exportContentPackageZip(new ReplacedPackageEntrySource(source, path, stale), "caxecraft.package.json") {
		case PackageZipExportRejected(EntryHashMismatch(actual, _)) if (actual == path): true;
		case _: false;
	};
}

/** Prove a package asset not selected by its inventory fails the closure. */
function unselectedAssetRejects(source:ContentPackageSource):Bool {
	final manifestBytes = switch source.read("caxecraft.package.json") {
		case PackageBytesRejected(_): return false;
		case PackageBytesRead(content): content.bytes;
	};
	final manifest = switch decodeContentPackageManifest(manifestBytes) {
		case ContentPackageManifestRejected(_): return false;
		case ContentPackageManifestReady(value): value;
	};
	final inventoryBytes = switch source.read("assets/manifest.json") {
		case PackageBytesRejected(_): return false;
		case PackageBytesRead(content): content.bytes;
	};
	final inventoryText = inventoryBytes.toString();
	final withoutTerrain = replaceOnce(inventoryText, '      "entities",\n      "terrain"', '      "entities"');
	if (withoutTerrain == inventoryText)
		return false;
	return switch verifyContentPackageAssetClosure(manifest, Bytes.ofString(withoutTerrain)) {
		case AssetClosureRejected(UnselectedAssetEntry("assets/atlases/terrain.png")): true;
		case _: false;
	};
}

/** Prove the archive ceiling rejects from declared facts before payload reads. */
function oversizedArchiveRejects(source:ContentPackageSource):Bool {
	final manifest = Bytes.ofString('{"schemaVersion":1,"packageId":"caxecraft:oversized","packageVersion":1,"roles":["content"],"entries":[{"kind":"content-pack","path":"packs/oversized.json","byteLength":16777216,"sha256":"0000000000000000000000000000000000000000000000000000000000000000"}],"dependencies":[]}');
	final replaced = new ReplacedPackageEntrySource(source, "caxecraft.package.json", manifest);
	return switch exportContentPackageZip(replaced, "caxecraft.package.json") {
		case PackageZipExportRejected(ArchiveByteBudgetExceeded(16777216)): true;
		case _: false;
	};
}

/** Replace one reviewed text fragment without applying a general rewrite. */
function replaceOnce(input:String, before:String, after:String):String {
	final index = input.indexOf(before);
	return index < 0 ? input : input.substring(0, index) + after + input.substring(index + before.length);
}

/** Preserve a rejected package as null without erasing the positive type. */
function ready(result:ContentPackageLoadResult):Null<LoadedContentPackage>
	return switch result {
		case ContentPackageReady(value): value;
		case ContentPackageRejected(_): null;
	};

/** Compare exact archive bytes without relying on a target helper. */
function sameBytes(left:Bytes, right:Bytes):Bool {
	if (left.length != right.length)
		return false;
	for (index in 0...left.length)
		if (left.get(index) != right.get(index))
			return false;
	return true;
}

/** Fold every ordered archive byte into the independently calculated oracle. */
function archiveFingerprint(bytes:Bytes):Int {
	var result = -2128831035;
	for (index in 0...bytes.length)
		result = (result ^ bytes.get(index)) * 16777619;
	return result;
}
