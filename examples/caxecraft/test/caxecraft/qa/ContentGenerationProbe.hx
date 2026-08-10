package caxecraft.qa;

import caxecraft.content.ActiveContent;
import caxecraft.content.ActiveContent.ContentPublicationError;
import caxecraft.content.ActiveContent.ContentPublicationResult;
import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.LevelContentResolver;
import caxecraft.content.LevelContentResolver.ActorPresentationResolution;
import caxecraft.content.LevelContentResolver.FluidContentResolution;
import caxecraft.content.LevelContentResolver.ItemContentResolution;
import caxecraft.content.LevelContentResolver.TerrainContentResolution;
import caxecraft.content.LoadedContentGeneration;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildError;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildFault;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildResult;
import caxecraft.content.LoadedContentGeneration.ContentGenerationBuildStage;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.LoadedContentGeneration.LoadedContentGenerationTrace;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanResult;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.World;
import caxecraft.qa.ResolvedLevelFixture.readFirstPlayableScenario;
import caxecraft.qa.ResolvedLevelFixture.resolveFirstPlayable;
import caxecraft.qa.FocusedContentFixture.FocusedContentRegistry;
import caxecraft.scenario.ContentId;

/**
 * Proves fresh candidate ownership and one-point content publication.
 *
 * The probe builds the real resolved first-playable plan, rejects construction
 * before every named stage, publishes repeated successful generations, and
 * confirms that stale or failed requests cannot change the active semantic
 * trace. All expected behavior stays in Haxe; the C harness only reports four
 * exported scalars after running the generated lifecycle.
 */
/** Complete Haxe-authored assertion result observed by the native harness. */
var observed:Int = 0;

/** Final active monotonic generation sequence after repeated publication. */
var finalGenerationId:Int = 0;

/** Number of successful post-startup publication swaps. */
var finalPublicationCount:Int = 0;

/** Compact target-neutral digest of the final active generation. */
var finalTraceDigest:Int = 0;

/** Run the ownership specification and expose its target-neutral result. */
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(finalGenerationId);
	Sys.println(finalPublicationCount);
	Sys.println(finalTraceDigest);
	#end
}

/** Return zero, or the stable number of the first broken ownership invariant. */
function selfCheck():Int {
	final scenario = readFirstPlayableScenario();
	if (scenario == null)
		return 1;
	final registry = new FocusedContentRegistry();
	final pair = switch resolveFirstPlayable(scenario, registry) {
		case LevelPlanResolved(plan, presentation): {plan: plan, presentation: presentation};
		case LevelPlanRejected(_): return 2;
	};

	switch LoadedContentGeneration.build(ContentGenerationId.fromSequence(0), pair.plan, pair.presentation) {
		case ContentGenerationRejected(InvalidGenerationId(0)):
		case _:
			return 3;
	}

	final alternatePresentation = switch resolveFirstPlayable(scenario, new AlternatePresentationRegistry()) {
		case LevelPlanResolved(_, presentation): presentation;
		case LevelPlanRejected(_): return 4;
	};
	switch LoadedContentGeneration.build(ContentGenerationId.fromSequence(1), pair.plan, alternatePresentation) {
		case ContentGenerationRejected(PresentationPlanMismatch):
		case _:
			return 5;
	}

	final initial = switch LoadedContentGeneration.build(ContentGenerationId.fromSequence(1), pair.plan, pair.presentation) {
		case ContentGenerationReady(generation): generation;
		case ContentGenerationRejected(_): return 6;
	};
	if (initial.generationId().value() != 1)
		return 70;
	if (initial.session().characterCount() != 3)
		return 71;
	if (initial.session().actorControllerSnapshots().length != 2)
		return 72;
	if (initial.actorBindings().length != 2)
		return 73;
	if (initial.itemBindings().length != 1)
		return 74;
	if (initial.semanticTrace().activeItems != 1)
		return 76;

	final active = new ActiveContent(initial);
	final initialTrace = active.semanticTrace();
	final faults = [
		FailBeforePresentation,
		FailBeforeTerrain,
		FailBeforeFluids,
		FailBeforeItems,
		FailBeforePlayer,
		FailBeforeActors,
		FailBeforeFinalization
	];
	final stages = [
		PresentationPreflight,
		TerrainConstruction,
		FluidConstruction,
		ItemConstruction,
		PlayerConstruction,
		ActorConstruction,
		GenerationFinalization
	];
	for (index in 0...faults.length) {
		switch LoadedContentGeneration.buildWithFault(ContentGenerationId.fromSequence(2), pair.plan, pair.presentation, faults[index]) {
			case ContentGenerationRejected(InjectedFailure(stage)) if (stageCode(stage) == stageCode(stages[index])):
			case _:
				return 8 + index;
		}
		if (!sameTrace(initialTrace, active.semanticTrace()) || active.generationId().value() != 1 || active.publicationCount() != 0)
			return 20 + index;
	}

	final second = switch LoadedContentGeneration.build(ContentGenerationId.fromSequence(2), pair.plan, pair.presentation) {
		case ContentGenerationReady(generation): generation;
		case ContentGenerationRejected(_): return 30;
	};
	switch active.publish(second) {
		case ContentPublished(retired, selected) if (retired.value() == 1 && selected.value() == 2):
		case _:
			return 31;
	}
	if (active.generationId().value() != 2
		|| active.publicationCount() != 1
		|| initial.generationId().value() != 1
		|| !sameTrace(initialTrace, initial.semanticTrace()))
		return 32;

	switch active.publish(initial) {
		case ContentPublicationRejected(OutOfOrderGeneration(1, 2)):
		case _:
			return 33;
	}
	if (active.generationId().value() != 2 || active.publicationCount() != 1)
		return 34;

	final editable = World.coord(16, 4, 16);
	if (!active.session().removeTerrain(editable))
		return 35;
	final editedTrace = active.semanticTrace();
	if (sameTrace(initialTrace, editedTrace))
		return 36;
	switch LoadedContentGeneration.buildWithFault(ContentGenerationId.fromSequence(3), pair.plan, pair.presentation, FailBeforeActors) {
		case ContentGenerationRejected(InjectedFailure(ActorConstruction)):
		case _:
			return 37;
	}
	if (!sameTrace(editedTrace, active.semanticTrace()) || active.generationId().value() != 2)
		return 38;

	var sequence = 3;
	while (sequence <= 34) {
		final candidate = switch LoadedContentGeneration.build(ContentGenerationId.fromSequence(sequence), pair.plan, pair.presentation) {
			case ContentGenerationReady(generation): generation;
			case ContentGenerationRejected(_): return 40;
		};
		switch active.publish(candidate) {
			case ContentPublished(_, selected) if (selected.value() == sequence):
			case _:
				return 41;
		}
		sequence++;
	}
	if (active.generationId().value() != 34 || active.publicationCount() != 33)
		return 42;

	final trace = active.semanticTrace();
	if (trace.generationId != 34
		|| trace.worldState == editedTrace.worldState
		|| trace.characters != 3
		|| trace.controllers != 2
		|| trace.localPlayerId != 1
		|| trace.activeItems != 1)
		return 43;
	finalGenerationId = active.generationId().value();
	finalPublicationCount = active.publicationCount();
	finalTraceDigest = digestTrace(trace);
	return 0;
}

/** Convert one closed build stage to a stable scalar for cross-target checks. */
function stageCode(stage:ContentGenerationBuildStage):Int
	return switch stage {
		case PresentationPreflight: 1;
		case TerrainConstruction: 2;
		case FluidConstruction: 3;
		case ItemConstruction: 4;
		case PlayerConstruction: 5;
		case ActorConstruction: 6;
		case GenerationFinalization: 7;
	};

/** Compare every target-neutral generation-trace field. */
function sameTrace(left:LoadedContentGenerationTrace, right:LoadedContentGenerationTrace):Bool
	return left.generationId == right.generationId
		&& left.scenarioDigest == right.scenarioDigest
		&& left.resolvedDigest == right.resolvedDigest
		&& left.presentationDigest == right.presentationDigest
		&& left.worldState == right.worldState
		&& left.pendingWater == right.pendingWater
		&& left.activeItems == right.activeItems
		&& left.activeItemDigest == right.activeItemDigest
		&& left.characters == right.characters
		&& left.controllers == right.controllers
		&& left.localPlayerId == right.localPlayerId;

/** Fold the final trace into the small native harness envelope. */
function digestTrace(trace:LoadedContentGenerationTrace):Int {
	var digest = CaxecraftTrace.mix(trace.generationId, trace.scenarioDigest);
	digest = CaxecraftTrace.mix(digest, trace.resolvedDigest);
	digest = CaxecraftTrace.mix(digest, trace.presentationDigest);
	digest = CaxecraftTrace.mix(digest, trace.worldState);
	digest = CaxecraftTrace.mix(digest, trace.pendingWater);
	digest = CaxecraftTrace.mix(digest, trace.activeItems);
	digest = CaxecraftTrace.mix(digest, trace.activeItemDigest);
	digest = CaxecraftTrace.mix(digest, trace.characters);
	digest = CaxecraftTrace.mix(digest, trace.controllers);
	return CaxecraftTrace.mix(digest, trace.localPlayerId);
}

/**
 * Resolve the same mechanics with a deliberately different presentation cell.
 *
 * This test-only registry proves that a construction plan cannot be paired with
 * presentation facts from a neighboring resolution, even when every content ID
 * and simulation choice still looks plausible.
 */
final class AlternatePresentationRegistry implements LevelContentResolver {
	final base:FocusedContentRegistry = new FocusedContentRegistry();

	/** Construct one stateless test resolver over the reviewed base registry. */
	public function new() {}

	/** Preserve ordinary terrain resolution; only presentation differs. */
	public function resolveTerrain(id:ContentId):TerrainContentResolution
		return base.resolveTerrain(id);

	/** Preserve fluid simulation while selecting a neighboring visual cell. */
	public function resolveFluid(id:ContentId):FluidContentResolution
		return switch base.resolveFluid(id) {
			case FluidContentResolved(simulation, cell): FluidContentResolved(simulation, cell + 1);
			case UnknownFluidContent: UnknownFluidContent;
		};

	/** Preserve ordinary item resolution. */
	public function resolveItem(id:ContentId):ItemContentResolution
		return base.resolveItem(id);

	/** Select neighboring actor cells so presentation mismatch covers actors too. */
	public function resolveActorPresentation(id:ContentId):ActorPresentationResolution
		return switch base.resolveActorPresentation(id) {
			case ActorPresentationResolved(asset, cell): ActorPresentationResolved(asset, cell + 1);
			case UnknownActorPresentation: UnknownActorPresentation;
		};

	/** Preserve ordinary stateful-object resolution. */
	public function resolveStatefulObject(id:ContentId, state:ContentId):caxecraft.content.LevelContentResolver.StatefulObjectContentResolution
		return base.resolveStatefulObject(id, state);

	/** Preserve ordinary non-player-character mechanics resolution. */
	public function resolveNpc(id:ContentId):ActorContentResolution
		return base.resolveNpc(id);

	/** Preserve ordinary enemy mechanics resolution. */
	public function resolveEnemy(id:ContentId):ActorContentResolution
		return base.resolveEnemy(id);
}
