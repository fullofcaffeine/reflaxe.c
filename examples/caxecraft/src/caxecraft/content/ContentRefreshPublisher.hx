package caxecraft.content;

import caxecraft.content.ContentPackageRefresh.ContentRefreshFile;
import caxecraft.content.ContentPackageRefresh.ContentRefreshPlan;
import haxe.io.Path;
import sys.FileSystem;
import sys.io.File;

/**
 * Publishes one complete content-refresh plan without exposing partial files.
 *
 * The planner validates all final bytes before this module writes them. This
 * module stages each changed file, keeps each old file, and then replaces the
 * complete group. An editor and the refresh command can therefore share one
 * save operation.
 */
enum ContentRefreshPublication {
	/** All changed files now contain the planned bytes. */
	RefreshPublished(changedFiles:Int, cleanupWarnings:Array<String>);

	/** Publication stopped and restored every original file. */
	RefreshPublishRejected(detail:String);
}

/** Publish all changed files, or restore all original files after an error. */
function publishContentRefresh(root:String, plan:ContentRefreshPlan):ContentRefreshPublication {
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
		rollbackContentRefresh(root, changed, suffix, staged, backedUp, published);
		return RefreshPublishRejected(error.message);
	}
	final warnings:Array<String> = [];
	for (file in changed) {
		final backup = Path.join([root, file.logicalPath]) + suffix + ".old";
		try {
			FileSystem.deleteFile(backup);
		} catch (error:haxe.Exception) {
			warnings.push('${file.logicalPath}: ${error.message}');
		}
	}
	return RefreshPublished(changed.length, warnings);
}

/** Restore originals and remove staged files after a publication error. */
private function rollbackContentRefresh(root:String, files:Array<ContentRefreshFile>, suffix:String, staged:Int, backedUp:Int, published:Int):Void {
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
