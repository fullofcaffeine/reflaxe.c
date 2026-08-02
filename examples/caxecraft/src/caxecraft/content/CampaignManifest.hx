package caxecraft.content;

import caxecraft.content.ContentJson.ContentJsonNode;
import caxecraft.content.ContentPackagePath.ContentPackagePathResult;
import caxecraft.content.RuntimeSchema.RuntimeSchemaDiagnostic;
import caxecraft.content.RuntimeSchema.RuntimeSchemaErrorKind;
import caxecraft.content.RuntimeSchema.RuntimeSchemaReader;
import haxe.io.Bytes;

/**
 * Defines and validates the small runtime index that turns independent maps
 * into a campaign.
 *
 * A campaign does not embed maps or generated gameplay data. It names exact
 * package-relative CAXEMAP files and the exits that connect them. The decoder
 * admits the whole closed document before a level can be selected, so unknown
 * fields, unsafe paths, stale file receipts, unresolved edges, and required
 * transition cycles fail before gameplay state is involved.
 */
/** Stable public identity of one campaign. */
abstract CampaignId(String) {
	private inline function new(value:String)
		this = value;

	/** Return the validated namespaced spelling used by saves and diagnostics. */
	public inline function text():String
		return this;

	/** Construct only after the decoder validates the canonical ID grammar. */
	@:allow(caxecraft.content.CampaignManifestDecoder)
	static inline function admitted(value:String):CampaignId
		return new CampaignId(value);
}

/** Stable identity of one independently loadable map within a campaign. */
abstract CampaignLevelId(String) {
	private inline function new(value:String)
		this = value;

	/** Return the validated manifest-local spelling. */
	public inline function text():String
		return this;

	/** Construct only after the decoder validates the closed key grammar. */
	@:allow(caxecraft.content.CampaignManifestDecoder)
	static inline function admitted(value:String):CampaignLevelId
		return new CampaignLevelId(value);
}

/** Stable identity of one authored level exit. */
abstract CampaignExitId(String) {
	private inline function new(value:String)
		this = value;

	/** Return the validated manifest-local spelling. */
	public inline function text():String
		return this;

	/** Construct only after the decoder validates the closed key grammar. */
	@:allow(caxecraft.content.CampaignManifestDecoder)
	static inline function admitted(value:String):CampaignExitId
		return new CampaignExitId(value);
}

/** Stable identity of a spawn/entrance contract in a destination map. */
abstract CampaignEntranceId(String) {
	private inline function new(value:String)
		this = value;

	/** Return the validated manifest-local spelling. */
	public inline function text():String
		return this;

	/** Construct only after the decoder validates the closed key grammar. */
	@:allow(caxecraft.content.CampaignManifestDecoder)
	static inline function admitted(value:String):CampaignEntranceId
		return new CampaignEntranceId(value);
}

/** One exact map source admitted by the campaign manifest. */
final class CampaignLevel {
	/** Manifest-local level identity. */
	public final id:CampaignLevelId;

	/** Canonical package-relative path to independently valid CAXEMAP bytes. */
	public final logicalPath:String;

	/** Exact reviewed source length used to reject stale handoffs. */
	public final byteLength:Int;

	/** Lowercase SHA-256 expectation authored independently of the runtime. */
	public final sha256:String;

	/** Source coordinate retained for deterministic graph diagnostics. */
	public final sourceLine:Int;

	/** Source coordinate retained for deterministic graph diagnostics. */
	public final sourceColumn:Int;

	/** Retain one fully validated immutable level record. */
	@:allow(caxecraft.content.CampaignManifestDecoder)
	private function new(id:CampaignLevelId, logicalPath:String, byteLength:Int, sha256:String, line:Int, column:Int) {
		this.id = id;
		this.logicalPath = logicalPath;
		this.byteLength = byteLength;
		this.sha256 = sha256;
		sourceLine = line;
		sourceColumn = column;
	}
}

/** One directed exit from a source level to a named destination entrance. */
final class CampaignTransition {
	/** Exit identity raised by the source level or application boundary. */
	public final exit:CampaignExitId;

	/** Level in which the exit is meaningful. */
	public final sourceLevel:CampaignLevelId;

	/** Independently loadable level selected after the exit. */
	public final destinationLevel:CampaignLevelId;

	/** Destination spawn contract; the first slice admits `default`. */
	public final destinationEntrance:CampaignEntranceId;

	/** Whether campaign completion requires this directed edge. */
	public final required:Bool;

	/** Source coordinate retained for deterministic edge diagnostics. */
	public final sourceLine:Int;

	/** Source coordinate retained for deterministic edge diagnostics. */
	public final sourceColumn:Int;

	/** Retain one fully resolved immutable transition record. */
	@:allow(caxecraft.content.CampaignManifestDecoder)
	private function new(exit:CampaignExitId, sourceLevel:CampaignLevelId, destinationLevel:CampaignLevelId, destinationEntrance:CampaignEntranceId,
			required:Bool, line:Int, column:Int) {
		this.exit = exit;
		this.sourceLevel = sourceLevel;
		this.destinationLevel = destinationLevel;
		this.destinationEntrance = destinationEntrance;
		this.required = required;
		sourceLine = line;
		sourceColumn = column;
	}
}

/** Complete immutable campaign index safe for runtime level selection. */
final class CampaignManifest {
	/** Stable campaign identity. */
	public final id:CampaignId;

	/** Monotonic authored campaign-content version. */
	public final version:Int;

	final entryId:CampaignLevelId;
	final entryIndexValue:Int;
	final admittedLevels:Array<CampaignLevel>;
	final admittedTransitions:Array<CampaignTransition>;

	/** Construct only after all schema and graph checks pass. */
	@:allow(caxecraft.content.CampaignManifestDecoder)
	private function new(id:CampaignId, version:Int, entry:CampaignLevelId, entryIndex:Int, levels:Array<CampaignLevel>,
			transitions:Array<CampaignTransition>) {
		this.id = id;
		this.version = version;
		entryId = entry;
		entryIndexValue = entryIndex;
		admittedLevels = levels.copy();
		admittedTransitions = transitions.copy();
	}

	/** Identity of the map that starts a new campaign. */
	public inline function entryLevelId():CampaignLevelId
		return entryId;

	/** Number of independently loadable maps in this campaign. */
	public inline function levelCount():Int
		return admittedLevels.length;

	/** Number of typed directed edges in this campaign. */
	public inline function transitionCount():Int
		return admittedTransitions.length;

	/** Find one validated level without exposing the manifest's owned Array. */
	public function level(id:CampaignLevelId):Null<CampaignLevel> {
		for (candidate in admittedLevels)
			if (candidate.id.text() == id.text())
				return candidate;
		return null;
	}

	/** Return the already-proved entry record. */
	public inline function entryLevel():CampaignLevel
		return admittedLevels[entryIndexValue];

	/** Find one exit without exposing the manifest's owned transition Array. */
	public function transition(exit:CampaignExitId):Null<CampaignTransition> {
		for (candidate in admittedTransitions)
			if (candidate.exit.text() == exit.text())
				return candidate;
		return null;
	}

	/**
	 * Return the sole authored way forward from one level, or no automatic choice.
	 *
	 * The first playable campaign is linear, so its application shell can request
	 * “continue” without knowing a content-specific exit name. Zero matches means
	 * the level is terminal; two or more means a later branching UI must make the
	 * choice explicitly. Manifest order never silently chooses between branches.
	 */
	public function unambiguousTransitionFrom(source:CampaignLevelId):Null<CampaignTransition> {
		var found:Null<CampaignTransition> = null;
		for (candidate in admittedTransitions)
			if (candidate.sourceLevel.text() == source.text()) {
				if (found != null)
					return null;
				found = candidate;
			}
		return found;
	}

	/** Read one already-bounds-checked level for deterministic tests and UI lists. */
	public inline function levelAt(index:Int):CampaignLevel
		return admittedLevels[index];

	/** Read one already-bounds-checked transition for deterministic iteration. */
	public inline function transitionAt(index:Int):CampaignTransition
		return admittedTransitions[index];
}

/** A complete typed manifest, or the first located closed rejection. */
enum CampaignManifestReadResult {
	/** Every level, path, receipt, reference, and required edge was admitted. */
	CampaignManifestReady(manifest:CampaignManifest);

	/** No partially decoded campaign escaped the schema boundary. */
	CampaignManifestRejected(diagnostic:RuntimeSchemaDiagnostic);
}

/** Decode one bounded UTF-8 campaign document into the closed product model. */
function decodeCampaignManifest(input:Bytes):CampaignManifestReadResult
	return CampaignManifestDecoder.decode(input);

/** Request-local decoder that owns all intermediate source locations. */
private final class CampaignManifestDecoder {
	/** Maximum maps admitted by the first runtime campaign format. */
	static inline final MAXIMUM_LEVELS:Int = 64;

	/** Maximum directed exits admitted by the first runtime campaign format. */
	static inline final MAXIMUM_TRANSITIONS:Int = 256;

	/** Parse, validate, and resolve one complete campaign graph. */
	public static function decode(input:Bytes):CampaignManifestReadResult {
		final reader = new RuntimeSchemaReader();
		final root = reader.parse(input);
		if (root == null)
			return rejected(reader);
		final fields = reader.object(root, "campaign", [
			"schemaVersion",
			"campaignId",
			"campaignVersion",
			"entryLevel",
			"levels",
			"transitions"
		]);
		if (fields == null)
			return rejected(reader);

		final schemaNode = reader.field(fields, "schemaVersion");
		final schemaVersion = reader.integer(schemaNode, "schemaVersion", 1, 1);
		if (schemaVersion == null)
			return rejected(reader);
		final campaignNode = reader.field(fields, "campaignId");
		final campaignText = readContentId(reader, campaignNode, "campaignId");
		final campaignVersion = reader.integer(reader.field(fields, "campaignVersion"), "campaignVersion", 1, 2147483647);
		final entryNode = reader.field(fields, "entryLevel");
		final entryText = readKey(reader, entryNode, "entryLevel");
		if (campaignText == null || campaignVersion == null || entryText == null)
			return rejected(reader);

		final levels = readLevels(reader, reader.field(fields, "levels"));
		if (levels == null)
			return rejected(reader);
		final entryIndex = levelIndex(levels, entryText);
		if (entryIndex < 0) {
			reader.reject(entryNode, SchemaUnresolvedReference("entryLevel", entryText, "campaign level"));
			return rejected(reader);
		}

		final transitions = readTransitions(reader, reader.field(fields, "transitions"), levels);
		if (transitions == null)
			return rejected(reader);
		if (!validateRequiredAcyclic(reader, levels, transitions))
			return rejected(reader);

		return CampaignManifestReady(new CampaignManifest(CampaignId.admitted(campaignText), campaignVersion, CampaignLevelId.admitted(entryText), entryIndex,
			levels, transitions));
	}

	/** Parse a canonical namespaced campaign identity. */
	static function readContentId(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = reader.string(node, path, 128);
		if (value == null)
			return null;
		if (!RuntimeSchemaReader.validContentId(value)) {
			reader.reject(node, SchemaInvalidContentId(path));
			return null;
		}
		return value;
	}

	/** Parse a bounded lower-case manifest-local identity. */
	static function readKey(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = reader.string(node, path, 128);
		if (value == null)
			return null;
		if (!RuntimeSchemaReader.validMessageId(value)) {
			reader.reject(node, SchemaInvalidString(path));
			return null;
		}
		return value;
	}

	/** Parse one independently loadable map source and its reviewed receipt. */
	static function readLevels(reader:RuntimeSchemaReader, node:ContentJsonNode):Null<Array<CampaignLevel>> {
		final values = reader.array(node, "levels", 1, MAXIMUM_LEVELS);
		if (values == null)
			return null;
		final levels:Array<CampaignLevel> = [];
		for (index in 0...values.length) {
			final path = "levels[" + index + "]";
			final fields = reader.object(values[index], path, ["id", "path", "byteLength", "sha256"]);
			if (fields == null)
				return null;
			final idNode = reader.field(fields, "id");
			final id = readKey(reader, idNode, path + ".id");
			final pathNode = reader.field(fields, "path");
			final logicalPath = readMapPath(reader, pathNode, path + ".path");
			final byteLength = reader.integer(reader.field(fields, "byteLength"), path + ".byteLength", 1, ContentPackageStore.MAXIMUM_PACKAGE_BYTES);
			final sha256 = readSha256(reader, reader.field(fields, "sha256"), path + ".sha256");
			if (id == null || logicalPath == null || byteLength == null || sha256 == null)
				return null;
			for (previous in levels) {
				if (previous.id.text() == id) {
					reader.reject(idNode, SchemaDuplicateId("levels", id));
					return null;
				}
				if (previous.logicalPath == logicalPath) {
					reader.reject(pathNode, SchemaDuplicateLogicalPath(logicalPath));
					return null;
				}
			}
			if (levels.length > 0 && RuntimeSchemaReader.compareUtf8(levels[levels.length - 1].id.text(), id) > 0) {
				reader.reject(idNode, SchemaNonCanonicalOrder("levels"));
				return null;
			}
			levels.push(new CampaignLevel(CampaignLevelId.admitted(id), logicalPath, byteLength, sha256, idNode.line, idNode.column));
		}
		return levels;
	}

	/** Parse one portable package path that specifically names CAXEMAP bytes. */
	static function readMapPath(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = reader.string(node, path, 512);
		if (value == null)
			return null;
		var accepted = switch ContentPackagePath.parse(value) {
			case PathAccepted(_): true;
			case PathRejected(_): false;
		};
		if (accepted)
			accepted = value.indexOf("scenarios/") == 0 && value.length > 18 && value.substring(value.length - 8) == ".caxemap";
		if (accepted) {
			for (index in 0...value.length) {
				final code = value.charCodeAt(index);
				if (!((code >= 0x61 && code <= 0x7a) || (code >= 0x30 && code <= 0x39) || code == 0x2d || code == 0x2e || code == 0x2f))
					accepted = false;
			}
		}
		if (!accepted) {
			reader.reject(node, SchemaInvalidLogicalPath(path));
			return null;
		}
		return value;
	}

	/** Parse one exact lowercase SHA-256 expectation. */
	static function readSha256(reader:RuntimeSchemaReader, node:ContentJsonNode, path:String):Null<String> {
		final value = reader.string(node, path, 64);
		if (value == null)
			return null;
		var valid = value.length == 64;
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (!((code >= 0x30 && code <= 0x39) || (code >= 0x61 && code <= 0x66)))
				valid = false;
		}
		if (!valid) {
			reader.reject(node, SchemaInvalidString(path));
			return null;
		}
		return value;
	}

	/** Parse and resolve every exit against the already complete level table. */
	static function readTransitions(reader:RuntimeSchemaReader, node:ContentJsonNode, levels:Array<CampaignLevel>):Null<Array<CampaignTransition>> {
		final values = reader.array(node, "transitions", 1, MAXIMUM_TRANSITIONS);
		if (values == null)
			return null;
		final transitions:Array<CampaignTransition> = [];
		for (index in 0...values.length) {
			final path = "transitions[" + index + "]";
			final fields = reader.object(values[index], path, ["exit", "sourceLevel", "destinationLevel", "destinationEntrance", "required"]);
			if (fields == null)
				return null;
			final exitNode = reader.field(fields, "exit");
			final exit = readKey(reader, exitNode, path + ".exit");
			final sourceNode = reader.field(fields, "sourceLevel");
			final source = readKey(reader, sourceNode, path + ".sourceLevel");
			final destinationNode = reader.field(fields, "destinationLevel");
			final destination = readKey(reader, destinationNode, path + ".destinationLevel");
			final entranceNode = reader.field(fields, "destinationEntrance");
			final entrance = readKey(reader, entranceNode, path + ".destinationEntrance");
			final required = reader.boolean(reader.field(fields, "required"), path + ".required");
			if (exit == null || source == null || destination == null || entrance == null || required == null)
				return null;
			if (entrance != "default") {
				reader.reject(entranceNode, SchemaInvalidClosedValue(path + ".destinationEntrance", entrance));
				return null;
			}
			if (levelIndex(levels, source) < 0) {
				reader.reject(sourceNode, SchemaUnresolvedReference(path + ".sourceLevel", source, "campaign level"));
				return null;
			}
			if (levelIndex(levels, destination) < 0) {
				reader.reject(destinationNode, SchemaUnresolvedReference(path + ".destinationLevel", destination, "campaign level"));
				return null;
			}
			for (previous in transitions)
				if (previous.exit.text() == exit) {
					reader.reject(exitNode, SchemaDuplicateId("transitions", exit));
					return null;
				}
			if (transitions.length > 0 && RuntimeSchemaReader.compareUtf8(transitions[transitions.length - 1].exit.text(), exit) > 0) {
				reader.reject(exitNode, SchemaNonCanonicalOrder("transitions"));
				return null;
			}
			transitions.push(new CampaignTransition(CampaignExitId.admitted(exit), CampaignLevelId.admitted(source), CampaignLevelId.admitted(destination),
				CampaignEntranceId.admitted(entrance), required, exitNode.line, exitNode.column));
		}
		return transitions;
	}

	/** Reject required-edge cycles with one deterministic level coordinate. */
	static function validateRequiredAcyclic(reader:RuntimeSchemaReader, levels:Array<CampaignLevel>, transitions:Array<CampaignTransition>):Bool {
		final incoming:Array<Int> = [];
		for (_ in levels)
			incoming.push(0);
		for (transition in transitions)
			if (transition.required)
				incoming[levelIndex(levels, transition.destinationLevel.text())]++;
		final ready:Array<Int> = [];
		for (index in 0...incoming.length)
			if (incoming[index] == 0)
				ready.push(index);
		var cursor = 0;
		var visited = 0;
		while (cursor < ready.length) {
			final sourceIndex = ready[cursor++];
			visited++;
			for (transition in transitions) {
				if (!transition.required || transition.sourceLevel.text() != levels[sourceIndex].id.text())
					continue;
				final destinationIndex = levelIndex(levels, transition.destinationLevel.text());
				incoming[destinationIndex]--;
				if (incoming[destinationIndex] == 0)
					ready.push(destinationIndex);
			}
		}
		if (visited == levels.length)
			return true;
		for (index in 0...levels.length)
			if (incoming[index] > 0) {
				reader.rejectAt(levels[index].sourceLine, levels[index].sourceColumn, SchemaRequiredTransitionCycle(levels[index].id.text()));
				return false;
			}
		return false;
	}

	/** Find one level by its validated manifest-local identity. */
	static function levelIndex(levels:Array<CampaignLevel>, id:String):Int {
		for (index in 0...levels.length)
			if (levels[index].id.text() == id)
				return index;
		return -1;
	}

	/** Preserve a deterministic fallback if a future decoder path forgets to reject. */
	static function rejected(reader:RuntimeSchemaReader):CampaignManifestReadResult {
		final failure = reader.failure;
		return failure == null ? CampaignManifestRejected({line: 1, column: 1, kind: SchemaInvalidInvariant("campaign")}) : CampaignManifestRejected(failure);
	}
}
