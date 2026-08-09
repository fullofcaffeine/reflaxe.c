package caxecraft.content;

import caxecraft.content.ActorCompositionPlanner.CharacterSpawnPlan;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnRole;
import caxecraft.content.ActorPublication.ActorPublicationError;
import caxecraft.content.ActorPublication.ActorPublicationResult;
import caxecraft.content.ActorPublication.publishActorPlans;
import caxecraft.content.LevelContentResolver.ItemStorageCode;
import caxecraft.content.LevelContentResolver.StatefulObjectBounds;
import caxecraft.content.LevelContentResolver.StatefulObjectCollisionProfile;
import caxecraft.content.LevelContentResolver.StatefulObjectStateMechanics;
import caxecraft.content.ResolvedLevelPlan.ResolvedActorPresentation;
import caxecraft.content.ResolvedLevelPlan.ResolvedFlowBinding;
import caxecraft.content.ResolvedLevelPlan.ResolvedFluid;
import caxecraft.content.ResolvedLevelPlan.ResolvedFluidPlacement;
import caxecraft.content.ResolvedLevelPlan.ResolvedFluidPresentation;
import caxecraft.content.ResolvedLevelPlan.ResolvedItem;
import caxecraft.content.ResolvedLevelPlan.ResolvedItemPresentation;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPresentationPlan;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelSemanticTrace;
import caxecraft.content.ResolvedLevelPlan.ResolvedPlayer;
import caxecraft.content.ResolvedLevelPlan.ResolvedStatefulObject;
import caxecraft.content.ResolvedLevelPlan.ResolvedStatefulObjectPresentation;
import caxecraft.content.ResolvedLevelPlan.ResolvedTerrainRun;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.domain.World;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioId;

/**
 * Builds one complete, unpublished simulation from resolved content.
 *
 * A candidate owns a fresh `GameSession`, the immutable resolution facts that
 * explain how it was built, stable authored bindings, and presentation
 * requests. Construction may fail after changing that fresh session, but the
 * candidate cannot become observable until every stage succeeds. Publishing is
 * a separate operation owned by `ActiveContent`.
 */
/**
 * Monotonic identity selected by the load coordinator for one candidate.
 *
 * The abstract prevents a generation ID from being confused with a terrain
 * code, entity ID, or count while retaining one direct integer in generated C.
 * `build` rejects zero and negative sequence values before allocating a session.
 */
abstract ContentGenerationId(Int) {
	private inline function new(value:Int)
		this = value;

	/** Wrap a coordinator sequence value; candidate construction validates it. */
	public static inline function fromSequence(value:Int):ContentGenerationId
		return new ContentGenerationId(value);

	/** Reveal the ordered scalar at publication, diagnostics, and trace boundaries. */
	public inline function value():Int
		return this;

	/** True only for an identity that may enter candidate construction. */
	public inline function isValid():Bool
		return this > 0;
}

/** Stable actor identity and presentation role retained by a loaded generation. */
typedef LoadedActorBinding = {
	/** Authored CAXEMAP identity used by saves, flow, and editor selection. */
	final authoredId:ScenarioId;

	/** Runtime entity identity used by the loaded `GameSession`. */
	final entityId:EntityId;

	/** Content-pack identity that selected mechanics and presentation. */
	final contentId:ContentId;

	/** Closed presentation/gameplay role resolved for the placed actor. */
	final role:CharacterSpawnRole;

	/** Manifest-validated cell in the native entity atlas. */
	final presentationCellIndex:Int;
}

/** Stable item identity and compact storage retained by a loaded generation. */
typedef LoadedItemBinding = {
	/** Authored CAXEMAP identity used by saves, flow, and editor selection. */
	final authoredId:ScenarioId;

	/** Content-pack identity used to resolve mechanics and presentation. */
	final contentId:ContentId;

	/** Nominal pack-local item storage selected before session construction. */
	final storage:ItemStorageCode;

	/** Positive authored stack quantity retained for gameplay binding. */
	final quantity:Int;

	/** Copy-owned authored placement retained outside fixed activity storage. */
	final transform:ScenarioTransform;
}

/** Immutable identity, proximity, and placement for one generic interactable. */
typedef LoadedStatefulObjectBinding = {
	/** Stable CAXEMAP identity used by CaxeFlow events and state observations. */
	final authoredId:ScenarioId;

	/** Content profile used to resolve the visual for the current authored state. */
	final contentId:ContentId;

	/** Maximum interaction distance in integer thousandths of a block. */
	final interactionRadiusMilli:Int;

	/** Validated box dimensions used by state-dependent collision and drawing. */
	final bounds:StatefulObjectBounds;

	/** Complete closed mechanics for every compatible CaxeFlow state. */
	final states:Array<StatefulObjectStateMechanics>;

	/** Copy-owned placement used by interaction and presentation. */
	final transform:ScenarioTransform;
}

/** Named construction stages used by diagnostics and fault-injection evidence. */
enum ContentGenerationBuildStage {
	/** Check that construction and presentation came from the same resolution. */
	PresentationPreflight;

	/** Fill the fixed world from compact, ordered terrain runs. */
	TerrainConstruction;

	/** Install bounded initial fluid volumes and permanent sources. */
	FluidConstruction;

	/** Activate the fixed authored-item slots represented by this generation. */
	ItemConstruction;

	/** Construct and bind the locally controlled character. */
	PlayerConstruction;

	/** Construct and publish the complete authored actor set into the candidate. */
	ActorConstruction;

	/** Package all complete state into the immutable generation owner. */
	GenerationFinalization;
}

/** Exact reason no complete candidate generation escaped construction. */
enum ContentGenerationBuildError {
	/** The caller supplied zero or a negative generation sequence. */
	InvalidGenerationId(value:Int);

	/** Construction and presentation facts did not describe the same content. */
	PresentationPlanMismatch;

	/** One supposedly resolved terrain run was rejected by session storage. */
	TerrainRunRejected(index:Int);

	/** Compact terrain runs did not cover the complete fixed world. */
	TerrainCoverageMismatch(actual:Int, expected:Int);

	/** One supposedly resolved fluid could not enter the fresh session. */
	FluidRejected(index:Int);

	/** One supposedly resolved item could not enter fixed session storage. */
	ItemRejected(index:Int);

	/** The resolved local-player value could not bind to the fresh session. */
	PlayerRejected;

	/** The complete actor construction set was rejected before candidate success. */
	ActorsRejected(error:ActorPublicationError);

	#if caxecraft_generation_testing
	/** Test-only failure proving one incomplete stage never reaches publication. */
	InjectedFailure(stage:ContentGenerationBuildStage);
	#end
}

/** A complete unpublished owner, or one rejection with no candidate value. */
enum ContentGenerationBuildResult {
	/** Every construction stage completed and the generation is safe to publish. */
	ContentGenerationReady(generation:LoadedContentGeneration);

	/** Construction failed; any partly filled fresh session is unreachable. */
	ContentGenerationRejected(error:ContentGenerationBuildError);
}

#if caxecraft_generation_testing
/**
 * Test-only fault selector compiled out of ordinary game builds.
 *
 * This closed enum lets the focused Eval/native probe stop before each
 * construction stage without adding callbacks, mutable global switches, or
 * content-shaped invalid data to the production API.
 */
enum ContentGenerationBuildFault {
	/** Run every production construction stage normally. */
	NoGenerationFault;

	/** Reject before construction/presentation compatibility checks. */
	FailBeforePresentation;

	/** Reject before allocating and filling a fresh world. */
	FailBeforeTerrain;

	/** Reject after terrain but before initial fluid state. */
	FailBeforeFluids;

	/** Reject after fluids but before authored-item activation. */
	FailBeforeItems;

	/** Reject after items but before local-player construction. */
	FailBeforePlayer;

	/** Reject after the player but before authored actor construction. */
	FailBeforeActors;

	/** Reject after the complete session but before the owner can escape. */
	FailBeforeFinalization;
}
#end

/**
 * Compact observable state used to prove publication and rollback behavior.
 *
 * Every field is target-neutral. The trace intentionally excludes object
 * addresses, allocation order, and generated names, so Eval and native C can
 * compare the same ownership result.
 */
typedef LoadedContentGenerationTrace = {
	/** Monotonic selected generation sequence. */
	final generationId:Int;

	/** Stable digest of the authored scenario identity. */
	final scenarioDigest:Int;

	/** Folded terrain, fluid, item, actor, flow, and player resolution facts. */
	final resolvedDigest:Int;

	/** Digest of the separate renderer/read-model requests. */
	final presentationDigest:Int;

	/** Current deterministic whole-world state hash. */
	final worldState:Int;

	/** Number of deterministic fluid work items still queued. */
	final pendingWater:Int;

	/** Number of authored item slots still active in the current session. */
	final activeItems:Int;

	/** Ordered compact storage digest for active authored items. */
	final activeItemDigest:Int;

	/** Number of currently owned character snapshots, including the player. */
	final characters:Int;

	/** Number of authored non-player controller states. */
	final controllers:Int;

	/** Compact local-player entity identity, or zero if ownership is broken. */
	final localPlayerId:Int;
}

/**
 * Owns one complete loaded simulation and the facts needed to interpret it.
 *
 * A class is used because the session has mutable identity and one generation
 * lifetime. The constructor is private: callers can receive only a candidate
 * that passed `build`. The plan and presentation values are immutable owners
 * themselves, while returned binding Arrays are defensive copies.
 */
final class LoadedContentGeneration {
	final generationIdValue:ContentGenerationId;
	final resolvedPlan:ResolvedLevelPlan;
	final presentationPlan:ResolvedLevelPresentationPlan;
	final resolvedTrace:ResolvedLevelSemanticTrace;
	final ownedSession:GameSession;
	final actors:Array<LoadedActorBinding>;
	final items:Array<LoadedItemBinding>;
	final statefulObjects:Array<LoadedStatefulObjectBinding>;

	private function new(generationId:ContentGenerationId, plan:ResolvedLevelPlan, presentation:ResolvedLevelPresentationPlan,
			trace:ResolvedLevelSemanticTrace, session:GameSession, actors:Array<LoadedActorBinding>, items:Array<LoadedItemBinding>,
			statefulObjects:Array<LoadedStatefulObjectBinding>) {
		generationIdValue = generationId;
		resolvedPlan = plan;
		presentationPlan = presentation;
		resolvedTrace = trace;
		ownedSession = session;
		this.actors = actors.copy();
		this.items = items.copy();
		this.statefulObjects = copyStatefulObjectBindings(statefulObjects);
	}

	/**
	 * Build one fresh candidate without receiving or mutating the active generation.
	 *
	 * Resolution and presentation compatibility are checked before session
	 * construction. Later failures may leave only the new local session partially
	 * filled; because no `LoadedContentGeneration` escapes, dropping the rejection
	 * discards that complete candidate lifetime.
	 */
	public static function build(generationId:ContentGenerationId, plan:ResolvedLevelPlan,
			presentation:ResolvedLevelPresentationPlan):ContentGenerationBuildResult {
		return buildInternal(generationId, plan, presentation #if caxecraft_generation_testing, NoGenerationFault #end);
	}

	#if caxecraft_generation_testing
	/**
	 * Build with one deterministic pre-stage rejection for ownership testing.
	 *
	 * This method does not exist in ordinary builds. It exercises the same
	 * production implementation and can only stop earlier; it cannot force a
	 * candidate to publish or bypass a validation rule.
	 */
	public static function buildWithFault(generationId:ContentGenerationId, plan:ResolvedLevelPlan, presentation:ResolvedLevelPresentationPlan,
			fault:ContentGenerationBuildFault):ContentGenerationBuildResult
		return buildInternal(generationId, plan, presentation, fault);
	#end

	/** Monotonic identity used by the publication owner and diagnostics. */
	public inline function generationId():ContentGenerationId
		return generationIdValue;

	/**
	 * Borrow this generation's live session for the current main-loop operation.
	 *
	 * Callers must not retain the reference across `ActiveContent.publish`.
	 * Publication itself never mutates this session; it replaces the generation
	 * owner that future operations query.
	 */
	public inline function session():GameSession
		return ownedSession;

	/** Immutable private construction plan retained for runtime flow bindings. */
	public inline function plan():ResolvedLevelPlan
		return resolvedPlan;

	/** Immutable presentation request owner retained beside the simulation. */
	public inline function presentation():ResolvedLevelPresentationPlan
		return presentationPlan;

	/** Return copy-owned actor bindings in deterministic authored order. */
	public function actorBindings():Array<LoadedActorBinding>
		return actors.copy();

	/** Return copy-owned item bindings in deterministic authored-ID order. */
	public function itemBindings():Array<LoadedItemBinding>
		return items.copy();

	/** Return copy-owned generic interactable bindings in authored-ID order. */
	public function statefulObjectBindings():Array<LoadedStatefulObjectBinding>
		return copyStatefulObjectBindings(statefulObjects);

	/** Return copy-owned CaxeFlow IDs retained by the resolved plan. */
	public inline function flowBindings():Array<ResolvedFlowBinding>
		return resolvedPlan.flowBindings();

	/**
	 * Observe the currently owned simulation without exposing host identities.
	 *
	 * This trace changes when gameplay mutates the session. A rejected candidate
	 * build has no route to this owner, so the active trace must remain byte-for-
	 * byte equivalent at the scalar level.
	 */
	public function semanticTrace():LoadedContentGenerationTrace {
		var activeItems = 0;
		var activeItemDigest = 0;
		for (index in 0...items.length)
			if (ownedSession.authoredItemIsActive(index)) {
				activeItems++;
				activeItemDigest = CaxecraftTrace.mix(activeItemDigest, items[index].storage.value());
			}
		final localId = ownedSession.hasLocalPlayer() ? ownedSession.readLocalPlayer().id.storageCode() : 0;
		return {
			generationId: generationIdValue.value(),
			scenarioDigest: resolvedTrace.scenarioDigest,
			resolvedDigest: digestResolvedTrace(resolvedTrace),
			presentationDigest: resolvedTrace.presentationDigest,
			worldState: ownedSession.worldStateHash(),
			pendingWater: ownedSession.pendingWaterWork(),
			activeItems: activeItems,
			activeItemDigest: activeItemDigest,
			characters: ownedSession.characterCount(),
			controllers: ownedSession.actorControllerSnapshots().length,
			localPlayerId: localId
		};
	}

	private static function buildInternal(generationId:ContentGenerationId, plan:ResolvedLevelPlan, presentation:ResolvedLevelPresentationPlan
			#if caxecraft_generation_testing, fault:ContentGenerationBuildFault #end
	):ContentGenerationBuildResult {
		if (!generationId.isValid())
			return ContentGenerationRejected(InvalidGenerationId(generationId.value()));
		#if caxecraft_generation_testing
		if (fault == FailBeforePresentation)
			return ContentGenerationRejected(InjectedFailure(PresentationPreflight));
		#end

		final terrain = plan.terrainRuns();
		final fluids = plan.fluids();
		final items = plan.items();
		final statefulObjects = plan.statefulObjects();
		final actors = plan.actors();
		final player = plan.player();
		final presentationFluids = presentation.fluidRequests();
		final presentationItems = presentation.itemRequests();
		final presentationActors = presentation.actorRequests();
		final presentationStatefulObjects = presentation.statefulObjectRequests();
		if (!plan.acceptsPresentation(presentation)
			|| !presentationMatches(fluids, items, statefulObjects, actors, presentationFluids, presentationItems, presentationStatefulObjects,
				presentationActors))
			return ContentGenerationRejected(PresentationPlanMismatch);

		final trace = plan.semanticTrace(presentation);
		final actorBindings = buildActorBindings(actors, presentationActors);
		final itemBindings = buildItemBindings(items);
		final statefulObjectBindings = buildStatefulObjectBindings(statefulObjects);
		#if caxecraft_generation_testing
		if (fault == FailBeforeTerrain)
			return ContentGenerationRejected(InjectedFailure(TerrainConstruction));
		#end

		final session = new GameSession();
		var terrainIndex = 0;
		for (runIndex in 0...terrain.length) {
			final run:ResolvedTerrainRun = terrain[runIndex];
			terrainIndex = session.writeTerrainRunDuringLoad(terrainIndex, run.storage.value(), run.count);
			if (terrainIndex < 0)
				return ContentGenerationRejected(TerrainRunRejected(runIndex));
		}
		if (terrainIndex != World.VOLUME)
			return ContentGenerationRejected(TerrainCoverageMismatch(terrainIndex, World.VOLUME));

		#if caxecraft_generation_testing
		if (fault == FailBeforeFluids)
			return ContentGenerationRejected(InjectedFailure(FluidConstruction));
		#end
		for (index in 0...fluids.length)
			if (!installFluid(session, fluids[index]))
				return ContentGenerationRejected(FluidRejected(index));

		#if caxecraft_generation_testing
		if (fault == FailBeforeItems)
			return ContentGenerationRejected(InjectedFailure(ItemConstruction));
		#end
		for (index in 0...items.length)
			if (!session.activateAuthoredItemDuringLoad(index))
				return ContentGenerationRejected(ItemRejected(index));

		#if caxecraft_generation_testing
		if (fault == FailBeforePlayer)
			return ContentGenerationRejected(InjectedFailure(PlayerConstruction));
		#end
		if (!installPlayer(session, player))
			return ContentGenerationRejected(PlayerRejected);

		#if caxecraft_generation_testing
		if (fault == FailBeforeActors)
			return ContentGenerationRejected(InjectedFailure(ActorConstruction));
		#end
		switch publishActorPlans(session, actors) {
			case ActorsPublished(count) if (count == actors.length):
			case ActorPublicationRejected(error):
				return ContentGenerationRejected(ActorsRejected(error));
			case ActorsPublished(_):
				return ContentGenerationRejected(ActorsRejected(SessionPublicationRejected));
		}

		#if caxecraft_generation_testing
		if (fault == FailBeforeFinalization)
			return ContentGenerationRejected(InjectedFailure(GenerationFinalization));
		#end
		return ContentGenerationReady(new LoadedContentGeneration(generationId, plan, presentation, trace, session, actorBindings, itemBindings,
			statefulObjectBindings));
	}

	/** Check every presentation identity before constructing a mutable session. */
	static function presentationMatches(fluids:Array<ResolvedFluid>, items:Array<ResolvedItem>, statefulObjects:Array<ResolvedStatefulObject>,
			actors:Array<CharacterSpawnPlan>, presentationFluids:Array<ResolvedFluidPresentation>, presentationItems:Array<ResolvedItemPresentation>,
			presentationStatefulObjects:Array<ResolvedStatefulObjectPresentation>, presentationActors:Array<ResolvedActorPresentation>):Bool {
		if (fluids.length != presentationFluids.length
			|| items.length != presentationItems.length
			|| statefulObjects.length != presentationStatefulObjects.length
			|| actors.length != presentationActors.length)
			return false;
		for (index in 0...fluids.length)
			if (fluids[index].authoredId != presentationFluids[index].authoredId
				|| fluids[index].contentId != presentationFluids[index].contentId
				|| presentationFluids[index].cellIndex < 0)
				return false;
		for (index in 0...items.length)
			if (items[index].authoredId != presentationItems[index].authoredId
				|| items[index].contentId != presentationItems[index].contentId)
				return false;
		for (index in 0...statefulObjects.length)
			if (statefulObjects[index].authoredId != presentationStatefulObjects[index].authoredId
				|| statefulObjects[index].contentId != presentationStatefulObjects[index].contentId
				|| statefulObjects[index].initialState != presentationStatefulObjects[index].state
				|| presentationStatefulObjects[index].asset.length == 0
				|| presentationStatefulObjects[index].cellIndex < 0)
				return false;
		for (index in 0...actors.length)
			if (actors[index].authoredId != presentationActors[index].authoredId
				|| actors[index].contentId != presentationActors[index].contentId
				|| presentationActors[index].cellIndex < 0
				|| !sameRole(actors[index].role, presentationActors[index].role))
				return false;
		return true;
	}

	/** Convert one resolved fluid into fresh candidate-session state. */
	static function installFluid(session:GameSession, fluid:ResolvedFluid):Bool {
		return switch fluid.placement {
			case InitialVolume(bounds):
				session.placeInitialWaterVolume(World.coord(bounds.origin.x, bounds.origin.y, bounds.origin.z), bounds.size.width, bounds.size.height,
					bounds.size.depth);
			case Source(point):
				session.placeWaterSource(World.coord(point.x, point.y, point.z));
		};
	}

	/** Construct and bind the resolved local player into the fresh session. */
	static function installPlayer(session:GameSession, player:ResolvedPlayer):Bool {
		final transform = player.transform;
		return session.bindLocalPlayer(startCharacter(player.entityId,
			createBody(transform.xMilli / 1000.0, transform.yMilli / 1000.0, transform.zMilli / 1000.0), player.aquaticProfile, player.initialHealth));
	}

	/** Copy stable actor bindings before any mutable session construction starts. */
	static function buildActorBindings(plans:Array<CharacterSpawnPlan>, presentation:Array<ResolvedActorPresentation>):Array<LoadedActorBinding> {
		final result:Array<LoadedActorBinding> = [];
		for (index in 0...plans.length) {
			final plan = plans[index];
			result.push({
				authoredId: plan.authoredId,
				entityId: plan.entityId,
				contentId: plan.contentId,
				role: copyRole(plan.role),
				presentationCellIndex: presentation[index].cellIndex
			});
		}
		return result;
	}

	/** Copy item bindings before any mutable session construction starts. */
	static function buildItemBindings(plans:Array<ResolvedItem>):Array<LoadedItemBinding> {
		final result:Array<LoadedItemBinding> = [];
		for (plan in plans)
			result.push({
				authoredId: plan.authoredId,
				contentId: plan.contentId,
				storage: plan.storage,
				quantity: plan.quantity,
				transform: copyTransform(plan.transform)
			});
		return result;
	}

	/** Copy generic interactable bindings before mutable flow state is installed. */
	static function buildStatefulObjectBindings(plans:Array<ResolvedStatefulObject>):Array<LoadedStatefulObjectBinding> {
		final result:Array<LoadedStatefulObjectBinding> = [];
		for (plan in plans)
			result.push({
				authoredId: plan.authoredId,
				contentId: plan.contentId,
				interactionRadiusMilli: plan.interactionRadiusMilli,
				bounds: {
					widthMilli: plan.bounds.widthMilli,
					heightMilli: plan.bounds.heightMilli,
					depthMilli: plan.bounds.depthMilli
				},
				states: copyStatefulObjectStates(plan.states),
				transform: copyTransform(plan.transform)
			});
		return result;
	}

	/** Copy loaded bindings without sharing their nested state arrays. */
	static function copyStatefulObjectBindings(values:Array<LoadedStatefulObjectBinding>):Array<LoadedStatefulObjectBinding> {
		final result:Array<LoadedStatefulObjectBinding> = [];
		for (value in values)
			result.push({
				authoredId: value.authoredId,
				contentId: value.contentId,
				interactionRadiusMilli: value.interactionRadiusMilli,
				bounds: {
					widthMilli: value.bounds.widthMilli,
					heightMilli: value.bounds.heightMilli,
					depthMilli: value.bounds.depthMilli
				},
				states: copyStatefulObjectStates(value.states),
				transform: copyTransform(value.transform)
			});
		return result;
	}

	/** Deep-copy state mechanics before the mutable session is constructed. */
	static function copyStatefulObjectStates(values:Array<StatefulObjectStateMechanics>):Array<StatefulObjectStateMechanics> {
		final result:Array<StatefulObjectStateMechanics> = [];
		for (value in values)
			result.push({
				state: value.state,
				collision: switch value.collision {
					case StatefulObjectPassable: StatefulObjectPassable;
					case StatefulObjectSolid: StatefulObjectSolid;
				},
				visible: value.visible
			});
		return result;
	}

	/** Compare the two closed role variants without relying on enum identity. */
	static function sameRole(left:CharacterSpawnRole, right:CharacterSpawnRole):Bool
		return switch [left, right] {
			case [DialogueNpc(leftDialogue), DialogueNpc(rightDialogue)]: leftDialogue == rightDialogue;
			case [EnemyActor, EnemyActor]: true;
			case _: false;
		};

	/** Copy the one role payload retained in a generation binding. */
	static function copyRole(role:CharacterSpawnRole):CharacterSpawnRole
		return switch role {
			case DialogueNpc(dialogue): DialogueNpc(dialogue);
			case EnemyActor: EnemyActor;
		};

	/** Copy placement scalars so a binding never aliases a caller-owned record. */
	static function copyTransform(value:ScenarioTransform):ScenarioTransform
		return {
			xMilli: value.xMilli,
			yMilli: value.yMilli,
			zMilli: value.zMilli,
			yawDegrees: value.yawDegrees
		};

	/** Fold the complete immutable plan trace into one compact comparison value. */
	static function digestResolvedTrace(trace:ResolvedLevelSemanticTrace):Int {
		var digest = CaxecraftTrace.mix(trace.scenarioDigest, trace.worldCells);
		digest = CaxecraftTrace.mix(digest, trace.terrainRuns);
		digest = CaxecraftTrace.mix(digest, trace.terrainDigest);
		digest = CaxecraftTrace.mix(digest, trace.fluids);
		digest = CaxecraftTrace.mix(digest, trace.fluidDigest);
		digest = CaxecraftTrace.mix(digest, trace.items);
		digest = CaxecraftTrace.mix(digest, trace.itemDigest);
		digest = CaxecraftTrace.mix(digest, trace.statefulObjects);
		digest = CaxecraftTrace.mix(digest, trace.statefulObjectDigest);
		digest = CaxecraftTrace.mix(digest, trace.actors);
		digest = CaxecraftTrace.mix(digest, trace.actorDigest);
		digest = CaxecraftTrace.mix(digest, trace.flowBindings);
		digest = CaxecraftTrace.mix(digest, trace.flowDigest);
		return CaxecraftTrace.mix(digest, trace.playerDigest);
	}
}
