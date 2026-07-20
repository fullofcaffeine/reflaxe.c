package caxecraft.qa;

import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioLimits;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioValidator;
import caxecraft.scenario.ScenarioWriter;
import haxe.io.Bytes;
import sys.io.File;

/** Executable Eval evidence for the staged codec and its fail-closed boundary. */
final class ScenarioCodecProbe {
	static function main():Void {
		final canonical = File.getBytes("scenarios/minimal.caxemap");
		final scenario = readValid(canonical);
		final written = ScenarioWriter.write(scenario);
		require(written.compare(canonical) == 0, "canonical fixture did not round-trip byte-identically");
		require(ScenarioWriter.write(scenario).compare(written) == 0, "repeated writes changed bytes");
		final crlfScenario = readValid(Bytes.ofString(StringTools.replace(canonical.toString(), "\n", "\r\n")));
		require(ScenarioWriter.write(crlfScenario).compare(canonical) == 0, "CRLF input did not converge on canonical LF bytes");
		require(ScenarioWriter.write(reverseUnordered(scenario))
			.compare(canonical) == 0, "reversed unordered collections did not converge on canonical bytes");
		final fullCanonical = File.getBytes("scenarios/codec-full.caxemap");
		final fullScenario = readValid(fullCanonical);
		require(ScenarioWriter.write(fullScenario).compare(fullCanonical) == 0, "full grammar fixture did not round-trip byte-identically");

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
		checkBoundedFailures(canonical);

		Sys.println('scenario-codec: ${written.length} + ${fullCanonical.length} bytes, staged round-trip and 6 fail-closed families');
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
		final diagnostic = switch ScenarioLexer.read(bytes) {
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
		require(diagnostic != null, '$label unexpectedly produced a plausible Scenario');
		require(kindName(diagnostic.kind) == expectedKind, '$label produced ${kindName(diagnostic.kind)} instead of $expectedKind');
		require(diagnostic.coordinate.line == expectedLine, '$label points to line ${diagnostic.coordinate.line} instead of $expectedLine');
	}

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
			case DuplicateId(_): "duplicate-id";
			case DuplicateTag(_): "duplicate-tag";
			case UnresolvedReference(_): "unresolved-reference";
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

	static function reverseUnordered(source:Scenario):Scenario {
		return {
			formatVersion: source.formatVersion,
			requiredFeatures: reversed(source.requiredFeatures),
			optionalFeatures: reversed(source.optionalFeatures),
			id: source.id,
			assetPack: source.assetPack,
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
		return id.text() == "caxecraft:ivvy";

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
