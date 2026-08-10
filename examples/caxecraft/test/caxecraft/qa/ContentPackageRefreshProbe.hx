package caxecraft.qa;

import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageModel.ContentPackageProvenance;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackageManifest.ContentPackageManifestReadResult;
import caxecraft.content.ContentPackageManifest.decodeContentPackageManifest;
import caxecraft.content.ContentPackageRefresh.ContentRefreshResult;
import caxecraft.content.ContentPackageRefresh.planContentPackageRefresh;
import caxecraft.content.ContentPackageSource;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.RuntimeContentDigest.runtimeSha256Hex;
import haxe.io.Bytes;
import sys.io.File;

/** Replace one creator-owned file while preserving confined source provenance. */
private final class ReplacedContentSource implements ContentPackageSource {
	final source:ContentPackageSource;
	final path:String;
	final replacement:Bytes;

	/** Retain the replacement bytes used by one in-memory refresh request. */
	public function new(source:ContentPackageSource, path:String, replacement:Bytes) {
		this.source = source;
		this.path = path;
		this.replacement = replacement;
	}

	/** Return changed bytes only for the selected package-relative path. */
	public function read(logicalPath:String):ContentPackageReadResult {
		final original = source.read(logicalPath);
		if (logicalPath != path)
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

/** Proves the read-only package planner and the editor-owned replacement seam. */
final class ContentPackageRefreshProbe {
	/** Run current, canonical-input, changed-input, and unknown-level scenarios. */
	public static function main():Void {
		final store = switch ContentPackageStore.open(".", "content-refresh-probe", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
			case PackageStoreRejected(error): throw new haxe.Exception('package root rejected: ${Std.string(error)}');
			case PackageStoreOpened(value): value;
		};
		final current = ready(planContentPackageRefresh(store, "caxecraft.package.json", null, null));
		require(!current.hasChanges(), "the checked-in package needs a refresh");

		final levelPath = "scenarios/first-playable/map.caxemap";
		final canonical = File.getBytes(levelPath);
		final crlf = Bytes.ofString(StringTools.replace(canonical.toString(), "\n", "\r\n"));
		final normalized = ready(planContentPackageRefresh(store, "caxecraft.package.json", levelPath, crlf));
		require(!normalized.hasChanges(), "an in-memory CRLF edit did not converge on the checked-in canonical package");

		final changedBytes = Bytes.ofString(StringTools.replace(canonical.toString(), "Caxecraft: Evergrove Prologue", "Caxecraft: Evergrove Preview"));
		final changed = ready(planContentPackageRefresh(store, "caxecraft.package.json", levelPath, changedBytes));
		require(changed.fileCount() == 4, "one runtime map edit did not plan four dependency-ordered files");
		require(changed.fileAt(0).logicalPath == levelPath, "the map was not first in refresh order");
		require(changed.fileAt(1).logicalPath == "campaigns/first-adventure/campaign.json", "the campaign was not second in refresh order");
		require(changed.fileAt(2).logicalPath == "packs/caxecraft/base/runtime-content.json", "the runtime receipt was not third in refresh order");
		require(changed.fileAt(3).logicalPath == "caxecraft.package.json", "the outer package was not last in refresh order");
		for (index in 0...changed.fileCount())
			require(changed.fileAt(index).changed(), 'planned dependency did not change: ${changed.fileAt(index).logicalPath}');

		final contentPath = "packs/caxecraft/base/content.json";
		final contentBytes = File.getBytes(contentPath);
		final changedContent = Bytes.ofString("\n" + contentBytes.toString());
		final contentPlan = ready(planContentPackageRefresh(new ReplacedContentSource(store, contentPath, changedContent), "caxecraft.package.json", null,
			null));
		final runtimeReceipt = file(contentPlan, "packs/caxecraft/base/runtime-content.json");
		final outerPackage = file(contentPlan, "caxecraft.package.json");
		final expectedHash = runtimeSha256Hex(changedContent);
		require(runtimeReceipt.changed() && outerPackage.changed(), "a base-pack edit did not refresh both dependent receipts");
		require(runtimeReceipt.next.toString().indexOf('"byteLength": ${changedContent.length}') >= 0
			&& runtimeReceipt.next.toString().indexOf(expectedHash) >= 0,
			"the runtime receipt lost the changed base-pack receipt");
		final nextManifest = switch decodeContentPackageManifest(outerPackage.next) {
			case ContentPackageManifestRejected(diagnostic): throw new haxe.Exception('refreshed package did not decode: ${Std.string(diagnostic)}');
			case ContentPackageManifestReady(manifest): manifest;
		};
		var contentReceiptFound = false;
		for (index in 0...nextManifest.entryCount()) {
			final entry = nextManifest.entryAt(index);
			if (entry.logicalPath.text() == contentPath)
				contentReceiptFound = entry.byteLength == changedContent.length && entry.sha256 == expectedHash;
		}
		require(contentReceiptFound, "the outer package lost the changed base-pack receipt");

		final uiPath = "locales/ui.json";
		final uiBytes = File.getBytes(uiPath);
		final changedUi = Bytes.ofString(StringTools.replace(uiBytes.toString(), "E TALK", "E INTERACT"));
		final uiPlan = ready(planContentPackageRefresh(new ReplacedContentSource(store, uiPath, changedUi), "caxecraft.package.json", null, null));
		final uiPackage = file(uiPlan, "caxecraft.package.json");
		final expectedUiHash = runtimeSha256Hex(changedUi);
		final nextUiManifest = switch decodeContentPackageManifest(uiPackage.next) {
			case ContentPackageManifestRejected(diagnostic): throw new haxe.Exception('UI refresh package did not decode: ${Std.string(diagnostic)}');
			case ContentPackageManifestReady(manifest): manifest;
		};
		var uiReceiptFound = false;
		for (index in 0...nextUiManifest.entryCount()) {
			final entry = nextUiManifest.entryAt(index);
			if (entry.logicalPath.text() == uiPath)
				uiReceiptFound = entry.byteLength == changedUi.length && entry.sha256 == expectedUiHash;
		}
		require(uiReceiptFound, "the outer package lost the changed UI-catalog receipt");

		final assetPath = "assets/manifest.json";
		final assetBytes = File.getBytes(assetPath);
		final changedAssets = Bytes.ofString("\n" + assetBytes.toString());
		final assetPlan = ready(planContentPackageRefresh(new ReplacedContentSource(store, assetPath, changedAssets), "caxecraft.package.json", null, null));
		final assetRuntime = file(assetPlan, "packs/caxecraft/base/runtime-content.json");
		final assetPackage = file(assetPlan, "caxecraft.package.json");
		final expectedAssetHash = runtimeSha256Hex(changedAssets);
		require(assetRuntime.changed() && assetPackage.changed(), "an asset-manifest edit did not refresh both dependent receipts");
		require(assetRuntime.next.toString().indexOf(expectedAssetHash) >= 0, "the runtime receipt lost the changed asset-manifest receipt");
		final nextAssetManifest = switch decodeContentPackageManifest(assetPackage.next) {
			case ContentPackageManifestRejected(diagnostic):
				throw new haxe.Exception('asset refresh package did not decode: ${Std.string(diagnostic)}');
			case ContentPackageManifestReady(manifest): manifest;
		};
		var assetReceiptFound = false;
		for (index in 0...nextAssetManifest.entryCount()) {
			final entry = nextAssetManifest.entryAt(index);
			if (entry.logicalPath.text() == assetPath)
				assetReceiptFound = entry.byteLength == changedAssets.length && entry.sha256 == expectedAssetHash;
		}
		require(assetReceiptFound, "the outer package lost the changed asset-manifest receipt");
		switch planContentPackageRefresh(new ReplacedContentSource(store, contentPath, Bytes.ofString("{")), "caxecraft.package.json", null, null) {
			case ContentRefreshRejected(ContentPackRejected(_)):
			case ContentRefreshReady(_):
				throw new haxe.Exception("a malformed base pack produced a plausible refresh plan");
			case ContentRefreshRejected(error):
				throw new haxe.Exception('a malformed base pack produced the wrong error: ${Std.string(error)}');
		}

		switch planContentPackageRefresh(store, "caxecraft.package.json", "scenarios/missing.caxemap", null) {
			case ContentRefreshReady(_):
				throw new haxe.Exception("an unknown level produced a plausible refresh plan");
			case ContentRefreshRejected(UnknownLevel(path)):
				require(path == "scenarios/missing.caxemap", "the unknown-level error lost its path");
			case ContentRefreshRejected(error):
				throw new haxe.Exception('an unknown level produced the wrong error: ${Std.string(error)}');
		}
		Sys.println("content-package-refresh: current package, map, base-pack, UI, and asset receipt cascades, canonical editor bytes, and unknown-level rejection passed");
	}

	/** Unwrap one expected complete plan. */
	static function ready(result:ContentRefreshResult):caxecraft.content.ContentPackageRefresh.ContentRefreshPlan
		return switch result {
			case ContentRefreshRejected(error): throw new haxe.Exception('refresh planning failed: ${Std.string(error)}');
			case ContentRefreshReady(plan): plan;
		};

	/** Find one exact planned path or stop before a partial assertion can pass. */
	static function file(plan:caxecraft.content.ContentPackageRefresh.ContentRefreshPlan,
			logicalPath:String):caxecraft.content.ContentPackageRefresh.ContentRefreshFile {
		for (index in 0...plan.fileCount())
			if (plan.fileAt(index).logicalPath == logicalPath)
				return plan.fileAt(index);
		throw new haxe.Exception('refresh plan omitted $logicalPath');
	}

	/** Stop with one focused assertion message. */
	static function require(condition:Bool, message:String):Void {
		if (!condition)
			throw new haxe.Exception(message);
	}
}
