package caxecraft.content.hosted;

/**
	Names the closed outcomes produced by Haxe-authored POSIX package I/O.

	Product code converts these platform results immediately into
	`ContentPackageError`, so file-descriptor and system-call details never cross
	into gameplay or editor APIs.
**/
enum PosixPackageStatus {
	PosixOk;
	PosixInvalidArgument;
	PosixRootUnavailable;
	PosixRootNotDirectory;
	PosixRootChanged;
	PosixEntryMissing;
	PosixEntrySymlink;
	PosixEntryNotFile;
	PosixEntryTooLarge;
	PosixEntryChanged;
	PosixEntryExists;
	PosixReadFailed;
	PosixWriteFailed;
	PosixRenameFailed;
	PosixDeleteFailed;
	PosixCloseFailed;
}
