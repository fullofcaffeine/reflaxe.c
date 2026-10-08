package caxecraft.content;

import caxecraft.content.ContentPackageRefresh.ContentRefreshFile;
import caxecraft.content.ContentPackageRefresh.ContentRefreshPlan;
#if (c && caxecraft_posix_hosted)
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.hosted.PosixPackageApi.createExact as createPosixFile;
import caxecraft.content.hosted.PosixPackageApi.deleteEntry as deletePosixEntry;
import caxecraft.content.hosted.PosixPackageApi.inspect as inspectPosixFile;
import caxecraft.content.hosted.PosixPackageApi.openRoot as openPosixRoot;
import caxecraft.content.hosted.PosixPackageApi.readExact as readPosixFile;
import caxecraft.content.hosted.PosixPackageApi.renameSibling as renamePosixSibling;
import caxecraft.content.hosted.PosixPackageApi.PosixRootInspection;
import caxecraft.content.hosted.PosixPackageStatus;
import caxecraft.content.hosted.PosixPackageStatus.*;
import caxecraft.content.hosted.PosixSystem;
import haxe.io.Bytes;
#elseif eval
import haxe.io.Path;
import sys.FileSystem;
import sys.io.File;
#end

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
	#if (c && caxecraft_posix_hosted)
	return publishNativeContentRefresh(root, plan);
	#elseif eval
	return publishEvalContentRefresh(root, plan);
	#else
	return RefreshPublishRejected("content publication is unavailable on this target");
	#end
}

#if (c && caxecraft_posix_hosted)
/** One validated target and its private sibling names for a native transaction. */
private typedef NativeRefreshFile = {
	/** Planned source and final bytes. */
	final file:ContentRefreshFile;

	/** Existing package entry that the plan replaces. */
	final target:ContentPackagePath;

	/** Exclusive stage entry that receives `file.next`. */
	final temporary:ContentPackagePath;

	/** Private sibling that retains the original during commit. */
	final backup:ContentPackagePath;
}

/** Process-local sequence keeps simultaneous saves from sharing stage names. */
private var nativePublicationSerial:Int = 0;

/** Publish through typed POSIX statuses without making hosted exceptions reachable. */
private function publishNativeContentRefresh(root:String, plan:ContentRefreshPlan):ContentRefreshPublication {
	if (root.length == 0 || root.indexOf("\x00") >= 0)
		return RefreshPublishRejected("native content publication rejected its root");
	final changed:Array<ContentRefreshFile> = [];
	for (index in 0...plan.fileCount())
		if (plan.fileAt(index).changed())
			changed.push(plan.fileAt(index));
	if (changed.length == 0)
		return RefreshPublished(0, []);

	nativePublicationSerial++;
	final nonce = PosixSystem.processId() + "-" + nativePublicationSerial;
	final files:Array<NativeRefreshFile> = [];
	for (index in 0...changed.length) {
		final file = changed[index];
		final target = switch ContentPackagePath.parse(file.logicalPath) {
			case PathRejected(_): return RefreshPublishRejected('native content publication rejected path `${file.logicalPath}`');
			case PathAccepted(value): value;
		};
		final stageStem = '.caxecraft-refresh-$nonce-$index';
		final temporary = switch ContentPackagePath.parse(siblingSpelling(target, stageStem + ".new")) {
			case PathRejected(_): return RefreshPublishRejected('native content publication could not name a stage for `${file.logicalPath}`');
			case PathAccepted(value): value;
		};
		final backup = switch ContentPackagePath.parse(siblingSpelling(target, stageStem + ".old")) {
			case PathRejected(_): return RefreshPublishRejected('native content publication could not name a backup for `${file.logicalPath}`');
			case PathAccepted(value): value;
		};
		files.push({
			file: file,
			target: target,
			temporary: temporary,
			backup: backup
		});
	}

	final rootBuffer = Bytes.ofString(root + "\x00");
	final rootInspection = openPosixRoot(rootBuffer);
	if (rootInspection.status != PosixOk)
		return nativeRejected("opening the package root", root, rootInspection.status, true);

	for (entry in files) {
		final sourceStatus = exactBytesStatus(rootBuffer, rootInspection, entry.target, entry.file.previous);
		if (sourceStatus != PosixOk)
			return nativeRejected("checking planned source bytes", entry.file.logicalPath, sourceStatus, true);
		final temporaryStatus = absenceStatus(rootBuffer, rootInspection, entry.temporary);
		if (temporaryStatus != PosixOk)
			return nativeRejected("checking the private stage name", entry.file.logicalPath, temporaryStatus, true);
		final backupStatus = absenceStatus(rootBuffer, rootInspection, entry.backup);
		if (backupStatus != PosixOk)
			return nativeRejected("checking the private backup name", entry.file.logicalPath, backupStatus, true);
	}

	var staged = 0;
	var backedUp = 0;
	var published = 0;
	for (index in 0...files.length) {
		final entry = files[index];
		final created = createPosixFile(rootBuffer, rootInspection.device, rootInspection.inode, entry.temporary, entry.file.next);
		staged = created.stageOwned ? index + 1 : index;
		if (created.status != PosixOk) {
			final restored = rollbackNativeContentRefresh(rootBuffer, rootInspection, files, staged, backedUp, published);
			return nativeRejected("writing a private stage", entry.file.logicalPath, created.status, restored);
		}
		final stagedStatus = exactBytesStatus(rootBuffer, rootInspection, entry.temporary, entry.file.next);
		if (stagedStatus != PosixOk) {
			final restored = rollbackNativeContentRefresh(rootBuffer, rootInspection, files, staged, backedUp, published);
			return nativeRejected("verifying staged bytes", entry.file.logicalPath, stagedStatus, restored);
		}
	}
	for (entry in files) {
		final renameStatus = renamePosixSibling(rootBuffer, rootInspection.device, rootInspection.inode, entry.target, entry.backup);
		if (renameStatus != PosixOk) {
			final restored = rollbackNativeContentRefresh(rootBuffer, rootInspection, files, staged, backedUp, published);
			return nativeRejected("retaining original bytes", entry.file.logicalPath, renameStatus, restored);
		}
		backedUp++;
	}
	for (entry in files) {
		final renameStatus = renamePosixSibling(rootBuffer, rootInspection.device, rootInspection.inode, entry.temporary, entry.target);
		if (renameStatus != PosixOk) {
			final restored = rollbackNativeContentRefresh(rootBuffer, rootInspection, files, staged, backedUp, published);
			return nativeRejected("publishing staged bytes", entry.file.logicalPath, renameStatus, restored);
		}
		published++;
	}

	final warnings:Array<String> = [];
	for (entry in files) {
		final cleanup = deletePosixEntry(rootBuffer, rootInspection.device, rootInspection.inode, entry.backup);
		if (cleanup != PosixOk)
			warnings.push('${entry.file.logicalPath}: backup cleanup failed (${nativeStatusName(cleanup)})');
	}
	return RefreshPublished(files.length, warnings);
}

/** Read one confined entry and compare every byte with the planned owner. */
private function exactBytesStatus(root:Bytes, rootInspection:PosixRootInspection, path:ContentPackagePath, expected:Bytes):PosixPackageStatus {
	final inspected = inspectPosixFile(root, rootInspection.device, rootInspection.inode, path, ContentPackageStore.MAXIMUM_PACKAGE_BYTES);
	if (inspected.status != PosixOk)
		return inspected.status;
	if (inspected.size != expected.length)
		return PosixEntryChanged;
	final actual = Bytes.alloc(inspected.size);
	final read = readPosixFile(root, rootInspection.device, rootInspection.inode, path, inspected.size, inspected.device, inspected.inode,
		inspected.modifiedSeconds, inspected.modifiedNanoseconds, actual);
	if (read != PosixOk)
		return read;
	return actual.compare(expected) == 0 ? PosixOk : PosixEntryChanged;
}

/** Return `PosixOk` only when a private transaction name is absent. */
private function absenceStatus(root:Bytes, rootInspection:PosixRootInspection, path:ContentPackagePath):PosixPackageStatus {
	final inspected = inspectPosixFile(root, rootInspection.device, rootInspection.inode, path, ContentPackageStore.MAXIMUM_PACKAGE_BYTES);
	return inspected.status == PosixEntryMissing ? PosixOk : inspected.status == PosixOk ? PosixEntryExists : inspected.status;
}

/** Restore originals and remove every attempted stage after native failure. */
private function rollbackNativeContentRefresh(root:Bytes, rootInspection:PosixRootInspection, files:Array<NativeRefreshFile>, staged:Int, backedUp:Int,
		published:Int):Bool {
	var restored = true;
	var index = published - 1;
	while (index >= 0) {
		if (deletePosixEntry(root, rootInspection.device, rootInspection.inode, files[index].target) != PosixOk)
			restored = false;
		index--;
	}
	index = backedUp - 1;
	while (index >= 0) {
		if (renamePosixSibling(root, rootInspection.device, rootInspection.inode, files[index].backup, files[index].target) != PosixOk)
			restored = false;
		index--;
	}
	index = 0;
	while (index < staged) {
		if (deletePosixEntry(root, rootInspection.device, rootInspection.inode, files[index].temporary) != PosixOk)
			restored = false;
		index++;
	}
	return restored;
}

/** Place a private transaction leaf beside one validated package entry. */
private function siblingSpelling(target:ContentPackagePath, leaf:String):String {
	var result = "";
	for (index in 0...(target.componentCount() - 1)) {
		if (result.length > 0)
			result += "/";
		result += target.component(index);
	}
	return result.length == 0 ? leaf : result + "/" + leaf;
}

/** Build one bounded diagnostic without asking a target exception for text. */
private function nativeRejected(stage:String, path:String, status:PosixPackageStatus, restored:Bool):ContentRefreshPublication {
	final rollback = restored ? "" : "; rollback was incomplete";
	return RefreshPublishRejected('$stage for `$path` failed (${nativeStatusName(status)})$rollback');
}

/** Give each closed POSIX outcome a stable creator-facing spelling. */
private function nativeStatusName(status:PosixPackageStatus):String
	return switch status {
		case PosixOk: "ok";
		case PosixInvalidArgument: "invalid argument";
		case PosixRootUnavailable: "root unavailable";
		case PosixRootNotDirectory: "root is not a directory";
		case PosixRootChanged: "root changed";
		case PosixEntryMissing: "entry missing";
		case PosixEntrySymlink: "symbolic link rejected";
		case PosixEntryNotFile: "entry is not a regular file";
		case PosixEntryTooLarge: "entry is too large";
		case PosixEntryChanged: "entry changed";
		case PosixEntryExists: "entry already exists";
		case PosixReadFailed: "read failed";
		case PosixWriteFailed: "write failed";
		case PosixRenameFailed: "rename failed";
		case PosixDeleteFailed: "delete failed";
		case PosixCloseFailed: "close failed";
	};
#end

#if eval
/** Use the standard Haxe filesystem as the target-neutral semantic oracle. */
private function publishEvalContentRefresh(root:String, plan:ContentRefreshPlan):ContentRefreshPublication {
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
#end
