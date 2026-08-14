package caxecraft.app;

import caxecraft.app.ActivePlayableLevel.PlayableLevelCreationResult;
import caxecraft.app.ActivePlayableLevel.PlayableLevelPreparationError;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.LevelContentResolver;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadError;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelLoadResult;
import caxecraft.content.RuntimeLevelLoader.loadRuntimeLevel;
import caxecraft.scenario.ScenarioContentRegistry;
import haxe.io.Bytes;

/**
 * Owns the disposable real-engine level used by editor Test Play.
 *
 * The ordinary game level remains untouched while this owner loads validated
 * editor bytes through the same runtime path. `stop` drops the complete level,
 * including its `GameSession`, so gameplay changes cannot enter editor history
 * or the ordinary game. A class fits this explicit start-to-stop lifetime.
 */
final class EditorTestPlayRuntime {
	final validationRegistry:ScenarioContentRegistry;
	final resolutionRegistry:LevelContentResolver;
	var active:Null<ActivePlayableLevel>;
	var nextGenerationSequence:Int;

	/** Retain the engine registries across many disposable editor runs. */
	public function new(validationRegistry:ScenarioContentRegistry, resolutionRegistry:LevelContentResolver, firstGenerationSequence:Int) {
		this.validationRegistry = validationRegistry;
		this.resolutionRegistry = resolutionRegistry;
		active = null;
		nextGenerationSequence = firstGenerationSequence;
	}

	/**
	 * Build one fresh level from the current validated editor bytes.
	 *
	 * The load is synchronous and uses no filesystem, compiler, or process
	 * restart. A rejection leaves this owner idle and returns the exact stage.
	 */
	public function start(request:EditorTestPlayRequest):EditorTestPlayStartResult {
		if (active != null)
			return EditorTestPlayRejected(EditorTestPlayAlreadyRunning);
		final generationSequence = nextGenerationSequence;
		final candidate = switch loadRuntimeLevel(InMemoryBytes(request.canonical, "editor-test-play", "editor/draft.caxemap"),
			ContentGenerationId.fromSequence(generationSequence), validationRegistry, resolutionRegistry, request.playerOptions) {
			case RuntimeLevelReady(value): value;
			case RuntimeLevelRejected(error): return EditorTestPlayRejected(EditorTestPlayLoadRejected(error));
		};
		return switch ActivePlayableLevel.create(candidate) {
			case PlayableLevelCreated(value):
				active = value;
				nextGenerationSequence++;
				EditorTestPlayStarted;
			case PlayableLevelCreationRejected(error):
				EditorTestPlayRejected(EditorTestPlayPresentationRejected(error));
		};
	}

	/** Borrow the disposable level for one frame, or null while editing. */
	public inline function level():Null<ActivePlayableLevel>
		return active;

	/** Drop all runtime mutations and return whether Test Play was active. */
	public function stop():Bool {
		if (active == null)
			return false;
		active = null;
		return true;
	}
}

/** Complete typed input for one disposable editor run. */
typedef EditorTestPlayRequest = {
	/** Canonical bytes returned by editor validation. */
	final canonical:Bytes;

	/** Local-player identity, health, and aquatic behavior for this run. */
	final playerOptions:LevelPlayerOptions;
}

/** Exact stage that prevented editor Test Play from starting. */
enum EditorTestPlayError {
	/** A second start request arrived before the current run stopped. */
	EditorTestPlayAlreadyRunning;

	/** Parsing, validation, resolution, or `GameSession` construction failed. */
	EditorTestPlayLoadRejected(error:RuntimeLevelLoadError);

	/** The complete level lacks a binding required by the current game shell. */
	EditorTestPlayPresentationRejected(error:PlayableLevelPreparationError);
}

/** One complete disposable level started, or no active level escaped. */
enum EditorTestPlayStartResult {
	EditorTestPlayStarted;
	EditorTestPlayRejected(error:EditorTestPlayError);
}
