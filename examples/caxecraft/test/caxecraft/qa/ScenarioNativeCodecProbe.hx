package caxecraft.qa;

import caxecraft.content.ContentPackageStore;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioLimits;
import caxecraft.scenario.ScenarioMessages.resolveScenarioMessage;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;
import haxe.io.Bytes;

/**
	Proves that the production CAXEMAP reader can run on Eval and generated C.

	The probe reads one checked-in synthetic map through the same bounded content
	store used by the game. The reloadable `.caxemap` file stays the sole source
	of fixture facts; no compiled Haxe copy can drift from it.
**/
var observed:Int = 0;

/** Exact authored-byte count recorded by the successful semantic trace. */
var traceBytes:Int = 0;

/** Validated world width recorded by the successful semantic trace. */
var traceWidth:Int = 0;

/** Validated world height recorded by the successful semantic trace. */
var traceHeight:Int = 0;

/** Validated world depth recorded by the successful semantic trace. */
var traceDepth:Int = 0;

/** Validated palette-entry count recorded by the successful semantic trace. */
var tracePalette:Int = 0;

/** Validated fluid-placement count recorded by the successful semantic trace. */
var traceFluids:Int = 0;

/** Validated object count recorded by the successful semantic trace. */
var traceObjects:Int = 0;

/** Validated dialogue count recorded by the successful semantic trace. */
var traceDialogues:Int = 0;

/** Validated objective count recorded by the successful semantic trace. */
var traceObjectives:Int = 0;

/** Load the canonical synthetic map through the bounded package store. */
function fixtureBytes():Bytes {
	final store = switch ContentPackageStore.open(".", "caxecraft-scenario-native-codec", ScenarioLimits.MAX_FILE_BYTES) {
		case PackageStoreOpened(value): value;
		case PackageStoreRejected(_): return Bytes.alloc(0);
	};
	return switch store.read("test/fixtures/caxemap/runtime-presentation.caxemap") {
		case PackageBytesRead(value):
			final result = Bytes.alloc(value.bytes.length);
			result.blit(0, value.bytes, 0, value.bytes.length);
			result;
		case PackageBytesRejected(_): Bytes.alloc(0);
	};
}

/**
	Run the same semantic checks on both hosts.

	Eval prints the result for the shared runner. Generated C stores it so the
	independent native harness can run the generated lifecycle and read one
	scalar without selecting console I/O.
**/
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(traceBytes);
	Sys.println(traceWidth);
	Sys.println(traceHeight);
	Sys.println(traceDepth);
	Sys.println(tracePalette);
	Sys.println(traceFluids);
	Sys.println(traceObjects);
	Sys.println(traceDialogues);
	Sys.println(traceObjectives);
	#end
}

/**
	Return zero when parsing, validation, and the bounded negative corpus agree.

	Each nonzero value names one stable stage of the probe rather than exposing a
	host-specific exception or diagnostic string.
**/
function selfCheck():Int {
	final source = fixtureBytes();
	if (source.length == 0)
		return 1;
	final scenario = readValid(source);
	if (scenario == null)
		return 2;
	if (scenario.formatVersion != 1
		|| scenario.id.text() != "qa.runtime-presentation"
		|| scenario.assetPack.text() != "packs/caxecraft/base"
		|| scenario.world.size.width != 32
		|| scenario.world.size.height != 16
		|| scenario.world.size.depth != 32
		|| scenario.world.palette.length != 2
		|| scenario.world.chunks.length != 1
		|| scenario.world.fluids.length != 1
		|| scenario.objects.length != 8
		|| scenario.story.dialogues.length != 1
		|| scenario.story.objectives.length != 5)
		return 3;
	if (resolveScenarioMessage(scenario.messages, new LocaleId("es-mx"), new MessageId("speaker.guide")) != "Guia de prueba")
		return 4;
	traceBytes = source.length;
	traceWidth = scenario.world.size.width;
	traceHeight = scenario.world.size.height;
	traceDepth = scenario.world.size.depth;
	tracePalette = scenario.world.palette.length;
	traceFluids = scenario.world.fluids.length;
	traceObjects = scenario.objects.length;
	traceDialogues = scenario.story.dialogues.length;
	traceObjectives = scenario.story.objectives.length;

	final malformed = Bytes.alloc(1);
	malformed.set(0, 0x80);
	switch ScenarioLexer.read(malformed) {
		case ReadError([{kind: MalformedUtf8(0)}]):
		case _:
			return 5;
	}
	switch ScenarioLexer.read(Bytes.ofString("CAXEMAP 2\n")) {
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError([{kind: UnknownVersion(2)}]):
				case _:
					return 6;
			}
		case _:
			return 6;
	}
	switch ScenarioLexer.read(Bytes.ofString("CAXEMAP 1\n")) {
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError([{kind: MissingRecord(EndMapRecord)}]):
				case _:
					return 7;
			}
		case _:
			return 7;
	}
	switch firstDiagnosticKind(replaceFirst(source, "feature required caxecraft:core\n",
		"feature required caxecraft:core\nfeature required caxecraft:core\n")) {
		case DuplicateContentId(id) if (id.text() == "caxecraft:core"):
		case _:
			return 8;
	}
	switch firstDiagnosticKind(replaceFirst(source, "dialogue.guide 2500", "dialogue.missing 2500")) {
		case UnresolvedReference(id) if (id.text() == "dialogue.missing"):
		case _:
			return 9;
	}
	switch firstDiagnosticKind(replaceFirst(source, "placement entity caxecraft:mossling", "placement entity caxecraft:nia")) {
		case UnresolvedContent(id) if (id.text() == "caxecraft:nia"):
		case _:
			return 10;
	}
	return 0;
}

/** Parse and validate one byte vector without letting a partial model escape. */
function readValid(source:Bytes):Null<Scenario> {
	return switch ScenarioLexer.read(source) {
		case ReadError(_):
			null;
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError(_):
					null;
				case ReadOk(parsed):
					switch ScenarioValidator.validate(parsed, new NativeProbeContentRegistry()) {
						case ReadError(_): null;
						case ReadOk(scenario): scenario;
					}
			}
	}
}

/** Return the first codec diagnostic from the exact production pipeline. */
function firstDiagnosticKind(source:Bytes):Null<ScenarioDiagnosticKind> {
	return switch ScenarioLexer.read(source) {
		case ReadError(diagnostics):
			diagnostics[0].kind;
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError(diagnostics):
					diagnostics[0].kind;
				case ReadOk(parsed):
					switch ScenarioValidator.validate(parsed, new NativeProbeContentRegistry()) {
						case ReadError(diagnostics): diagnostics[0].kind;
						case ReadOk(_): null;
					}
			}
	}
}

/**
	Replace one exact byte sequence without introducing a second text parser.

	The mutation helper exists only to build negative fixtures from the same
	checked-in map. Returning an empty vector when the needle is absent makes the
	probe fail closed through its expected diagnostic checks.
**/
function replaceFirst(source:Bytes, needle:String, replacement:String):Bytes {
	final needleBytes = Bytes.ofString(needle);
	final replacementBytes = Bytes.ofString(replacement);
	if (needleBytes.length == 0 || needleBytes.length > source.length)
		return Bytes.alloc(0);
	for (start in 0...(source.length - needleBytes.length + 1)) {
		var matches = true;
		for (offset in 0...needleBytes.length)
			if (source.get(start + offset) != needleBytes.get(offset))
				matches = false;
		if (matches) {
			final result = Bytes.alloc(source.length - needleBytes.length + replacementBytes.length);
			result.blit(0, source, 0, start);
			result.blit(start, replacementBytes, 0, replacementBytes.length);
			final suffixStart = start + needleBytes.length;
			result.blit(start + replacementBytes.length, source, suffixStart, source.length - suffixStart);
			return result;
		}
	}
	return Bytes.alloc(0);
}

/**
	Supplies only the content identities needed to validate the synthetic map.

	This test double is intentionally private and contains no gameplay behavior.
	The later runtime-loader vertical resolves the same authored IDs through the
	real validated content pack.
**/
private final class NativeProbeContentRegistry implements ScenarioContentRegistry {
	public function new() {}

	public function supportsFeature(id:ContentId):Bool
		return id.text() == "caxecraft:core";

	public function isAirBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air";

	public function hasBlock(id:ContentId):Bool
		return switch id.text() {
			case "caxecraft:air" | "caxecraft:ash" | "caxecraft:bedrock" | "caxecraft:dirt" | "caxecraft:grass" | "caxecraft:leaves" | "caxecraft:sand" |
				"caxecraft:snow" | "caxecraft:stone" | "caxecraft:wood": true;
			case _: false;
		}

	public function blockStorageCode(id:ContentId):Int
		return hasBlock(id) ? 0 : -1;

	public function blockContentIdForStorageCode(code:Int):Null<ContentId>
		return code == 0 ? new ContentId("caxecraft:air") : null;

	public function hasFluid(id:ContentId):Bool
		return id.text() == "caxecraft:water";

	public function hasItem(id:ContentId):Bool
		return id.text() == "caxecraft:bread" || id.text() == "caxecraft:tideweave-suit";

	public function itemStorageCode(id:ContentId):Int
		return hasItem(id) ? 0 : -1;

	public function hasEntity(id:ContentId):Bool
		return id.text() == "caxecraft:mossling";

	public function hasNpc(id:ContentId):Bool
		return id.text() == "caxecraft:nia";

	public function hasPrefab(id:ContentId):Bool
		return false;

	public function hasStatefulObject(id:ContentId):Bool
		return id.text() == "caxecraft:glyph-control";

	public function hasState(id:ContentId):Bool
		return id.text() == "caxecraft:active" || id.text() == "caxecraft:idle";

	public function statefulObjectHasState(objectType:ContentId, state:ContentId):Bool
		return hasStatefulObject(objectType) && hasState(state);

	public function hasEffect(id:ContentId):Bool
		return false;

	public function hasSignal(id:ContentId):Bool
		return false;

	public function maximumItemQuantity(id:ContentId):Int
		return hasItem(id) ? 64 : 0;
}
