package caxecraft.tools;

import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageRefresh.ContentRefreshError;
import caxecraft.content.ContentPackageRefresh.ContentRefreshFile;
import caxecraft.content.ContentPackageRefresh.ContentRefreshPlan;
import caxecraft.content.ContentPackageRefresh.ContentRefreshResult;
import caxecraft.content.ContentPackageRefresh.planContentPackageRefresh;
import caxecraft.content.ContentPackageStore;
import haxe.io.Path;
import sys.FileSystem;
import sys.io.File;

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
		return publish(root, plan) ? 0 : 2;
	}

	/** Show each changed path and its old and new byte counts. */
	static function showPlan(plan:ContentRefreshPlan):Void {
		for (index in 0...plan.fileCount()) {
			final file = plan.fileAt(index);
			if (file.changed())
				Sys.println('content-refresh: ${file.logicalPath}: ${file.previous.length} -> ${file.next.length} bytes');
		}
	}

	/** Publish all changed files or restore every original after an error. */
	static function publish(root:String, plan:ContentRefreshPlan):Bool {
		final changed:Array<ContentRefreshFile> = [];
		for (index in 0...plan.fileCount())
			if (plan.fileAt(index).changed())
				changed.push(plan.fileAt(index));
		final suffix = '.caxecraft-refresh-${Date.now().getTime()}';
		var staged = 0;
		var backedUp = 0;
		var published = 0;
		try {
			for (file in changed) {
				final target = Path.join([root, file.logicalPath]);
				if (File.getBytes(target).compare(file.previous) != 0)
					throw new haxe.Exception('source changed while planning: ${file.logicalPath}');
				final temporary = target + suffix + ".new";
				final backup = target + suffix + ".old";
				if (FileSystem.exists(temporary) || FileSystem.exists(backup))
					throw new haxe.Exception('stale refresh file blocks publication: ${file.logicalPath}');
				File.saveBytes(temporary, file.next);
				if (File.getBytes(temporary).compare(file.next) != 0)
					throw new haxe.Exception('staged bytes changed after write: ${file.logicalPath}');
				staged++;
			}
			for (file in changed) {
				final target = Path.join([root, file.logicalPath]);
				FileSystem.rename(target, target + suffix + ".old");
				backedUp++;
			}
			for (file in changed) {
				final target = Path.join([root, file.logicalPath]);
				FileSystem.rename(target + suffix + ".new", target);
				published++;
			}
		} catch (error:haxe.Exception) {
			rollback(root, changed, suffix, staged, backedUp, published);
			Sys.stderr().writeString('content-refresh: no changes published: ${error.message}\n');
			return false;
		}
		for (file in changed) {
			final backup = Path.join([root, file.logicalPath]) + suffix + ".old";
			try {
				FileSystem.deleteFile(backup);
			} catch (error:haxe.Exception) {
				Sys.stderr().writeString('content-refresh: updated content, but could not remove backup $backup: ${error.message}\n');
			}
		}
		Sys.println('content-refresh: updated ${changed.length} files without rebuilding Caxecraft');
		return true;
	}

	/** Restore originals and remove every staged file after a publication error. */
	static function rollback(root:String, files:Array<ContentRefreshFile>, suffix:String, staged:Int, backedUp:Int, published:Int):Void {
		var index = published - 1;
		while (index >= 0) {
			final target = Path.join([root, files[index].logicalPath]);
			if (FileSystem.exists(target))
				FileSystem.deleteFile(target);
			index--;
		}
		index = backedUp - 1;
		while (index >= 0) {
			final target = Path.join([root, files[index].logicalPath]);
			final backup = target + suffix + ".old";
			if (FileSystem.exists(backup))
				FileSystem.rename(backup, target);
			index--;
		}
		index = 0;
		while (index < staged) {
			final temporary = Path.join([root, files[index].logicalPath]) + suffix + ".new";
			if (FileSystem.exists(temporary))
				FileSystem.deleteFile(temporary);
			index++;
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
