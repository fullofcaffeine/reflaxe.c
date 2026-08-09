package caxecraft.pilot;

import caxecraft.pilot.PilotCheckpoint.PilotCheckpointKind;
import caxecraft.pilot.PilotScript.PilotAction;
import haxe.io.Bytes;

/** The stable semantic values that a content journey can observe. */
enum abstract RuntimePilotExpectationKind(Int) to Int {
	/** Current application screen, such as `title`, `campaign`, or `playing`. */
	var Screen = 0;

	/** Current game mode, such as `creative` or `adventure`. */
	var Mode = 1;

	/** Stable ID of the selected or active level. */
	var Level = 2;

	/** Stable ID of the active objective. */
	var Objective = 3;

	/** Sequence number of the active runtime-content generation. */
	var Generation = 4;

	/** Number of successful level publications after startup. */
	var Publications = 5;

	/** Integer voxel cell that contains the local player's feet. */
	var Position = 6;

	/** Stable aquatic state: `dry`, `wading`, `floating`, or `submerged`. */
	var Medium = 7;

	/** Stable equipped content ID, or `none`. */
	var Equipment = 8;

	/** Number of lanterns in the bounded inventory. */
	var Lanterns = 9;

	/** Number of mined sand blocks in the bounded inventory. */
	var Sand = 10;

	/** Stable ID of the authored dialogue currently shown, or `none`. */
	var Dialogue = 11;
}

/** One source-located expectation owned by a runtime Piloscript file. */
final class RuntimePilotExpectation {
	/** Frame after whose action the game compares this expectation. */
	public final frame:Int;

	/** Closed application fact selected for comparison. */
	public final kind:RuntimePilotExpectationKind;

	/** Manually authored expected spelling. Numeric facts use decimal text. */
	public final expected:String;

	/** Source line used if the real game state differs. */
	public final line:Int;

	/** Keep the expected value immutable after validation. */
	public function new(frame:Int, kind:RuntimePilotExpectationKind, expected:String, line:Int) {
		this.frame = frame;
		this.kind = kind;
		this.expected = expected;
		this.line = line;
	}
}

/** Generic application state exposed to one Piloscript observation point. */
typedef RuntimePilotObservation = {
	/** Stable screen spelling selected by the application adapter. */
	final screen:String;

	/** Stable game-mode spelling selected by the application adapter. */
	final mode:String;

	/** Selected or active level ID supplied by runtime campaign content. */
	final level:String;

	/** Active objective ID supplied by runtime level content. */
	final objective:String;

	/** Authored dialogue ID selected by the latest CaxeFlow presentation event. */
	final dialogue:String;

	/** Active generation sequence supplied by `ActivePlayableLevel`. */
	final generation:Int;

	/** Successful publication count supplied by `ActivePlayableLevel`. */
	final publications:Int;

	/** Integer voxel position derived from the committed local-player body. */
	final cellX:Int;

	final cellY:Int;
	final cellZ:Int;

	/** Stable aquatic state selected by the generic application adapter. */
	final aquaticMedium:String;

	/** Stable equipped item ID, or `none`. */
	final aquaticEquipment:String;

	/** Bounded inventory counts that content rewards can observe. */
	final lanterns:Int;

	final sand:Int;
}

/** One fail-closed parser or semantic comparison error. */
final class RuntimePilotDiagnostic {
	/** Safe source label selected by the caller. */
	public final source:String;

	/** One-based source line, or zero for a whole-file error. */
	public final line:Int;

	/** Short explanation that includes the rejected value when useful. */
	public final message:String;

	/** Retain an immutable error that callers can print without parsing text. */
	public function new(source:String, line:Int, message:String) {
		this.source = source;
		this.line = line;
		this.message = message;
	}
}

/** Parsing returns one complete script or one located error, never partial data. */
enum RuntimePilotReadResult {
	/** The complete source passed syntax, bounds, and uniqueness checks. */
	RuntimePilotReady(script:RuntimePilotScript);

	/** The source failed before it could become executable input. */
	RuntimePilotRejected(diagnostic:RuntimePilotDiagnostic);
}

/** Comparing one frame returns success or the first independent mismatch. */
enum RuntimePilotRunResult {
	/** Every expectation owned by this frame matched the real application state. */
	RuntimePilotFrameAccepted;

	/** One expected value differed from the real application state. */
	RuntimePilotFrameRejected(diagnostic:RuntimePilotDiagnostic);
}

/** Internal result for the closed text-to-action vocabulary. */
private enum RuntimePilotActionReadResult {
	KnownRuntimePilotAction(action:PilotAction);
	UnknownRuntimePilotAction;
}

/** Internal result for the closed text-to-observation vocabulary. */
private enum RuntimePilotExpectationReadResult {
	KnownRuntimePilotExpectation(kind:RuntimePilotExpectationKind);
	UnknownRuntimePilotExpectation;
}

/**
 * Loads and runs one bounded content journey from text read after startup.
 *
 * Piloscript supplies player actions and independent expected values. This
 * class validates those bytes and compares them with a small generic view of
 * the real application. It contains no campaign names, level IDs, objective
 * IDs, or prose. The final frame always becomes `Quit`, including every later
 * frame, so malformed host timing cannot create an interactive test channel.
 */
final class RuntimePilotScript {
	/** Runtime journey limit. At 50 ms per frame, this permits 20 seconds of play. */
	public static inline final ABSOLUTE_FRAME_LIMIT:Int = 400;

	static inline final MAXIMUM_SOURCE_BYTES:Int = 64 * 1024;
	static inline final MAXIMUM_EXPECTATIONS_PER_FRAME:Int = 8;

	final source:String;
	final name:String;
	final actions:Array<PilotAction>;
	final checkpoints:Array<Null<PilotCheckpoint>>;
	final expectations:Array<RuntimePilotExpectation>;
	final hash:Int;

	/** Construct only after the parser validates every record. */
	private function new(source:String, name:String, actions:Array<PilotAction>, checkpoints:Array<Null<PilotCheckpoint>>,
			expectations:Array<RuntimePilotExpectation>, hash:Int) {
		this.source = source;
		this.name = name;
		this.actions = actions;
		this.checkpoints = checkpoints;
		this.expectations = expectations;
		this.hash = hash;
	}

	/**
	 * Parse one complete `PILOSCRIPT 1` byte vector.
	 *
	 * Records must follow the header, `name`, and `frames` lines. Blank lines and
	 * lines that start with `#` are ignored. The first error stops parsing and
	 * reports its one-based line. The parser never returns a partial script.
	 */
	public static function read(bytes:Bytes, source:String):RuntimePilotReadResult {
		if (bytes.length == 0)
			return rejected(source, 0, "Piloscript is empty");
		if (bytes.length > MAXIMUM_SOURCE_BYTES)
			return rejected(source, 0, "Piloscript exceeds the 65536-byte limit");
		if (!validSourceLabel(source))
			return rejected("piloscript", 0, "source label is empty or contains a control character");

		final lines = bytes.toString().split("\n");
		var stage = 0;
		var stableName = "";
		var actions:Array<PilotAction> = [];
		var actionOwned:Array<Bool> = [];
		var checkpoints:Array<Null<PilotCheckpoint>> = [];
		var expectations:Array<RuntimePilotExpectation> = [];
		var ended = false;
		var lineIndex = 0;
		while (lineIndex < lines.length) {
			final lineNumber = lineIndex + 1;
			final physicalLine = lines[lineIndex];
			final raw = physicalLine.length > 0
				&& physicalLine.charAt(physicalLine.length - 1) == "\r" ? physicalLine.substring(0, physicalLine.length - 1) : physicalLine;
			lineIndex++;
			final words = wordsOf(raw);
			if (words.length == 0 || words[0].charAt(0) == "#")
				continue;
			if (ended)
				return rejected(source, lineNumber, "content appears after end");
			if (stage == 0) {
				if (words.length != 2 || words[0] != "PILOSCRIPT" || words[1] != "1")
					return rejected(source, lineNumber, "expected PILOSCRIPT 1 header");
				stage = 1;
				continue;
			}
			if (stage == 1) {
				if (words.length != 2 || words[0] != "name" || !validToken(words[1], 64))
					return rejected(source, lineNumber, "expected a safe name record");
				stableName = words[1];
				stage = 2;
				continue;
			}
			if (stage == 2) {
				if (words.length != 2 || words[0] != "frames")
					return rejected(source, lineNumber, "expected a frames record");
				final limit = nonNegativeInteger(words[1], ABSOLUTE_FRAME_LIMIT);
				if (limit < 2 || limit > ABSOLUTE_FRAME_LIMIT)
					return rejected(source, lineNumber, "frame limit must be from 2 through 400");
				var frame = 0;
				while (frame < limit) {
					actions.push(PilotAction.Idle);
					actionOwned.push(false);
					checkpoints.push(null);
					frame++;
				}
				stage = 3;
				continue;
			}

			if (words.length == 1 && words[0] == "end") {
				ended = true;
				continue;
			}
			if (words.length < 3)
				return rejected(source, lineNumber, "record is incomplete");
			if (words[0] == "hold") {
				if (words.length != 4)
					return rejected(source, lineNumber, "hold record must have first frame, last frame, and action name");
				final firstFrame = nonNegativeInteger(words[1], actions.length - 2);
				final lastFrame = nonNegativeInteger(words[2], actions.length - 2);
				if (firstFrame < 0 || lastFrame < firstFrame || lastFrame >= actions.length - 1)
					return rejected(source, lineNumber, "hold range must be ordered and before the final quit frame");
				final heldAction = switch readAction(words[3]) {
					case UnknownRuntimePilotAction:
						return rejected(source, lineNumber, "unknown action " + words[3]);
					case KnownRuntimePilotAction(action): action;
				};
				var heldFrame = firstFrame;
				while (heldFrame <= lastFrame) {
					if (actionOwned[heldFrame])
						return rejected(source, lineNumber, "hold range overlaps another action");
					actions[heldFrame] = heldAction;
					actionOwned[heldFrame] = true;
					heldFrame++;
				}
				continue;
			}
			final frame = nonNegativeInteger(words[1], actions.length - 2);
			if (frame < 0 || frame >= actions.length - 1)
				return rejected(source, lineNumber, "record frame must be before the final quit frame");

			if (words[0] == "action") {
				if (words.length != 3)
					return rejected(source, lineNumber, "action record must have frame and action name");
				if (actionOwned[frame])
					return rejected(source, lineNumber, "duplicate action for frame");
				switch readAction(words[2]) {
					case UnknownRuntimePilotAction:
						return rejected(source, lineNumber, "unknown action " + words[2]);
					case KnownRuntimePilotAction(action):
						actions[frame] = action;
						actionOwned[frame] = true;
				}
				continue;
			}
			if (words[0] == "checkpoint") {
				if (words.length != 4 || words[2] != "capture" || !validCaptureLabel(words[3]))
					return rejected(source, lineNumber, "capture label must be title-selection, campaign-selection, level-selection, or final");
				if (checkpoints[frame] != null)
					return rejected(source, lineNumber, "duplicate checkpoint for frame");
				checkpoints[frame] = new PilotCheckpoint(words[3], PilotCheckpointKind.CaptureScreenshot);
				continue;
			}
			if (words[0] == "expect") {
				if (words.length != 4 || !validExpectedValue(words[3]))
					return rejected(source, lineNumber, "expect record must have frame, fact, and one safe value");
				if (expectationCount(expectations, frame) >= MAXIMUM_EXPECTATIONS_PER_FRAME)
					return rejected(source, lineNumber, "frame exceeds the eight-expectation limit");
				switch readExpectation(words[2]) {
					case UnknownRuntimePilotExpectation:
						return rejected(source, lineNumber, "unknown expectation " + words[2]);
					case KnownRuntimePilotExpectation(kind):
						if (hasExpectation(expectations, frame, kind))
							return rejected(source, lineNumber, "duplicate expectation for frame and fact");
						if ((kind == Generation || kind == Publications || kind == Lanterns || kind == Sand)
							&& !validNonNegativeInteger(words[3]))
							return rejected(source, lineNumber, "numeric expectation must be a non-negative decimal integer");
						expectations.push(new RuntimePilotExpectation(frame, kind, words[3], lineNumber));
				}
				continue;
			}
			return rejected(source, lineNumber, "unknown record " + words[0]);
		}

		if (stage < 3)
			return rejected(source, lines.length, "Piloscript ended before its required header records");
		if (!ended)
			return rejected(source, lines.length, "Piloscript requires a final end record");
		actions[actions.length - 1] = PilotAction.Quit;
		return RuntimePilotReady(new RuntimePilotScript(source, stableName, actions, checkpoints, expectations,
			computeHash(stableName, actions, checkpoints, expectations)));
	}

	/** Stable script identity supplied by the runtime source. */
	public inline function stableName():String
		return name;

	/** Validated frame count, including the forced final quit frame. */
	public inline function frameLimit():Int
		return actions.length;

	/** Deterministic non-security fingerprint of all executable script facts. */
	public inline function inputHash():Int
		return hash;

	/** Return the authored action, or permanent quit at and after the limit. */
	public function actionAt(frame:Int):PilotAction
		return frame < 0 || frame >= actions.length - 1 ? PilotAction.Quit : actions[frame];

	/** Return the frame capture request, if the runtime source declared one. */
	public function checkpointAt(frame:Int):Null<PilotCheckpoint>
		return frame < 0 || frame >= checkpoints.length ? null : checkpoints[frame];

	/** Number of semantic comparisons declared for one frame. */
	public function expectationCountAt(frame:Int):Int
		return frame < 0 || frame >= actions.length ? 0 : expectationCount(expectations, frame);

	/** Return one already-bounded expectation for a caller-owned loop. */
	public function expectationAt(frame:Int, index:Int):RuntimePilotExpectation {
		var found = 0;
		for (expectation in expectations)
			if (expectation.frame == frame) {
				if (found == index)
					return expectation;
				found++;
			}
		return expectations[0];
	}

	/** Compare this frame with real generic application state. */
	public function observe(frame:Int, observation:RuntimePilotObservation):RuntimePilotRunResult {
		if (frame < 0 || frame >= actions.length)
			return RuntimePilotFrameAccepted;
		for (expectation in expectations) {
			if (expectation.frame != frame)
				continue;
			final actual = observedValue(expectation.kind, observation);
			if (actual != expectation.expected)
				return RuntimePilotFrameRejected(new RuntimePilotDiagnostic(source, expectation.line,
					'expected ${expectationName(expectation.kind)} ${expectation.expected} but observed $actual'));
		}
		return RuntimePilotFrameAccepted;
	}

	/** True on the final frame and every later frame. */
	public inline function complete(frame:Int):Bool
		return frame >= actions.length - 1;

	/** Convert one application observation to the expected stable spelling. */
	static function observedValue(kind:RuntimePilotExpectationKind, observation:RuntimePilotObservation):String {
		if (kind == Screen)
			return observation.screen;
		if (kind == Mode)
			return observation.mode;
		if (kind == Level)
			return observation.level;
		if (kind == Objective)
			return observation.objective;
		if (kind == Dialogue)
			return observation.dialogue;
		if (kind == Generation)
			return Std.string(observation.generation);
		if (kind == Publications)
			return Std.string(observation.publications);
		if (kind == Position)
			return Std.string(observation.cellX) + "," + Std.string(observation.cellY) + "," + Std.string(observation.cellZ);
		if (kind == Medium)
			return observation.aquaticMedium;
		if (kind == Equipment)
			return observation.aquaticEquipment;
		if (kind == Lanterns)
			return Std.string(observation.lanterns);
		return Std.string(observation.sand);
	}

	/** Stable diagnostic name for one closed observed fact. */
	static function expectationName(kind:RuntimePilotExpectationKind):String {
		if (kind == Screen)
			return "screen";
		if (kind == Mode)
			return "mode";
		if (kind == Level)
			return "level";
		if (kind == Objective)
			return "objective";
		if (kind == Dialogue)
			return "dialogue";
		if (kind == Generation)
			return "generation";
		if (kind == Publications)
			return "publications";
		if (kind == Position)
			return "position";
		if (kind == Medium)
			return "medium";
		if (kind == Equipment)
			return "equipment";
		if (kind == Lanterns)
			return "lanterns";
		return "sand";
	}

	/** Parse one closed input action without aliases or case folding. */
	static function readAction(value:String):RuntimePilotActionReadResult {
		if (value == "idle")
			return KnownRuntimePilotAction(PilotAction.Idle);
		if (value == "forward")
			return KnownRuntimePilotAction(PilotAction.Forward);
		if (value == "forward-turn")
			return KnownRuntimePilotAction(PilotAction.ForwardTurn);
		if (value == "forward-jump")
			return KnownRuntimePilotAction(PilotAction.ForwardJump);
		if (value == "jump")
			return KnownRuntimePilotAction(PilotAction.Jump);
		if (value == "backward-jump")
			return KnownRuntimePilotAction(PilotAction.BackwardJump);
		if (value == "left-jump")
			return KnownRuntimePilotAction(PilotAction.LeftJump);
		if (value == "right-jump")
			return KnownRuntimePilotAction(PilotAction.RightJump);
		if (value == "left-descend")
			return KnownRuntimePilotAction(PilotAction.LeftDescend);
		if (value == "right-rise")
			return KnownRuntimePilotAction(PilotAction.RightRise);
		if (value == "forward-left")
			return KnownRuntimePilotAction(PilotAction.ForwardLeft);
		if (value == "forward-rise")
			return KnownRuntimePilotAction(PilotAction.ForwardRise);
		if (value == "rise")
			return KnownRuntimePilotAction(PilotAction.Rise);
		if (value == "backward")
			return KnownRuntimePilotAction(PilotAction.Backward);
		if (value == "left")
			return KnownRuntimePilotAction(PilotAction.Left);
		if (value == "right")
			return KnownRuntimePilotAction(PilotAction.Right);
		if (value == "turn-left")
			return KnownRuntimePilotAction(PilotAction.TurnLeft);
		if (value == "turn-right")
			return KnownRuntimePilotAction(PilotAction.TurnRight);
		if (value == "forward-descend")
			return KnownRuntimePilotAction(PilotAction.ForwardDescend);
		if (value == "descend")
			return KnownRuntimePilotAction(PilotAction.Descend);
		if (value == "right-look")
			return KnownRuntimePilotAction(PilotAction.RightLook);
		if (value == "look-down")
			return KnownRuntimePilotAction(PilotAction.LookDown);
		if (value == "look-up")
			return KnownRuntimePilotAction(PilotAction.LookUp);
		if (value == "look-left")
			return KnownRuntimePilotAction(PilotAction.LookLeft);
		if (value == "mine")
			return KnownRuntimePilotAction(PilotAction.Mine);
		if (value == "place")
			return KnownRuntimePilotAction(PilotAction.Place);
		if (value == "interact")
			return KnownRuntimePilotAction(PilotAction.Interact);
		if (value == "travel")
			return KnownRuntimePilotAction(PilotAction.Travel);
		if (value == "pause")
			return KnownRuntimePilotAction(PilotAction.Pause);
		if (value == "capture")
			return KnownRuntimePilotAction(PilotAction.Capture);
		if (value == "select-next")
			return KnownRuntimePilotAction(PilotAction.SelectNext);
		if (value == "select-sword")
			return KnownRuntimePilotAction(PilotAction.SelectSword);
		if (value == "strike")
			return KnownRuntimePilotAction(PilotAction.Strike);
		if (value == "select-berries")
			return KnownRuntimePilotAction(PilotAction.SelectBerries);
		if (value == "eat-berries")
			return KnownRuntimePilotAction(PilotAction.EatBerries);
		if (value == "menu-next")
			return KnownRuntimePilotAction(PilotAction.MenuNext);
		if (value == "menu-confirm")
			return KnownRuntimePilotAction(PilotAction.MenuConfirm);
		return UnknownRuntimePilotAction;
	}

	/** Parse one closed observation name. */
	static function readExpectation(value:String):RuntimePilotExpectationReadResult {
		if (value == "screen")
			return KnownRuntimePilotExpectation(Screen);
		if (value == "mode")
			return KnownRuntimePilotExpectation(Mode);
		if (value == "level")
			return KnownRuntimePilotExpectation(Level);
		if (value == "objective")
			return KnownRuntimePilotExpectation(Objective);
		if (value == "dialogue")
			return KnownRuntimePilotExpectation(Dialogue);
		if (value == "generation")
			return KnownRuntimePilotExpectation(Generation);
		if (value == "publications")
			return KnownRuntimePilotExpectation(Publications);
		if (value == "position")
			return KnownRuntimePilotExpectation(Position);
		if (value == "medium")
			return KnownRuntimePilotExpectation(Medium);
		if (value == "equipment")
			return KnownRuntimePilotExpectation(Equipment);
		if (value == "lanterns")
			return KnownRuntimePilotExpectation(Lanterns);
		if (value == "sand")
			return KnownRuntimePilotExpectation(Sand);
		return UnknownRuntimePilotExpectation;
	}

	/** Reject ambiguous duplicate facts on one frame. */
	static function hasExpectation(values:Array<RuntimePilotExpectation>, frame:Int, kind:RuntimePilotExpectationKind):Bool {
		for (value in values)
			if (value.frame == frame && value.kind == kind)
				return true;
		return false;
	}

	/** Count the flat expectation records owned by one frame. */
	static function expectationCount(values:Array<RuntimePilotExpectation>, frame:Int):Int {
		var count = 0;
		for (value in values)
			if (value.frame == frame)
				count++;
		return count;
	}

	/** Split spaces and tabs without accepting quoted or multiline values. */
	static function wordsOf(value:String):Array<String> {
		final words:Array<String> = [];
		var current = "";
		var index = 0;
		while (index < value.length) {
			final character = value.charAt(index);
			if (character == " " || character == "\t") {
				if (current.length > 0) {
					words.push(current);
					current = "";
				}
			} else {
				current += character;
			}
			index++;
		}
		if (current.length > 0)
			words.push(current);
		return words;
	}

	/** Accept lowercase identifiers safe for reports and capture filenames. */
	static function validToken(value:String, maximumLength:Int):Bool {
		if (value.length == 0 || value.length > maximumLength || value.charAt(0) == "-" || value.charAt(value.length - 1) == "-")
			return false;
		var index = 0;
		while (index < value.length) {
			final code = value.charCodeAt(index);
			if (!((code >= 97 && code <= 122) || (code >= 48 && code <= 57) || code == 45))
				return false;
			index++;
		}
		return true;
	}

	/** Admit only generic visual roles with fixed native output filenames. */
	static inline function validCaptureLabel(value:String):Bool
		return value == "title-selection" || value == "campaign-selection" || value == "level-selection" || value == "final";

	/** Accept one bounded stable ID or decimal value without whitespace. */
	static function validExpectedValue(value:String):Bool {
		if (value.length == 0 || value.length > 128)
			return false;
		var index = 0;
		while (index < value.length) {
			final code = value.charCodeAt(index);
			if (code <= 32 || code == 127)
				return false;
			index++;
		}
		return true;
	}

	/** Accept a decimal integer whose normalized spelling is non-negative. */
	static inline function validNonNegativeInteger(value:String):Bool
		return nonNegativeInteger(value, 2147483647) >= 0;

	/** Parse one canonical bounded integer, or return -1 for invalid text. */
	static function nonNegativeInteger(value:String, maximum:Int):Int {
		if (value.length == 0 || (value.length > 1 && value.charCodeAt(0) == 48))
			return -1;
		var result = 0;
		var index = 0;
		while (index < value.length) {
			final code = value.charCodeAt(index);
			if (code < 48 || code > 57 || result > 214748364)
				return -1;
			final digit = code - 48;
			if (result == 214748364 && digit > 7)
				return -1;
			result = result * 10 + digit;
			index++;
		}
		return result <= maximum ? result : -1;
	}

	/** Keep diagnostics free from empty labels and terminal control bytes. */
	static function validSourceLabel(value:String):Bool {
		if (value.length == 0 || value.length > 160)
			return false;
		var index = 0;
		while (index < value.length) {
			final code = value.charCodeAt(index);
			if (code < 32 || code == 127)
				return false;
			index++;
		}
		return true;
	}

	/** Build one typed rejection without repeating constructor syntax. */
	static inline function rejected(source:String, line:Int, message:String):RuntimePilotReadResult
		return RuntimePilotRejected(new RuntimePilotDiagnostic(source, line, message));

	/** Hash the accepted model, not whitespace or comments. */
	static function computeHash(name:String, actions:Array<PilotAction>, checkpoints:Array<Null<PilotCheckpoint>>,
			expectations:Array<RuntimePilotExpectation>):Int {
		var hash = hashText(-2128831035, name);
		var frame = 0;
		while (frame < actions.length) {
			hash = mix(hash, actions[frame]);
			final checkpoint = checkpoints[frame];
			if (checkpoint != null)
				hash = hashText(mix(hash, 1), checkpoint.label);
			else
				hash = mix(hash, 0);
			frame++;
		}
		for (expectation in expectations) {
			hash = mix(hash, expectation.frame);
			hash = mix(hash, expectation.kind);
			hash = hashText(hash, expectation.expected);
		}
		return hash;
	}

	/** Mix one string by Unicode scalar values for target-neutral identity. */
	static function hashText(hash:Int, value:String):Int {
		var result = mix(hash, value.length);
		var index = 0;
		while (index < value.length) {
			result = mix(result, value.charCodeAt(index));
			index++;
		}
		return result;
	}

	/** FNV-style 32-bit mix used only for reproducible test identity. */
	static inline function mix(hash:Int, value:Int):Int
		return (hash ^ value) * 16777619;
}
