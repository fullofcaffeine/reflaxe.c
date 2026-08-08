package caxecraft.content;

import caxecraft.content.CampaignManifest.CampaignManifestReadResult;
import caxecraft.content.CampaignManifest.decodeCampaignManifest;
import caxecraft.content.CampaignManifest.writeCampaignManifest;
import caxecraft.content.ContentPackageManifest.ContentPackageEntry;
import caxecraft.content.ContentPackageManifest.ContentPackageEntryKind;
import caxecraft.content.ContentPackageManifest.ContentPackageLoadResult;
import caxecraft.content.ContentPackageManifest.ContentPackageManifestReadResult;
import caxecraft.content.ContentPackageManifest.decodeContentPackageManifest;
import caxecraft.content.ContentPackageManifest.loadContentPackage;
import caxecraft.content.ContentPackageManifest.writeContentPackageManifest;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageProvenance;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.RuntimeAssetInventory;
import caxecraft.content.RuntimeContentDigest.runtimeSha256Hex;
import caxecraft.content.RuntimeContentPack.RuntimeContentPackResult;
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.content.RuntimeContentReceiptWriter.writeRuntimeContentReceipt;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;
import caxecraft.scenario.ScenarioWriter;
import haxe.io.Bytes;

/**
 * Plans one complete metadata refresh before any file changes on disk.
 *
 * A creator edits a map. This module validates and canonicalizes that map,
 * then updates the campaign receipt, runtime receipt, and outer package
 * receipt in dependency order. Editors can pass unsaved map bytes to the same
 * operation. The command-line adapter publishes the returned files.
 */
/** One final file and the bytes that existed before this refresh. */
final class ContentRefreshFile {
	/** Package-relative file name. */
	public final logicalPath:String;

	/** Bytes that the source returned before planning. */
	public final previous:Bytes;

	/** Complete final bytes that can replace `previous`. */
	public final next:Bytes;

	/** Retain independent byte owners for one planned file. */
	public function new(logicalPath:String, previous:Bytes, next:Bytes) {
		this.logicalPath = logicalPath;
		this.previous = previous.sub(0, previous.length);
		this.next = next.sub(0, next.length);
	}

	/** True when publication must replace this file. */
	public inline function changed():Bool
		return previous.compare(next) != 0;
}

/** Complete ordered refresh plan. No file has changed when this value exists. */
final class ContentRefreshPlan {
	final plannedFiles:Array<ContentRefreshFile>;

	/** Take a private copy of the dependency-ordered file list. */
	public function new(files:Array<ContentRefreshFile>)
		plannedFiles = files.copy();

	/** Number of files in the complete dependency graph. */
	public inline function fileCount():Int
		return plannedFiles.length;

	/** Read one plan item in publication order. */
	public inline function fileAt(index:Int):ContentRefreshFile
		return plannedFiles[index];

	/** True when at least one planned file differs from its source bytes. */
	public function hasChanges():Bool {
		for (file in plannedFiles)
			if (file.changed())
				return true;
		return false;
	}
}

/** Exact stage that stopped a refresh before publication. */
enum ContentRefreshError {
	SourceRejected(logicalPath:String, error:ContentPackageError);
	PackageRejected(detail:String);
	CampaignRejected(logicalPath:String, detail:String);
	PackageGraphRejected(detail:String);
	ContentPackRejected(detail:String);
	LevelRejected(logicalPath:String, detail:String);
	UnknownLevel(logicalPath:String);
	ReplacementNeedsLevel;
}

/** Planning returns all final bytes or one closed error. */
enum ContentRefreshResult {
	ContentRefreshReady(plan:ContentRefreshPlan);
	ContentRefreshRejected(error:ContentRefreshError);
}

/** Internal byte read that keeps source errors closed and located. */
private enum RefreshReadResult {
	RefreshBytesReady(bytes:Bytes);
	RefreshBytesRejected(error:ContentRefreshError);
}

/** Read final planned bytes before falling through to the original source. */
private final class RefreshOverlaySource implements ContentPackageSource {
	final source:ContentPackageSource;
	final files:Array<ContentRefreshFile>;

	/** Borrow the source and complete immutable-by-API plan for final checks. */
	public function new(source:ContentPackageSource, files:Array<ContentRefreshFile>) {
		this.source = source;
		this.files = files;
	}

	/** Return planned bytes for a changed path and source bytes for every other path. */
	public function read(logicalPath:String):ContentPackageReadResult {
		for (file in files)
			if (file.logicalPath == logicalPath)
				return loaded(logicalPath, file.next);
		return source.read(logicalPath);
	}

	/** Attach bounded in-memory provenance to one planned byte vector. */
	function loaded(logicalPath:String, bytes:Bytes):ContentPackageReadResult
		return switch ContentPackagePath.parse(logicalPath) {
			case PathRejected(error): PackageBytesRejected(InvalidLogicalPath(error));
			case PathAccepted(path): PackageBytesRead(new LoadedPackageBytes(bytes.sub(0, bytes.length),
					new ContentPackageProvenance("content-refresh-plan", path, bytes.length, 1)));
		};
}

/**
 * Compute one package refresh without filesystem write authority.
 *
 * Pass `levelPath` to refresh one map. Pass `null` to refresh every map in the
 * campaign. `replacement` lets the editor plan an unsaved map and requires an
 * exact `levelPath`.
 */
function planContentPackageRefresh(source:ContentPackageSource, manifestPath:String, levelPath:Null<String>, replacement:Null<Bytes>):ContentRefreshResult {
	if (replacement != null && levelPath == null)
		return ContentRefreshRejected(ReplacementNeedsLevel);
	final packageBytes = switch readRefreshBytes(source, manifestPath) {
		case RefreshBytesRejected(error): return ContentRefreshRejected(error);
		case RefreshBytesReady(bytes): bytes;
	};
	final packageManifest = switch decodeContentPackageManifest(packageBytes) {
		case ContentPackageManifestRejected(diagnostic): return ContentRefreshRejected(PackageRejected(Std.string(diagnostic)));
		case ContentPackageManifestReady(manifest): manifest;
	};
	final campaignEntry = soleEntry(packageManifest, CampaignManifest);
	if (campaignEntry == null)
		return ContentRefreshRejected(PackageGraphRejected("the package must declare exactly one campaign manifest"));
	final contentEntry = soleEntry(packageManifest, ContentPack);
	final uiEntry = soleEntry(packageManifest, LocalizationCatalog);
	final runtimeEntry = soleEntry(packageManifest, RuntimeContent);
	final assetEntry = soleEntry(packageManifest, AssetManifest);
	if (contentEntry == null || uiEntry == null || runtimeEntry == null || assetEntry == null)
		return ContentRefreshRejected(PackageGraphRejected("the package is missing its content pack, UI catalog, runtime receipt, or asset manifest"));

	final campaignPath = campaignEntry.logicalPath.text();
	final campaignBytes = switch readRefreshBytes(source, campaignPath) {
		case RefreshBytesRejected(error): return ContentRefreshRejected(error);
		case RefreshBytesReady(bytes): bytes;
	};
	final campaign = switch decodeCampaignManifest(campaignBytes) {
		case CampaignManifestRejected(diagnostic): return ContentRefreshRejected(CampaignRejected(campaignPath, Std.string(diagnostic)));
		case CampaignManifestReady(manifest): manifest;
	};
	if (levelPath != null && !campaignHasLevel(campaign, levelPath))
		return ContentRefreshRejected(UnknownLevel(levelPath));

	final contentBytes = switch readRefreshBytes(source, contentEntry.logicalPath.text()) {
		case RefreshBytesRejected(error): return ContentRefreshRejected(error);
		case RefreshBytesReady(bytes): bytes;
	};
	final registry = switch RuntimeContentPack.decode(contentBytes, RuntimeAssetInventory.reviewedBase()) {
		case RuntimeContentPackRejected(diagnostic): return ContentRefreshRejected(ContentPackRejected(Std.string(diagnostic)));
		case RuntimeContentPackReady(value): value;
	};

	final files:Array<ContentRefreshFile> = [];
	final mapReceipts:Array<ContentReceipt> = [];
	for (index in 0...campaign.levelCount()) {
		final level = campaign.levelAt(index);
		if (levelPath != null && level.logicalPath != levelPath)
			continue;
		final sourceBytes = switch readRefreshBytes(source, level.logicalPath) {
			case RefreshBytesRejected(error): return ContentRefreshRejected(error);
			case RefreshBytesReady(bytes): bytes;
		};
		final candidate = replacement != null ? replacement : sourceBytes;
		final canonical = switch canonicalLevel(candidate, registry, level.logicalPath) {
			case ContentRefreshRejected(error): return ContentRefreshRejected(error);
			case ContentRefreshReady(plan): plan.fileAt(0).next;
		};
		files.push(new ContentRefreshFile(level.logicalPath, sourceBytes, canonical));
		mapReceipts.push(receipt(level.logicalPath, canonical));
	}

	final nextCampaign = writeCampaignManifest(campaign, mapReceipts);
	files.push(new ContentRefreshFile(campaignPath, campaignBytes, nextCampaign));
	final packageUpdates = mapReceipts.copy();
	packageUpdates.push(receipt(campaignPath, nextCampaign));

	final runtimePath = runtimeEntry.logicalPath.text();
	final runtimeBytes = switch readRefreshBytes(source, runtimePath) {
		case RefreshBytesRejected(error): return ContentRefreshRejected(error);
		case RefreshBytesReady(bytes): bytes;
	};
	final runtimeMapPath = "scenarios/first-playable/map.caxemap";
	final runtimeMapBytes = plannedOrSource(files, source, runtimeMapPath);
	if (runtimeMapBytes == null)
		return ContentRefreshRejected(PackageGraphRejected('the runtime map is missing: $runtimeMapPath'));
	final uiBytes = switch readRefreshBytes(source, uiEntry.logicalPath.text()) {
		case RefreshBytesRejected(error): return ContentRefreshRejected(error);
		case RefreshBytesReady(bytes): bytes;
	};
	final assetBytes = switch readRefreshBytes(source, assetEntry.logicalPath.text()) {
		case RefreshBytesRejected(error): return ContentRefreshRejected(error);
		case RefreshBytesReady(bytes): bytes;
	};
	final assetReceipt = receipt(assetEntry.logicalPath.text(), assetBytes);
	if (assetReceipt.sha256 != RuntimeAssetInventory.REVIEWED_MANIFEST_SHA256)
		return ContentRefreshRejected(PackageGraphRejected("the asset manifest does not match the reviewed runtime inventory"));
	final nextRuntime = writeRuntimeContentReceipt(RuntimeAssetInventory.reviewedBase().manifestId(), assetReceipt,
		receipt(contentEntry.logicalPath.text(), contentBytes), receipt(uiEntry.logicalPath.text(), uiBytes), receipt(runtimeMapPath, runtimeMapBytes));
	files.push(new ContentRefreshFile(runtimePath, runtimeBytes, nextRuntime));
	packageUpdates.push(receipt(runtimePath, nextRuntime));

	final nextPackage = writeContentPackageManifest(packageManifest, packageUpdates);
	files.push(new ContentRefreshFile(manifestPath, packageBytes, nextPackage));
	final overlay = new RefreshOverlaySource(source, files);
	switch loadContentPackage(overlay, manifestPath) {
		case ContentPackageRejected(error):
			return ContentRefreshRejected(PackageGraphRejected(Std.string(error)));
		case ContentPackageReady(_):
	}
	return ContentRefreshReady(new ContentRefreshPlan(files));
}

/** Canonicalize one map through the same typed parser and content registry as play. */
private function canonicalLevel(bytes:Bytes, registry:RuntimeContentRegistry, logicalPath:String):ContentRefreshResult {
	final scenario = switch ScenarioLexer.read(bytes) {
		case ReadError(diagnostics): return ContentRefreshRejected(LevelRejected(logicalPath, Std.string(diagnostics[0])));
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError(diagnostics): return ContentRefreshRejected(LevelRejected(logicalPath, Std.string(diagnostics[0])));
				case ReadOk(parsed):
					switch ScenarioValidator.validate(parsed, registry) {
						case ReadError(diagnostics): return ContentRefreshRejected(LevelRejected(logicalPath, Std.string(diagnostics[0])));
						case ReadOk(value): value;
					}
			}
	};
	final canonical = ScenarioWriter.write(scenario);
	return ContentRefreshReady(new ContentRefreshPlan([new ContentRefreshFile(logicalPath, bytes, canonical)]));
}

/** Read one source file into an independent owner. */
private function readRefreshBytes(source:ContentPackageSource, logicalPath:String):RefreshReadResult
	return switch source.read(logicalPath) {
		case PackageBytesRejected(error): RefreshBytesRejected(SourceRejected(logicalPath, error));
		case PackageBytesRead(value): RefreshBytesReady(value.bytes.sub(0, value.bytes.length));
	};

/** Find exactly one entry of a required kind. */
private function soleEntry(manifest:ContentPackageManifest.ContentPackageManifest, kind:ContentPackageEntryKind):Null<ContentPackageEntry> {
	var found:Null<ContentPackageEntry> = null;
	for (index in 0...manifest.entryCount()) {
		final candidate = manifest.entryAt(index);
		if (!sameKind(candidate.kind, kind))
			continue;
		if (found != null)
			return null;
		found = candidate;
	}
	return found;
}

/** Compare closed entry kinds without relying on enum integer layout. */
private function sameKind(left:ContentPackageEntryKind, right:ContentPackageEntryKind):Bool
	return packageKindRank(left) == packageKindRank(right);

/** Give each closed entry kind a local comparison rank. */
private function packageKindRank(kind:ContentPackageEntryKind):Int
	return switch kind {
		case Asset: 0;
		case AssetManifest: 1;
		case CampaignManifest: 2;
		case ContentPack: 3;
		case Level: 4;
		case LocalizationCatalog: 5;
		case RuntimeContent: 6;
	};

/** True when the campaign owns one exact map path. */
private function campaignHasLevel(manifest:CampaignManifest.CampaignManifest, logicalPath:String):Bool {
	for (index in 0...manifest.levelCount())
		if (manifest.levelAt(index).logicalPath == logicalPath)
			return true;
	return false;
}

/** Compute one receipt with the repository-owned digest worker. */
private function receipt(logicalPath:String, bytes:Bytes):ContentReceipt
	return new ContentReceipt(logicalPath, bytes.length, runtimeSha256Hex(bytes));

/** Return final planned bytes for one path, or read its unchanged source bytes. */
private function plannedOrSource(files:Array<ContentRefreshFile>, source:ContentPackageSource, logicalPath:String):Null<Bytes> {
	for (file in files)
		if (file.logicalPath == logicalPath)
			return file.next;
	return switch readRefreshBytes(source, logicalPath) {
		case RefreshBytesRejected(_): null;
		case RefreshBytesReady(bytes): bytes;
	};
}
