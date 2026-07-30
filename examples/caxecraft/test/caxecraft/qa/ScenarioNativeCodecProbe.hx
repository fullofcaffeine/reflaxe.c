package caxecraft.qa;

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
import caxecraft.scenario.ScenarioMessages.resolveScenarioMessage;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;
import haxe.io.Bytes;

/**
	Proves that the production CAXEMAP reader can run on Eval and generated C.

	The checked-in first-playable text is repeated here as an in-memory fixture so
	the first compiler probe measures only `Bytes`, parsing, and validation. A
	separate staleness check keeps these bytes equal to the authored map. Runtime
	file access remains owned by `haxe_c-xge.20.4.3.4`; this module does not claim
	that the game can load a path yet.
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

/** Assemble the exact fixture from C-portable literal chunks. */
function firstPlayableBytes():Bytes {
	final first = Bytes.ofString(firstPlayablePrefix());
	final second = Bytes.ofString(firstPlayableSuffix());
	final result = Bytes.alloc(first.length + second.length);
	result.blit(0, first, 0, first.length);
	result.blit(first.length, second, 0, second.length);
	return result;
}

/** Return the first 3,225 bytes, below C11's portable string-literal limit. */
function firstPlayablePrefix():String
	return "CAXEMAP 1\nfeature required caxecraft:core\nmap adventure.first-playable\nasset-pack packs/caxecraft/base\ndefault-locale en\nlocale en\n  message adventure_progress \"ADVENTURE: PROLOGUE IN PROGRESS\"\n  message adventure_tagline \"SAVE CEESH. FIND IVVY. STOP BROWSER.\"\n  message berry_pickup_one \"+1 BERRY\"\n  message berry_pickup_two \"+2 BERRIES\"\n  message berry_recovery \"BERRIES: +1 HEART\"\n  message berry_stack_full \"BERRIES FULL: USE ONE FIRST\"\n  message block_stack_full \"BLOCK STACK FULL: USE ONE FIRST\"\n  message copper_strike \"COPPER STRIKE\"\n  message haxirio_fallen \"HAXIRIO HAS FALLEN\"\n  message mossling_alert \"MOSSLING ALERT\"\n  message mossling_dropped_berries \"MOSSLING DROPPED BERRIES\"\n  message mossling_windup \"MOSSLING WINDUP: DODGE\"\n  message nia_gift \"NIA: BERRIES FOR THE ROAD, HAXIRIO\"\n  message nia_talk \"E  TALK TO NIA\"\n  message nia_welcome \"NIA: THE GROVE LISTENS. E: A SMALL GIFT\"\n  message no_berries \"NO BERRIES LEFT\"\n  message return_to_meadow \"E  RETURN TO THE MEADOW\"\n  message scenario_title \"Caxecraft: Evergrove Prologue\"\n  message telegraphed_hit \"TELEGRAPHED HIT: DODGE THE NEXT\"\nend locale\nlocale es-mx\n  message adventure_progress \"AVENTURA: PROLOGO EN CONSTRUCCION\"\n  message adventure_tagline \"SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.\"\n  message berry_pickup_one \"+1 BAYA\"\n  message berry_pickup_two \"+2 BAYAS\"\n  message berry_recovery \"BAYAS: +1 CORAZON\"\n  message berry_stack_full \"BAYAS LLENAS: USA UNA PRIMERO\"\n  message block_stack_full \"PILA DE BLOQUES LLENA: USA UNO\"\n  message copper_strike \"GOLPE DE COBRE\"\n  message haxirio_fallen \"HAXIRIO HA CAIDO\"\n  message mossling_alert \"MUSGUITO ALERTA\"\n  message mossling_dropped_berries \"EL MUSGUITO SOLTO BAYAS\"\n  message mossling_windup \"MUSGUITO CARGANDO: ESQUIVA\"\n  message nia_gift \"NIA: BAYAS PARA EL CAMINO, HAXIRIO\"\n  message nia_talk \"E  HABLAR CON NIA\"\n  message nia_welcome \"NIA: EL BOSQUE TE ESCUCHA. E: REGALO\"\n  message no_berries \"NO QUEDAN BAYAS\"\n  message return_to_meadow \"E  VOLVER AL PRADO\"\n  message scenario_title \"Caxecraft: Prologo de la Arboleda Eterna\"\n  message telegraphed_hit \"ATAQUE AVISADO: ESQUIVA EL PROXIMO\"\nend locale\ntitle message scenario_title\nmode adventure\nworld 32 16 32\npalette 0 caxecraft:air\npalette 1 caxecraft:bedrock\npalette 2 caxecraft:dirt\npalette 3 caxecraft:grass\npalette 4 caxecraft:sand\npalette 5 caxecraft:wood\npalette 6 caxecraft:leaves\npalette 7 caxecraft:snow\npalette 8 caxecraft:ash\nchunk world.base 0 0 0 32 16 32\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 1\n  run 4 6\n  run 3 25\n  run 0 352\n  run 1 32\n  run 2 96\n  run 4 2\n  run 0 4\n  run 4 2\n  run 3 24\n  run 0 352\n  run 1 32\n  run 2 96\n  run 4 2\n  run 0 4\n  run 4 2\n  run 3 24\n  run 0 352\n  run 1 32\n  run 2 96\n  run 4 2\n  run 0 4\n  run 4 2\n  run 3 24\n  run 0 352\n  run 1 32\n  run 2 96\n  run 4 2\n  run 0 4\n  run 4 2\n  run 3 24\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 1\n  run 4 6\n  run 3 25\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 89\n  run 6 3\n  run 0 260\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 26\n  run 5 1\n  run 0 31\n  run 5 1\n  run 0 30\n  run 6 3\n  run 0 30\n  run 6 1\n  run 0 229\n";

/** Return the remaining 1,873 bytes of the exact fixture. */
function firstPlayableSuffix():String
	return "  run 1 32\n  run 2 96\n  run 3 32\n  run 0 89\n  run 6 3\n  run 0 260\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 3 32\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\n  run 1 32\n  run 2 96\n  run 7 10\n  run 3 12\n  run 8 10\n  run 0 352\nend chunk\nfluid water.pool caxecraft:water volume 2 4 2 4 1 4\nfluid water.spring caxecraft:water source 3 4 3\nobject enemy.mossling\n  tag enemy\n  placement entity caxecraft:mossling 15500 5000 13800 0\nend object\nobject guide.nia\n  tag friend\n  placement npc caxecraft:nia dialogue.nia.welcome 17500 5000 13500 270\nend object\nobject item.tideweave\n  tag quest\n  placement item caxecraft:tideweave-suit 1 4500 5000 4500 0\nend object\nobject player.start\n  tag player\n  placement player-spawn 16500 5000 16500 0\nend object\ndialogue dialogue.nia.welcome\n  line speaker guide.nia message nia_welcome\nend dialogue\nobjective objective.prologue active title message adventure_progress\n  body message adventure_tagline\nend objective\nend-map\n";

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
	final source = firstPlayableBytes();
	if (source.length != 5098)
		return 1;
	final scenario = readValid(source);
	if (scenario == null)
		return 2;
	if (scenario.formatVersion != 1
		|| scenario.id.text() != "adventure.first-playable"
		|| scenario.assetPack.text() != "packs/caxecraft/base"
		|| scenario.world.size.width != 32
		|| scenario.world.size.height != 16
		|| scenario.world.size.depth != 32
		|| scenario.world.palette.length != 9
		|| scenario.world.chunks.length != 1
		|| scenario.world.fluids.length != 2
		|| scenario.objects.length != 4
		|| scenario.story.dialogues.length != 1
		|| scenario.story.objectives.length != 1)
		return 3;
	if (resolveScenarioMessage(scenario.messages, new LocaleId("es-mx"), new MessageId("nia_welcome")) != "NIA: EL BOSQUE TE ESCUCHA. E: REGALO")
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
	switch firstDiagnosticKind(replaceFirst(source, "dialogue.nia.welcome 17500", "dialogue.missing 17500")) {
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
	Supplies only the content identities needed to validate the embedded map.

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

	public function hasFluid(id:ContentId):Bool
		return id.text() == "caxecraft:water";

	public function hasItem(id:ContentId):Bool
		return id.text() == "caxecraft:tideweave-suit";

	public function itemStorageCode(id:ContentId):Int
		return hasItem(id) ? 0 : -1;

	public function hasEntity(id:ContentId):Bool
		return id.text() == "caxecraft:mossling";

	public function hasNpc(id:ContentId):Bool
		return id.text() == "caxecraft:nia";

	public function hasPrefab(id:ContentId):Bool
		return false;

	public function hasStatefulObject(id:ContentId):Bool
		return false;

	public function hasState(id:ContentId):Bool
		return false;

	public function hasEffect(id:ContentId):Bool
		return false;

	public function hasSignal(id:ContentId):Bool
		return false;

	public function maximumItemQuantity(id:ContentId):Int
		return hasItem(id) ? 64 : 0;
}
