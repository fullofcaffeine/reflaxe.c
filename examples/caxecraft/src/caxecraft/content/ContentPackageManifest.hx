package caxecraft.content;

import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentJsonWriter.appendJsonString;
import caxecraft.content.ContentPackageModel.ContentPackageError;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.RuntimeContentDigest.runtimeSha256;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaErrorKind;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import haxe.io.Bytes;

/**
 * Defines what a Caxecraft package contributes independently of how it travels.
 *
 * The manifest names existing campaign, level, content, localization, asset
 * inventory, and selected binary files without duplicating their schemas. The
 * asset inventory remains the authority for visual selection and provenance;
 * this manifest repeats selected paths and digests only so every file that must
 * travel has one transport receipt. A directory or ZIP source exposes the same
 * logical paths and bytes, then this module performs the same role, dependency,
 * length, and SHA-256 checks for either.
 */
/** Stable identity shared by package manifests and dependency declarations. */
abstract ContentPackageId(String) {
	private inline function new(value:String)
		this = value;

	/** Return the validated namespaced spelling. */
	public inline function text():String
		return this;

	/** Construct only after the package decoder validates the ID grammar. */
	@:allow(caxecraft.content.ContentPackageManifestDecoder)
	static inline function admitted(value:String):ContentPackageId
		return new ContentPackageId(value);
}

/** Closed capabilities one package may compose. `Mod` is intent, not a file schema. */
enum ContentPackageRole {
	Assets;
	Campaign;
	Content;
	Levels;
	Localization;
	Mod;
}

/** Existing payload schema or binary kind named by one owned path. */
enum ContentPackageEntryKind {
	Asset;
	AssetManifest;
	CampaignManifest;
	ContentPack;
	Level;
	LocalizationCatalog;
	RuntimeContent;
}

/** One owned payload path and its independently authored byte receipt. */
final class ContentPackageEntry {
	/** Existing schema or binary family expected at this path. */
	public final kind:ContentPackageEntryKind;

	/** Portable package-relative name used by every source implementation. */
	public final logicalPath:ContentPackagePath;

	/** Exact authored byte count checked before semantic use. */
	public final byteLength:Int;

	/** Lowercase SHA-256 receipt checked against the loaded bytes. */
	public final sha256:String;

	@:allow(caxecraft.content.ContentPackageManifestDecoder)
	private function new(kind, logicalPath, byteLength, sha256) {
		this.kind = kind;
		this.logicalPath = logicalPath;
		this.byteLength = byteLength;
		this.sha256 = sha256;
	}
}

/** Inclusive compatible version interval for one separately installed package. */
final class ContentPackageDependency {
	/** Stable identity of the separately supplied package. */
	public final id:ContentPackageId;

	/** Lowest compatible positive package version, inclusive. */
	public final minimumVersion:Int;

	/** Highest compatible positive package version, inclusive. */
	public final maximumVersion:Int;

	@:allow(caxecraft.content.ContentPackageManifestDecoder)
	private function new(id, minimumVersion, maximumVersion) {
		this.id = id;
		this.minimumVersion = minimumVersion;
		this.maximumVersion = maximumVersion;
	}
}

/** Complete immutable semantic package description. */
final class ContentPackageManifest {
	/** Stable identity used by dependencies and package selection. */
	public final id:ContentPackageId;

	/** Monotonic positive version of this package's authored contents. */
	public final version:Int;

	final roles:Array<ContentPackageRole>;
	final entries:Array<ContentPackageEntry>;
	final dependencies:Array<ContentPackageDependency>;

	/** Take ownership of fresh decoder arrays that have no outside alias. */
	@:allow(caxecraft.content.ContentPackageManifestDecoder)
	private function new(id, version, roles, entries, dependencies) {
		this.id = id;
		this.version = version;
		this.roles = roles;
		this.entries = entries;
		this.dependencies = dependencies;
	}

	/** Number of declared semantic roles. */
	public inline function roleCount():Int
		return roles.length;

	/** Read one role in canonical order. */
	public inline function roleAt(index:Int):ContentPackageRole
		return roles[index];

	/** Number of exact owned payload paths. */
	public inline function entryCount():Int
		return entries.length;

	/** Read one entry in canonical path order. */
	public inline function entryAt(index:Int):ContentPackageEntry
		return entries[index];

	/** Number of bounded external package requirements. */
	public inline function dependencyCount():Int
		return dependencies.length;

	/** Read one dependency in canonical ID order. */
	public inline function dependencyAt(index:Int):ContentPackageDependency
		return dependencies[index];
}

/** A complete semantic manifest or its first located schema rejection. */
enum ContentPackageManifestReadResult {
	ContentPackageManifestReady(manifest:ContentPackageManifest);
	ContentPackageManifestRejected(diagnostic:RuntimeSchemaDiagnostic);
}

/** Exact reason a semantic manifest could not become a verified package. */
enum ContentPackageLoadError {
	ContentPackageManifestSourceRejected(error:ContentPackageError);
	ContentPackageManifestSchemaRejected(diagnostic:RuntimeSchemaDiagnostic);
	ContentPackageEntrySourceRejected(path:String, error:ContentPackageError);
	ContentPackageEntryLengthMismatch(path:String, expected:Int, actual:Int);
	ContentPackageEntryHashMismatch(path:String, expected:String);
}

/** Verified receipt for one payload entry, independent of source transport. */
final class ContentPackageEntryReceipt {
	/** Verified logical path from the owning manifest entry. */
	public final logicalPath:ContentPackagePath;

	/** Observed byte count after the source read matched the expectation. */
	public final byteLength:Int;

	/** Authored digest after the loaded bytes matched it. */
	public final sha256:String;

	@:allow(caxecraft.content.ContentPackageManifest)
	private function new(logicalPath, byteLength, sha256) {
		this.logicalPath = logicalPath;
		this.byteLength = byteLength;
		this.sha256 = sha256;
	}
}

/** Manifest plus proof that every owned path matched its authored receipt. */
final class LoadedContentPackage {
	/** Fully decoded semantic manifest whose entries all passed verification. */
	public final manifest:ContentPackageManifest;

	final receipts:Array<ContentPackageEntryReceipt>;

	/** Take ownership of the fresh verifier receipt Array. */
	@:allow(caxecraft.content.ContentPackageManifest)
	private function new(manifest, receipts) {
		this.manifest = manifest;
		this.receipts = receipts;
	}

	/** Number of verified payload files. */
	public inline function receiptCount():Int
		return receipts.length;

	/** Read one receipt in manifest entry order. */
	public inline function receiptAt(index:Int):ContentPackageEntryReceipt
		return receipts[index];
}

/** A fully verified package or one exact source/schema/receipt failure. */
enum ContentPackageLoadResult {
	ContentPackageReady(value:LoadedContentPackage);
	ContentPackageRejected(error:ContentPackageLoadError);
}

/** Decode bounded manifest bytes without touching a package source. */
function decodeContentPackageManifest(input:Bytes):ContentPackageManifestReadResult
	return ContentPackageManifestDecoder.decode(input);

/**
 * Write one validated package with replacement receipts for changed entries.
 *
 * Roles, paths, kinds, dependencies, and their canonical order come from the
 * decoded model. Refresh planning supplies only final byte counts and digests.
 */
function writeContentPackageManifest(manifest:ContentPackageManifest, updates:Array<ContentReceipt>):Bytes {
	final output = new StringBuf();
	output.add('{\n  "schemaVersion": 1,\n  "packageId": ');
	appendJsonString(output, manifest.id.text());
	output.add(',\n  "packageVersion": ${manifest.version},\n  "roles": [\n');
	for (index in 0...manifest.roleCount()) {
		output.add('    ');
		appendJsonString(output, packageRoleText(manifest.roleAt(index)));
		output.add(index + 1 == manifest.roleCount() ? '\n' : ',\n');
	}
	output.add('  ],\n  "entries": [\n');
	for (index in 0...manifest.entryCount()) {
		final entry = manifest.entryAt(index);
		final logicalPath = entry.logicalPath.text();
		final update = packageReceiptFor(updates, logicalPath);
		final byteLength = update == null ? entry.byteLength : update.byteLength;
		final sha256 = update == null ? entry.sha256 : update.sha256;
		output.add('    {\n      "kind": ');
		appendJsonString(output, packageKindText(entry.kind));
		output.add(',\n      "path": ');
		appendJsonString(output, logicalPath);
		output.add(',\n      "byteLength": $byteLength,\n      "sha256": ');
		appendJsonString(output, sha256);
		output.add(index + 1 == manifest.entryCount() ? '\n    }\n' : '\n    },\n');
	}
	output.add('  ],\n  "dependencies": [');
	if (manifest.dependencyCount() > 0)
		output.add('\n');
	for (index in 0...manifest.dependencyCount()) {
		final dependency = manifest.dependencyAt(index);
		output.add('    {\n      "packageId": ');
		appendJsonString(output, dependency.id.text());
		output.add(',\n      "minimumVersion": ${dependency.minimumVersion},\n      "maximumVersion": ${dependency.maximumVersion}');
		output.add(index + 1 == manifest.dependencyCount() ? '\n    }\n' : '\n    },\n');
	}
	output.add(manifest.dependencyCount() == 0 ? ']\n}\n' : '  ]\n}\n');
	return Bytes.ofString(output.toString());
}

/** Find one exact package-entry replacement. */
private function packageReceiptFor(updates:Array<ContentReceipt>, logicalPath:String):Null<ContentReceipt> {
	for (update in updates)
		if (update.logicalPath == logicalPath)
			return update;
	return null;
}

/** Return the canonical JSON spelling for one admitted role. */
private function packageRoleText(role:ContentPackageRole):String
	return switch role {
		case Assets: "assets";
		case Campaign: "campaign";
		case Content: "content";
		case Levels: "levels";
		case Localization: "localization";
		case Mod: "mod";
	};

/** Return the canonical JSON spelling for one admitted entry kind. */
private function packageKindText(kind:ContentPackageEntryKind):String
	return switch kind {
		case Asset: "asset";
		case AssetManifest: "asset-manifest";
		case CampaignManifest: "campaign";
		case ContentPack: "content-pack";
		case Level: "level";
		case LocalizationCatalog: "localization";
		case RuntimeContent: "runtime-content";
	};

/** Read a manifest and verify all its payload entries through one source capability. */
function loadContentPackage(source:ContentPackageSource, manifestPath:String):ContentPackageLoadResult {
	return switch source.read(manifestPath) {
		case PackageBytesRejected(error): ContentPackageRejected(ContentPackageManifestSourceRejected(error));
		case PackageBytesRead(content):
			switch decodeContentPackageManifest(content.bytes) {
				case ContentPackageManifestRejected(diagnostic): ContentPackageRejected(ContentPackageManifestSchemaRejected(diagnostic));
				case ContentPackageManifestReady(manifest): verifyContentPackage(source, manifest);
			}
	};
}

/** Verify every declared path and receipt without caring whether the source is a directory or ZIP. */
function verifyContentPackage(source:ContentPackageSource, manifest:ContentPackageManifest):ContentPackageLoadResult {
	final receipts:Array<ContentPackageEntryReceipt> = [];
	for (index in 0...manifest.entryCount()) {
		final entry = manifest.entryAt(index);
		final path = entry.logicalPath.text();
		final content = switch source.read(path) {
			case PackageBytesRejected(error): return ContentPackageRejected(ContentPackageEntrySourceRejected(path, error));
			case PackageBytesRead(value): value;
		};
		if (content.bytes.length != entry.byteLength)
			return ContentPackageRejected(ContentPackageEntryLengthMismatch(path, entry.byteLength, content.bytes.length));
		if (!sha256Matches(content.bytes, entry.sha256))
			return ContentPackageRejected(ContentPackageEntryHashMismatch(path, entry.sha256));
		receipts.push(new ContentPackageEntryReceipt(entry.logicalPath, entry.byteLength, entry.sha256));
	}
	return ContentPackageReady(new LoadedContentPackage(manifest, receipts));
}

/** Request-local decoder; no partial typed value escapes its first failure. */
private final class ContentPackageManifestDecoder {
	static inline final MAXIMUM_ENTRIES = 512;
	static inline final MAXIMUM_DEPENDENCIES = 128;
	static inline final MAXIMUM_TOTAL_BYTES = 268435456;

	/** Parse and cross-check one complete package manifest. */
	public static function decode(input:Bytes):ContentPackageManifestReadResult {
		final reader = new RuntimeSchemaReader();
		final root = reader.parse(input);
		if (root == null)
			return rejected(reader);
		final fields = reader.object(root, "package", [
			"schemaVersion",
			"packageId",
			"packageVersion",
			"roles",
			"entries",
			"dependencies"
		]);
		if (fields == null)
			return rejected(reader);
		final schemaNode = reader.field(fields, "schemaVersion");
		final schema = reader.integer(schemaNode, "schemaVersion", 1, 2147483647);
		if (schema == null)
			return rejected(reader);
		if (schema != 1) {
			reader.reject(schemaNode, SchemaUnsupportedVersion("schemaVersion", 1));
			return rejected(reader);
		}
		final idNode = reader.field(fields, "packageId");
		final idText = reader.string(idNode, "packageId", 128);
		if (idText == null)
			return rejected(reader);
		if (!RuntimeSchemaReader.validContentId(idText)) {
			reader.reject(idNode, SchemaInvalidContentId("packageId"));
			return rejected(reader);
		}
		final version = reader.integer(reader.field(fields, "packageVersion"), "packageVersion", 1, 2147483647);
		if (version == null)
			return rejected(reader);
		final roles = readRoles(reader, reader.field(fields, "roles"));
		if (roles == null)
			return rejected(reader);
		final entries = readEntries(reader, reader.field(fields, "entries"), roles);
		if (entries == null)
			return rejected(reader);
		if (!validateRoleCoverage(reader, roles, entries, root))
			return rejected(reader);
		final dependencies = readDependencies(reader, reader.field(fields, "dependencies"), idText);
		if (dependencies == null)
			return rejected(reader);
		return ContentPackageManifestReady(new ContentPackageManifest(ContentPackageId.admitted(idText), version, roles, entries, dependencies));
	}

	/** Parse unique roles in the format's fixed order. */
	static function readRoles(reader:RuntimeSchemaReader, node:ContentJsonNode):Null<Array<ContentPackageRole>> {
		final values = reader.array(node, "roles", 1, 6);
		if (values == null)
			return null;
		final result:Array<ContentPackageRole> = [];
		var previous = -1;
		for (index in 0...values.length) {
			final path = "roles[" + index + "]";
			final text = reader.string(values[index], path, 32);
			if (text == null)
				return null;
			final role = parseRole(text);
			if (role == null) {
				reader.reject(values[index], SchemaInvalidClosedValue(path, text));
				return null;
			}
			final rank = roleRank(role);
			if (rank == previous) {
				reader.reject(values[index], SchemaDuplicateValue("roles", text));
				return null;
			}
			if (rank < previous) {
				reader.reject(values[index], SchemaNonCanonicalOrder("roles"));
				return null;
			}
			result.push(role);
			previous = rank;
		}
		return result;
	}

	/** Parse exact payload inventory and validate portable path uniqueness. */
	static function readEntries(reader:RuntimeSchemaReader, node:ContentJsonNode, roles:Array<ContentPackageRole>):Null<Array<ContentPackageEntry>> {
		final values = reader.array(node, "entries", 1, MAXIMUM_ENTRIES);
		if (values == null)
			return null;
		final result:Array<ContentPackageEntry> = [];
		var totalBytes = 0;
		for (index in 0...values.length) {
			final base = "entries[" + index + "]";
			final fields = reader.object(values[index], base, ["kind", "path", "byteLength", "sha256"]);
			if (fields == null)
				return null;
			final kindNode = reader.field(fields, "kind");
			final kindText = reader.string(kindNode, base + ".kind", 32);
			if (kindText == null)
				return null;
			final kind = parseKind(kindText);
			if (kind == null) {
				reader.reject(kindNode, SchemaInvalidClosedValue(base + ".kind", kindText));
				return null;
			}
			final required = requiredRole(kind);
			if (!hasRole(roles, required)) {
				reader.reject(kindNode, SchemaRoleEntryMismatch(base, roleText(required)));
				return null;
			}
			final pathNode = reader.field(fields, "path");
			final pathText = reader.string(pathNode, base + ".path", ContentPackagePath.MAXIMUM_PATH_BYTES);
			if (pathText == null)
				return null;
			final logicalPath = switch ContentPackagePath.parse(pathText) {
				case PathAccepted(value): value;
				case PathRejected(_):
					reader.reject(pathNode, SchemaInvalidLogicalPath(base + ".path"));
					return null;
			};
			for (previous in result) {
				if (previous.logicalPath.text() == pathText) {
					reader.reject(pathNode, SchemaDuplicateLogicalPath(pathText));
					return null;
				}
				if (previous.logicalPath.asciiCaseEquals(logicalPath)) {
					reader.reject(pathNode, SchemaCaseCollidingLogicalPath(pathText, previous.logicalPath.text()));
					return null;
				}
			}
			if (result.length > 0 && RuntimeSchemaReader.compareUtf8(result[result.length - 1].logicalPath.text(), pathText) > 0) {
				reader.reject(pathNode, SchemaNonCanonicalOrder("entries"));
				return null;
			}
			final byteLength = reader.integer(reader.field(fields, "byteLength"), base + ".byteLength", 1, ContentPackageStore.MAXIMUM_PACKAGE_BYTES);
			final sha256 = readSha256(reader, reader.field(fields, "sha256"), base + ".sha256");
			if (byteLength == null || sha256 == null)
				return null;
			if (totalBytes > MAXIMUM_TOTAL_BYTES - byteLength) {
				reader.reject(values[index], SchemaPackageByteBudgetExceeded(MAXIMUM_TOTAL_BYTES));
				return null;
			}
			totalBytes += byteLength;
			result.push(new ContentPackageEntry(kind, logicalPath, byteLength, sha256));
		}
		return result;
	}

	/** Require every non-mod role to own at least one matching payload. */
	static function validateRoleCoverage(reader:RuntimeSchemaReader, roles:Array<ContentPackageRole>, entries:Array<ContentPackageEntry>,
			root:ContentJsonNode):Bool {
		for (role in roles) {
			if (roleRank(role) == 5)
				continue;
			var found = false;
			for (entry in entries)
				if (roleRank(requiredRole(entry.kind)) == roleRank(role))
					found = true;
			if (!found) {
				reader.reject(root, SchemaMissingRoleEntry(roleText(role)));
				return false;
			}
		}
		return true;
	}

	/** Parse unique sorted dependency intervals without resolving installation state. */
	static function readDependencies(reader:RuntimeSchemaReader, node:ContentJsonNode, ownId:String):Null<Array<ContentPackageDependency>> {
		final values = reader.array(node, "dependencies", 0, MAXIMUM_DEPENDENCIES);
		if (values == null)
			return null;
		final result:Array<ContentPackageDependency> = [];
		for (index in 0...values.length) {
			final base = "dependencies[" + index + "]";
			final fields = reader.object(values[index], base, ["packageId", "minimumVersion", "maximumVersion"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "packageId");
			final id = reader.string(idNode, base + ".packageId", 128);
			if (id == null)
				return null;
			if (!RuntimeSchemaReader.validContentId(id)) {
				reader.reject(idNode, SchemaInvalidContentId(base + ".packageId"));
				return null;
			}
			if (id == ownId) {
				reader.reject(idNode, SchemaSelfDependency(id));
				return null;
			}
			for (previous in result)
				if (previous.id.text() == id) {
					reader.reject(idNode, SchemaDuplicateDependency(id));
					return null;
				}
			if (result.length > 0 && RuntimeSchemaReader.compareUtf8(result[result.length - 1].id.text(), id) > 0) {
				reader.reject(idNode, SchemaNonCanonicalOrder("dependencies"));
				return null;
			}
			final minimum = reader.integer(reader.field(fields, "minimumVersion"), base + ".minimumVersion", 1, 2147483647);
			final maximum = reader.integer(reader.field(fields, "maximumVersion"), base + ".maximumVersion", 1, 2147483647);
			if (minimum == null || maximum == null)
				return null;
			if (minimum > maximum) {
				reader.reject(idNode, SchemaIncompatibleVersionBounds(id, minimum, maximum));
				return null;
			}
			result.push(new ContentPackageDependency(ContentPackageId.admitted(id), minimum, maximum));
		}
		return result;
	}

	static function parseRole(value:String):Null<ContentPackageRole>
		return switch value {
			case "assets": Assets;
			case "campaign": Campaign;
			case "content": Content;
			case "levels": Levels;
			case "localization": Localization;
			case "mod": Mod;
			case _: null;
		};

	static function roleRank(role:ContentPackageRole):Int
		return switch role {
			case Assets: 0;
			case Campaign: 1;
			case Content: 2;
			case Levels: 3;
			case Localization: 4;
			case Mod: 5;
		};

	static function roleText(role:ContentPackageRole):String
		return switch role {
			case Assets: "assets";
			case Campaign: "campaign";
			case Content: "content";
			case Levels: "levels";
			case Localization: "localization";
			case Mod: "mod";
		};

	static function hasRole(roles:Array<ContentPackageRole>, expected:ContentPackageRole):Bool {
		for (role in roles)
			if (roleRank(role) == roleRank(expected))
				return true;
		return false;
	}

	static function parseKind(value:String):Null<ContentPackageEntryKind>
		return switch value {
			case "asset": Asset;
			case "asset-manifest": AssetManifest;
			case "campaign": CampaignManifest;
			case "content-pack": ContentPack;
			case "level": Level;
			case "localization": LocalizationCatalog;
			case "runtime-content": RuntimeContent;
			case _: null;
		};

	static function requiredRole(kind:ContentPackageEntryKind):ContentPackageRole
		return switch kind {
			case Asset | AssetManifest: Assets;
			case CampaignManifest: Campaign;
			case ContentPack | RuntimeContent: Content;
			case Level: Levels;
			case LocalizationCatalog: Localization;
		};

	static function readSha256(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = reader.string(node, path, 64);
		if (value == null)
			return null;
		var valid = value.length == 64;
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (!((code >= 48 && code <= 57) || (code >= 97 && code <= 102)))
				valid = false;
		}
		if (!valid) {
			reader.reject(node, SchemaInvalidString(path));
			return null;
		}
		return value;
	}

	static function rejected(reader:RuntimeSchemaReader):ContentPackageManifestReadResult {
		final failure = reader.failure;
		return failure == null ? ContentPackageManifestRejected({
			line: 1,
			column: 1,
			kind: SchemaInvalidInvariant("package")
		}) : ContentPackageManifestRejected(failure);
	}
}

/** Compare computed digest bytes with one validated lowercase hexadecimal receipt. */
private function sha256Matches(input:Bytes, expected:String):Bool {
	final digest = runtimeSha256(input);
	for (index in 0...digest.length) {
		final high = hex(expected.charCodeAt(index * 2));
		final low = hex(expected.charCodeAt(index * 2 + 1));
		if (digest.get(index) != (high << 4 | low))
			return false;
	}
	return true;
}

/** Decode one already-validated lowercase hexadecimal digit. */
private inline function hex(code:Int):Int
	return code <= 57 ? code - 48 : code - 87;
