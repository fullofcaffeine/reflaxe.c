package caxecraft.qa;

import caxecraft.content.ContentPackageManifest.ContentPackageEntryKind;
import caxecraft.content.ContentPackageManifest.ContentPackageLoadError;
import caxecraft.content.ContentPackageManifest.ContentPackageLoadResult;
import caxecraft.content.ContentPackageManifest.ContentPackageManifestReadResult;
import caxecraft.content.ContentPackageManifest.ContentPackageRole;
import caxecraft.content.ContentPackageManifest.decodeContentPackageManifest;
import caxecraft.content.ContentPackageManifest.loadContentPackage;
import caxecraft.content.ContentPackageManifest.verifyContentPackage;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.RuntimeSchema.RuntimeSchemaErrorKind;
import haxe.io.Bytes;

/**
 * Proves one transport-neutral package from authored manifest to real files.
 *
 * The checked-in manifest is the independent expectation: it lists the exact
 * campaign, levels, content, localization, and runtime-selected assets that a
 * future directory or ZIP source must expose. This probe loads those files
 * through `ContentPackageStore`; it does not copy or reinterpret their existing
 * schemas. Small inline manifests challenge the package-only rules without
 * manufacturing extra product files.
 */
/** First broken package invariant, or zero after every scenario passes. */
var observed:Int = 0;

/** Compact proof of package version, role count, entry count, and dependency count. */
var traceIdentity:Int = 0;

/** Sum of independently authored byte receipts for every owned payload entry. */
var traceBytes:Int = 0;

/** Closed entry-kind trace proving the real manifest composes all admitted file roles. */
var traceKinds:Int = 0;

/** Run the shared proof on Eval or expose its scalar result to native C. */
function main():Void {
	observed = selfCheck();
	#if !c
	Sys.println(observed);
	Sys.println(traceIdentity);
	Sys.println(traceBytes);
	Sys.println(traceKinds);
	#end
}

/** Return zero only when semantic parsing and directory verification agree. */
function selfCheck():Int {
	final store = switch ContentPackageStore.open(".", "first-adventure-directory", ContentPackageStore.MAXIMUM_PACKAGE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return 1;
	};
	final loaded = switch loadContentPackage(store, "caxecraft.package.json") {
		case ContentPackageReady(value): value;
		case ContentPackageRejected(_): return 2;
	};
	final manifest = loaded.manifest;
	if (manifest.id.text() != "caxecraft:first-adventure"
		|| manifest.version != 1
		|| manifest.roleCount() != 5
		|| manifest.entryCount() != 15
		|| manifest.dependencyCount() != 0
		|| loaded.receiptCount() != 15)
		return 3;
	if (!sameRole(manifest.roleAt(0), Assets)
		|| !sameRole(manifest.roleAt(1), Campaign)
		|| !sameRole(manifest.roleAt(2), Content)
		|| !sameRole(manifest.roleAt(3), Levels)
		|| !sameRole(manifest.roleAt(4), Localization))
		return 4;
	var bytes = 0;
	var kinds = 0;
	for (index in 0...loaded.receiptCount()) {
		final receipt = loaded.receiptAt(index);
		final entry = manifest.entryAt(index);
		if (receipt.logicalPath.text() != entry.logicalPath.text()
			|| receipt.byteLength != entry.byteLength
			|| receipt.sha256 != entry.sha256)
			return 5;
		bytes += receipt.byteLength;
		kinds |= kindBit(entry.kind);
	}
	if (bytes != 9483471 || kinds != 127)
		return 6;
	if (!verifyDecoderRejections())
		return 7;
	if (!verifyEntryFailures(store))
		return 8;
	if (!verifyModDependencyModel())
		return 9;
	traceIdentity = manifest.version * 100000 + manifest.roleCount() * 1000 + manifest.entryCount() * 10 + manifest.dependencyCount();
	traceBytes = bytes;
	traceKinds = kinds;
	return traceIdentity == 105150 ? 0 : 10;
}

/** Challenge malformed, identity, role, path, ordering, and dependency input. */
function verifyDecoderRejections():Bool {
	if (!rejects("{", 1))
		return false;
	if (!rejects(document("caxecraft:test", 2, '["content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), "[]"), 2))
		return false;
	if (!rejects(document("Bad Id", 1, '["content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), "[]"), 3))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["unknown"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), "[]"), 4))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["content","content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), "[]"), 5))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["levels","content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), "[]"), 6))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["levels"]', oneEntry("level", "../escape.caxemap", 1, ZERO_HASH), "[]"), 7))
		return false;
	final duplicateEntries = '['
		+ entry("level", "scenarios/a.caxemap", 1, ZERO_HASH)
		+ ','
		+ entry("level", "scenarios/a.caxemap", 1, ZERO_HASH)
		+ ']';
	if (!rejects(document("caxecraft:test", 1, '["levels"]', duplicateEntries, "[]"), 8))
		return false;
	final collidingEntries = '['
		+ entry("level", "scenarios/A.caxemap", 1, ZERO_HASH)
		+ ','
		+ entry("level", "scenarios/a.caxemap", 1, ZERO_HASH)
		+ ']';
	if (!rejects(document("caxecraft:test", 1, '["levels"]', collidingEntries, "[]"), 9))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["levels"]', oneEntry("unknown", "scenarios/a.caxemap", 1, ZERO_HASH), "[]"), 10))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["content"]', oneEntry("level", "scenarios/a.caxemap", 1, ZERO_HASH), "[]"), 11))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["content","levels"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), "[]"), 12))
		return false;
	final duplicateDependencies = '[' + dependency("vendor:base", 1, 2) + ',' + dependency("vendor:base", 1, 2) + ']';
	if (!rejects(document("caxecraft:test", 1, '["content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), duplicateDependencies), 13))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH),
		'[' + dependency("caxecraft:test", 1, 2) + ']'),
		14))
		return false;
	if (!rejects(document("caxecraft:test", 1, '["content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH),
		'[' + dependency("vendor:base", 3, 2) + ']'),
		15))
		return false;
	final unsortedDependencies = '[' + dependency("vendor:z", 1, 2) + ',' + dependency("vendor:a", 1, 2) + ']';
	if (!rejects(document("caxecraft:test", 1, '["content"]', oneEntry("content-pack", "packs/test/content.json", 1, ZERO_HASH), unsortedDependencies), 16))
		return false;
	return rejects(document("caxecraft:test", 1, '["assets"]', oversizedInventory(), "[]"), 17);
}

/** Prove missing and stale real files fail before a loaded package exists. */
function verifyEntryFailures(store:ContentPackageStore):Bool {
	final missing = decode(document("caxecraft:test", 1, '["levels"]', oneEntry("level", "scenarios/missing.caxemap", 1, ZERO_HASH), "[]"));
	if (missing == null)
		return false;
	switch verifyContentPackage(store, missing) {
		case ContentPackageRejected(ContentPackageEntrySourceRejected("scenarios/missing.caxemap", EntryMissing)):
		case _:
			return false;
	}
	final staleLength = decode(document("caxecraft:test", 1, '["levels"]',
		oneEntry("level", "scenarios/first-playable/map.caxemap", 5097, "e7bd9c1c8c64d84940880e8b42fd8aa2f9df9c4229ba160fdb35785e0f368644"), "[]"));
	if (staleLength == null)
		return false;
	switch verifyContentPackage(store, staleLength) {
		case ContentPackageRejected(ContentPackageEntryLengthMismatch("scenarios/first-playable/map.caxemap", 5097, 5098)):
		case _:
			return false;
	}
	final staleHash = decode(document("caxecraft:test", 1, '["levels"]', oneEntry("level", "scenarios/first-playable/map.caxemap", 5098, ZERO_HASH), "[]"));
	if (staleHash == null)
		return false;
	return switch verifyContentPackage(store, staleHash) {
		case ContentPackageRejected(ContentPackageEntryHashMismatch("scenarios/first-playable/map.caxemap", ZERO_HASH)): true;
		case _: false;
	};
}

/** Prove `mod` composes existing entry kinds and carries bounded dependencies. */
function verifyModDependencyModel():Bool {
	final source = document("community:example-mod", 1, '["content","mod"]', oneEntry("content-pack", "packs/example/content.json", 1, ZERO_HASH),
		'[' + dependency("caxecraft:base", 2, 5) + ']');
	final manifest = decode(source);
	if (manifest == null || manifest.roleCount() != 2 || manifest.dependencyCount() != 1 || !sameRole(manifest.roleAt(1), Mod))
		return false;
	final dependency = manifest.dependencyAt(0);
	return dependency.id.text() == "caxecraft:base" && dependency.minimumVersion == 2 && dependency.maximumVersion == 5;
}

/** Decode a positive inline manifest while preserving null as test failure. */
function decode(source:String):Null<caxecraft.content.ContentPackageManifest.ContentPackageManifest> {
	return switch decodeContentPackageManifest(Bytes.ofString(source)) {
		case ContentPackageManifestReady(value): value;
		case ContentPackageManifestRejected(_): null;
	};
}

/** Match one exact package-only schema rejection. */
function rejects(source:String, expected:Int):Bool {
	final matched = switch decodeContentPackageManifest(Bytes.ofString(source)) {
		case ContentPackageManifestReady(_): false;
		case ContentPackageManifestRejected(diagnostic):
			final result = switch [expected, diagnostic.kind] {
				case [1, SchemaJsonSyntax(_)]: true;
				case [2, SchemaUnsupportedVersion("schemaVersion", 1)]: true;
				case [3, SchemaInvalidContentId("packageId")]: true;
				case [4, SchemaInvalidClosedValue("roles[0]", "unknown")]: true;
				case [5, SchemaDuplicateValue("roles", "content")]: true;
				case [6, SchemaNonCanonicalOrder("roles")]: true;
				case [7, SchemaInvalidLogicalPath("entries[0].path")]: true;
				case [8, SchemaDuplicateLogicalPath("scenarios/a.caxemap")]: true;
				case [9, SchemaCaseCollidingLogicalPath(_, _)]: true;
				case [10, SchemaInvalidClosedValue("entries[0].kind", "unknown")]: true;
				case [11, SchemaRoleEntryMismatch("entries[0]", "levels")]: true;
				case [12, SchemaMissingRoleEntry("levels")]: true;
				case [13, SchemaDuplicateDependency("vendor:base")]: true;
				case [14, SchemaSelfDependency("caxecraft:test")]: true;
				case [15, SchemaIncompatibleVersionBounds("vendor:base", 3, 2)]: true;
				case [16, SchemaNonCanonicalOrder("dependencies")]: true;
				case [17, SchemaPackageByteBudgetExceeded(268435456)]: true;
				case _: false;
			};
			if (!result && expected == 9)
				traceKinds = switch diagnostic.kind {
					case SchemaNonCanonicalOrder(_): 1901;
					case SchemaDuplicateLogicalPath(_): 1902;
					case SchemaInvalidLogicalPath(_): 1903;
					case SchemaCaseCollidingLogicalPath(_, _): 1904;
					case _: 1999;
				};
			result;
	};
	#if !c
	if (!matched)
		Sys.println(900 + expected);
	#end
	if (!matched && traceKinds < 1000)
		traceKinds = 900 + expected;
	return matched;
}

/** Build one closed package document from independently chosen JSON fragments. */
function document(id:String, schemaVersion:Int, roles:String, entries:String, dependencies:String):String
	return '{"schemaVersion":' + schemaVersion + ',"packageId":"' + id + '","packageVersion":1,"roles":' + roles + ',"entries":' + entries
		+ ',"dependencies":' + dependencies + '}';

/** Build one-entry JSON Array. */
function oneEntry(kind:String, path:String, byteLength:Int, sha256:String):String
	return '[' + entry(kind, path, byteLength, sha256) + ']';

/** Build one package entry using only test-controlled canonical strings. */
function entry(kind:String, path:String, byteLength:Int, sha256:String):String
	return '{"kind":"' + kind + '","path":"' + path + '","byteLength":' + byteLength + ',"sha256":"' + sha256 + '"}';

/** Build one dependency interval using only test-controlled canonical IDs. */
function dependency(id:String, minimumVersion:Int, maximumVersion:Int):String
	return '{"packageId":"' + id + '","minimumVersion":' + minimumVersion + ',"maximumVersion":' + maximumVersion + '}';

/** Build 17 sorted maximum-size entries, one more than the 256 MiB package budget. */
function oversizedInventory():String {
	var result = "[";
	for (index in 0...17) {
		if (index > 0)
			result += ",";
		final digits = index < 10 ? "0" + index : "" + index;
		result += entry("asset", "assets/" + digits + ".bin", ContentPackageStore.MAXIMUM_PACKAGE_BYTES, ZERO_HASH);
	}
	return result + "]";
}

/** Compare role enums without relying on target stringification. */
function sameRole(left:ContentPackageRole, right:ContentPackageRole):Bool {
	return switch [left, right] {
		case [Assets, Assets] | [Campaign, Campaign] | [Content, Content] | [Levels, Levels] | [Localization, Localization] | [Mod, Mod]: true;
		case _: false;
	};
}

/** Map each admitted entry kind to one stable bit in the tracer. */
function kindBit(kind:ContentPackageEntryKind):Int {
	return switch kind {
		case Asset: 1;
		case AssetManifest: 2;
		case CampaignManifest: 4;
		case ContentPack: 8;
		case Level: 16;
		case LocalizationCatalog: 32;
		case RuntimeContent: 64;
	};
}

/** Reviewed wrong digest used only to prove stale-receipt rejection. */
inline final ZERO_HASH = "0000000000000000000000000000000000000000000000000000000000000000";
