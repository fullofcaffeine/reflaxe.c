package caxecraft.content;

import caxecraft.content.ContentPackageAssetClosure.ContentPackageAssetClosureError;
import caxecraft.content.ContentPackageAssetClosure.verifyContentPackageAssetClosure;
import caxecraft.content.ContentPackageManifest.ContentPackageEntry;
import caxecraft.content.ContentPackageManifest.ContentPackageEntryKind;
import caxecraft.content.ContentPackageManifest.ContentPackageManifest;
import caxecraft.content.ContentPackageManifest.ContentPackageManifestReadResult;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackageModel.ContentPackageReadResult;
import caxecraft.content.ContentPackagePath.ContentPackagePathError;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.ContentPackageZipSource;
import caxecraft.content.RuntimeContentDigest.runtimeSha256;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import haxe.io.Bytes;

/**
 * Builds one reproducible stored ZIP from a fully validated package directory.
 *
 * The manifest decides which files travel. This module reads and receipts each
 * declared file, checks the asset inventory's runtime selection, sorts logical
 * paths, and writes fixed ZIP metadata into one owned byte vector. Repeating
 * the export from the same authored bytes therefore produces the same archive;
 * filesystem order and modification times never enter the output.
 */
/** Exact reason no canonical archive bytes were allowed to escape. */
enum ContentPackageZipExportError {
	InvalidManifestPath(error:ContentPackagePathError);
	ManifestSourceRejected(error:ContentPackageError);
	ManifestSchemaRejected(diagnostic:RuntimeSchemaDiagnostic);
	ManifestPathDeclaredAsEntry(path:String);
	EntrySourceRejected(path:String, error:ContentPackageError);
	EntryLengthMismatch(path:String, expected:Int, actual:Int);
	EntryHashMismatch(path:String, expected:String);
	AssetClosureRejected(error:ContentPackageAssetClosureError);
	ArchiveEntryCountExceeded(maximum:Int);
	ArchiveByteBudgetExceeded(maximum:Int);
}

/** Export either yields one complete owned ZIP or one closed rejection. */
enum ContentPackageZipExportResult {
	PackageZipExported(bytes:Bytes);
	PackageZipExportRejected(error:ContentPackageZipExportError);
}

/** Internal verified-read result that prevents partial entries from escaping. */
private enum ContentPackageZipEntryReadResult {
	ExportEntryBytes(bytes:Bytes);
	ExportEntryRejected(error:ContentPackageZipExportError);
}

/** One verified payload and its precomputed canonical ZIP facts. */
private final class ContentPackageZipExportEntry {
	/** Validated package-relative spelling. */
	public final path:ContentPackagePath;

	/** Exact UTF-8 name bytes written into both ZIP records. */
	public final name:Bytes;

	/** Independently owned payload bytes read from the source. */
	public final payload:Bytes;

	/** PKWARE CRC-32 bit pattern for the payload. */
	public final crc32:Int;

	/** Local-header offset assigned after canonical sorting. */
	public var localOffset:Int = 0;

	public function new(path:ContentPackagePath, payload:Bytes) {
		this.path = path;
		this.name = Bytes.ofString(path.text());
		this.payload = payload;
		this.crc32 = zipCrc32(payload);
	}
}

/**
 * Export one manifest and every file it owns into the admitted stored profile.
 *
 * The source may contain unrelated authoring files; they are ignored because
 * the package manifest, not directory enumeration, is the transport authority.
 * No bytes escape until every selected entry and cross-manifest asset fact has
 * passed its length and SHA-256 checks.
 */
function exportContentPackageZip(source:ContentPackageSource, manifestPath:String):ContentPackageZipExportResult {
	final parsedManifestPath = switch ContentPackagePath.parse(manifestPath) {
		case PathRejected(error): return PackageZipExportRejected(InvalidManifestPath(error));
		case PathAccepted(value): value;
	};
	final manifestBytes = switch source.read(manifestPath) {
		case PackageBytesRejected(error): return PackageZipExportRejected(ManifestSourceRejected(error));
		case PackageBytesRead(value): value.bytes;
	};
	final manifest = switch caxecraft.content.ContentPackageManifest.decodeContentPackageManifest(manifestBytes) {
		case ContentPackageManifestRejected(diagnostic): return PackageZipExportRejected(ManifestSchemaRejected(diagnostic));
		case ContentPackageManifestReady(value): value;
	};
	for (index in 0...manifest.entryCount()) {
		final path = manifest.entryAt(index).logicalPath.text();
		if (path == manifestPath)
			return PackageZipExportRejected(ManifestPathDeclaredAsEntry(path));
	}
	if (manifest.entryCount() + 1 > ContentPackageZipSource.MAXIMUM_ARCHIVE_ENTRIES)
		return PackageZipExportRejected(ArchiveEntryCountExceeded(ContentPackageZipSource.MAXIMUM_ARCHIVE_ENTRIES));

	// Declared lengths are already schema-bounded. Account for every local and
	// central record before reading payloads, so an archive that cannot fit never
	// forces a large allocation or digest pass merely to reach the same answer.
	var plannedBytes = 22;
	final manifestNameBytes = Bytes.ofString(parsedManifestPath.text()).length;
	final manifestContribution = 76 + manifestNameBytes * 2 + manifestBytes.length;
	if (plannedBytes > ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES - manifestContribution)
		return PackageZipExportRejected(ArchiveByteBudgetExceeded(ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES));
	plannedBytes += manifestContribution;
	for (index in 0...manifest.entryCount()) {
		final declared = manifest.entryAt(index);
		final nameBytes = Bytes.ofString(declared.logicalPath.text()).length;
		final contribution = 76 + nameBytes * 2 + declared.byteLength;
		if (plannedBytes > ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES - contribution)
			return PackageZipExportRejected(ArchiveByteBudgetExceeded(ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES));
		plannedBytes += contribution;
	}

	// The inventory is tiny compared with the selected PNGs. Prove their complete
	// cross-manifest closure first, so a stale or unselected entry cannot force the
	// exporter through the expensive payload path before it fails.
	var assetInventoryBytes:Null<Bytes> = null;
	for (index in 0...manifest.entryCount()) {
		final declared = manifest.entryAt(index);
		switch declared.kind {
			case AssetManifest if (assetInventoryBytes == null):
				assetInventoryBytes = switch readVerifiedEntry(source, declared) {
					case ExportEntryRejected(error): return PackageZipExportRejected(error);
					case ExportEntryBytes(bytes): bytes;
				};
			case _:
		}
	}
	final closureInput = assetInventoryBytes == null ? Bytes.alloc(0) : assetInventoryBytes;
	switch verifyContentPackageAssetClosure(manifest, closureInput) {
		case AssetClosureRejected(error):
			return PackageZipExportRejected(AssetClosureRejected(error));
		case AssetClosureReady(_):
	}

	final entries:Array<ContentPackageZipExportEntry> = [new ContentPackageZipExportEntry(parsedManifestPath, manifestBytes)];
	for (index in 0...manifest.entryCount()) {
		final declared = manifest.entryAt(index);
		final payload = switch declared.kind {
			case AssetManifest:
				if (assetInventoryBytes == null)
					return PackageZipExportRejected(AssetClosureRejected(AssetManifestEntryMissing));
				assetInventoryBytes;
			case _:
				switch readVerifiedEntry(source, declared) {
					case ExportEntryRejected(error): return PackageZipExportRejected(error);
					case ExportEntryBytes(bytes): bytes;
				};
		};
		entries.push(new ContentPackageZipExportEntry(declared.logicalPath, payload));
	}
	entries.sort(compareExportEntries);
	var total = 22;
	for (entry in entries) {
		final contribution = 76 + entry.name.length * 2 + entry.payload.length;
		if (total > ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES - contribution)
			return PackageZipExportRejected(ArchiveByteBudgetExceeded(ContentPackageZipSource.MAXIMUM_ARCHIVE_BYTES));
		total += contribution;
	}
	final output = Bytes.alloc(total);
	var cursor = 0;
	for (entry in entries) {
		entry.localOffset = cursor;
		writeLocal(output, cursor, entry);
		cursor += 30 + entry.name.length + entry.payload.length;
	}
	final centralOffset = cursor;
	for (entry in entries) {
		writeCentral(output, cursor, entry);
		cursor += 46 + entry.name.length;
	}
	final centralSize = cursor - centralOffset;
	writeU32(output, cursor, 0x06054b50);
	writeU16(output, cursor + 4, 0);
	writeU16(output, cursor + 6, 0);
	writeU16(output, cursor + 8, entries.length);
	writeU16(output, cursor + 10, entries.length);
	writeU32(output, cursor + 12, centralSize);
	writeU32(output, cursor + 16, centralOffset);
	writeU16(output, cursor + 20, 0);
	return PackageZipExported(output);
}

/** Read and receipt one declared entry before it joins the export plan. */
private function readVerifiedEntry(source:ContentPackageSource, declared:ContentPackageEntry):ContentPackageZipEntryReadResult {
	final path = declared.logicalPath.text();
	final payload = switch source.read(path) {
		case PackageBytesRejected(error): return ExportEntryRejected(EntrySourceRejected(path, error));
		case PackageBytesRead(value): value.bytes;
	};
	if (payload.length != declared.byteLength)
		return ExportEntryRejected(EntryLengthMismatch(path, declared.byteLength, payload.length));
	if (!sha256Matches(payload, declared.sha256))
		return ExportEntryRejected(EntryHashMismatch(path, declared.sha256));
	return ExportEntryBytes(payload);
}

/** Sort by exact UTF-8 path bytes, matching manifest canonical ordering. */
private function compareExportEntries(left:ContentPackageZipExportEntry, right:ContentPackageZipExportEntry):Int
	return RuntimeSchemaReader.compareUtf8(left.path.text(), right.path.text());

/** Write one normalized local record followed by exact name and payload bytes. */
private function writeLocal(output:Bytes, offset:Int, entry:ContentPackageZipExportEntry):Void {
	writeU32(output, offset, 0x04034b50);
	writeU16(output, offset + 4, 10);
	writeU16(output, offset + 6, utf8Flags(entry.name));
	writeU16(output, offset + 8, 0);
	writeU16(output, offset + 10, 0);
	writeU16(output, offset + 12, 0x21);
	writeU32(output, offset + 14, entry.crc32);
	writeU32(output, offset + 18, entry.payload.length);
	writeU32(output, offset + 22, entry.payload.length);
	writeU16(output, offset + 26, entry.name.length);
	writeU16(output, offset + 28, 0);
	copyBytes(output, offset + 30, entry.name);
	copyBytes(output, offset + 30 + entry.name.length, entry.payload);
}

/** Write one normalized Unix regular-file central record. */
private function writeCentral(output:Bytes, offset:Int, entry:ContentPackageZipExportEntry):Void {
	writeU32(output, offset, 0x02014b50);
	writeU16(output, offset + 4, 0x031e);
	writeU16(output, offset + 6, 10);
	writeU16(output, offset + 8, utf8Flags(entry.name));
	writeU16(output, offset + 10, 0);
	writeU16(output, offset + 12, 0);
	writeU16(output, offset + 14, 0x21);
	writeU32(output, offset + 16, entry.crc32);
	writeU32(output, offset + 20, entry.payload.length);
	writeU32(output, offset + 24, entry.payload.length);
	writeU16(output, offset + 28, entry.name.length);
	writeU16(output, offset + 30, 0);
	writeU16(output, offset + 32, 0);
	writeU16(output, offset + 34, 0);
	writeU16(output, offset + 36, 0);
	writeU32(output, offset + 38, 0x81a40000);
	writeU32(output, offset + 42, entry.localOffset);
	copyBytes(output, offset + 46, entry.name);
}

/** Set the language-encoding flag only when a UTF-8 multibyte unit appears. */
private function utf8Flags(name:Bytes):Int {
	for (index in 0...name.length)
		if (name.get(index) >= 128)
			return 0x0800;
	return 0;
}

/** Copy one complete owned vector into a pre-sized output range. */
private function copyBytes(output:Bytes, offset:Int, source:Bytes):Void
	for (index in 0...source.length)
		output.set(offset + index, source.get(index));

/** Compute the PKWARE CRC-32 independently of the semantic SHA-256 receipt. */
private function zipCrc32(bytes:Bytes):Int {
	var crc = -1;
	for (index in 0...bytes.length) {
		crc ^= bytes.get(index);
		for (_ in 0...8)
			crc = crc >>> 1 ^ ((crc & 1) == 0 ? 0 : 0xedb88320);
	}
	return ~crc;
}

/** Compare one payload with its already validated lowercase SHA-256 receipt. */
private function sha256Matches(input:Bytes, expected:String):Bool {
	final digest = runtimeSha256(input);
	for (index in 0...digest.length) {
		final high = hexDigit(expected.charCodeAt(index * 2));
		final low = hexDigit(expected.charCodeAt(index * 2 + 1));
		if (digest.get(index) != high * 16 + low)
			return false;
	}
	return true;
}

/** Decode one lowercase hexadecimal digit from a validated package receipt. */
private inline function hexDigit(code:Int):Int
	return code <= 57 ? code - 48 : code - 87;

/** Write one unsigned little-endian 16-bit field. */
private function writeU16(bytes:Bytes, offset:Int, value:Int):Void {
	bytes.set(offset, value & 0xff);
	bytes.set(offset + 1, value >>> 8 & 0xff);
}

/** Write one unsigned little-endian 32-bit bit pattern. */
private function writeU32(bytes:Bytes, offset:Int, value:Int):Void {
	bytes.set(offset, value & 0xff);
	bytes.set(offset + 1, value >>> 8 & 0xff);
	bytes.set(offset + 2, value >>> 16 & 0xff);
	bytes.set(offset + 3, value >>> 24 & 0xff);
}
