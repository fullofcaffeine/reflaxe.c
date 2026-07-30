package caxecraft.content;

import caxecraft.content.ContentPackagePath.ContentPackagePathError;
import haxe.io.Bytes;

/**
	Defines the values that cross Caxecraft's package-read boundary.

	The game receives owned bytes, a small source receipt, or one closed error.
	Host paths, file descriptors, POSIX status values, and Raylib handles stay
	below this model, so gameplay and editor code cannot accidentally acquire
	ambient filesystem authority.
**/
/** Stable error categories shared by Eval and hosted native adapters. */
enum ContentPackageError {
	InvalidLogicalPath(error:ContentPackagePathError);
	InvalidByteLimit;
	RootUnavailable;
	RootNotDirectory;
	RootChanged;
	EntryMissing;
	EntryIsSymlink;
	EntryNotFile;
	EntryTooLarge(maximumBytes:Int);
	EntryChangedDuringRead;
	ReadFailed;
	CloseFailed;
	UnsupportedCapability;
}

/**
	A small receipt identifying the source of one successful byte vector.

	The root label is application-owned and deliberately replaces the unrestricted
	host path in ordinary diagnostics. `readAttempts` is one for a stable file and
	two when the bounded changed-file retry succeeded.
**/
final class ContentPackageProvenance {
	/** Human-facing capability name selected by the application. */
	public final rootLabel:String;

	/** Validated package-relative path used for this read. */
	public final logicalPath:ContentPackagePath;

	/** Exact number of bytes returned to the caller. */
	public final byteLength:Int;

	/** Number of complete metadata/read attempts, bounded to one or two. */
	public final readAttempts:Int;

	public function new(rootLabel:String, logicalPath:ContentPackagePath, byteLength:Int, readAttempts:Int) {
		this.rootLabel = rootLabel;
		this.logicalPath = logicalPath;
		this.byteLength = byteLength;
		this.readAttempts = readAttempts;
	}
}

/**
	One independently owned package payload and its source receipt.

	The caller owns `bytes`; native code only borrowed its temporary allocation
	during the read call and retains no pointer afterward.
**/
final class LoadedPackageBytes {
	/** Exact bytes read from the opened regular file. */
	public final bytes:Bytes;

	/** Receipt for the capability and logical path that produced `bytes`. */
	public final provenance:ContentPackageProvenance;

	public function new(bytes:Bytes, provenance:ContentPackageProvenance) {
		this.bytes = bytes;
		this.provenance = provenance;
	}
}

/** Opening a root either creates one capability or returns a closed error. */
enum ContentPackageOpenResult {
	PackageStoreOpened(store:ContentPackageStore);
	PackageStoreRejected(error:ContentPackageError);
}

/** Reading a logical path either transfers owned bytes or returns an error. */
enum ContentPackageReadResult {
	PackageBytesRead(content:LoadedPackageBytes);
	PackageBytesRejected(error:ContentPackageError);
}
