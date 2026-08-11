package caxecraft.tools;

import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageRefresh.ContentRefreshError;
import caxecraft.content.ContentPackageRefresh.ContentRefreshPlan;
import caxecraft.content.ContentPackageRefresh.ContentRefreshResult;
import caxecraft.content.ContentPackageRefresh.planContentPackageRefresh;
import caxecraft.content.ContentRefreshPublisher.ContentRefreshPublication;
import caxecraft.content.ContentRefreshPublisher.publishContentRefresh;
import caxecraft.content.ContentPackageStore;

/**
 * Checks or refreshes Caxecraft package metadata after a content edit.
 *
 * The command opens one confined package source, asks the shared Haxe planner
 * for all final bytes, and writes changed files as one rollback-capable group.
 * It never compiles or replaces the Caxecraft executable.
 */
final class ContentRefreshMain {
	/** Parse the small command surface and return a process status. */
	public static function main():Void {
		final status = operate(Sys.args());
		if (status != 0)
			Sys.exit(status);
	}

	/** Run one check or write request. */
	static function operate(arguments:Array<String>):Int {
		var root = ".";
		var manifest = "caxecraft.package.json";
		var level:Null<String> = null;
		var checkOnly = false;
		var index = 0;
		while (index < arguments.length) {
			final argument = arguments[index];
			if (argument == "--check")
				checkOnly = true;
			else if (argument == "--package-root") {
				index++;
				if (index >= arguments.length)
					return usage("--package-root requires a directory");
				root = arguments[index];
			} else if (argument == "--manifest") {
				index++;
				if (index >= arguments.length)
					return usage("--manifest requires a package-relative path");
				manifest = arguments[index];
			} else if (argument == "--level") {
				index++;
				if (index >= arguments.length)
					return usage("--level requires a package-relative CAXEMAP path");
				level = arguments[index];
			} else
				return usage('unknown argument: $argument');
			index++;
		}

		final store = switch ContentPackageStore.open(root, "content-refresh", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
			case PackageStoreRejected(error):
				Sys.stderr().writeString('content-refresh: cannot open package root: ${Std.string(error)}\n');
				return 2;
			case PackageStoreOpened(value): value;
		};
		final plan = switch planContentPackageRefresh(store, manifest, level, null) {
			case ContentRefreshRejected(error):
				Sys.stderr().writeString('content-refresh: ${describe(error)}\n');
				return 2;
			case ContentRefreshReady(value): value;
		};
		showPlan(plan);
		if (!plan.hasChanges()) {
			Sys.println("content-refresh: package metadata is current");
			return 0;
		}
		if (checkOnly) {
			Sys.stderr().writeString("content-refresh: package metadata needs a refresh\n");
			return 1;
		}
		return switch publishContentRefresh(root, plan) {
			case RefreshPublishRejected(detail):
				Sys.stderr().writeString('content-refresh: no changes published: $detail\n');
				2;
			case RefreshPublished(changed, warnings):
				for (warning in warnings)
					Sys.stderr().writeString('content-refresh: updated content, but could not remove backup $warning\n');
				Sys.println('content-refresh: updated $changed files without rebuilding Caxecraft');
				0;
		};
	}

	/** Show each changed path and its old and new byte counts. */
	static function showPlan(plan:ContentRefreshPlan):Void {
		for (index in 0...plan.fileCount()) {
			final file = plan.fileAt(index);
			if (file.changed())
				Sys.println('content-refresh: ${file.logicalPath}: ${file.previous.length} -> ${file.next.length} bytes');
		}
	}

	/** Convert one closed planning error into an actionable command message. */
	static function describe(error:ContentRefreshError):String
		return switch error {
			case SourceRejected(path, reason): '$path could not be read: ${Std.string(reason)}';
			case PackageRejected(detail): 'the package manifest is invalid: $detail';
			case CampaignRejected(path, detail): '$path is invalid: $detail';
			case PackageGraphRejected(detail): 'the package graph is inconsistent: $detail';
			case ContentPackRejected(detail): 'the content pack is invalid: $detail';
			case LevelRejected(path, detail): '$path is invalid: $detail';
			case UnknownLevel(path): 'the campaign does not own level $path';
			case ReplacementNeedsLevel: "an in-memory replacement requires one level path";
		};

	/** Show one usage error and the complete supported command form. */
	static function usage(error:String):Int {
		Sys.stderr().writeString('content-refresh: $error\n');
		Sys.stderr().writeString("usage: ContentRefreshMain [--check] [--package-root DIR] [--manifest PATH] [--level PATH]\n");
		return 2;
	}
}
