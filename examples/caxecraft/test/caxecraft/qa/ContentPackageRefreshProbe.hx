package caxecraft.qa;

import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageRefresh.ContentRefreshResult;
import caxecraft.content.ContentPackageRefresh.planContentPackageRefresh;
import caxecraft.content.ContentPackageStore;
import haxe.io.Bytes;
import sys.io.File;

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

		switch planContentPackageRefresh(store, "caxecraft.package.json", "scenarios/missing.caxemap", null) {
			case ContentRefreshReady(_):
				throw new haxe.Exception("an unknown level produced a plausible refresh plan");
			case ContentRefreshRejected(UnknownLevel(path)):
				require(path == "scenarios/missing.caxemap", "the unknown-level error lost its path");
			case ContentRefreshRejected(error):
				throw new haxe.Exception('an unknown level produced the wrong error: ${Std.string(error)}');
		}
		Sys.println("content-package-refresh: current package, canonical editor bytes, four-file dependency cascade, and unknown-level rejection passed");
	}

	/** Unwrap one expected complete plan. */
	static function ready(result:ContentRefreshResult):caxecraft.content.ContentPackageRefresh.ContentRefreshPlan
		return switch result {
			case ContentRefreshRejected(error): throw new haxe.Exception('refresh planning failed: ${Std.string(error)}');
			case ContentRefreshReady(plan): plan;
		};

	/** Stop with one focused assertion message. */
	static function require(condition:Bool, message:String):Void {
		if (!condition)
			throw new haxe.Exception(message);
	}
}
