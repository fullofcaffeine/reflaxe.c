package caxecraft.app;

import caxecraft.content.ActiveContent;
import caxecraft.content.ActiveContent.ContentPublicationError;
import caxecraft.content.ActiveContent.ContentPublicationResult;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnRole;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.LoadedContentGeneration.LoadedContentGeneration;
import caxecraft.content.LoadedContentGeneration.LoadedContentGenerationTrace;
import caxecraft.content.LoadedContentGeneration.LoadedActorBinding;
import caxecraft.content.LoadedContentGeneration.LoadedStatefulObjectBinding;
import caxecraft.content.LevelContentResolver.StatefulObjectBounds;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelCandidate;
import caxecraft.content.RuntimeLevelLoader.RuntimeLevelPresentation;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;

/**
	Keeps every application-facing view of one loaded level together.

	The runtime loader owns the complete simulation generation. The Raylib shell
	also needs a smaller set of derived facts: its actor views, item
	placements, player spawn, water presentation cell, and source path. Preparing
	those facts as one immutable value means a frame cannot accidentally pair a
	new session with old presentation data after a campaign transition.

	See `docs/haxe-code-architecture.md` for the broader ownership pattern: use a
	class here because one view belongs to one generation identity and lifetime;
	a loose record would make it easier to assemble unrelated fields.
**/
/** Why a complete runtime level cannot satisfy the current playable shell. */
enum PlayableLevelPreparationError {
	/** The current fixed HUD has no dialogue actor to present. */
	DialogueActorMissing;

	/** The current fixed combat presentation has no enemy actor to present. */
	EnemyActorMissing;

	/** More than one enemy actor would make the fixed combat slot ambiguous. */
	EnemyActorAmbiguous;

	/** The admitted playable renderer currently requires one water presentation. */
	FluidPresentationMissing;

	/** The first renderer can draw only one connected presentation cell. */
	FluidPresentationAmbiguous(firstCell:Int, otherCell:Int);
}

/** One dialogue actor identity and its validated atlas selection. */
typedef PlayableDialogueActor = {
	/** Stable CaxeMap identity used by agent observations and content logic. */
	final authoredId:ScenarioId;

	/** Runtime identity used for interaction and character observation. */
	final entityId:EntityId;

	/** Validated atlas selected by the actor's content profile. */
	final presentationAsset:String;

	/** Validated cell in that atlas. */
	final presentationCellIndex:Int;
}

/** Copy generic dialogue bindings into the smaller application view. */
private function collectDialogueActors(bindings:Array<LoadedActorBinding>):Array<PlayableDialogueActor> {
	final actors:Array<PlayableDialogueActor> = [];
	for (binding in bindings)
		switch binding.role {
			case DialogueNpc(_):
				actors.push({
					authoredId: binding.authoredId,
					entityId: binding.entityId,
					presentationAsset: binding.presentationAsset,
					presentationCellIndex: binding.presentationCellIndex
				});
			case EnemyActor:
		};
	return actors;
}

#if caxecraft_runtime_level_testing
/** Test the same ordered copy operation that prepares a published level. */
function collectDialogueActorsForTesting(bindings:Array<LoadedActorBinding>):Array<PlayableDialogueActor>
	return collectDialogueActors(bindings);
#end

/** A ready initial owner, or the exact presentation preflight that rejected it. */
enum PlayableLevelCreationResult {
	/** The initial generation and every derived app view agree. */
	PlayableLevelCreated(owner:ActivePlayableLevel);

	/** No active application owner was constructed. */
	PlayableLevelCreationRejected(error:PlayableLevelPreparationError);
}

/** Why a candidate did not replace the level visible to the game loop. */
enum PlayableLevelPublicationError {
	/** App-facing bindings could not be prepared, so publication was not attempted. */
	PlayableLevelCandidateRejected(error:PlayableLevelPreparationError);

	/** The lower generation owner rejected an older or duplicate candidate. */
	PlayableLevelGenerationRejected(error:ContentPublicationError);
}

/** Result of publishing one prepared level at a main-loop frame boundary. */
enum PlayableLevelPublicationResult {
	/** Future operations observe the new generation and its matching app views. */
	PlayableLevelPublished(retired:ContentGenerationId, active:ContentGenerationId);

	/** The previous generation and all matching app views remain active. */
	PlayableLevelPublicationRejected(error:PlayableLevelPublicationError);
}

/** Immutable application bindings derived from one runtime level candidate. */
final class PlayableLevelView {
	final loadedGeneration:LoadedContentGeneration;
	final levelPresentation:RuntimeLevelPresentation;
	final sourcePath:String;
	final dialogueActors:Array<PlayableDialogueActor>;
	final statefulObjects:Array<LoadedStatefulObjectBinding>;
	final enemyId:EntityId;
	final enemyAsset:String;
	final enemyCell:Int;
	final items:Array<LoadedWorldItem>;
	final playerSpawn:ScenarioTransform;
	final waterCell:Int;

	/** Retain only facts proven to belong to the supplied generation. */
	@:allow(caxecraft.app.ActivePlayableLevel)
	private function new(candidate:RuntimeLevelCandidate, loadedDialogueActors:Array<PlayableDialogueActor>, enemyActorId:EntityId,
			enemyPresentationAsset:String, enemyPresentationCell:Int, loadedStatefulObjects:Array<LoadedStatefulObjectBinding>,
			loadedItems:Array<LoadedWorldItem>, spawn:ScenarioTransform, waterPresentationCell:Int) {
		loadedGeneration = candidate.generation();
		levelPresentation = candidate.presentation();
		sourcePath = candidate.receipt().logicalPath;
		dialogueActors = loadedDialogueActors.copy();
		statefulObjects = loadedStatefulObjects.copy();
		enemyId = enemyActorId;
		enemyAsset = enemyPresentationAsset;
		enemyCell = enemyPresentationCell;
		items = loadedItems.copy();
		playerSpawn = spawn;
		waterCell = waterPresentationCell;
	}

	/** Package-relative map path that produced this exact view. */
	public inline function logicalPath():String
		return sourcePath;

	/** Immutable title and starting-objective facts paired with this generation. */
	public inline function presentation():RuntimeLevelPresentation
		return levelPresentation;

	/** Resolve this level's validated scenario title for one requested locale. */
	public inline function scenarioTitle(locale:LocaleId):String
		return levelPresentation.scenarioTitle(locale);

	/** Resolve this level's authored Adventure summary for the menu screens. */
	public inline function adventureTagline(locale:LocaleId):String
		return levelPresentation.adventureTagline(locale);

	/** Resolve this level's first initially active objective title. */
	public inline function initialObjectiveTitle(locale:LocaleId):String
		return levelPresentation.initialObjectiveTitle(locale);

	/** Resolve the body paired with this level's first initially active objective. */
	public inline function initialObjectiveBody(locale:LocaleId):String
		return levelPresentation.initialObjectiveBody(locale);

	/** Return the stable identity of this level's first active objective. */
	public inline function initialObjectiveId():Null<ScenarioId>
		return levelPresentation.initialObjectiveId();

	/** Resolve the current objective title from this level's validated map data. */
	public inline function objectiveTitle(id:Null<ScenarioId>, locale:LocaleId):String
		return levelPresentation.objectiveTitle(id, locale);

	/** Number of dialogue actors retained in deterministic authored order. */
	public inline function dialogueActorCount():Int
		return dialogueActors.length;

	/** Runtime identity for one bounds-checked dialogue actor. */
	public inline function dialogueActorIdAt(index:Int):EntityId
		return dialogueActors[index].entityId;

	/** Stable authored identity for one bounds-checked dialogue actor. */
	public inline function dialogueActorAuthoredIdAt(index:Int):ScenarioId
		return dialogueActors[index].authoredId;

	/** True when one retained dialogue actor has the supplied runtime identity. */
	public function hasDialogueActor(id:EntityId):Bool {
		for (actor in dialogueActors)
			if (actor.entityId == id)
				return true;
		return false;
	}

	/** Temporary fixed-combat enemy selected by generic authored role. */
	public inline function enemyActorId():EntityId
		return enemyId;

	/** Validated atlas selected by the current enemy profile. */
	public inline function enemyActorPresentationAsset():String
		return enemyAsset;

	/** Validated atlas for one bounds-checked dialogue actor. */
	public inline function dialogueActorPresentationAssetAt(index:Int):String
		return dialogueActors[index].presentationAsset;

	/** Validated atlas cell for one bounds-checked dialogue actor. */
	public inline function dialogueActorPresentationCellAt(index:Int):Int
		return dialogueActors[index].presentationCellIndex;

	/** Validated entity-atlas cell selected by the enemy actor content. */
	public inline function enemyActorPresentationCell():Int
		return enemyCell;

	/** Number of generic interactables retained in deterministic authored order. */
	public inline function statefulObjectCount():Int
		return statefulObjects.length;

	/** Stable authored identity for one bounds-checked generic interactable. */
	public inline function statefulObjectIdAt(index:Int):ScenarioId
		return statefulObjects[index].authoredId;

	/** Content profile for one bounds-checked generic interactable. */
	public inline function statefulObjectContentIdAt(index:Int):ContentId
		return statefulObjects[index].contentId;

	/** Copy-owned placement for one bounds-checked generic interactable. */
	public function statefulObjectTransformAt(index:Int):ScenarioTransform {
		final value = statefulObjects[index].transform;
		return {
			xMilli: value.xMilli,
			yMilli: value.yMilli,
			zMilli: value.zMilli,
			yawDegrees: value.yawDegrees
		};
	}

	/** Copy-owned dimensions for one generic object's visual and collision box. */
	public function statefulObjectBoundsAt(index:Int):StatefulObjectBounds {
		final value = statefulObjects[index].bounds;
		return {widthMilli: value.widthMilli, heightMilli: value.heightMilli, depthMilli: value.depthMilli};
	}

	/** Number of immutable item placements belonging to this level. */
	public inline function loadedItemCount():Int
		return items.length;

	/** Read one bounds-checked item while retaining Array ownership in this view. */
	public inline function loadedItemAt(index:Int):LoadedWorldItem
		return items[index];

	/** Authored default entrance used for revival and the first transition slice. */
	public inline function spawnTransform():ScenarioTransform
		return playerSpawn;

	/** One validated cell used by the current bounded water renderer. */
	public inline function waterPresentationCell():Int
		return waterCell;

	/** Generation handed only to the owner that controls publication. */
	@:allow(caxecraft.app.ActivePlayableLevel)
	private inline function generation():LoadedContentGeneration
		return loadedGeneration;
}

/**
	Owns the playable generation and all Raylib-shell bindings selected with it.

	`publish` prepares a private `PlayableLevelView` first. It then asks the
	lower `ActiveContent` owner to select the generation and, only on success,
	replaces the matching view. Caxecraft's frame loop is single-threaded and this
	method invokes no callback between those assignments, so readers can observe
	only the complete old state before the call or the complete new state after it.
**/
final class ActivePlayableLevel {
	final content:ActiveContent;
	var activeView:PlayableLevelView;

	/** Construct only after the initial candidate passes every app preflight. */
	private function new(initial:PlayableLevelView) {
		content = new ActiveContent(initial.generation());
		activeView = initial;
	}

	/** Prepare the first complete view without admitting an empty live state. */
	public static function create(initial:RuntimeLevelCandidate):PlayableLevelCreationResult {
		return switch prepare(initial) {
			case PlayableLevelPrepared(view): PlayableLevelCreated(new ActivePlayableLevel(view));
			case PlayableLevelNotPrepared(error): PlayableLevelCreationRejected(error);
		};
	}

	/** Identity that future frame operations will observe. */
	public inline function generationId():ContentGenerationId
		return content.generationId();

	/** Borrow the current session for one frame or command; reacquire after publish. */
	public inline function session():GameSession
		return content.session();

	/** Borrow the immutable app bindings paired with `session()`. */
	public inline function level():PlayableLevelView
		return activeView;

	/** Number of complete successful replacements after the initial level. */
	public inline function publicationCount():Int
		return content.publicationCount();

	/** Capture target-neutral evidence from the generation paired with `level()`. */
	public inline function semanticTrace():LoadedContentGenerationTrace
		return content.semanticTrace();

	/**
		Prepare and select one newer level without exposing partial application state.

		All fallible parsing, runtime construction, and app binding work has completed
		before the lower owner changes. An ordering rejection also leaves both owners
		unchanged. After a successful lower swap, result construction completes before
		the non-failing app-view assignment.
	**/
	public function publish(candidate:RuntimeLevelCandidate):PlayableLevelPublicationResult {
		final prepared = switch prepare(candidate) {
			case PlayableLevelPrepared(view): view;
			case PlayableLevelNotPrepared(error): return PlayableLevelPublicationRejected(PlayableLevelCandidateRejected(error));
		};
		return switch content.publish(prepared.generation()) {
			case ContentPublicationRejected(error):
				PlayableLevelPublicationRejected(PlayableLevelGenerationRejected(error));
			case ContentPublished(retired, selected):
				final result = PlayableLevelPublished(retired, selected);
				activeView = prepared;
				result;
		};
	}

	/** Derive every temporary app binding before a level can become visible. */
	static function prepare(candidate:RuntimeLevelCandidate):PlayableLevelPreparationResult {
		final actorBindings = candidate.generation().actorBindings();
		final dialogueActors = collectDialogueActors(actorBindings);
		var enemyActorId = EntityId.invalid();
		var enemyPresentationAsset = "";
		var enemyPresentationCell = -1;
		for (binding in actorBindings)
			switch binding.role {
				case DialogueNpc(_):
				case EnemyActor:
					if (enemyActorId.isValid())
						return PlayableLevelNotPrepared(EnemyActorAmbiguous);
					enemyActorId = binding.entityId;
					enemyPresentationAsset = binding.presentationAsset;
					enemyPresentationCell = binding.presentationCellIndex;
			};
		if (dialogueActors.length == 0)
			return PlayableLevelNotPrepared(DialogueActorMissing);
		if (!enemyActorId.isValid())
			return PlayableLevelNotPrepared(EnemyActorMissing);

		final fluidPresentation = candidate.generation().presentation().fluidRequests();
		if (fluidPresentation.length == 0)
			return PlayableLevelNotPrepared(FluidPresentationMissing);
		final waterPresentationCell = fluidPresentation[0].cellIndex;
		for (request in fluidPresentation)
			if (request.cellIndex != waterPresentationCell)
				return PlayableLevelNotPrepared(FluidPresentationAmbiguous(waterPresentationCell, request.cellIndex));

		final loadedItems:Array<LoadedWorldItem> = [];
		for (binding in candidate.generation().itemBindings())
			loadedItems.push({
				storageCode: binding.storage.value(),
				quantity: binding.quantity,
				xMilli: binding.transform.xMilli,
				yMilli: binding.transform.yMilli,
				zMilli: binding.transform.zMilli
			});
		return PlayableLevelPrepared(new PlayableLevelView(candidate, dialogueActors, enemyActorId, enemyPresentationAsset, enemyPresentationCell,
			candidate.generation().statefulObjectBindings(), loadedItems, candidate.generation().plan().player().transform, waterPresentationCell));
	}
}

/** Private preparation state that prevents a partly filled view from escaping. */
private enum PlayableLevelPreparationResult {
	/** Every field was derived from one candidate. */
	PlayableLevelPrepared(view:PlayableLevelView);

	/** The candidate remains unpublished. */
	PlayableLevelNotPrepared(error:PlayableLevelPreparationError);
}
