package caxecraft.content;

import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageModel.ContentPackageProvenance;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import haxe.io.Bytes;
#if c
import caxecraft.content.hosted.PosixPackageApi.inspect as inspectPosixFile;
import caxecraft.content.hosted.PosixPackageApi.openRoot as openPosixRoot;
import caxecraft.content.hosted.PosixPackageApi.readExact as readPosixFile;
import caxecraft.content.hosted.PosixPackageApi.PosixRootInspection;
import caxecraft.content.hosted.PosixPackageStatus;
#elseif eval
import sys.FileSystem;
import sys.io.File;
#end

/**
	Reads exact package bytes without giving game code ambient filesystem access.

	The application first selects one root and a diagnostic label. Each read then
	accepts only a validated logical path below that root. Eval uses the standard
	Haxe host filesystem as a semantic oracle; hosted C uses the confined POSIX
	adapter. Other profiles reject the capability explicitly.

	The native path deliberately separates inspection from reading. Inspection
	closes every descriptor before Haxe allocates the output buffer. The read
	reopens and rechecks the same root/file identity, writes into Haxe-owned
	storage for one call, and closes before returning. A changing file gets one
	complete retry and can never publish a mixed byte vector.
**/
final class ContentPackageStore {
	/** Product-wide upper bound inherited from the CAXEMAP byte contract. */
	public static inline final MAXIMUM_PACKAGE_BYTES:Int = 16 * 1024 * 1024;

	final rootLabel:String;
	final maximumBytes:Int;

	#if c
	final rootBuffer:Bytes;
	final rootInspection:PosixRootInspection;
	#elseif eval
	final canonicalRoot:String;
	#end

	#if c
	private function new(rootLabel:String, maximumBytes:Int, rootBuffer:Bytes, rootInspection:PosixRootInspection) {
		this.rootLabel = rootLabel;
		this.maximumBytes = maximumBytes;
		this.rootBuffer = rootBuffer;
		this.rootInspection = rootInspection;
	}
	#elseif eval
	private function new(rootLabel:String, maximumBytes:Int, canonicalRoot:String) {
		this.rootLabel = rootLabel;
		this.maximumBytes = maximumBytes;
		this.canonicalRoot = canonicalRoot;
	}
	#else
	private function new(rootLabel:String, maximumBytes:Int) {
		this.rootLabel = rootLabel;
		this.maximumBytes = maximumBytes;
	}
	#end

	/**
		Select one content root without exposing its host path to later callers.

		`rootLabel` is the safe name retained in provenance and diagnostics.
		`maximumBytes` is a per-store limit and may not exceed the product-wide
		16 MiB ceiling. Unsupported targets return `UnsupportedCapability`.
	**/
	public static function open(rootPath:String, rootLabel:String, maximumBytes:Int):ContentPackageOpenResult {
		if (maximumBytes <= 0 || maximumBytes > MAXIMUM_PACKAGE_BYTES)
			return PackageStoreRejected(InvalidByteLimit);
		if (rootPath.length == 0 || rootPath.indexOf("\x00") >= 0 || rootLabel.length == 0)
			return PackageStoreRejected(RootUnavailable);

		#if c
		final rootBuffer = nulTerminated(rootPath);
		final inspection = openPosixRoot(rootBuffer);
		if (inspection.status == PosixOk)
			return PackageStoreOpened(new ContentPackageStore(rootLabel, maximumBytes, rootBuffer, inspection));
		if (inspection.status == PosixRootNotDirectory)
			return PackageStoreRejected(RootNotDirectory);
		if (inspection.status == PosixCloseFailed)
			return PackageStoreRejected(CloseFailed);
		return PackageStoreRejected(RootUnavailable);
		#elseif eval
		try {
			final canonicalRoot = withoutTrailingSlash(FileSystem.fullPath(rootPath));
			if (!FileSystem.isDirectory(canonicalRoot))
				return PackageStoreRejected(RootNotDirectory);
			return PackageStoreOpened(new ContentPackageStore(rootLabel, maximumBytes, canonicalRoot));
		} catch (_:haxe.Exception) {
			return PackageStoreRejected(RootUnavailable);
		}
		#else
		return PackageStoreRejected(UnsupportedCapability);
		#end
	}

	/**
		Read one untrusted logical spelling through this root capability.

		Path validation happens before filesystem access. Success transfers an
		independent `Bytes` owner; failure returns no partial bytes and leaves the
		store reusable for another request.
	**/
	public function read(logicalPath:String):ContentPackageReadResult {
		return switch ContentPackagePath.parse(logicalPath) {
			case PathRejected(error):
				PackageBytesRejected(InvalidLogicalPath(error));
			case PathAccepted(path):
				#if c
				readNative(path);
				#elseif eval
				readEval(path);
				#else
				PackageBytesRejected(UnsupportedCapability);
				#end
		}
	}

	#if c
	/**
		Attempt a complete inspect/allocate/read transaction at most twice.

		No native descriptor is live when `Bytes.alloc` runs. A terminal
		allocation failure therefore relies only on the normal hxrt managed
		cleanup contract and cannot leak a package file handle.
	**/
	function readNative(path:ContentPackagePath):ContentPackageReadResult {
		var attempt = 1;
		while (attempt <= 2) {
			final inspected = inspectPosixFile(rootBuffer, rootInspection.device, rootInspection.inode, path, maximumBytes);
			if (inspected.status != PosixOk)
				return PackageBytesRejected(nativeError(inspected.status));

			final exact = Bytes.alloc(inspected.size);
			final read = readPosixFile(rootBuffer, rootInspection.device, rootInspection.inode, path, inspected.size, inspected.device, inspected.inode,
				inspected.modifiedSeconds, inspected.modifiedNanoseconds, exact);
			if (read == PosixOk) {
				return PackageBytesRead(new LoadedPackageBytes(exact, new ContentPackageProvenance(rootLabel, path, inspected.size, attempt)));
			}
			if (read != PosixEntryChanged || attempt == 2)
				return PackageBytesRejected(nativeError(read));
			attempt++;
		}
		return PackageBytesRejected(EntryChangedDuringRead);
	}

	/** Convert one native adapter status into the stable product error model. */
	function nativeError(status:PosixPackageStatus):ContentPackageError {
		if (status == PosixRootUnavailable)
			return RootUnavailable;
		if (status == PosixRootNotDirectory)
			return RootNotDirectory;
		if (status == PosixRootChanged)
			return RootChanged;
		if (status == PosixEntryMissing)
			return EntryMissing;
		if (status == PosixEntrySymlink)
			return EntryIsSymlink;
		if (status == PosixEntryNotFile)
			return EntryNotFile;
		if (status == PosixEntryTooLarge)
			return EntryTooLarge(maximumBytes);
		if (status == PosixEntryChanged)
			return EntryChangedDuringRead;
		if (status == PosixCloseFailed)
			return CloseFailed;
		return ReadFailed;
	}

	/** Encode one runtime string as a caller-owned NUL-terminated byte buffer. */
	static function nulTerminated(value:String):Bytes
		return Bytes.ofString(value + "\x00");

	#elseif eval
	/**
		Run the same logical policy through the pinned Eval filesystem.

		Canonical-prefix checks reject symlinks before reading. File identity,
		size, and modification time are compared around `File.getBytes`; a change
		restarts the complete operation once.
	**/
	function readEval(path:ContentPackagePath):ContentPackageReadResult {
		var attempt = 1;
		while (attempt <= 2) {
			final admitted = admitEvalPath(path);
			switch admitted {
				case EvalPathRejected(error):
					return PackageBytesRejected(error);
				case EvalPathReady(fullPath):
					try {
						final before = FileSystem.stat(fullPath);
						if (FileSystem.isDirectory(fullPath))
							return PackageBytesRejected(EntryNotFile);
						if (before.size > maximumBytes)
							return PackageBytesRejected(EntryTooLarge(maximumBytes));
						final bytes = File.getBytes(fullPath);
						final after = FileSystem.stat(fullPath);
						if (bytes.length == before.size && sameEvalFile(before, after))
							return PackageBytesRead(new LoadedPackageBytes(bytes, new ContentPackageProvenance(rootLabel, path, bytes.length, attempt)));
					} catch (_:haxe.Exception) {
						return PackageBytesRejected(ReadFailed);
					}
			}
			if (attempt == 2)
				return PackageBytesRejected(EntryChangedDuringRead);
			attempt++;
		}
		return PackageBytesRejected(EntryChangedDuringRead);
	}

	/**
		Resolve every existing prefix and require its canonical spelling to remain
		the exact lexical child of the selected canonical root.
	**/
	function admitEvalPath(path:ContentPackagePath):EvalPathAdmission {
		var prefix = canonicalRoot;
		for (index in 0...path.componentCount()) {
			prefix = joinRoot(prefix, path.component(index));
			if (!FileSystem.exists(prefix))
				return EvalPathRejected(EntryMissing);
			try {
				if (withoutTrailingSlash(FileSystem.fullPath(prefix)) != prefix)
					return EvalPathRejected(EntryIsSymlink);
				if (index + 1 < path.componentCount() && !FileSystem.isDirectory(prefix))
					return EvalPathRejected(EntryNotFile);
			} catch (_:haxe.Exception) {
				return EvalPathRejected(ReadFailed);
			}
		}
		return EvalPathReady(prefix);
	}

	/** Compare all Eval file facts available through the standard host API. */
	static function sameEvalFile(left:sys.FileStat, right:sys.FileStat):Bool
		return left.dev == right.dev
			&& left.ino == right.ino
			&& left.size == right.size
			&& left.mtime.getTime() == right.mtime.getTime();

	/** Append one validated component without creating a double root slash. */
	static function joinRoot(root:String, component:String):String
		return root == "/" ? root + component : root + "/" + component;

	/** Keep filesystem comparison stable while preserving the POSIX root `/`. */
	static function withoutTrailingSlash(value:String):String
		return value.length > 1 && value.charCodeAt(value.length - 1) == 47 ? value.substring(0, value.length - 1) : value;
	#end
}

#if eval
/**
	Private Eval-only path admission result.

	This stays below the product result enum because a canonical host path must
	never escape into ordinary game or editor code.
**/
private enum EvalPathAdmission {
	EvalPathReady(fullPath:String);
	EvalPathRejected(error:ContentPackageError);
}
#end
