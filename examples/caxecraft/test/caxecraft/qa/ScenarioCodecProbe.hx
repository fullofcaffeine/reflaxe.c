package caxecraft.qa;

import caxecraft.scenario.ContentId;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioLimits;
import caxecraft.scenario.ScenarioMessages;
import caxecraft.scenario.ScenarioMessages.resolveScenarioMessage;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioValidator;
import caxecraft.scenario.ScenarioWriter;
import haxe.io.Bytes;
import sys.io.File;

/** Executable Eval evidence for the staged codec and its fail-closed boundary. */
final class ScenarioCodecProbe {
	static function main():Void {
		final canonical = File.getBytes("test/fixtures/caxemap/minimal.caxemap");
		final scenario = readValid(canonical);
		final written = ScenarioWriter.write(scenario);
		require(written.compare(canonical) == 0, "canonical fixture did not round-trip byte-identically");
		require(ScenarioWriter.write(scenario).compare(written) == 0, "repeated writes changed bytes");
		final crlfScenario = readValid(Bytes.ofString(StringTools.replace(canonical.toString(), "\n", "\r\n")));
		require(ScenarioWriter.write(crlfScenario).compare(canonical) == 0, "CRLF input did not converge on canonical LF bytes");
		require(ScenarioWriter.write(reverseUnordered(scenario))
			.compare(canonical) == 0, "reversed unordered collections did not converge on canonical bytes");
		final fullCanonical = File.getBytes("test/fixtures/caxemap/codec-full.caxemap");
		final fullScenario = readValid(fullCanonical);
		require(ScenarioWriter.write(fullScenario).compare(fullCanonical) == 0, "full grammar fixture did not round-trip byte-identically");
		final localizedCanonical = File.getBytes("test/fixtures/caxemap/localized.caxemap");
		final localizedScenario = readValid(localizedCanonical);
		require(ScenarioWriter.write(localizedScenario).compare(localizedCanonical) == 0, "localized fixture did not round-trip byte-identically");
		require(ScenarioWriter.write(reverseUnordered(localizedScenario)).compare(localizedCanonical) == 0,
			"reversed locale and message catalogs did not converge on canonical bytes");
		checkLocalizedCatalog(localizedCanonical, localizedScenario);
		final shippedCanonical = File.getBytes("scenarios/first-playable/map.caxemap");
		final shippedScenario = readValid(shippedCanonical);
		require(ScenarioWriter.write(shippedScenario).compare(shippedCanonical) == 0, "shipped first-playable CaxeMap did not round-trip byte-identically");
		require(resolveScenarioMessage(shippedScenario.messages, new LocaleId("es-mx"), new MessageId("nia_welcome")) == "NIA: EL BOSQUE TE ESCUCHA. E: REGALO",
			"shipped Nia dialogue did not come from the embedded Spanish catalog");

		final malformed = Bytes.alloc(10);
		final malformedValues = [0x43, 0x41, 0x58, 0x45, 0x4d, 0x41, 0x50, 0x20, 0xc0, 0x0a];
		for (index in 0...malformedValues.length)
			malformed.set(index, malformedValues[index]);
		expectFailure(malformed, "malformed-utf8", "malformed-utf8", 1);
		expectFailure(Bytes.ofString('CAXEMAP 1\ntitle literal "bad\\q"\n'), "invalid-escape", "invalid-escape", 2);
		expectFailure(replace(canonical, "world 4 4 4", "world 2147483648 4 4"), "integer-range", "integer-out-of-range", 7);
		expectFailure(replace(canonical, "run 0 12", "run 0 11"), "run-total", "invalid-run-total", 10);
		expectFailure(replace(canonical, "feature required caxecraft:core", "feature required unknown:core"), "required-feature", "unknown-required-feature",
			2);
		expectFailure(replace(canonical, "dialogue.ivvy.hello 2500", "dialogue.missing 2500"), "unresolved-reference", "unresolved-reference", 24);
		checkTokenKinds(canonical, fullCanonical);
		checkIntegerSpellings(fullCanonical);
		checkMissingRecords(canonical);
		checkMissingBlockRecords(canonical, fullCanonical);
		checkDuplicateSingletons(canonical);
		checkDuplicateCoordinates(canonical, fullCanonical);
		checkSemanticLimitsAndContent(canonical);
		checkDiagnosticPayloads(canonical, fullCanonical);
		checkRoleSpecificReferences(canonical, fullCanonical);
		checkNarratorIdentity(canonical);
		checkFlowVariableScopes(fullCanonical);
		checkFlowBoundsAndGrammar(canonical, fullCanonical);
		checkBoundedFailures(canonical);

		Sys.println('scenario-codec: ${written.length} + ${fullCanonical.length} + ${shippedCanonical.length} bytes, staged round-trip and exact malformed-input audit');
	}

	static function checkTokenKinds(canonical:Bytes, fullCanonical:Bytes):Void {
		expectFailureAt(replace(canonical, "mode creative", '"mode" creative'), "quoted-record-name", "unexpected-record", 6, 1, 6);
		expectFailureAt(replace(canonical, "mode creative", 'mode "creative"'), "quoted-mode", "invalid-token", 6, 6, 6);
		expectFailureAt(replace(canonical, "(flag quest.met-ivvy false)", '(flag quest.met-ivvy "false")'), "quoted-boolean", "invalid-token", 45, 3, 45);
		expectFailureAt(replace(fullCanonical, "objective objective.bridge hidden", 'objective objective.bridge "hidden"'), "quoted-objective-state",
			"invalid-token", 60, 1, 60);
		expectFailureAt(replace(fullCanonical, "greater-or-equal 1)", '"greater-or-equal" 1)'), "quoted-comparison", "invalid-token", 103, 3, 103);
		expectFailureAt(replace(fullCanonical, "if (always)", 'if "(" always ")"'), "quoted-predicate-delimiters", "invalid-token", 121, 3, 121);
	}

	static function checkNarratorIdentity(canonical:Bytes):Void {
		final legacy = replace(canonical, "line speaker guide.ivvy", "line guide.ivvy");
		require(ScenarioWriter.write(readValid(legacy)).compare(canonical) == 0, "legacy speaker spelling did not converge on canonical bytes");
		final speakerCanonical = ScenarioWriter.write(readValid(replace(canonical, "guide.ivvy", "speaker")));
		final speakerLegacy = replace(speakerCanonical, "line speaker speaker", "line speaker");
		require(ScenarioWriter.write(readValid(speakerLegacy)).compare(speakerCanonical) == 0,
			"a legacy object named speaker did not converge on explicit canonical bytes");
		final source = replace(canonical, "guide.ivvy", "narrator");
		final scenario = readValid(source);
		final speaker = scenario.story.dialogues[0].lines[0].speaker;
		require(speaker != null && speaker.text() == "narrator", "an object named narrator became narration");
		require(ScenarioWriter.write(scenario).compare(source) == 0, "the explicit narrator speaker did not round-trip byte-identically");
	}

	static function checkIntegerSpellings(fullCanonical:Bytes):Void {
		expectCanonicalMutation(fullCanonical, "variable player.score player counter 0", "variable player.score player counter 2147483647");
		expectCanonicalMutation(fullCanonical, "variable player.score player counter 0", "variable player.score player counter -2147483648");
		expectFailureAt(replace(fullCanonical, "variable player.score player counter 0", "variable player.score player counter +1"), "leading-plus",
			"invalid-token", 71, 1, 71);
		expectFailureAt(replace(fullCanonical, "variable player.score player counter 0", "variable player.score player counter 01"), "leading-zero",
			"invalid-token", 71, 1, 71);
		expectFailureAt(replace(fullCanonical, "variable player.score player counter 0", "variable player.score player counter -0"), "negative-zero",
			"invalid-token", 71, 1, 71);
	}

	static function expectCanonicalMutation(source:Bytes, needle:String, replacement:String):Void {
		final mutated = replace(source, needle, replacement);
		final scenario = readValid(mutated);
		require(ScenarioWriter.write(scenario).compare(mutated) == 0, 'canonical mutation `$replacement` did not round-trip');
	}

	static function checkMissingRecords(canonical:Bytes):Void {
		expectExactFailureAt(Bytes.alloc(0), "missing-header", 1, 1, 0, "MissingRecord(FormatHeader)", kind -> switch kind {
			case MissingRecord(FormatHeader): true;
			case _: false;
		});
		for (missing in [
			{label: "map", line: "map tutorial.first\n", expected: MapRecord},
			{label: "asset-pack", line: "asset-pack packs/caxecraft/base\n", expected: AssetPackRecord},
			{label: "title", line: "title literal \"First map tutorial\"\n", expected: TitleRecord},
			{label: "mode", line: "mode creative\n", expected: ModeRecord},
			{label: "world", line: "world 4 4 4\n", expected: WorldRecord}
		])
			expectExactFailureAt(replace(canonical, missing.line, ""), 'missing-${missing.label}', 1, 1, 1, 'MissingRecord(${missing.label})',
				kind -> switch kind {
					case MissingRecord(value): expectedRecordEquals(value, missing.expected);
					case _: false;
				});
		expectExactFailureAt(replace(canonical, "end-map\n", ""), "missing-end-map", 49, 1, 49, "MissingRecord(EndMapRecord)", kind -> switch kind {
			case MissingRecord(EndMapRecord): true;
			case _: false;
		});
		expectExactFailureAt(replace(canonical, "feature required caxecraft:core\n", ""), "missing-core-feature", 1, 1, 1, "MissingRecord(CoreFeatureRecord)",
			kind -> switch kind {
				case MissingRecord(CoreFeatureRecord): true;
				case _: false;
			});
		expectExactFailureAt(replace(canonical, "palette 0 caxecraft:air", "palette 0 caxecraft:bedrock"), "missing-air-palette", 7, 1, 7,
			"MissingRecord(AirPaletteRecord)", kind -> switch kind {
				case MissingRecord(AirPaletteRecord): true;
				case _: false;
			});
		final chunk = "chunk world.base 0 0 0 4 4 4\n  run 1 4\n  run 0 12\n  run 1 4\n  run 0 12\n  run 1 4\n  run 0 12\n  run 1 4\n  run 0 12\nend chunk\n";
		expectExactFailureAt(replace(canonical, chunk, ""), "missing-chunk-coverage", 7, 1, 7, "MissingRecord(CompleteChunkCoverage)", kind -> switch kind {
			case MissingRecord(CompleteChunkCoverage): true;
			case _: false;
		});
		final player = "object player.start\n  tag player\n  placement player-spawn 1500 1000 1500 0\nend object\n";
		expectExactFailureAt(replace(canonical, player, ""), "missing-player-spawn", 7, 1, 7, "MissingRecord(SinglePlayerSpawn)", kind -> switch kind {
			case MissingRecord(SinglePlayerSpawn): true;
			case _: false;
		});
	}

	static function checkLocalizedCatalog(canonical:Bytes, scenario:Scenario):Void {
		final spanish = resolveScenarioMessage(scenario.messages, new LocaleId("es-mx"), new MessageId("dialogue.ivvy.hello"));
		require(spanish == "¿Lista para construir?", "exact Spanish message lookup returned the wrong text");
		final fallback = resolveScenarioMessage(scenario.messages, new LocaleId("fr"), new MessageId("dialogue.ivvy.hello"));
		require(fallback == "Ready to build?", "unknown locale did not use the declared default catalog");
		require(resolveScenarioMessage(scenario.messages, new LocaleId("en"), new MessageId("dialogue.missing")) == null,
			"unknown message unexpectedly resolved");

		expectFailure(replace(canonical, "default-locale en", "default-locale fr"), "unknown-default-locale", "unknown-default-locale", 5);
		expectFailure(replace(canonical, "locale es-mx", "locale en"), "duplicate-locale", "duplicate-locale", 12);
		expectFailure(replace(canonical, '  message scenario.tutorial.title "First map tutorial"',
			'  message scenario.tutorial.title "First map tutorial"\n  message scenario.tutorial.title "Duplicate"'),
			"duplicate-message", "duplicate-message", 11);
		expectFailure(replace(canonical, '  message objective.meet-ivvy.body "Camina hacia Ivvy cerca del prado."\n', ""), "missing-translation",
			"missing-translation", 12);
		expectFailure(replace(canonical, '  message scenario.tutorial.title "Tutorial del primer mapa"',
			'  message scenario.tutorial.title "Tutorial del primer mapa"\n  message scenario.extra "Extra"'),
			"unknown-translation", "unknown-translation", 17);
		expectFailure(replace(canonical, "title message scenario.tutorial.title", "title message scenario.missing"), "unresolved-message",
			"unresolved-message", 18);
		expectFailure(replace(canonical, "default-locale en\n", ""), "missing-default-locale", "missing-record", 1);
		expectFailure(truncateAfterLast(canonical, '  message scenario.tutorial.title "Tutorial del primer mapa"\n'), "missing-end-locale", "missing-record",
			12);
	}

	static function checkMissingBlockRecords(canonical:Bytes, fullCanonical:Bytes):Void {
		expectMissing(truncateAfterLast(canonical, "  run 0 12\n"), "missing-end-chunk", EndChunkRecord, 10, 1, 10);
		expectMissing(truncateAfterLast(canonical, "  placement checkpoint 1500 1000 1500 0\n"), "missing-end-object", EndObjectRecord, 20, 1, 20);
		expectMissing(replace(canonical, "  placement checkpoint 1500 1000 1500 0\n", ""), "missing-object-placement", ObjectPlacementRecord, 20, 1, 20);
		expectMissing(truncateAfterLast(canonical, "  line speaker guide.ivvy literal \"Ready to build?\"\n"), "missing-end-dialogue", EndDialogueRecord, 36,
			1, 36);
		expectMissing(replace(fullCanonical, "  body literal \"Lower the bridge.\"\n", ""), "missing-journal-body", JournalBodyRecord, 57, 1, 57);
		expectMissing(truncateAfterFirst(fullCanonical, "  body literal \"Lower the bridge.\"\n"), "missing-end-journal", EndJournalRecord, 57, 1, 57);
		expectMissing(replace(canonical, "  body literal \"Walk to Ivvy near the meadow.\"\n", ""), "missing-objective-body", ObjectiveBodyRecord, 39, 1, 39);
		expectMissing(truncateAfterLast(canonical, "  body literal \"Walk to Ivvy near the meadow.\"\n"), "missing-end-objective", EndObjectiveRecord, 39, 1,
			39);
		expectMissing(truncateAfterLast(fullCanonical, "  objective objective.bridge\n"), "missing-end-route", EndRouteRecord, 66, 1, 66);
		expectMissing(truncateAfterLast(fullCanonical, "  do set-flag map.ready true\n"), "missing-end-sequence", EndSequenceRecord, 74, 1, 74);
		expectMissing(truncateAfterLast(canonical, "  do objective objective.meet-ivvy complete\n"), "missing-end-rule", EndRuleRecord, 43, 1, 43);
		expectMissing(truncateAfterLast(fullCanonical, "  do choose player.score 2\n"), "missing-choice", ChoiceRecord, 86, 3, 86);
		expectMissing(truncateAfterLast(fullCanonical, "    do set-object-state state.bridge caxecraft:open\n"), "missing-end-choice", EndChoiceRecord, 87, 3,
			87);
		expectMissing(replace(fullCanonical, "  data \"snow=gentle\"\n", ""), "missing-extension-data", ExtensionDataRecord, 140, 1, 140);
		expectMissing(truncateAfterLast(fullCanonical, "  data \"snow=gentle\"\n"), "missing-end-extension", EndExtensionRecord, 140, 1, 140);
	}

	static function checkDuplicateCoordinates(canonical:Bytes, fullCanonical:Bytes):Void {
		expectExactFailureAt(replace(canonical, "feature required caxecraft:core\n", "feature required caxecraft:core\nfeature optional caxecraft:core\n"),
			"duplicate-feature", 3, 1, 3, "DuplicateContentId(caxecraft:core)", kind -> switch kind {
				case DuplicateContentId(id): id.text() == "caxecraft:core";
				case _: false;
			});
		expectExactFailureAt(replace(canonical, "palette 1 caxecraft:bedrock", "palette 0 caxecraft:bedrock"), "duplicate-palette-code", 9, 1, 9,
			"DuplicatePaletteCode(0)", kind -> switch kind {
				case DuplicatePaletteCode(0): true;
				case _: false;
			});
		final chunk = "chunk world.base 0 0 0 4 4 4\n  run 1 4\n  run 0 12\n  run 1 4\n  run 0 12\n  run 1 4\n  run 0 12\n  run 1 4\n  run 0 12\nend chunk\n";
		expectDuplicateId(replace(canonical, chunk, chunk + chunk), "duplicate-chunk", "world.base", 20, 1, 20);
		expectDuplicateId(replace(canonical, "object guide.ivvy", "object checkpoint.start"), "duplicate-object", "checkpoint.start", 24, 1, 24);
		expectExactFailureAt(replace(canonical, "  tag friend\n", "  tag friend\n  tag friend\n"), "duplicate-tag", 26, 3, 26,
			"DuplicateTag(guide.ivvy, friend)", kind -> switch kind {
				case DuplicateTag(objectId, tag): objectId.text() == "guide.ivvy" && tag.text() == "friend";
				case _: false;
			});
		final dialogue = "dialogue dialogue.ivvy.hello\n  line speaker guide.ivvy literal \"Ready to build?\"\nend dialogue\n";
		expectDuplicateId(replace(canonical, dialogue, dialogue + dialogue), "duplicate-dialogue", "dialogue.ivvy.hello", 39, 1, 39);
		final objective = "objective objective.meet-ivvy active title literal \"Meet Ivvy\"\n  body literal \"Walk to Ivvy near the meadow.\"\nend objective\n";
		expectDuplicateId(replace(canonical, objective, objective + objective), "duplicate-objective", "objective.meet-ivvy", 42, 1, 42);
		final journal = "journal journal.bridge title literal \"Bridge notes\"\n  body literal \"Lower the bridge.\"\nend journal\n";
		expectDuplicateId(replace(fullCanonical, journal, journal + journal), "duplicate-journal", "journal.bridge", 60, 1, 60);
		final route = "route route.castle title literal \"Road to the castle\"\n  objective objective.ivvy\n  objective objective.bridge\nend route\n";
		expectDuplicateId(replace(fullCanonical, route, route + route), "duplicate-route", "route.castle", 70, 1, 70);
		expectDuplicateId(replace(canonical, "variable quest.met-ivvy quest flag false\n",
			"variable quest.met-ivvy quest flag false\nvariable quest.met-ivvy quest flag false\n"),
			"duplicate-variable", "quest.met-ivvy", 43, 1, 43);
		expectDuplicateId(replace(fullCanonical, "  parameter enabled flag true\n", "  parameter enabled flag true\n  parameter enabled flag false\n"),
			"duplicate-sequence-parameter", "enabled", 76, 3, 76);
		final sequence = "sequence sequence.helper\n  parameter enabled flag true\n  do set-flag map.ready true\nend sequence\n";
		expectDuplicateId(replace(fullCanonical, sequence, sequence + sequence), "duplicate-sequence", "sequence.helper", 78, 1, 78);
		final rule = "rule rule.meet-ivvy priority 100 once\n  when enter-zone zone.welcome\n  if (flag quest.met-ivvy false)\n  do dialogue dialogue.ivvy.hello\n  do set-flag quest.met-ivvy true\n  do objective objective.meet-ivvy complete\nend rule\n";
		expectDuplicateId(replace(canonical, rule, rule + rule), "duplicate-rule", "rule.meet-ivvy", 50, 1, 50);
		final extension = "extension caxecraft:weather weather.settings\n  data \"snow=gentle\"\nend extension\n";
		expectExactFailureAt(replace(fullCanonical, extension, extension + extension), "duplicate-extension", 143, 1, 143,
			"InvalidExtension(weather.settings)", kind -> switch kind {
				case InvalidExtension(id): id.text() == "weather.settings";
				case _: false;
			});
	}

	static function checkDuplicateSingletons(canonical:Bytes):Void {
		for (duplicate in [
			{label: "map", line: "map tutorial.first\n", expectedLine: 4},
			{label: "asset-pack", line: "asset-pack packs/caxecraft/base\n", expectedLine: 5},
			{label: "title", line: "title literal \"First map tutorial\"\n", expectedLine: 6},
			{label: "mode", line: "mode creative\n", expectedLine: 7},
			{label: "world", line: "world 4 4 4\n", expectedLine: 8}
		])
			expectFailureAt(replace(canonical, duplicate.line, duplicate.line + duplicate.line), 'duplicate-${duplicate.label}', "invalid-token",
				duplicate.expectedLine, 1, duplicate.expectedLine);
	}

	static function checkSemanticLimitsAndContent(canonical:Bytes):Void {
		expectLimit(replace(canonical, "world 4 4 4", "world 129 4 4"), "world-width", WorldWidth, ScenarioLimits.MAX_WORLD_WIDTH, 7, 1, 7);
		expectLimit(replace(canonical, "world 4 4 4", "world 4 65 4"), "world-height", WorldHeight, ScenarioLimits.MAX_WORLD_HEIGHT, 7, 1, 7);
		expectLimit(replace(canonical, "world 4 4 4", "world 4 4 129"), "world-depth", WorldDepth, ScenarioLimits.MAX_WORLD_DEPTH, 7, 1, 7);
		expectExactFailureAt(replace(canonical, "palette 1 caxecraft:bedrock", "palette 1 missing:block"), "unresolved-block-content", 9, 1, 9,
			"UnresolvedContent(missing:block)", kind -> switch kind {
				case UnresolvedContent(id): id.text() == "missing:block";
				case _: false;
			});
	}

	static function checkDiagnosticPayloads(canonical:Bytes, fullCanonical:Bytes):Void {
		expectExactFailureAt(replace(canonical, "CAXEMAP 1", "CAXEMAP 2"), "unknown-version-payload", 1, 9, 1, "UnknownVersion(2)", kind -> switch kind {
			case UnknownVersion(2): true;
			case _: false;
		});
		expectExactFailureAt(replace(canonical, "feature required caxecraft:core", "feature required missing:core"), "unknown-feature-payload", 2, 1, 2,
			"UnknownRequiredFeature(missing:core)", kind -> switch kind {
				case UnknownRequiredFeature(id): id.text() == "missing:core";
				case _: false;
			});
		expectExactFailureAt(replace(canonical, "world 4 4 4", "world 2147483648 4 4"), "integer-range-payload", 7, 1, 7, "IntegerOutOfRange",
			kind -> switch kind {
				case IntegerOutOfRange: true;
				case _: false;
			});
		expectExactFailureAt(replace(canonical, "run 0 12", "run 0 11"), "run-total-payload", 10, 1, 10, "InvalidRunTotal(world.base, 64, 60)",
			kind -> switch kind {
				case InvalidRunTotal(id, 64, 60): id.text() == "world.base";
				case _: false;
			});
		expectExactFailureAt(replace(canonical, "dialogue.ivvy.hello 2500", "dialogue.missing 2500"), "unresolved-reference-payload", 24, 1, 24,
			"UnresolvedReference(dialogue.missing)", kind -> switch kind {
				case UnresolvedReference(id): id.text() == "dialogue.missing";
				case _: false;
			});
		expectExactFailureAt(replace(canonical, "placement checkpoint 1500", "placement checkpoint 4500"), "impossible-placement-payload", 20, 1, 20,
			"ImpossiblePlacement(checkpoint.start)", kind -> switch kind {
				case ImpossiblePlacement(id): id.text() == "checkpoint.start";
				case _: false;
			});
		expectExactFailureAt(replace(fullCanonical, "extension caxecraft:weather", "extension missing:weather"), "invalid-extension-payload", 140, 1, 140,
			"InvalidExtension(weather.settings)", kind -> switch kind {
				case InvalidExtension(id): id.text() == "weather.settings";
				case _: false;
			});
	}

	static function checkRoleSpecificReferences(canonical:Bytes, fullCanonical:Bytes):Void {
		expectInvalidRule(replace(canonical, "when enter-zone zone.welcome", "when enter-zone guide.ivvy"), "npc-used-as-zone", "rule.meet-ivvy", 44, 3, 44);
		expectInvalidRule(replace(fullCanonical, "when block-changed zone.test", "when block-changed state.bridge"), "stateful-used-as-zone", "rule.block",
			79, 3, 79);
		expectInvalidRule(replace(fullCanonical, "when entity-defeated entity.browser", "when entity-defeated npc.ivvy"), "npc-used-as-entity", "rule.defeat",
			96, 3, 96);
		expectInvalidRule(replace(fullCanonical, "do checkpoint checkpoint.start", "do checkpoint npc.ivvy"), "npc-used-as-checkpoint", "rule.leave", 111, 3,
			111);
	}

	static function checkFlowVariableScopes(fullCanonical:Bytes):Void {
		expectCanonicalMutation(fullCanonical, "do set-flag map.ready true", "do set-flag enabled false");
		expectCanonicalMutation(fullCanonical, "do set-flag map.ready true", "do set-flag local.choice false");
		expectDuplicateId(replace(fullCanonical, "parameter enabled flag true", "parameter map.ready flag true"), "parameter-variable-collision", "map.ready",
			75, 3, 75);
		expectInvalidRule(replace(fullCanonical, "do call sequence.helper value flag true", "do call sequence.helper variable local.choice"),
			"rule-cannot-read-sequence-local", "rule.signal", 122, 3, 122);
		expectInvalidRule(replace(fullCanonical, "when state-changed map.ready", "when state-changed local.choice"), "local-change-cannot-cross-ticks",
			"rule.state", 125, 3, 125);
	}

	static function checkFlowBoundsAndGrammar(canonical:Bytes, fullCanonical:Bytes):Void {
		final deepSequences = new StringBuf();
		for (index in 0...ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH + 1) {
			deepSequences.add('sequence depth.s$index\n');
			if (index < ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH)
				deepSequences.add('  do call depth.s${index + 1}\n');
			deepSequences.add("end sequence\n");
		}
		expectExactFailureAt(replace(canonical, "rule rule.meet-ivvy", deepSequences.toString() + "rule rule.meet-ivvy"), "sequence-call-depth", 43, 1, 43,
			"LimitExceeded(SequenceCallDepth, 16)", kind -> switch kind {
				case LimitExceeded(SequenceCallDepth, maximum): maximum == ScenarioLimits.MAX_SEQUENCE_CALL_DEPTH;
				case _: false;
			});

		final cycle = "sequence depth.a\n  do call depth.b\nend sequence\nsequence depth.b\n  do call depth.a\nend sequence\n";
		expectExactFailureAt(replace(canonical, "rule rule.meet-ivvy", cycle + "rule rule.meet-ivvy"), "sequence-cycle", 43, 1, 43, "RuleCycle(depth.a)",
			kind -> switch kind {
				case RuleCycle(id): id.text() == "depth.a";
				case _: false;
			});

		final choices = new StringBuf();
		choices.add("  do choose player.score 1\n");
		choices.add("  choice weight 1\n");
		for (_ in 0...ScenarioLimits.MAX_ACTIONS_PER_RULE)
			choices.add("    do add-counter player.score 1\n");
		choices.add("  end choice\n");
		final originalChoices = "  do choose player.score 2\n  choice weight 1\n    do set-state quest.bridge caxecraft:open\n    do set-object-state state.bridge caxecraft:open\n  end choice\n  choice weight 2\n    do effect caxecraft:spark at state.bridge\n  end choice\n";
		expectExactFailureAt(replace(fullCanonical, originalChoices, choices.toString()), "choice-action-budget", 83, 1, 83, "LimitExceeded(RuleActions, 64)",
			kind -> switch kind {
				case LimitExceeded(RuleActions, maximum): maximum == ScenarioLimits.MAX_ACTIONS_PER_RULE;
				case _: false;
			});
		expectFailureAt(replace(fullCanonical, "do call sequence.helper value flag true", "do call sequence.helper object player.one"),
			"object-sequence-argument", "invalid-token", 122, 3, 122);
		expectExactFailureAt(replace(fullCanonical, "    do set-state quest.bridge caxecraft:open", "    do choose player.score 1"), "nested-choice", 88, 5,
			88, "UnexpectedRecord(do)", kind -> switch kind {
				case UnexpectedRecord("do"): true;
				case _: false;
			});
	}

	static function checkBoundedFailures(canonical:Bytes):Void {
		final oversized = Bytes.alloc(ScenarioLimits.MAX_FILE_BYTES + 1);
		expectLexerFailure(oversized, "limit-exceeded");

		final tooManyRecords = new StringBuf();
		for (_ in 0...ScenarioLimits.MAX_RECORDS + 1)
			tooManyRecords.add("x\n");
		expectLexerFailure(Bytes.ofString(tooManyRecords.toString()), "limit-exceeded");

		final tooMuchText = new StringBuf();
		tooMuchText.add('CAXEMAP 1\ntitle literal "');
		for (_ in 0...ScenarioLimits.MAX_TEXT_SCALARS + 1)
			tooMuchText.add("a");
		tooMuchText.add('"\n');
		expectLexerFailure(Bytes.ofString(tooMuchText.toString()), "limit-exceeded");

		var nested = "(always)";
		for (_ in 0...ScenarioLimits.MAX_PREDICATE_DEPTH + 1)
			nested = '(not $nested)';
		expectFailure(replace(canonical, "(flag quest.met-ivvy false)", nested), "predicate-depth", "invalid-token", 45);

		final actions = new StringBuf();
		for (_ in 0...ScenarioLimits.MAX_ACTIONS_PER_RULE + 1)
			actions.add("  do set-flag quest.met-ivvy true\n");
		final overloadedRule = replace(canonical, "  do dialogue dialogue.ivvy.hello\n", actions.toString());
		expectFailure(overloadedRule, "rule-action-limit", "limit-exceeded", 43);

		final invalidUtf8:Array<Array<Int>> = [[0x80], [0xc2], [0xe0, 0x80, 0x80], [0xed, 0xa0, 0x80], [0xf4, 0x90, 0x80, 0x80]];
		for (values in invalidUtf8) {
			final bytes = Bytes.alloc(values.length);
			for (index in 0...values.length)
				bytes.set(index, values[index]);
			expectLexerFailure(bytes, "malformed-utf8");
		}
	}

	static function expectLexerFailure(bytes:Bytes, expectedKind:String):Void {
		switch ScenarioLexer.read(bytes) {
			case ReadError(diagnostics):
				require(kindName(diagnostics[0].kind) == expectedKind, 'lexer produced ${kindName(diagnostics[0].kind)} instead of $expectedKind');
			case ReadOk(_):
				fail('lexer unexpectedly accepted bounded negative input');
		}
	}

	static function readValid(bytes:Bytes):caxecraft.scenario.Scenario {
		return switch ScenarioLexer.read(bytes) {
			case ReadError(diagnostics): fail('lexer rejected canonical fixture: ${Std.string(diagnostics[0])}');
			case ReadOk(records):
				switch ScenarioParser.parse(records) {
					case ReadError(diagnostics): fail('parser rejected canonical fixture: ${Std.string(diagnostics[0])}');
					case ReadOk(parsed):
						switch ScenarioValidator.validate(parsed, new ProbeContentRegistry()) {
							case ReadError(diagnostics): fail('validator rejected canonical fixture: ${Std.string(diagnostics[0])}');
							case ReadOk(scenario): scenario;
						}
				}
		}
	}

	static function expectFailure(bytes:Bytes, label:String, expectedKind:String, expectedLine:Int):Void {
		expectFailureAt(bytes, label, expectedKind, expectedLine, -1, -1);
	}

	static function expectFailureAt(bytes:Bytes, label:String, expectedKind:String, expectedLine:Int, expectedColumn:Int, expectedRecord:Int):Void {
		final diagnostic = firstFailure(bytes);
		require(diagnostic != null, '$label unexpectedly produced a plausible Scenario');
		require(kindName(diagnostic.kind) == expectedKind, '$label produced ${kindName(diagnostic.kind)} instead of $expectedKind');
		checkCoordinate(diagnostic, label, expectedLine, expectedColumn, expectedRecord);
	}

	static function expectExactFailureAt(bytes:Bytes, label:String, expectedLine:Int, expectedColumn:Int, expectedRecord:Int, expectedDescription:String,
			matches:ScenarioDiagnosticKind->Bool):Void {
		final diagnostic = firstFailure(bytes);
		require(diagnostic != null, '$label unexpectedly produced a plausible Scenario');
		require(matches(diagnostic.kind), '$label produced ${Std.string(diagnostic.kind)} instead of $expectedDescription');
		checkCoordinate(diagnostic, label, expectedLine, expectedColumn, expectedRecord);
	}

	static function expectDuplicateId(bytes:Bytes, label:String, expectedId:String, line:Int, column:Int, record:Int):Void
		expectExactFailureAt(bytes, label, line, column, record, 'DuplicateId($expectedId)', kind -> switch kind {
			case DuplicateId(id): id.text() == expectedId;
			case _: false;
		});

	static function expectMissing(bytes:Bytes, label:String, expected:ScenarioExpectedRecord, line:Int, column:Int, record:Int):Void
		expectExactFailureAt(bytes, label, line, column, record, 'MissingRecord(${Std.string(expected)})', kind -> switch kind {
			case MissingRecord(value): value == expected;
			case _: false;
		});

	static function expectInvalidRule(bytes:Bytes, label:String, expectedId:String, line:Int, column:Int, record:Int):Void
		expectExactFailureAt(bytes, label, line, column, record, 'InvalidRule($expectedId)', kind -> switch kind {
			case InvalidRule(id): id.text() == expectedId;
			case _: false;
		});

	static function expectLimit(bytes:Bytes, label:String, expectedLimit:ScenarioLimitKind, expectedMaximum:Int, line:Int, column:Int, record:Int):Void
		expectExactFailureAt(bytes, label, line, column, record, 'LimitExceeded(${Std.string(expectedLimit)}, $expectedMaximum)', kind -> switch kind {
			case LimitExceeded(limit, maximum): limit == expectedLimit && maximum == expectedMaximum;
			case _: false;
		});

	static function firstFailure(bytes:Bytes):Null<ScenarioDiagnostic> {
		return switch ScenarioLexer.read(bytes) {
			case ReadError(diagnostics): diagnostics[0];
			case ReadOk(records):
				switch ScenarioParser.parse(records) {
					case ReadError(diagnostics): diagnostics[0];
					case ReadOk(parsed):
						switch ScenarioValidator.validate(parsed, new ProbeContentRegistry()) {
							case ReadError(diagnostics): diagnostics[0];
							case ReadOk(_): null;
						}
				}
		}
	}

	static function checkCoordinate(diagnostic:ScenarioDiagnostic, label:String, expectedLine:Int, expectedColumn:Int, expectedRecord:Int):Void {
		require(diagnostic.coordinate.line == expectedLine, '$label points to line ${diagnostic.coordinate.line} instead of $expectedLine');
		if (expectedColumn >= 0)
			require(diagnostic.coordinate.column == expectedColumn, '$label points to column ${diagnostic.coordinate.column} instead of $expectedColumn');
		if (expectedRecord >= 0)
			require(diagnostic.coordinate.record == expectedRecord, '$label points to record ${diagnostic.coordinate.record} instead of $expectedRecord');
	}

	static function expectedRecordEquals(left:ScenarioExpectedRecord, right:ScenarioExpectedRecord):Bool
		return left == right;

	static function kindName(value:ScenarioDiagnosticKind):String {
		return switch value {
			case MalformedUtf8(_): "malformed-utf8";
			case UnknownVersion(_): "unknown-version";
			case UnknownRequiredFeature(_): "unknown-required-feature";
			case InvalidToken: "invalid-token";
			case InvalidEscape: "invalid-escape";
			case UnexpectedRecord(_): "unexpected-record";
			case MissingRecord(_): "missing-record";
			case IntegerOutOfRange: "integer-out-of-range";
			case LimitExceeded(_, _): "limit-exceeded";
			case InvalidRunTotal(_, _, _): "invalid-run-total";
			case DuplicateContentId(_): "duplicate-content-id";
			case DuplicatePaletteCode(_): "duplicate-palette-code";
			case DuplicateId(_): "duplicate-id";
			case DuplicateTag(_, _): "duplicate-tag";
			case DuplicateLocale(_): "duplicate-locale";
			case DuplicateMessage(_, _): "duplicate-message";
			case UnknownDefaultLocale(_): "unknown-default-locale";
			case UnresolvedMessage(_): "unresolved-message";
			case MissingTranslation(_, _): "missing-translation";
			case UnknownTranslation(_, _): "unknown-translation";
			case UnresolvedReference(_): "unresolved-reference";
			case UnresolvedContent(_): "unresolved-content";
			case ImpossiblePlacement(_): "impossible-placement";
			case InvalidRule(_): "invalid-rule";
			case RuleCycle(_): "rule-cycle";
			case InvalidExtension(_): "invalid-extension";
			case EventBudgetExhausted(_): "event-budget-exhausted";
			case PersistenceFailed(_): "persistence-failed";
		}
	}

	static function replace(source:Bytes, needle:String, replacement:String):Bytes {
		final text = source.toString();
		final changed = StringTools.replace(text, needle, replacement);
		require(changed != text, 'negative fixture mutation `$needle` did not apply');
		return Bytes.ofString(changed);
	}

	static function truncateAfterLast(source:Bytes, needle:String):Bytes {
		final text = source.toString();
		final at = text.lastIndexOf(needle);
		require(at >= 0, 'truncation marker `$needle` was not found');
		return Bytes.ofString(text.substring(0, at + needle.length));
	}

	static function truncateAfterFirst(source:Bytes, needle:String):Bytes {
		final text = source.toString();
		final at = text.indexOf(needle);
		require(at >= 0, 'truncation marker `$needle` was not found');
		return Bytes.ofString(text.substring(0, at + needle.length));
	}

	static function reverseUnordered(source:Scenario):Scenario {
		final messages = switch source.messages {
			case NoMessageCatalog: NoMessageCatalog;
			case EmbeddedMessageCatalog(catalog):
				EmbeddedMessageCatalog({
					defaultLocale: catalog.defaultLocale,
					locales: [
						for (locale in reversed(catalog.locales))
							{
								id: locale.id,
								messages: reversed(locale.messages)
							}
					]
				});
		};
		return {
			formatVersion: source.formatVersion,
			requiredFeatures: reversed(source.requiredFeatures),
			optionalFeatures: reversed(source.optionalFeatures),
			id: source.id,
			assetPack: source.assetPack,
			messages: messages,
			title: source.title,
			mode: source.mode,
			world: {
				size: source.world.size,
				palette: reversed(source.world.palette),
				chunks: reversed(source.world.chunks)
			},
			objects: reversed(source.objects),
			story: {
				dialogues: reversed(source.story.dialogues),
				journal: reversed(source.story.journal),
				objectives: reversed(source.story.objectives),
				routes: reversed(source.story.routes)
			},
			flow: {
				variables: reversed(source.flow.variables),
				sequences: reversed(source.flow.sequences),
				rules: reversed(source.flow.rules)
			},
			extensions: reversed(source.extensions)
		};
	}

	static function reversed<T>(source:Array<T>):Array<T> {
		final result = source.copy();
		result.reverse();
		return result;
	}

	static function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}

	static function fail<T>(message:String):T {
		throw message;
	}
}

private final class ProbeContentRegistry implements ScenarioContentRegistry {
	public function new() {}

	public function supportsFeature(id:ContentId):Bool
		return id.text() == "caxecraft:core";

	public function isAirBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air";

	public function hasBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air" || id.text() == "caxecraft:bedrock";

	public function hasItem(id:ContentId):Bool
		return id.text() == "caxecraft:haxe-pick";

	public function hasEntity(id:ContentId):Bool
		return id.text() == "caxecraft:browser";

	public function hasNpc(id:ContentId):Bool
		return id.text() == "caxecraft:ivvy" || id.text() == "caxecraft:nia";

	public function hasPrefab(id:ContentId):Bool
		return id.text() == "caxecraft:house";

	public function hasStatefulObject(id:ContentId):Bool
		return id.text() == "caxecraft:bridge";

	public function hasState(id:ContentId):Bool
		return id.text() == "caxecraft:closed" || id.text() == "caxecraft:open";

	public function hasEffect(id:ContentId):Bool
		return id.text() == "caxecraft:spark";

	public function hasSignal(id:ContentId):Bool
		return id.text() == "caxecraft:bridge-lowered";

	public function maximumItemQuantity(id:ContentId):Int
		return hasItem(id) ? 64 : 0;
}
