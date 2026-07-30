package caxecraft.content.hosted;

import c.CArray;
import c.UInt8;
import caxecraft.content.ContentPackagePath;
import caxecraft.content.hosted.PosixPackageStatus.*;
import haxe.io.Bytes;

/**
	Reads confined POSIX package bytes with behavior authored in Haxe.

	System headers supply only ABI declarations and constants through
	`PosixSystem`. This module owns path-component traversal, no-follow policy,
	file identity checks, exact bounded reads, retries, and descriptor cleanup.
	Keeping those decisions in Haxe exercises haxe.c's metal surface while
	leaving ordinary game and editor code unaware of POSIX.

	This is not the general Haxe filesystem API. `sys.io.File` is the natural
	future haxe.c surface for trusted ordinary paths; this module exists because
	an untrusted content path additionally needs directory confinement,
	no-symlink traversal, and changed-during-read rejection that `File.getBytes`
	does not promise.

	Every managed allocation happens before opening a descriptor or after all
	descriptors close. Small primitive records carry live descriptors between
	helpers without allocating a managed object.
**/
/** Phantom compile-time identity for the 4096-byte stack read buffer. */
private final class PosixReadChunk {}

/** One descriptor-producing step without a managed allocation. */
private typedef PosixDescriptorResult = {
	/** Closed result of the attempted operation. */
	final status:PosixPackageStatus;

	/** Owned descriptor on `PosixOk`, otherwise `-1`. */
	final descriptor:Int;
}

/**
	Root facts retained by `ContentPackageStore` after the descriptor closes.

	Device and inode identity let every later read reject a selected root that
	was replaced between requests.
**/
typedef PosixRootInspection = {
	/** Closed outcome of selecting the root. */
	final status:PosixPackageStatus;

	/** Filesystem device identity, valid only on `PosixOk`. */
	final device:PosixDeviceId;

	/** Inode identity, valid only on `PosixOk`. */
	final inode:PosixInodeId;
}

/**
	Regular-file facts captured before Haxe allocates the result bytes.

	The reader reopens the file and compares every fact before and after copying
	bytes. A mismatch rejects the whole attempt rather than publishing a mixture
	of two file versions.
**/
typedef PosixFileInspection = {
	/** Closed outcome of confined traversal and inspection. */
	final status:PosixPackageStatus;

	/** Exact admitted byte count, valid only on `PosixOk`. */
	final size:Int;

	/** Filesystem device identity. */
	final device:PosixDeviceId;

	/** Inode identity within `device`. */
	final inode:PosixInodeId;

	/** Whole modification-time seconds. */
	final modifiedSeconds:PosixTime;

	/** Modification-time nanoseconds. */
	final modifiedNanoseconds:c.IntPtr;
}

/**
	One-shot failures used by the Haxe-authored native regression.

	This state exists only with `caxecraft_package_store_testing`. Production
	builds contain neither the state nor its public control functions.
**/
enum PosixPackageTestFault {
	NoFault;
	CloseOnce;
	ReadOnce;
	ShortReadOnce;
	ChangedOnce;
	RootChangedOnce;
}

private inline final READ_CHUNK_BYTES:Int = 4096;
#if caxecraft_package_store_testing
private var selectedTestFaultCode:Int = 0;
private var liveTestDescriptors:Int = 0;
#end

/**
	Select and inspect one application-owned root.

	The root may be absolute or relative because it establishes the capability;
	later logical paths remain strictly relative. The returned identity contains
	no open descriptor.
**/
function openRoot(root:Bytes):PosixRootInspection {
	final descriptor = openRootDescriptor(root);
	if (descriptor < 0)
		return failedRoot(rootOpenFailureStatus());

	var facts = emptyStat();
	var status = if (PosixSystem.inspectDescriptor(descriptor, c.Ref.to(facts)) != 0) {
		PosixRootUnavailable;
	} else if (!isDirectory(facts)) {
		PosixRootNotDirectory;
	} else {
		PosixOk;
	}
	final closeStatus = closeOwnedDescriptor(descriptor);
	if (closeStatus != PosixOk)
		status = closeStatus;
	if (status != PosixOk)
		return failedRoot(status);
	return {
		status: PosixOk,
		device: facts.device,
		inode: facts.inode
	};
}

/**
	Inspect one confined regular file and close every descriptor before return.

	`path` has already passed the target-neutral package spelling policy. This
	function still traverses its components individually with `openat` and
	`O_NOFOLLOW`, so a host symlink cannot reinterpret that safe spelling.
**/
function inspect(root:Bytes, rootDevice:PosixDeviceId, rootInode:PosixInodeId, path:ContentPackagePath, maximumBytes:Int):PosixFileInspection {
	final components = componentBuffers(path);
	final verifiedRoot = openVerifiedRoot(root, rootDevice, rootInode);
	if (verifiedRoot.status != PosixOk)
		return failedFile(verifiedRoot.status);

	final opened = openConfinedEntry(verifiedRoot.descriptor, components);
	final rootClose = closeOwnedDescriptor(verifiedRoot.descriptor);
	if (rootClose != PosixOk) {
		if (opened.descriptor >= 0)
			closeOwnedDescriptor(opened.descriptor);
		return failedFile(rootClose);
	}
	if (opened.status != PosixOk)
		return failedFile(opened.status);

	var facts = emptyStat();
	var status = if (PosixSystem.inspectDescriptor(opened.descriptor, c.Ref.to(facts)) != 0) {
		PosixReadFailed;
	} else if (!isRegularFile(facts)) {
		PosixEntryNotFile;
	} else {
		final zero = zeroOffset();
		final maximum:PosixOffset = c.IntConvert.exact(maximumBytes);
		facts.size < zero
		|| facts.size > maximum ? PosixEntryTooLarge : PosixOk;
	}
	final fileClose = closeOwnedDescriptor(opened.descriptor);
	if (fileClose != PosixOk)
		status = fileClose;
	if (status != PosixOk)
		return failedFile(status);

	final sizeBits:UInt = c.IntConvert.modulo(facts.size);
	final size:Int = sizeBits;
	return {
		status: PosixOk,
		size: size,
		device: facts.device,
		inode: facts.inode,
		modifiedSeconds: facts.modified.seconds,
		modifiedNanoseconds: facts.modified.nanoseconds
	};
}

/**
	Reopen, verify, and exactly fill caller-owned Haxe bytes.

	`output` must have `expectedSize` bytes. The function does no managed
	allocation after opening the root. It copies through fixed stack storage,
	checks for early end-of-file and growth, compares metadata after reading,
	and closes both descriptors on every return edge.
**/
function readExact(root:Bytes, rootDevice:PosixDeviceId, rootInode:PosixInodeId, path:ContentPackagePath, expectedSize:Int, expectedDevice:PosixDeviceId,
		expectedInode:PosixInodeId, expectedModifiedSeconds:PosixTime, expectedModifiedNanoseconds:c.IntPtr, output:Bytes):PosixPackageStatus {
	if (expectedSize < 0 || output.length != expectedSize)
		return PosixInvalidArgument;
	final components = componentBuffers(path);
	final verifiedRoot = openVerifiedRoot(root, rootDevice, rootInode);
	if (verifiedRoot.status != PosixOk)
		return verifiedRoot.status;

	final opened = openConfinedEntry(verifiedRoot.descriptor, components);
	final rootClose = closeOwnedDescriptor(verifiedRoot.descriptor);
	if (rootClose != PosixOk) {
		if (opened.descriptor >= 0)
			closeOwnedDescriptor(opened.descriptor);
		return rootClose;
	}
	if (opened.status != PosixOk)
		return opened.status;

	var before = emptyStat();
	var status = if (PosixSystem.inspectDescriptor(opened.descriptor, c.Ref.to(before)) != 0) {
		PosixReadFailed;
	} else if (!isRegularFile(before)) {
		PosixEntryNotFile;
	} else if (!sameFile(before, expectedDevice, expectedInode, expectedSize, expectedModifiedSeconds, expectedModifiedNanoseconds)) {
		PosixEntryChanged;
	} else {
		readInto(opened.descriptor, output);
	}

	if (status == PosixOk) {
		var after = emptyStat();
		if (PosixSystem.inspectDescriptor(opened.descriptor, c.Ref.to(after)) != 0) {
			status = PosixReadFailed;
		} else if (takeTestFault(ChangedOnce)
			|| !sameFile(after, expectedDevice, expectedInode, expectedSize, expectedModifiedSeconds, expectedModifiedNanoseconds)) {
			status = PosixEntryChanged;
		}
	}

	final fileClose = closeOwnedDescriptor(opened.descriptor);
	return fileClose == PosixOk ? status : fileClose;
}

#if caxecraft_package_store_testing
/** Select one Haxe-owned one-shot failure for the next matching operation. */
function setTestFault(fault:PosixPackageTestFault):Void
	selectedTestFaultCode = testFaultCode(fault);

/** Report descriptors currently owned by this Haxe module. */
function testLiveDescriptors():Int
	return liveTestDescriptors;
#end

/** Open the selected root and record test ownership only after success. */
private function openRootDescriptor(root:Bytes):Int {
	final descriptor = PosixSystem.openPath(c.CStringBufferRef.to(root),
		PosixSystem.readOnly | PosixSystem.directory | PosixSystem.closeOnExec | PosixSystem.noFollow | PosixSystem.nonBlocking);
	return recordOpenedDescriptor(descriptor);
}

/**
	Reopen the root and prove it is the same selected directory.

	The returned primitive record transfers descriptor ownership to the caller
	only on `PosixOk`.
**/
private function openVerifiedRoot(root:Bytes, expectedDevice:PosixDeviceId, expectedInode:PosixInodeId):PosixDescriptorResult {
	final descriptor = openRootDescriptor(root);
	if (descriptor < 0)
		return failedDescriptor(rootOpenFailureStatus());

	var facts = emptyStat();
	var status = if (PosixSystem.inspectDescriptor(descriptor, c.Ref.to(facts)) != 0) {
		PosixRootUnavailable;
	} else if (!isDirectory(facts)) {
		PosixRootNotDirectory;
	} else if (takeTestFault(RootChangedOnce) || facts.device != expectedDevice || facts.inode != expectedInode) {
		PosixRootChanged;
	} else {
		PosixOk;
	}
	if (status != PosixOk) {
		final closeStatus = closeOwnedDescriptor(descriptor);
		if (closeStatus != PosixOk)
			status = closeStatus;
		return failedDescriptor(status);
	}
	return {status: PosixOk, descriptor: descriptor};
}

/**
	Traverse validated components relative to one verified root descriptor.

	Intermediate descriptors are closed as soon as the next child opens. The
	root descriptor remains borrowed from the caller; success transfers only the
	final file descriptor.
**/
private function openConfinedEntry(rootDescriptor:Int, components:Array<Bytes>):PosixDescriptorResult {
	var currentDirectory = rootDescriptor;
	var ownsCurrentDirectory = false;
	for (index in 0...components.length) {
		final directoryRequired = index + 1 < components.length;
		final component = components[index];
		var flags = PosixSystem.readOnly | PosixSystem.closeOnExec | PosixSystem.noFollow | PosixSystem.nonBlocking;
		if (directoryRequired)
			flags |= PosixSystem.directory;
		final opened = recordOpenedDescriptor(PosixSystem.openAt(currentDirectory, c.CStringBufferRef.to(component), flags));
		if (opened < 0) {
			var status = openFailureStatus(currentDirectory, component, PosixSystem.errorNumber);
			if (ownsCurrentDirectory) {
				final closeStatus = closeOwnedDescriptor(currentDirectory);
				if (closeStatus != PosixOk)
					status = closeStatus;
			}
			return failedDescriptor(status);
		}
		if (ownsCurrentDirectory) {
			final closeStatus = closeOwnedDescriptor(currentDirectory);
			if (closeStatus != PosixOk) {
				closeOwnedDescriptor(opened);
				return failedDescriptor(closeStatus);
			}
		}
		if (!directoryRequired)
			return {status: PosixOk, descriptor: opened};
		currentDirectory = opened;
		ownsCurrentDirectory = true;
	}
	if (ownsCurrentDirectory)
		closeOwnedDescriptor(currentDirectory);
	return failedDescriptor(PosixInvalidArgument);
}

/** Turn one failed `openat` into the narrowest stable package status. */
private function openFailureStatus(directoryDescriptor:Int, component:Bytes, openError:Int):PosixPackageStatus {
	if (openError == PosixSystem.noEntryError)
		return PosixEntryMissing;
	if (openError == PosixSystem.symbolicLinkError)
		return PosixEntrySymlink;

	var facts = emptyStat();
	if (PosixSystem.inspectAt(directoryDescriptor, c.CStringBufferRef.to(component), c.Ref.to(facts), PosixSystem.atSymlinkNoFollow) == 0) {
		if (isSymbolicLink(facts))
			return PosixEntrySymlink;
		if (openError == PosixSystem.notDirectoryError)
			return PosixEntryNotFile;
	}
	return PosixReadFailed;
}

/** Classify one root `open` failure before any later call can replace errno. */
private function rootOpenFailureStatus():PosixPackageStatus
	return PosixSystem.errorNumber == PosixSystem.notDirectoryError ? PosixRootNotDirectory : PosixRootUnavailable;

/**
	Copy exactly `output.length` bytes through fixed automatic storage.

	A zero read before the expected end or any non-interrupt error rejects the
	whole operation. One extra-byte probe rejects growth even if timestamps have
	coarse precision.
**/
private function readInto(descriptor:Int, output:Bytes):PosixPackageStatus {
	var chunk:CArray<UInt8, PosixReadChunk> = CArray.zero(READ_CHUNK_BYTES);
	var offset = 0;
	while (offset < output.length) {
		final remaining = output.length - offset;
		final requested = remaining < READ_CHUNK_BYTES ? remaining : READ_CHUNK_BYTES;
		final nativeRequested:c.UInt64 = c.IntConvert.modulo(requested);
		var nativeRead:c.Int64 = zeroInt64();
		if (takeTestFault(ReadOnce)) {
			nativeRead = c.IntConvert.exact(-1);
		} else if (takeTestFault(ShortReadOnce)) {
			nativeRead = zeroInt64();
		} else {
			nativeRead = PosixSystem.readBytes(descriptor, c.Ref.to(chunk[0]), nativeRequested);
			while (nativeRead < zeroInt64() && PosixSystem.errorNumber == PosixSystem.interruptedError)
				nativeRead = PosixSystem.readBytes(descriptor, c.Ref.to(chunk[0]), nativeRequested);
		}
		if (nativeRead < zeroInt64())
			return PosixReadFailed;
		final countBits:UInt = c.IntConvert.modulo(nativeRead);
		final count:Int = countBits;
		if (count <= 0)
			return PosixReadFailed;
		if (count > requested)
			return PosixReadFailed;
		for (index in 0...count)
			output.set(offset + index, c.IntConvert.exact(chunk[index]));
		offset += count;
	}
	final one:c.UInt64 = c.IntConvert.modulo(1);
	var extra = PosixSystem.readBytes(descriptor, c.Ref.to(chunk[0]), one);
	while (extra < zeroInt64() && PosixSystem.errorNumber == PosixSystem.interruptedError)
		extra = PosixSystem.readBytes(descriptor, c.Ref.to(chunk[0]), one);
	return extra == zeroInt64() ? PosixOk : PosixReadFailed;
}

/** Close one owned descriptor exactly once and preserve injected failure proof. */
private function closeOwnedDescriptor(descriptor:Int):PosixPackageStatus {
	final result = PosixSystem.closeDescriptor(descriptor);
	#if caxecraft_package_store_testing
	liveTestDescriptors--;
	#end
	return result == 0 && !takeTestFault(CloseOnce) ? PosixOk : PosixCloseFailed;
}

/** Record one successfully opened descriptor for test-only leak evidence. */
private function recordOpenedDescriptor(descriptor:Int):Int {
	#if caxecraft_package_store_testing
	if (descriptor >= 0)
		liveTestDescriptors++;
	#end
	return descriptor;
}

/** Consume one matching test failure without affecting production builds. */
private function takeTestFault(expected:PosixPackageTestFault):Bool {
	#if caxecraft_package_store_testing
	if (selectedTestFaultCode == testFaultCode(expected)) {
		selectedTestFaultCode = 0;
		return true;
	}
	#end
	return false;
}

/** Map the closed test-only fault enum onto process-local scalar state. */
private function testFaultCode(fault:PosixPackageTestFault):Int
	return switch fault {
		case NoFault: 0;
		case CloseOnce: 1;
		case ReadOnce: 2;
		case ShortReadOnce: 3;
		case ChangedOnce: 4;
		case RootChangedOnce: 5;
	};

/** Convert validated Haxe components to owned NUL-terminated call buffers. */
private function componentBuffers(path:ContentPackagePath):Array<Bytes> {
	final result:Array<Bytes> = [];
	for (index in 0...path.componentCount())
		result.push(Bytes.ofString(path.component(index) + "\x00"));
	return result;
}

/** Compare one opened file against the complete pre-allocation identity. */
private function sameFile(facts:PosixStat, device:PosixDeviceId, inode:PosixInodeId, size:Int, modifiedSeconds:PosixTime, modifiedNanoseconds:c.IntPtr):Bool {
	final exactSize:PosixOffset = c.IntConvert.exact(size);
	return facts.device == device
		&& facts.inode == inode
		&& facts.size == exactSize
		&& facts.modified.seconds == modifiedSeconds
		&& facts.modified.nanoseconds == modifiedNanoseconds;
}

/** Test whether POSIX mode bits describe a directory. */
private inline function isDirectory(facts:PosixStat):Bool
	return (facts.mode & PosixSystem.fileKindMask) == PosixSystem.directoryKind;

/** Test whether POSIX mode bits describe an ordinary regular file. */
private inline function isRegularFile(facts:PosixStat):Bool
	return (facts.mode & PosixSystem.fileKindMask) == PosixSystem.regularFileKind;

/** Test whether no-follow inspection observed a symbolic link. */
private inline function isSymbolicLink(facts:PosixStat):Bool
	return (facts.mode & PosixSystem.fileKindMask) == PosixSystem.symbolicLinkKind;

/** Build initialized header-owned storage for `fstat` and `fstatat`. */
private inline function emptyStat():PosixStat
	return c.StructInit.zero();

/** Exact header-owned file offset zero. */
private inline function zeroOffset():PosixOffset
	return c.IntConvert.exact(0);

/** Exact signed 64-bit zero used by the imported `read` result. */
private inline function zeroInt64():c.Int64
	return c.IntConvert.exact(0);

/** Construct a failed descriptor result with no ownership transfer. */
private inline function failedDescriptor(status:PosixPackageStatus):PosixDescriptorResult
	return {status: status, descriptor: -1};

/** Construct failed root facts with deterministic zeroed identities. */
private function failedRoot(status:PosixPackageStatus):PosixRootInspection {
	final facts = emptyStat();
	return {status: status, device: facts.device, inode: facts.inode};
}

/** Construct failed file facts with deterministic zeroed identities. */
private function failedFile(status:PosixPackageStatus):PosixFileInspection {
	final facts = emptyStat();
	return {
		status: status,
		size: 0,
		device: facts.device,
		inode: facts.inode,
		modifiedSeconds: facts.modified.seconds,
		modifiedNanoseconds: facts.modified.nanoseconds
	};
}
