package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioEnvironment.ScenarioCloudLayer;
import caxecraft.scenario.ScenarioEnvironment.ScenarioEnvironmentProfile;
import caxecraft.scenario.ScenarioEnvironment.ScenarioHorizonEdge;
import caxecraft.scenario.ScenarioEnvironment.ScenarioRgb;
import caxecraft.scenario.ScenarioEnvironment.ScenarioSun;
import caxecraft.scenario.ScenarioMessages.ScenarioLocaleCatalog;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioJournalEntry;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioStory.ScenarioRoute;
import caxecraft.scenario.ScenarioStory.ScenarioSpeakerName;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;
import caxecraft.scenario.ScenarioWorld.ScenarioFluid;
import caxecraft.scenario.ScenarioWorld.VoxelChunk;

/**
	Coordinates the top-level CAXEMAP grammar.

	Nested record families are delegated to readers named after the model they
	produce. This class only owns the document header, singleton fields, feature
	declarations, extensions, and final assembly.
**/
// Used only by the CAXEMAP parser. Game and editor code should call
// ScenarioParser.parse(...) instead of constructing this class.
@:noCompletion
final class ScenarioDocumentReader {
	final cursor:ScenarioRecordCursor;
	final worldReader:ScenarioWorldReader;
	final storyReader:ScenarioStoryReader;
	final flowReader:CaxeFlowReader;

	public function new(records:Array<ScenarioLexRecord>) {
		cursor = new ScenarioRecordCursor(records);
		worldReader = new ScenarioWorldReader(cursor);
		storyReader = new ScenarioStoryReader(cursor);
		flowReader = new CaxeFlowReader(cursor);
	}

	public function read():ScenarioReadResult<ParsedScenario> {
		if (!cursor.hasRecord())
			return cursor.failAt(null, MissingRecord(FormatHeader));
		final header = cursor.current();
		if (header.indent != 0 || header.tokens.length != 2 || ScenarioTokenGrammar.firstText(header) != "CAXEMAP")
			return cursor.failAt(header, UnexpectedRecord(ScenarioTokenGrammar.firstText(header)));
		final version = ScenarioTokenGrammar.integer(header.tokens[1]);
		if (version == null)
			return cursor.failToken(header.tokens[1], IntegerOutOfRange);
		if (version != 1)
			return cursor.failToken(header.tokens[1], UnknownVersion(version));
		cursor.locate(Header, header);
		cursor.advance();

		final requiredFeatures:Array<ContentId> = [];
		final optionalFeatures:Array<ContentId> = [];
		var mapId:Null<ScenarioId> = null;
		var assetPack:Null<LogicalPath> = null;
		var defaultLocale:Null<LocaleId> = null;
		final locales:Array<ScenarioLocaleCatalog> = [];
		var title:Null<ScenarioText> = null;
		var mode:Null<ScenarioMode> = null;
		var environment:Null<ScenarioEnvironment> = null;
		var worldSize:Null<VoxelSize> = null;
		final palette:Array<BlockPaletteEntry> = [];
		final chunks:Array<VoxelChunk> = [];
		final fluids:Array<ScenarioFluid> = [];
		final objects:Array<ScenarioObject> = [];
		final speakerNames:Array<ScenarioSpeakerName> = [];
		final dialogues:Array<ScenarioDialogue> = [];
		final journal:Array<ScenarioJournalEntry> = [];
		final objectives:Array<ScenarioObjective> = [];
		final routes:Array<ScenarioRoute> = [];
		final variables:Array<FlowVariable> = [];
		final sequences:Array<FlowSequence> = [];
		final rules:Array<FlowRule> = [];
		final extensions:Array<ScenarioExtension> = [];
		var sawEnd = false;

		while (cursor.hasRecord()) {
			final record = cursor.current();
			if (record.indent != 0)
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			switch ScenarioTokenGrammar.firstText(record) {
				case "feature":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 3))
						return cursor.failAt(record, InvalidToken);
					final feature = ScenarioTokenGrammar.contentId(record.tokens[2]);
					if (feature == null)
						return cursor.failToken(record.tokens[2], InvalidToken);
					switch ScenarioTokenGrammar.bareText(record.tokens[1]) {
						case "required": requiredFeatures.push(feature);
						case "optional": optionalFeatures.push(feature);
						case _: return cursor.failToken(record.tokens[1], InvalidToken);
					}
					cursor.locate(Feature(feature), record);
					cursor.advance();
				case "map":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 2) || mapId != null)
						return cursor.failAt(record, InvalidToken);
					mapId = ScenarioTokenGrammar.scenarioId(record.tokens[1]);
					if (mapId == null)
						return cursor.failToken(record.tokens[1], InvalidToken);
					cursor.locate(MapIdentity(mapId), record);
					cursor.advance();
				case "asset-pack":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 2) || assetPack != null)
						return cursor.failAt(record, InvalidToken);
					assetPack = ScenarioTokenGrammar.logicalPath(record.tokens[1]);
					if (assetPack == null)
						return cursor.failToken(record.tokens[1], InvalidToken);
					cursor.locate(AssetPack, record);
					cursor.advance();
				case "default-locale":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 2) || defaultLocale != null)
						return cursor.failAt(record, InvalidToken);
					defaultLocale = ScenarioTokenGrammar.localeId(record.tokens[1]);
					if (defaultLocale == null)
						return cursor.failToken(record.tokens[1], InvalidToken);
					cursor.locate(DefaultLocale, record);
					cursor.advance();
				case "locale":
					switch storyReader.readLocale() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(locale): locales.push(locale);
					}
				case "title":
					if (title != null)
						return cursor.failAt(record, InvalidToken);
					final parsed = ScenarioTokenGrammar.text(record, 1);
					if (parsed == null || parsed.next != record.tokens.length)
						return cursor.failAt(record, InvalidToken);
					title = parsed.value;
					cursor.locate(Title, record);
					switch parsed.value {
						case Message(id): cursor.locate(MessageReference(id), record);
						case Literal(_):
					}
					cursor.advance();
				case "mode":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 2) || mode != null)
						return cursor.failAt(record, InvalidToken);
					mode = switch ScenarioTokenGrammar.bareText(record.tokens[1]) {
						case "creative": Creative;
						case "adventure": Adventure;
						case _: null;
					}
					if (mode == null)
						return cursor.failToken(record.tokens[1], InvalidToken);
					cursor.locate(Mode, record);
					cursor.advance();
				case "environment":
					if (environment != null)
						return cursor.failAt(record, InvalidToken);
					switch readEnvironment() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(value): environment = value;
					}
				case "world":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 4) || worldSize != null)
						return cursor.failAt(record, InvalidToken);
					worldSize = ScenarioTokenGrammar.size(record, 1);
					if (worldSize == null)
						return cursor.failAt(record, IntegerOutOfRange);
					cursor.locate(World, record);
					cursor.advance();
				case "palette":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 3))
						return cursor.failAt(record, InvalidToken);
					final code = ScenarioTokenGrammar.integer(record.tokens[1]);
					final block = ScenarioTokenGrammar.contentId(record.tokens[2]);
					if (code == null || block == null)
						return cursor.failAt(record, InvalidToken);
					palette.push({code: code, blockType: block});
					cursor.locate(Palette(code), record);
					cursor.advance();
				case "chunk":
					switch worldReader.readChunk() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(chunk): chunks.push(chunk);
					}
				case "fluid":
					switch worldReader.readFluid() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(fluid): fluids.push(fluid);
					}
				case "object":
					switch worldReader.readObject() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(object): objects.push(object);
					}
				case "speaker":
					switch storyReader.readSpeakerName() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(speakerName): speakerNames.push(speakerName);
					}
				case "dialogue":
					switch storyReader.readDialogue() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(dialogue): dialogues.push(dialogue);
					}
				case "journal":
					switch storyReader.readJournal() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(entry): journal.push(entry);
					}
				case "objective":
					switch storyReader.readObjective() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(objective): objectives.push(objective);
					}
				case "route":
					switch storyReader.readRoute() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(route): routes.push(route);
					}
				case "variable":
					switch flowReader.readVariable() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(variable): variables.push(variable);
					}
				case "sequence":
					switch flowReader.readSequence() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(sequence): sequences.push(sequence);
					}
				case "rule":
					switch flowReader.readRule() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(rule): rules.push(rule);
					}
				case "extension":
					switch readExtension() {
						case ReadError(diagnostics): return ReadError(diagnostics);
						case ReadOk(extension): extensions.push(extension);
					}
				case "end-map":
					if (!ScenarioTokenGrammar.hasTokenCount(record, 1))
						return cursor.failAt(record, InvalidToken);
					sawEnd = true;
					cursor.advance();
					if (cursor.hasRecord())
						return cursor.failAt(cursor.current(), UnexpectedRecord(ScenarioTokenGrammar.firstText(cursor.current())));
				case other:
					return cursor.failAt(record, UnexpectedRecord(other));
			}
		}

		if (!sawEnd)
			return cursor.failAt(cursor.lastRecord(), MissingRecord(EndMapRecord));
		if (mapId == null)
			return cursor.failAt(header, MissingRecord(MapRecord));
		if (assetPack == null)
			return cursor.failAt(header, MissingRecord(AssetPackRecord));
		if (title == null)
			return cursor.failAt(header, MissingRecord(TitleRecord));
		if (mode == null)
			return cursor.failAt(header, MissingRecord(ModeRecord));
		if (worldSize == null)
			return cursor.failAt(header, MissingRecord(WorldRecord));
		if (locales.length != 0 && defaultLocale == null)
			return cursor.failAt(header, MissingRecord(DefaultLocaleRecord));

		return ReadOk({
			candidate: {
				formatVersion: 1,
				requiredFeatures: requiredFeatures,
				optionalFeatures: optionalFeatures,
				id: mapId,
				assetPack: assetPack,
				messages: defaultLocale == null ? NoMessageCatalog : EmbeddedMessageCatalog({
					defaultLocale: defaultLocale,
					locales: locales
				}),
				title: title,
				mode: mode,
				environment: environment,
				world: {
					size: worldSize,
					palette: palette,
					chunks: chunks,
					fluids: fluids
				},
				objects: objects,
				story: {
					speakerNames: speakerNames,
					dialogues: dialogues,
					journal: journal,
					objectives: objectives,
					routes: routes
				},
				flow: {variables: variables, sequences: sequences, rules: rules},
				extensions: extensions
			},
			recordCoordinates: cursor.recordCoordinates(),
			sourceLocations: cursor.sourceLocations()
		});
	}

	/** Read one optional, explicitly bounded visual environment block. */
	function readEnvironment():ScenarioReadResult<ScenarioEnvironment> {
		final header = cursor.current();
		if (!ScenarioTokenGrammar.hasTokenCount(header, 2) || ScenarioTokenGrammar.bareText(header.tokens[1]) != "voxel-horizon")
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Environment, header);
		cursor.advance();
		var sky:Null<ScenarioRgb> = null;
		var sun:Null<ScenarioSun> = null;
		var sunDeclared = false;
		var clouds:Null<ScenarioCloudLayer> = null;
		final edges:Array<ScenarioHorizonEdge> = [];
		var edgesDeclared = false;
		var continueWater:Null<Bool> = null;
		while (cursor.hasRecord() && !ScenarioTokenGrammar.isEnd(cursor.current(), "environment")) {
			final record = cursor.current();
			if (record.indent != 2)
				return cursor.failAt(record, UnexpectedRecord(ScenarioTokenGrammar.firstText(record)));
			switch ScenarioTokenGrammar.firstText(record) {
				case "sky":
					if (sky != null || !ScenarioTokenGrammar.hasTokenCount(record, 4))
						return cursor.failAt(record, InvalidToken);
					final red = ScenarioTokenGrammar.integer(record.tokens[1]);
					final green = ScenarioTokenGrammar.integer(record.tokens[2]);
					final blue = ScenarioTokenGrammar.integer(record.tokens[3]);
					if (red == null || green == null || blue == null || !isColor(red) || !isColor(green) || !isColor(blue))
						return cursor.failAt(record, IntegerOutOfRange);
					sky = {red: red, green: green, blue: blue};
				case "sun":
					if (sunDeclared)
						return cursor.failAt(record, InvalidToken);
					sunDeclared = true;
					if (ScenarioTokenGrammar.hasTokenCount(record, 2) && ScenarioTokenGrammar.bareText(record.tokens[1]) == "none")
						sun = null;
					else {
						if (!ScenarioTokenGrammar.hasTokenCount(record, 5))
							return cursor.failAt(record, InvalidToken);
						final x = ScenarioTokenGrammar.integer(record.tokens[1]);
						final y = ScenarioTokenGrammar.integer(record.tokens[2]);
						final z = ScenarioTokenGrammar.integer(record.tokens[3]);
						final radius = ScenarioTokenGrammar.integer(record.tokens[4]);
						if (x == null || y == null || z == null || radius == null || y <= 0 || radius < 250 || radius > 10000)
							return cursor.failAt(record, IntegerOutOfRange);
						sun = {
							x: x,
							y: y,
							z: z,
							radiusMilli: radius
						};
					}
				case "clouds":
					if (clouds != null || !ScenarioTokenGrammar.hasTokenCount(record, 4))
						return cursor.failAt(record, InvalidToken);
					final count = ScenarioTokenGrammar.integer(record.tokens[1]);
					final speed = ScenarioTokenGrammar.integer(record.tokens[2]);
					final seed = ScenarioTokenGrammar.integer(record.tokens[3]);
					if (count == null || speed == null || seed == null || count < 0 || count > 12 || speed < 0 || speed > 5000)
						return cursor.failAt(record, IntegerOutOfRange);
					clouds = {count: count, speedMilli: speed, seed: seed};
				case "edges":
					if (edgesDeclared || record.tokens.length < 2 || record.tokens.length > 5)
						return cursor.failAt(record, InvalidToken);
					edgesDeclared = true;
					if (record.tokens.length == 2 && ScenarioTokenGrammar.bareText(record.tokens[1]) == "none") {
						// An authored sky can deliberately omit every terrain horizon.
					} else
						for (index in 1...record.tokens.length) {
							final edge:Null<ScenarioHorizonEdge> = switch ScenarioTokenGrammar.bareText(record.tokens[index]) {
								case "north": North;
								case "south": South;
								case "east": East;
								case "west": West;
								case _: null;
							};
							if (edge == null || containsEdge(edges, edge))
								return cursor.failAt(record, InvalidToken);
							edges.push(edge);
						}
				case "continue-water":
					if (continueWater != null || !ScenarioTokenGrammar.hasTokenCount(record, 2))
						return cursor.failAt(record, InvalidToken);
					continueWater = switch ScenarioTokenGrammar.bareText(record.tokens[1]) {
						case "true": true;
						case "false": false;
						case _: null;
					};
					if (continueWater == null)
						return cursor.failAt(record, InvalidToken);
				case other:
					return cursor.failAt(record, UnexpectedRecord(other));
			}
			cursor.advance();
		}
		if (!cursor.hasRecord() || !ScenarioTokenGrammar.isEnd(cursor.current(), "environment"))
			return cursor.failAt(header, MissingRecord(EndEnvironmentRecord));
		cursor.advance();
		if (sky == null || !sunDeclared || clouds == null || !edgesDeclared || continueWater == null)
			return cursor.failAt(header, InvalidToken);
		return ReadOk({
			profile: ScenarioEnvironmentProfile.VoxelHorizon,
			sky: sky,
			sun: sun,
			clouds: clouds,
			edges: edges,
			continueWater: continueWater
		});
	}

	/** True when one parsed channel fits an eight-bit color component. */
	inline function isColor(value:Null<Int>):Bool
		return value != null && value >= 0 && value <= 255;

	/** Reject duplicate edge names while preserving authored order. */
	function containsEdge(edges:Array<ScenarioHorizonEdge>, candidate:ScenarioHorizonEdge):Bool {
		for (edge in edges)
			if (edge == candidate)
				return true;
		return false;
	}

	function readExtension():ScenarioReadResult<ScenarioExtension> {
		final header = cursor.current();
		if (!ScenarioTokenGrammar.hasTokenCount(header, 3))
			return cursor.failAt(header, InvalidToken);
		final feature = ScenarioTokenGrammar.contentId(header.tokens[1]);
		final id = ScenarioTokenGrammar.scenarioId(header.tokens[2]);
		if (feature == null || id == null)
			return cursor.failAt(header, InvalidToken);
		cursor.locate(Extension(feature, id), header);
		cursor.advance();
		if (!cursor.hasRecord()
			|| cursor.current().indent != 2
			|| !ScenarioTokenGrammar.hasTokenCount(cursor.current(), 2)
			|| ScenarioTokenGrammar.firstText(cursor.current()) != "data"
			|| !ScenarioTokenGrammar.isQuoted(cursor.current().tokens[1]))
			return cursor.failAt(header, MissingRecord(ExtensionDataRecord));
		final data = cursor.current().tokens[1].text;
		cursor.advance();
		if (!cursor.hasRecord() || !ScenarioTokenGrammar.isEnd(cursor.current(), "extension"))
			return cursor.failAt(header, MissingRecord(EndExtensionRecord));
		cursor.advance();
		return ReadOk({feature: feature, id: id, data: data});
	}
}
