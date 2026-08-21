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
	final scenario:Scenario;
	final parseState:EditorScenarioParseState;
}

/** Whether this image already owns exact source coordinates for its bytes. */
@:noCompletion
enum EditorScenarioParseState {
	/** The image came through the public reader and has exact parser metadata. */
	ParsedScenarioImage(parsed:ParsedScenario);

	/** A trusted copy-owning reducer wrote these bytes; validation parses on demand. */
	DeferredScenarioParse;
}

/** Internal success/failure result for the same snapshot boundary. */
@:noCompletion
enum EditorScenarioImageResult {
	ImageReady(image:EditorScenarioImage);
	ImageRejected(error:EditorError);
}

/**
	Copies general editor state through the public CAXEMAP codec.

	This avoids mutable array aliases between edit, history, and test play while
	also ensuring every editor-produced draft remains representable by the public
	file format. It performs no filesystem work. A separate reducer-owned capture
	writes canonical bytes but defers parsing when the reducer constructs or
	deep-copies all changed arrays and records and the session keeps the result
	private.

	The codec round trip is a stateless operation over caller-owned values, so
	module functions are clearer than a class containing only static methods.
	They must remain module-public because `EditorSession` and `EditorTestPlay`
	import them from other source files. `@:noCompletion` removes the annotated
	functions from ordinary IDE suggestions so application authors are guided
	toward `EditorSession`, the supported stateful API. It does not make the
	functions private, weaken type checking, or change generated C; explicit
	internal imports continue to work normally.
**/
@:noCompletion
function capture(scenario:Scenario):EditorScenarioImageResult {
	if (scenario.formatVersion != ScenarioWriter.FORMAT_VERSION)
		return ImageRejected(UnsupportedFormatVersion(scenario.formatVersion, ScenarioWriter.FORMAT_VERSION));
	if (containsNestedChoice(scenario))
		return ImageRejected(NestedChoiceIsNotRepresentable);
	return restore(ScenarioWriter.write(scenario));
}

/**
	Capture canonical bytes after a copy-owning reducer edit without parsing them.

	Eligible reducers either build values from scalar input or deep-copy every
	structured input that they retain. The remaining scenario values come from
	the session's private image. This lets the session publish exact canonical
	bytes and history immediately while deferring source-coordinate reconstruction
	until validation needs it. Do not use this boundary for a command that can
	retain caller-owned structured input.
**/
@:noCompletion
function captureReducerOwnedEdit(scenario:Scenario):EditorScenarioImageResult {
	if (scenario.formatVersion != ScenarioWriter.FORMAT_VERSION)
		return ImageRejected(UnsupportedFormatVersion(scenario.formatVersion, ScenarioWriter.FORMAT_VERSION));
	if (containsNestedChoice(scenario))
		return ImageRejected(NestedChoiceIsNotRepresentable);
	return ImageReady({
		bytes: ScenarioWriter.write(scenario),
		scenario: scenario,
		parseState: DeferredScenarioParse
	});
}

/** Restore an isolated editor image from canonical in-memory CAXEMAP bytes. */
@:noCompletion
function restore(bytes:Bytes):EditorScenarioImageResult {
	return switch ScenarioLexer.read(bytes) {
		case ReadError(diagnostics): ImageRejected(SnapshotRejected(diagnostics));
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError(diagnostics): ImageRejected(SnapshotRejected(diagnostics));
				case ReadOk(parsed): ImageReady({
						bytes: bytes.sub(0, bytes.length),
						scenario: parsed.candidate,
						parseState: ParsedScenarioImage(parsed)
					});
			}
	}
}

private function containsNestedChoice(scenario:Scenario):Bool {
	for (sequence in scenario.flow.sequences)
		if (!actionsAreRepresentable(sequence.actions, false))
			return true;
	for (rule in scenario.flow.rules)
		if (!actionsAreRepresentable(rule.actions, false))
			return true;
	return false;
}

private function actionsAreRepresentable(actions:Array<FlowAction>, insideChoice:Bool):Bool {
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
