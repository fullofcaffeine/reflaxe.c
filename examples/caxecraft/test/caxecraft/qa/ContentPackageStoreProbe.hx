package caxecraft.qa;

import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageOpenResult;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackagePath;
import caxecraft.content.ContentPackagePath.ContentPackagePathError;
import caxecraft.content.ContentPackageStore;
#if c
import caxecraft.content.hosted.PosixPackageApi.setTestFault as setPosixTestFault;
import caxecraft.content.hosted.PosixPackageApi.testLiveDescriptors as posixLiveDescriptors;
import caxecraft.content.hosted.PosixPackageApi.PosixPackageTestFault;
#end

/**
	Proves the typed package boundary on Eval and hosted generated C.

	The fixture reads one real binary file, rejects portable path escapes,
	symlinks, directories, missing files, and over-limit input, and records only
	target-neutral scalar evidence. The Haxe-authored native path also injects
	changed-read, root-change, read, short-read, and close failures. The small C
	harness observes exported results and ABI widths; it owns no package logic.
**/
var observed:Int = 0;

/** Exact successful payload length observed through the product API. */
var traceLength:Int = 0;

/** First byte of the exact successful payload. */
var traceFirstByte:Int = 0;

/** Last byte of the exact successful payload. */
var traceLastByte:Int = 0;

/** Number of complete read attempts used by the successful transaction. */
var traceAttempts:Int = 0;

/** Root used by both Eval and the native executable from the Caxecraft folder. */
inline final FIXTURE_ROOT = "test/fixtures/package-store/root";

/**
	Run the shared package checks and publish their scalar result.

	The native harness may inject one changed-file result before this entry point;
	the store must transparently succeed on its one permitted retry.
**/
function main():Void {
	#if c
	setPosixTestFault(ChangedOnce);
	var status = selfCheck();
	if (status == 0)
		status = nativeFailureCleanupCheck();
	observed = status;
	#else
	final status = selfCheck();
	Sys.println(status);
	Sys.println(traceLength);
	Sys.println(traceFirstByte);
	Sys.println(traceLastByte);
	#end
}

#if c
/**
	Prove Haxe closes every descriptor on injected native failure.

	The check uses the public package store for every operation. Test-only fault
	state lives beside the Haxe POSIX implementation, so handwritten C cannot
	accidentally become a second owner of cleanup behavior.
**/
function nativeFailureCleanupCheck():Int {
	setPosixTestFault(CloseOnce);
	switch ContentPackageStore.open(FIXTURE_ROOT, "qa-package", 8) {
		case PackageStoreRejected(CloseFailed):
		case _:
			return 12;
	}
	if (posixLiveDescriptors() != 0)
		return 13;

	final store = switch ContentPackageStore.open(FIXTURE_ROOT, "qa-package", 8) {
		case PackageStoreRejected(_):
			return 14;
		case PackageStoreOpened(value):
			value;
	}

	setPosixTestFault(RootChangedOnce);
	switch store.read("levels/valid.bin") {
		case PackageBytesRejected(RootChanged):
		case _:
			return 15;
	}
	if (posixLiveDescriptors() != 0)
		return 16;

	setPosixTestFault(ReadOnce);
	switch store.read("levels/valid.bin") {
		case PackageBytesRejected(ReadFailed):
		case _:
			return 17;
	}
	if (posixLiveDescriptors() != 0)
		return 18;

	setPosixTestFault(ShortReadOnce);
	switch store.read("levels/valid.bin") {
		case PackageBytesRejected(ReadFailed):
		case _:
			return 19;
	}
	return posixLiveDescriptors() == 0 ? 0 : 20;
}
#end

/** Return zero only when valid bytes and the bounded rejection matrix agree. */
function selfCheck():Int {
	switch ContentPackageStore.open("test/fixtures/package-store/missing", "missing", 8) {
		case PackageStoreRejected(RootUnavailable):
		case _:
			return 1;
	}
	switch ContentPackageStore.open(FIXTURE_ROOT + "/levels/valid.bin", "file", 8) {
		case PackageStoreRejected(RootNotDirectory):
		case _:
			return 2;
	}
	switch ContentPackageStore.open(FIXTURE_ROOT, "qa-package", 0) {
		case PackageStoreRejected(InvalidByteLimit):
		case _:
			return 3;
	}

	final store = switch ContentPackageStore.open(FIXTURE_ROOT, "qa-package", 8) {
		case PackageStoreRejected(_):
			return 4;
		case PackageStoreOpened(value):
			value;
	}

	if (!pathRejected(store, "", EmptyPath)
		|| !pathRejected(store, "/absolute", AbsolutePath)
		|| !pathRejected(store, "C:/drive", DriveOrDevicePath)
		|| !pathRejected(store, "\\\\server\\share", AlternateSeparator)
		|| !pathRejected(store, "levels\\valid.bin", AlternateSeparator)
		|| !pathRejected(store, "levels/\x00.bin", ContainsNul)
		|| !pathRejected(store, "levels//valid.bin", EmptyComponent(1))
		|| !pathRejected(store, "./valid.bin", DotComponent(0))
		|| !pathRejected(store, "../outside.bin", ParentComponent(0))
		|| !pathRejected(store, "NUL.txt", ReservedDeviceComponent(0))
		|| !pathRejected(store, "name.", TrailingDotOrSpace(0))
		|| !pathRejected(store, repeated("a", ContentPackagePath.MAXIMUM_COMPONENT_BYTES + 1), ComponentTooLong(0, ContentPackagePath.MAXIMUM_COMPONENT_BYTES))
		|| !pathRejected(store, componentPath(129, 1), TooManyComponents(ContentPackagePath.MAXIMUM_COMPONENTS))
		|| !pathRejected(store, componentPath(ContentPackagePath.MAXIMUM_COMPONENTS, 32), PathTooLong(ContentPackagePath.MAXIMUM_PATH_BYTES)))
		return 5;

	switch store.read("missing.bin") {
		case PackageBytesRejected(EntryMissing):
		case _:
			return 6;
	}
	switch store.read("escape.bin") {
		case PackageBytesRejected(EntryIsSymlink):
		case _:
			return 7;
	}
	switch store.read("levels") {
		case PackageBytesRejected(EntryNotFile):
		case _:
			return 8;
	}
	switch store.read("oversized.bin") {
		case PackageBytesRejected(EntryTooLarge(8)):
		case _:
			return 9;
	}

	return switch store.read("levels/valid.bin") {
		case PackageBytesRejected(_):
			10;
		case PackageBytesRead(content):
			if (content.bytes.length != 5
				|| content.bytes.get(0) != 67
				|| content.bytes.get(4) != 10
				|| content.provenance.rootLabel != "qa-package"
				|| content.provenance.logicalPath.text() != "levels/valid.bin"
				|| content.provenance.byteLength != content.bytes.length
				|| content.provenance.readAttempts < 1
				|| content.provenance.readAttempts > 2) 11; else {
				traceLength = content.bytes.length;
				traceFirstByte = content.bytes.get(0);
				traceLastByte = content.bytes.get(content.bytes.length - 1);
				traceAttempts = content.provenance.readAttempts;
				0;
			}
	}
}

/** Build one deterministic boundary spelling without depending on host APIs. */
function repeated(value:String, count:Int):String {
	var result = "";
	for (_ in 0...count)
		result += value;
	return result;
}

/**
	Build a portable multi-component path for count and byte-limit rejection.

	Every component is valid on its own. This isolates whether the complete path
	fails because it has too many components or too many UTF-8 bytes.
**/
function componentPath(componentCount:Int, componentLength:Int):String {
	final component = repeated("a", componentLength);
	var result = component;
	for (_ in 1...componentCount)
		result += "/" + component;
	return result;
}

/** Compare one invalid spelling without stringifying target-owned enums. */
function pathRejected(store:ContentPackageStore, spelling:String, expected:ContentPackagePathError):Bool {
	return switch store.read(spelling) {
		case PackageBytesRejected(InvalidLogicalPath(actual)):
			samePathError(actual, expected);
		case _:
			false;
	}
}

/** Keep the portable path-error comparison exhaustive and allocation-free. */
function samePathError(left:ContentPackagePathError, right:ContentPackagePathError):Bool {
	return switch [left, right] {
		case [EmptyPath, EmptyPath] | [AbsolutePath, AbsolutePath] | [DriveOrDevicePath, DriveOrDevicePath] | [AlternateSeparator, AlternateSeparator] |
			[ContainsNul, ContainsNul]:
			true;
		case [EmptyComponent(leftIndex), EmptyComponent(rightIndex)] | [DotComponent(leftIndex), DotComponent(rightIndex)] |
			[ParentComponent(leftIndex), ParentComponent(rightIndex)] | [ReservedDeviceComponent(leftIndex), ReservedDeviceComponent(rightIndex)] |
			[TrailingDotOrSpace(leftIndex), TrailingDotOrSpace(rightIndex)]:
			leftIndex == rightIndex;
		case [PathTooLong(leftMaximum), PathTooLong(rightMaximum)] | [TooManyComponents(leftMaximum), TooManyComponents(rightMaximum)]:
			leftMaximum == rightMaximum;
		case [
			ComponentTooLong(leftIndex, leftMaximum),
			ComponentTooLong(rightIndex, rightMaximum)
		]: leftIndex == rightIndex && leftMaximum == rightMaximum;
		case _:
			false;
	}
}
