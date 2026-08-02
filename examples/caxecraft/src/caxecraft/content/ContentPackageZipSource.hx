package caxecraft.content;

import caxecraft.content.ContentPackageModel.ContentPackageProvenance;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackageModel.LoadedPackageBytes;
import caxecraft.content.ContentPackagePath.ContentPackagePathError;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import haxe.io.Bytes;

/**
 * Reads one deliberately bounded ZIP profile as a logical package source.
 *
 * ZIP is only the hand-off container. This module validates the complete
 * archive structure once, owns an immutable-by-API copy, and then serves exact
 * package-relative bytes through `ContentPackageSource`. Semantic manifests,
 * schemas, dependencies, and gameplay therefore use the same path whether the
 * bytes came from an authoring directory or a shared ZIP.
 *
 * The first profile accepts single-disk, non-ZIP64 archives containing stored
 * regular files. Unsupported compression and optional record features fail
 * closed. That narrow contract is a useful, standard ZIP transport without
 * implying general `haxe.zip` support.
 */
/** Exact reason archive bytes could not become a trusted package source. */
enum ContentPackageZipError {
	InvalidArchiveByteLimit;
	InvalidSourceLabel;
	ArchiveTooLarge(maximumBytes:Int);
	ArchiveTooSmall;
	EndRecordMissing;
	ArchiveCommentUnsupported;
	ArchiveLayoutMismatch;
	MultiDiskUnsupported;
	Zip64Unsupported;
	EmptyArchive;
	EntryCountExceeded(maximumEntries:Int);
	CentralDirectoryOutOfBounds;
	CentralHeaderMalformed(index:Int);
	UnsupportedCreatorSystem(index:Int, system:Int);
	UnsupportedVersion(index:Int, version:Int);
	UnsupportedFlags(index:Int, flags:Int);
	UnsupportedCompression(index:Int, method:Int);
	ExtraFieldUnsupported(index:Int);
	EntryCommentUnsupported(index:Int);
	DirectoryEntryUnsupported(index:Int);
	SpecialEntryUnsupported(index:Int);
	InvalidUtf8EntryName(index:Int);
	Utf8FlagRequired(index:Int);
	InvalidEntryPath(index:Int, error:ContentPackagePathError);
	DuplicateEntryPath(path:String);
	CaseCollidingEntryPath(first:String, second:String);
	EntryTooLarge(index:Int, maximumBytes:Int);
	PayloadBudgetExceeded(maximumBytes:Int);
	LocalHeaderMalformed(index:Int);
	LocalHeaderMismatch(index:Int);
	EntryCrcMismatch(path:String);
}

/** Opening archive bytes either creates one source or returns one closed error. */
enum ContentPackageZipOpenResult {
	PackageZipOpened(source:ContentPackageZipSource);
	PackageZipRejected(error:ContentPackageZipError);
}

/** A complete validated index or its first deterministic archive error. */
private enum ContentPackageZipIndexResult {
	ZipIndexReady(entries:Array<ContentPackageZipEntry>);
	ZipIndexRejected(error:ContentPackageZipError);
}

/**
 * One immutable-by-API slice of the owned archive.
 *
 * A class is used instead of an open record so construction stays inside the
 * validator and callers cannot replace offsets after structural checks pass.
 */
private final class ContentPackageZipEntry {
	/** Validated portable name used for source lookup and provenance. */
	public final logicalPath:ContentPackagePath;

	/** First payload byte in the privately owned archive. */
	public final dataOffset:Int;

	/** Exact stored payload length. */
	public final byteLength:Int;

	/** Independently stored ZIP CRC-32 bit pattern. */
	public final crc32:Int;

	public function new(logicalPath:ContentPackagePath, dataOffset:Int, byteLength:Int, crc32:Int) {
		this.logicalPath = logicalPath;
		this.dataOffset = dataOffset;
		this.byteLength = byteLength;
		this.crc32 = crc32;
	}
}

/**
 * A read-only logical source backed by one completely validated ZIP archive.
 *
 * `open` copies the caller bytes before parsing, preventing later mutation from
 * invalidating offsets or CRC evidence. Reads return another independent byte
 * owner rather than exposing the archive storage itself.
 */
final class ContentPackageZipSource implements ContentPackageSource {
	/** Largest archive admitted by this first in-memory transport. */
	public static inline final MAXIMUM_ARCHIVE_BYTES:Int = 16 * 1024 * 1024;

	/** Largest number of central-directory records admitted in one package. */
	public static inline final MAXIMUM_ARCHIVE_ENTRIES:Int = 512;

	/**
	 * Largest sum of stored payload bytes in the first in-memory profile.
	 *
	 * Keeping one MiB below the archive ceiling leaves bounded room for names
	 * and record metadata. A future streaming profile can admit larger packages
	 * under its own separately tested memory contract.
	 */
	public static inline final MAXIMUM_PAYLOAD_BYTES:Int = 15 * 1024 * 1024;

	static inline final LOCAL_HEADER_SIGNATURE:Int = 0x04034b50;
	static inline final CENTRAL_HEADER_SIGNATURE:Int = 0x02014b50;
	static inline final END_RECORD_SIGNATURE:Int = 0x06054b50;
	static inline final UTF8_FLAG:Int = 0x0800;

	final archive:Bytes;
	final sourceLabel:String;
	final entries:Array<ContentPackageZipEntry>;

	private function new(archive:Bytes, sourceLabel:String, entries:Array<ContentPackageZipEntry>) {
		this.archive = archive;
		this.sourceLabel = sourceLabel;
		this.entries = entries;
	}

	/**
	 * Validate and take an independent copy of one complete ZIP byte vector.
	 *
	 * No source object escapes until every central record, local header, payload
	 * range, portable name, and CRC agrees. `maximumArchiveBytes` lets a caller
	 * choose a tighter policy but cannot exceed the product-wide 16 MiB ceiling.
	 */
	public static function open(input:Bytes, sourceLabel:String, maximumArchiveBytes:Int):ContentPackageZipOpenResult {
		if (maximumArchiveBytes <= 0 || maximumArchiveBytes > MAXIMUM_ARCHIVE_BYTES)
			return PackageZipRejected(InvalidArchiveByteLimit);
		if (sourceLabel.length == 0 || sourceLabel.indexOf("\x00") >= 0)
			return PackageZipRejected(InvalidSourceLabel);
		if (input.length > maximumArchiveBytes)
			return PackageZipRejected(ArchiveTooLarge(maximumArchiveBytes));
		if (input.length < 22)
			return PackageZipRejected(ArchiveTooSmall);

		final owned = input.sub(0, input.length);
		return switch buildIndex(owned) {
			case ZipIndexRejected(error): PackageZipRejected(error);
			case ZipIndexReady(entries): PackageZipOpened(new ContentPackageZipSource(owned, sourceLabel, entries));
		};
	}

	/** Return an independent copy of one validated stored entry. */
	public function read(logicalPath:String):ContentPackageReadResult {
		final requested = switch ContentPackagePath.parse(logicalPath) {
			case PathRejected(error): return PackageBytesRejected(InvalidLogicalPath(error));
			case PathAccepted(path): path;
		};
		for (entry in entries) {
			if (entry.logicalPath.text() == requested.text()) {
				final bytes = archive.sub(entry.dataOffset, entry.byteLength);
				return PackageBytesRead(new LoadedPackageBytes(bytes, new ContentPackageProvenance(sourceLabel, entry.logicalPath, entry.byteLength, 1)));
			}
		}
		return PackageBytesRejected(EntryMissing);
	}

	/** Build an exact local/central index without exposing partial validation. */
	static function buildIndex(bytes:Bytes):ContentPackageZipIndexResult {
		final endOffset = findEndRecord(bytes);
		if (endOffset < 0)
			return ZipIndexRejected(EndRecordMissing);
		if (!fits(endOffset, 22, bytes.length))
			return ZipIndexRejected(EndRecordMissing);

		final commentLength = readU16(bytes, endOffset + 20);
		if (endOffset + 22 + commentLength != bytes.length)
			return ZipIndexRejected(ArchiveLayoutMismatch);
		if (commentLength != 0)
			return ZipIndexRejected(ArchiveCommentUnsupported);

		final disk = readU16(bytes, endOffset + 4);
		final centralDisk = readU16(bytes, endOffset + 6);
		final entriesOnDisk = readU16(bytes, endOffset + 8);
		final entryCount = readU16(bytes, endOffset + 10);
		final centralSize = readU32(bytes, endOffset + 12);
		final centralOffset = readU32(bytes, endOffset + 16);
		if (entriesOnDisk == 0xffff || entryCount == 0xffff || centralSize < 0 || centralOffset < 0)
			return ZipIndexRejected(Zip64Unsupported);
		if (disk != 0 || centralDisk != 0 || entriesOnDisk != entryCount)
			return ZipIndexRejected(MultiDiskUnsupported);
		if (entryCount == 0)
			return ZipIndexRejected(EmptyArchive);
		if (entryCount > MAXIMUM_ARCHIVE_ENTRIES)
			return ZipIndexRejected(EntryCountExceeded(MAXIMUM_ARCHIVE_ENTRIES));
		if (!fits(centralOffset, centralSize, endOffset) || centralOffset + centralSize != endOffset)
			return ZipIndexRejected(CentralDirectoryOutOfBounds);

		final entries = new Array<ContentPackageZipEntry>();
		var centralCursor = centralOffset;
		var expectedLocalOffset = 0;
		var payloadBytes = 0;
		for (index in 0...entryCount) {
			if (!fits(centralCursor, 46, endOffset) || readU32(bytes, centralCursor) != CENTRAL_HEADER_SIGNATURE)
				return ZipIndexRejected(CentralHeaderMalformed(index));

			final versionMadeBy = readU16(bytes, centralCursor + 4);
			final versionNeeded = readU16(bytes, centralCursor + 6);
			final flags = readU16(bytes, centralCursor + 8);
			final method = readU16(bytes, centralCursor + 10);
			final modifiedTime = readU16(bytes, centralCursor + 12);
			final modifiedDate = readU16(bytes, centralCursor + 14);
			final crc32 = readU32(bytes, centralCursor + 16);
			final compressedSize = readU32(bytes, centralCursor + 20);
			final uncompressedSize = readU32(bytes, centralCursor + 24);
			final nameLength = readU16(bytes, centralCursor + 28);
			final extraLength = readU16(bytes, centralCursor + 30);
			final entryCommentLength = readU16(bytes, centralCursor + 32);
			final entryDisk = readU16(bytes, centralCursor + 34);
			final externalAttributes = readU32(bytes, centralCursor + 38);
			final localOffset = readU32(bytes, centralCursor + 42);
			final variableLength = nameLength + extraLength + entryCommentLength;
			if (!fits(centralCursor + 46, variableLength, endOffset))
				return ZipIndexRejected(CentralHeaderMalformed(index));
			if (compressedSize < 0 || uncompressedSize < 0 || localOffset < 0 || entryDisk == 0xffff)
				return ZipIndexRejected(Zip64Unsupported);
			if (entryDisk != 0)
				return ZipIndexRejected(MultiDiskUnsupported);
			final creatorSystem = versionMadeBy >>> 8;
			if (creatorSystem != 0 && creatorSystem != 3)
				return ZipIndexRejected(UnsupportedCreatorSystem(index, creatorSystem));
			if (versionNeeded < 10 || versionNeeded > 20)
				return ZipIndexRejected(UnsupportedVersion(index, versionNeeded));
			if (flags != 0 && flags != UTF8_FLAG)
				return ZipIndexRejected(UnsupportedFlags(index, flags));
			if (method != 0)
				return ZipIndexRejected(UnsupportedCompression(index, method));
			if (extraLength != 0)
				return ZipIndexRejected(ExtraFieldUnsupported(index));
			if (entryCommentLength != 0)
				return ZipIndexRejected(EntryCommentUnsupported(index));
			if (compressedSize != uncompressedSize)
				return ZipIndexRejected(LocalHeaderMismatch(index));
			if (uncompressedSize > MAXIMUM_ARCHIVE_BYTES)
				return ZipIndexRejected(EntryTooLarge(index, MAXIMUM_ARCHIVE_BYTES));
			if (payloadBytes > MAXIMUM_PAYLOAD_BYTES - uncompressedSize)
				return ZipIndexRejected(PayloadBudgetExceeded(MAXIMUM_PAYLOAD_BYTES));

			final nameOffset = centralCursor + 46;
			if (nameLength == 0 || !validUtf8(bytes, nameOffset, nameLength))
				return ZipIndexRejected(InvalidUtf8EntryName(index));
			if (flags == 0 && containsNonAscii(bytes, nameOffset, nameLength))
				return ZipIndexRejected(Utf8FlagRequired(index));
			final name = bytes.getString(nameOffset, nameLength);
			if (endsWithSlash(name))
				return ZipIndexRejected(DirectoryEntryUnsupported(index));
			final logicalPath = switch ContentPackagePath.parse(name) {
				case PathRejected(error): return ZipIndexRejected(InvalidEntryPath(index, error));
				case PathAccepted(path): path;
			};
			final kindError = specialEntryError(index, creatorSystem, externalAttributes);
			if (kindError != null)
				return ZipIndexRejected(kindError);
			for (previous in entries) {
				if (previous.logicalPath.text() == name)
					return ZipIndexRejected(DuplicateEntryPath(name));
				if (previous.logicalPath.asciiCaseEquals(logicalPath))
					return ZipIndexRejected(CaseCollidingEntryPath(previous.logicalPath.text(), name));
			}
			if (localOffset != expectedLocalOffset)
				return ZipIndexRejected(ArchiveLayoutMismatch);

			final localResult = validateLocal(bytes, index, centralOffset, localOffset, versionNeeded, flags, method, modifiedTime, modifiedDate, crc32,
				compressedSize, nameOffset, nameLength);
			final entry = switch localResult {
				case ZipLocalRejected(error): return ZipIndexRejected(error);
				case ZipLocalReady(value): value;
			};
			entries.push(new ContentPackageZipEntry(logicalPath, entry.dataOffset, uncompressedSize, crc32));
			expectedLocalOffset = entry.dataOffset + uncompressedSize;
			payloadBytes += uncompressedSize;
			centralCursor += 46 + variableLength;
		}
		if (centralCursor != endOffset || expectedLocalOffset != centralOffset)
			return ZipIndexRejected(ArchiveLayoutMismatch);
		return ZipIndexReady(entries);
	}

	/** Validate one local header against its authoritative central record. */
	static function validateLocal(bytes:Bytes, index:Int, centralOffset:Int, localOffset:Int, versionNeeded:Int, flags:Int, method:Int, modifiedTime:Int,
			modifiedDate:Int, crc32:Int, size:Int, centralNameOffset:Int, nameLength:Int):ContentPackageZipLocalResult {
		if (!fits(localOffset, 30, centralOffset) || readU32(bytes, localOffset) != LOCAL_HEADER_SIGNATURE)
			return ZipLocalRejected(LocalHeaderMalformed(index));
		final localNameLength = readU16(bytes, localOffset + 26);
		final localExtraLength = readU16(bytes, localOffset + 28);
		if (localExtraLength != 0)
			return ZipLocalRejected(ExtraFieldUnsupported(index));
		if (readU16(bytes, localOffset + 4) != versionNeeded
			|| readU16(bytes, localOffset + 6) != flags
			|| readU16(bytes, localOffset + 8) != method
			|| readU16(bytes, localOffset + 10) != modifiedTime
			|| readU16(bytes, localOffset + 12) != modifiedDate
			|| readU32(bytes, localOffset + 14) != crc32
			|| readU32(bytes, localOffset + 18) != size
			|| readU32(bytes, localOffset + 22) != size
			|| localNameLength != nameLength)
			return ZipLocalRejected(LocalHeaderMismatch(index));
		final localNameOffset = localOffset + 30;
		if (!fits(localNameOffset, localNameLength, centralOffset) || !sameBytes(bytes, localNameOffset, centralNameOffset, nameLength))
			return ZipLocalRejected(LocalHeaderMismatch(index));
		final dataOffset = localNameOffset + localNameLength;
		if (!fits(dataOffset, size, centralOffset))
			return ZipLocalRejected(LocalHeaderMalformed(index));
		if (crc32Of(bytes, dataOffset, size) != crc32)
			return ZipLocalRejected(EntryCrcMismatch(bytes.getString(centralNameOffset, nameLength)));
		return ZipLocalReady(new ContentPackageZipLocalEntry(dataOffset));
	}

	/** Find the final ordinary end record within the standard comment window. */
	static function findEndRecord(bytes:Bytes):Int {
		var cursor = bytes.length - 22;
		final minimum = bytes.length > 65557 ? bytes.length - 65557 : 0;
		while (cursor >= minimum) {
			if (readU32(bytes, cursor) == END_RECORD_SIGNATURE)
				return cursor;
			cursor--;
		}
		return -1;
	}

	/** Return whether a byte range lies wholly before one exclusive limit. */
	static inline function fits(offset:Int, length:Int, limit:Int):Bool
		return offset >= 0 && length >= 0 && offset <= limit && length <= limit - offset;

	/** Read an unsigned little-endian 16-bit value into a non-negative Int. */
	static inline function readU16(bytes:Bytes, offset:Int):Int
		return bytes.get(offset) | bytes.get(offset + 1) << 8;

	/** Read one little-endian 32-bit bit pattern; negative means its high bit is set. */
	static inline function readU32(bytes:Bytes, offset:Int):Int
		return bytes.get(offset) | bytes.get(offset + 1) << 8 | bytes.get(offset + 2) << 16 | bytes.get(offset + 3) << 24;

	/** Compare two equal-length byte regions without allocating slices. */
	static function sameBytes(bytes:Bytes, left:Int, right:Int, length:Int):Bool {
		for (index in 0...length)
			if (bytes.get(left + index) != bytes.get(right + index))
				return false;
		return true;
	}

	/** Return true when a validated name carries at least one UTF-8 multibyte unit. */
	static function containsNonAscii(bytes:Bytes, offset:Int, length:Int):Bool {
		for (index in 0...length)
			if (bytes.get(offset + index) >= 128)
				return true;
		return false;
	}

	/** Validate canonical UTF-8, rejecting overlong, surrogate, and out-of-range forms. */
	static function validUtf8(bytes:Bytes, offset:Int, length:Int):Bool {
		var cursor = offset;
		final end = offset + length;
		while (cursor < end) {
			final first = bytes.get(cursor);
			if (first < 128) {
				cursor++;
				continue;
			}
			if (first >= 0xc2 && first <= 0xdf) {
				if (!continuation(bytes, cursor + 1, end))
					return false;
				cursor += 2;
				continue;
			}
			if (first >= 0xe0 && first <= 0xef) {
				if (!continuation(bytes, cursor + 1, end) || !continuation(bytes, cursor + 2, end))
					return false;
				final second = bytes.get(cursor + 1);
				if ((first == 0xe0 && second < 0xa0) || (first == 0xed && second >= 0xa0))
					return false;
				cursor += 3;
				continue;
			}
			if (first >= 0xf0 && first <= 0xf4) {
				if (!continuation(bytes, cursor + 1, end)
					|| !continuation(bytes, cursor + 2, end)
					|| !continuation(bytes, cursor + 3, end))
					return false;
				final second = bytes.get(cursor + 1);
				if ((first == 0xf0 && second < 0x90) || (first == 0xf4 && second >= 0x90))
					return false;
				cursor += 4;
				continue;
			}
			return false;
		}
		return true;
	}

	/** Check one UTF-8 continuation byte without reading beyond the name. */
	static inline function continuation(bytes:Bytes, offset:Int, end:Int):Bool
		return offset < end && bytes.get(offset) >= 0x80 && bytes.get(offset) <= 0xbf;

	/** Reject directory, link, device, and volume-label metadata. */
	static function specialEntryError(index:Int, creatorSystem:Int, externalAttributes:Int):Null<ContentPackageZipError> {
		if (creatorSystem == 3) {
			final kind = externalAttributes >>> 16 & 0xf000;
			if (kind == 0x4000)
				return DirectoryEntryUnsupported(index);
			if (kind != 0x8000)
				return SpecialEntryUnsupported(index);
			return null;
		}
		final dosAttributes = externalAttributes & 0xff;
		if ((dosAttributes & 0x10) != 0)
			return DirectoryEntryUnsupported(index);
		if ((dosAttributes & 0x08) != 0)
			return SpecialEntryUnsupported(index);
		return null;
	}

	/** Recognize a ZIP directory spelling before portable path parsing can hide it. */
	static inline function endsWithSlash(value:String):Bool
		return value.length > 0 && value.charCodeAt(value.length - 1) == 47;

	/** Compute the PKWARE CRC-32 with all-ones pre/post conditioning. */
	static function crc32Of(bytes:Bytes, offset:Int, length:Int):Int {
		var crc = -1;
		for (index in 0...length) {
			crc ^= bytes.get(offset + index);
			for (_ in 0...8)
				crc = crc >>> 1 ^ ((crc & 1) == 0 ? 0 : 0xedb88320);
		}
		return ~crc;
	}
}

/** One validated local payload start, kept private until central agreement. */
private final class ContentPackageZipLocalEntry {
	/** First payload byte after the exact local header and name. */
	public final dataOffset:Int;

	public function new(dataOffset:Int) {
		this.dataOffset = dataOffset;
	}
}

/** Local-header validation either yields a payload start or one closed error. */
private enum ContentPackageZipLocalResult {
	ZipLocalReady(entry:ContentPackageZipLocalEntry);
	ZipLocalRejected(error:ContentPackageZipError);
}
