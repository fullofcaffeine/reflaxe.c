package caxecraft.qa;

import caxecraft.content.ContentPackageManifest.ContentPackageLoadResult;
import caxecraft.content.ContentPackageManifest.LoadedContentPackage;
import caxecraft.content.ContentPackageManifest.loadContentPackage;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.ContentPackageZipSource;
import caxecraft.content.ContentPackageZipSource.ContentPackageZipError;
import caxecraft.content.ContentPackageZipSource.ContentPackageZipOpenResult;
import haxe.io.Bytes;

/**
 * Proves that one frozen standard-tool ZIP remains readable and fail-closed.
 *
 * The deliberately small archive fixture comes from an independent standard
 * ZIP tool. Keeping it frozen prevents an exporter bug from changing the
 * reader's oracle. This probe owns stored-ZIP compatibility and adversarial
 * archive structure. The current package's full directory-to-ZIP parity is
 * owned separately by `ContentPackageZipExportProbe`.
 */
/** First broken ZIP-source invariant, or zero after every scenario passes. */
var observed:Int = 0;

/** Compact identity of the reviewed package frozen inside the ZIP fixture. */
var traceIdentity:Int = 0;

/** Exact verified semantic payload bytes reached through the ZIP source. */
var traceBytes:Int = 0;

/** Number of independently targeted archive rejections that matched. */
var traceRejections:Int = 0;

/** Run the shared proof on Eval or expose its scalar result to native C. */
function main():Void {
	observed = selfCheck();
	#if !c
	Sys.println(observed);
	Sys.println(traceIdentity);
	Sys.println(traceBytes);
	Sys.println(traceRejections);
	#end
}

/** Return zero only when the frozen package and every ZIP rejection agree. */
function selfCheck():Int {
	final fixtureStore = switch ContentPackageStore.open(".", "package-zip-fixtures", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	final archiveRead = switch fixtureStore.read("test/fixtures/package-zip/first-adventure-store.zip") {
		case PackageBytesRead(value): value;
		case PackageBytesRejected(_): return 2;
	};
	if (archiveRead.bytes.length != 45715)
		return 3;
	final pristine = archiveRead.bytes.sub(0, archiveRead.bytes.length);
	final zip = switch ContentPackageZipSource.open(archiveRead.bytes, "first-adventure-zip", ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES) {
		case PackageZipOpened(value): value;
		case PackageZipRejected(_): return 4;
	};

	// The source owns a copy: changing the caller buffer cannot change later reads.
	archiveRead.bytes.set(52, archiveRead.bytes.get(52) ^ 0xff);
	final zipPackage = ready(loadContentPackage(zip, "caxecraft.package.json"));
	if (zipPackage == null || zipPackage.manifest.id.text() != "caxecraft:first-adventure")
		return 5;
	if (!readRejections(zip))
		return 6;
	if (!archiveRejections(pristine, fixtureStore))
		return 7;

	traceIdentity = zipPackage.manifest.version * 100000 + zipPackage.manifest.roleCount() * 1000 + zipPackage.manifest.entryCount() * 10
		+ zipPackage.manifest.dependencyCount();
	for (index in 0...zipPackage.receiptCount())
		traceBytes += zipPackage.receiptAt(index).byteLength;
	return traceIdentity == 105070 && traceBytes == 42943 && traceRejections == 36 ? 0 : 8;
}

/** Preserve null as a failed positive package load. */
function ready(result:ContentPackageLoadResult):Null<LoadedContentPackage>
	return switch result {
		case ContentPackageReady(value): value;
		case ContentPackageRejected(_): null;
	};

/** Prove invalid and absent lookups remain ordinary source errors. */
function readRejections(source:ContentPackageZipSource):Bool {
	switch source.read("../escape") {
		case PackageBytesRejected(InvalidLogicalPath(ParentComponent(0))):
		case _:
			return false;
	}
	switch source.read("missing.json") {
		case PackageBytesRejected(EntryMissing):
		case _:
			return false;
	}
	return true;
}

/** Challenge each independently located archive field and bounded policy. */
function archiveRejections(pristine:Bytes, store:ContentPackageStore):Bool {
	if (!rejects(pristine, InvalidArchiveByteLimit, 1, 0)
		|| !rejects(pristine, InvalidSourceLabel, 2, -1)
		|| !rejects(pristine, ArchiveTooLarge(100), 3, 100)
		|| !rejects(Bytes.alloc(3), ArchiveTooSmall, 4, -1)
		|| !rejects(mutate(pristine, 5), EndRecordMissing, 5, -1)
		|| !rejects(mutate(pristine, 6), ArchiveCommentUnsupported, 6, -1)
		|| !rejects(mutate(pristine, 7), ArchiveLayoutMismatch, 7, -1)
		|| !rejects(mutate(pristine, 8), CentralDirectoryOutOfBounds, 8, -1)
		|| !rejects(mutate(pristine, 9), MultiDiskUnsupported, 9, -1)
		|| !rejects(mutate(pristine, 10), Zip64Unsupported, 10, -1)
		|| !rejects(mutate(pristine, 11), EntryCountExceeded(ContentPackageZipSource.MAXIMUM_ARCHIVE_ENTRIES), 11, -1)
		|| !rejects(mutate(pristine, 12), CentralHeaderMalformed(0), 12, -1)
		|| !rejects(mutate(pristine, 13), UnsupportedCreatorSystem(0, 19), 13, -1)
		|| !rejects(mutate(pristine, 14), UnsupportedVersion(0, 63), 14, -1)
		|| !rejects(mutate(pristine, 15), UnsupportedFlags(0, 1), 15, -1)
		|| !rejects(mutate(pristine, 16), UnsupportedFlags(0, 8), 16, -1)
		|| !rejects(mutate(pristine, 17), UnsupportedCompression(0, 8), 17, -1)
		|| !rejects(mutate(pristine, 18), ExtraFieldUnsupported(0), 18, -1)
		|| !rejects(mutate(pristine, 19), EntryCommentUnsupported(0), 19, -1)
		|| !rejects(mutate(pristine, 20), DirectoryEntryUnsupported(0), 20, -1)
		|| !rejects(mutate(pristine, 21), SpecialEntryUnsupported(0), 21, -1)
		|| !rejects(mutate(pristine, 22), InvalidUtf8EntryName(0), 22, -1)
		|| !rejects(mutate(pristine, 23), Utf8FlagRequired(0), 23, -1)
		|| !rejects(mutate(pristine, 24), InvalidEntryPath(0, AbsolutePath), 24, -1)
		|| !rejects(mutate(pristine, 25), PayloadBudgetExceeded(ContentPackageZipSource.MAXIMUM_PAYLOAD_BYTES), 25, -1)
		|| !rejects(mutate(pristine, 26), LocalHeaderMalformed(0), 26, -1)
		|| !rejects(mutate(pristine, 27), LocalHeaderMismatch(0), 27, -1)
		|| !rejects(mutate(pristine, 28), EntryCrcMismatch("caxecraft.package.json"), 28, -1)
		|| !rejects(mutate(pristine, 29), ArchiveLayoutMismatch, 29, -1)
		|| !rejects(mutate(pristine, 32), DirectoryEntryUnsupported(0), 32, -1)
		|| !rejects(truncateLastByte(pristine), EndRecordMissing, 33, -1)
		|| !rejects(mutate(pristine, 34), LocalHeaderMismatch(0), 34, -1)
		|| !rejects(mutate(pristine, 35), EntryTooLarge(0, ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES), 35, -1)
		|| !rejects(mutate(pristine, 36), EmptyArchive, 36, -1))
		return false;

	final equalArchive = switch store.read("test/fixtures/package-zip/equal-names-store.zip") {
		case PackageBytesRead(value): value.bytes;
		case PackageBytesRejected(_): return false;
	};
	if (!rejects(renameSecond(equalArchive, "alpha.txt"), DuplicateEntryPath("alpha.txt"), 30, -1)
		|| !rejects(renameSecond(equalArchive, "Alpha.txt"), CaseCollidingEntryPath("alpha.txt", "Alpha.txt"), 31, -1))
		return false;
	return true;
}

/** Open one candidate and count an exact expected transport rejection. */
function rejects(bytes:Bytes, expected:ContentPackageZipError, scenario:Int, maximum:Int):Bool {
	final limit = maximum < 0 ? ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES : maximum;
	final label = scenario == 2 ? "" : "adversarial-zip";
	final matched = switch ContentPackageZipSource.open(bytes, label, limit) {
		case PackageZipOpened(_): false;
		case PackageZipRejected(actual): sameError(actual, expected);
	};
	if (matched)
		traceRejections++;
	return matched;
}

/** Match closed errors without turning diagnostic payloads into strings. */
function sameError(actual:ContentPackageZipError, expected:ContentPackageZipError):Bool
	return switch [actual, expected] {
		case [InvalidArchiveByteLimit, InvalidArchiveByteLimit] | [InvalidSourceLabel, InvalidSourceLabel] | [ArchiveTooLarge(100), ArchiveTooLarge(100)] |
			[ArchiveTooSmall, ArchiveTooSmall] | [EndRecordMissing, EndRecordMissing] | [ArchiveCommentUnsupported, ArchiveCommentUnsupported] |
			[ArchiveLayoutMismatch, ArchiveLayoutMismatch] | [CentralDirectoryOutOfBounds, CentralDirectoryOutOfBounds] |
			[MultiDiskUnsupported, MultiDiskUnsupported] | [Zip64Unsupported, Zip64Unsupported] | [EmptyArchive, EmptyArchive] |
			[EntryCountExceeded(512), EntryCountExceeded(512)] | [CentralHeaderMalformed(0), CentralHeaderMalformed(0)] |
			[UnsupportedCreatorSystem(0, 19), UnsupportedCreatorSystem(0, 19)] | [UnsupportedVersion(0, 63), UnsupportedVersion(0, 63)] |
			[UnsupportedFlags(0, 1), UnsupportedFlags(0, 1)] | [UnsupportedFlags(0, 8), UnsupportedFlags(0, 8)] |
			[UnsupportedCompression(0, 8), UnsupportedCompression(0, 8)] | [ExtraFieldUnsupported(0), ExtraFieldUnsupported(0)] |
			[EntryCommentUnsupported(0), EntryCommentUnsupported(0)] | [DirectoryEntryUnsupported(0), DirectoryEntryUnsupported(0)] |
			[SpecialEntryUnsupported(0), SpecialEntryUnsupported(0)] | [InvalidUtf8EntryName(0), InvalidUtf8EntryName(0)] |
			[Utf8FlagRequired(0), Utf8FlagRequired(0)] | [InvalidEntryPath(0, AbsolutePath), InvalidEntryPath(0, AbsolutePath)] |
			[EntryTooLarge(0, 16777216), EntryTooLarge(0, 16777216)] | [PayloadBudgetExceeded(15728640), PayloadBudgetExceeded(15728640)] |
			[LocalHeaderMalformed(0), LocalHeaderMalformed(0)] | [LocalHeaderMismatch(0), LocalHeaderMismatch(0)] | [
				EntryCrcMismatch("caxecraft.package.json"),
				EntryCrcMismatch("caxecraft.package.json")
			] | [DuplicateEntryPath("alpha.txt"), DuplicateEntryPath("alpha.txt")] | [
				CaseCollidingEntryPath("alpha.txt", "Alpha.txt"),
				CaseCollidingEntryPath("alpha.txt", "Alpha.txt")
			]: true;
		case _: false;
	};

/** Apply one mutation at independently inspected APPNOTE field offsets. */
function mutate(source:Bytes, scenario:Int):Bytes {
	if (scenario == 6) {
		final result = appendByte(source, 0);
		writeU16(result, 45713, 1);
		return result;
	}
	if (scenario == 7)
		return appendByte(source, 0);
	if (scenario == 8)
		return prefixByte(source, 0);
	final result = source.sub(0, source.length);
	switch scenario {
		case 5:
			result.set(45693, 0);
		case 9:
			writeU16(result, 45697, 1);
		case 10:
			writeU16(result, 45703, 0xffff);
		case 11:
			writeU16(result, 45701, 513);
			writeU16(result, 45703, 513);
		case 12:
			result.set(45072, 0);
		case 13:
			result.set(45077, 19);
		case 14:
			writeU16(result, 45078, 63);
		case 15:
			writeU16(result, 45080, 1);
		case 16:
			writeU16(result, 45080, 8);
		case 17:
			writeU16(result, 45082, 8);
		case 18:
			writeU16(result, 45102, 1);
		case 19:
			writeU16(result, 45104, 1);
		case 20:
			result.set(45112, 0xed);
			result.set(45113, 0x41);
		case 21:
			result.set(45112, 0xff);
			result.set(45113, 0xa1);
		case 22:
			result.set(45118, 0xc0);
		case 23:
			result.set(45118, 0xc2);
			result.set(45119, 0xa2);
		case 24:
			result.set(45118, 47);
		case 25:
			writeU32(result, 45092, ContentPackageZipSource.MAXIMUM_PAYLOAD_BYTES + 1);
			writeU32(result, 45096, ContentPackageZipSource.MAXIMUM_PAYLOAD_BYTES + 1);
		case 26:
			result.set(0, 0);
		case 27:
			writeU16(result, 6, 8);
		case 28:
			result.set(52, result.get(52) ^ 0xff);
		case 29:
			writeU32(result, 45114, 1);
		case 32:
			result.set(51, 47);
			result.set(45139, 47);
		case 34:
			writeU32(result, 45092, 1637);
		case 35:
			writeU32(result, 45092, ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES + 1);
			writeU32(result, 45096, ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES + 1);
		case 36:
			writeU16(result, 45701, 0);
			writeU16(result, 45703, 0);
		case _:
	}
	return result;
}

/** Rename the second equal-length fixture entry in both directory records. */
function renameSecond(source:Bytes, name:String):Bytes {
	final result = source.sub(0, source.length);
	writeAscii(result, 83, name);
	writeAscii(result, 207, name);
	return result;
}

/** Append one byte while preserving all original archive offsets. */
function appendByte(source:Bytes, value:Int):Bytes {
	final result = Bytes.alloc(source.length + 1);
	for (index in 0...source.length)
		result.set(index, source.get(index));
	result.set(source.length, value);
	return result;
}

/** Remove the final end-record byte to exercise true archive truncation. */
function truncateLastByte(source:Bytes):Bytes
	return source.sub(0, source.length - 1);

/** Prefix one byte so every recorded absolute offset becomes stale. */
function prefixByte(source:Bytes, value:Int):Bytes {
	final result = Bytes.alloc(source.length + 1);
	result.set(0, value);
	for (index in 0...source.length)
		result.set(index + 1, source.get(index));
	return result;
}

/** Write one same-length ASCII fixture name. */
function writeAscii(bytes:Bytes, offset:Int, value:String):Void
	for (index in 0...value.length) {
		final code = value.charCodeAt(index);
		if (code == null)
			return;
		final byte:Int = code;
		bytes.set(offset + index, byte);
	}

/** Write one little-endian 16-bit fixture field. */
function writeU16(bytes:Bytes, offset:Int, value:Int):Void {
	bytes.set(offset, value & 0xff);
	bytes.set(offset + 1, value >>> 8 & 0xff);
}

/** Write one little-endian 32-bit fixture field. */
function writeU32(bytes:Bytes, offset:Int, value:Int):Void {
	bytes.set(offset, value & 0xff);
	bytes.set(offset + 1, value >>> 8 & 0xff);
	bytes.set(offset + 2, value >>> 16 & 0xff);
	bytes.set(offset + 3, value >>> 24 & 0xff);
}
