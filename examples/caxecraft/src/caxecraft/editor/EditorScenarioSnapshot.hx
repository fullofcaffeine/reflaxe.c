package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioWriter;
import haxe.io.Bytes;

/**
	One deep, canonical in-memory copy of a draft.

	This helper crosses editor source files but is not a supported UI entry
	point. `@:noCompletion` keeps that implementation detail out of normal code
	completion; it does not weaken visibility or type checking.
**/
@:noCompletion
typedef EditorScenarioImage = {
	final bytes:Bytes;
	final parsed:ParsedScenario;
}

/** Internal success/failure result for the same snapshot boundary. */
@:noCompletion
enum EditorScenarioImageResult {
	ImageReady(image:EditorScenarioImage);
	ImageRejected(error:EditorError);
}

/**
	Copies editor state through the public CAXEMAP codec.

	This avoids mutable array aliases between edit, history, and test play while
	also ensuring every editor-produced draft remains representable by the public
	file format. It performs no filesystem work.

	`@:noCompletion` marks this cross-file collaborator as internal editor
	machinery. Application code should use `EditorSession`.
**/
@:noCompletion
final class EditorScenarioSnapshot {
	public static function capture(scenario:Scenario):EditorScenarioImageResult {
		if (scenario.formatVersion != ScenarioWriter.FORMAT_VERSION)
			return ImageRejected(UnsupportedFormatVersion(scenario.formatVersion, ScenarioWriter.FORMAT_VERSION));
		if (containsNestedChoice(scenario))
			return ImageRejected(NestedChoiceIsNotRepresentable);
		return restore(ScenarioWriter.write(scenario));
	}

	public static function restore(bytes:Bytes):EditorScenarioImageResult {
		return switch ScenarioLexer.read(bytes) {
			case ReadError(diagnostics): ImageRejected(SnapshotRejected(diagnostics));
			case ReadOk(records):
				switch ScenarioParser.parse(records) {
					case ReadError(diagnostics): ImageRejected(SnapshotRejected(diagnostics));
					case ReadOk(parsed): ImageReady({bytes: bytes.sub(0, bytes.length), parsed: parsed});
				}
		}
	}

	static function containsNestedChoice(scenario:Scenario):Bool {
		for (sequence in scenario.flow.sequences)
			if (!actionsAreRepresentable(sequence.actions, false))
				return true;
		for (rule in scenario.flow.rules)
			if (!actionsAreRepresentable(rule.actions, false))
				return true;
		return false;
	}

	static function actionsAreRepresentable(actions:Array<FlowAction>, insideChoice:Bool):Bool {
		for (action in actions)
			switch action {
				case ChooseSeeded(_, choices):
					if (insideChoice)
						return false;
					for (choice in choices)
						if (!actionsAreRepresentable(choice.actions, true))
							return false;
				case _:
			}
		return true;
	}
}
