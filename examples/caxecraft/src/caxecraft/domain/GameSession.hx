package caxecraft.domain;

import caxecraft.domain.Aquatics.observe as observeAquatics;
import caxecraft.domain.ActorControllerDecision.ActorControllerDecision;
import caxecraft.domain.ActorControllerScheduler.acceptsMeleeDamage as controllerAcceptsMeleeDamage;
import caxecraft.domain.ActorControllerScheduler.interactionAvailable as controllerInteractionAvailable;
import caxecraft.domain.ActorControllerScheduler.planActorController;
import caxecraft.domain.ActorControllerScheduler.startActorController;
import caxecraft.domain.ActorControllerTick.ActorControllerTickResult;
import caxecraft.domain.ActorControllerTick.ActorControllerTickStatus;
import caxecraft.domain.Character.adoptProfile as adoptCharacterProfile;
import caxecraft.domain.Character.applyAttack as applyCharacterAttack;
import caxecraft.domain.Character.applyDamage as applyCharacterDamage;
import caxecraft.domain.Character.isValid as isValidCharacter;
import caxecraft.domain.Character.reviveAt as reviveCharacterAt;
import caxecraft.domain.Character.stepWithCollisions as advanceCharacterState;
import caxecraft.domain.Character.withVitals as withCharacterVitals;
import caxecraft.domain.CharacterPhysics.collisionBox as characterCollisionBox;
import caxecraft.domain.PlayerCamera.PlayerCameraMode;
import caxecraft.domain.PlayerCamera.PlayerCameraView;
import caxecraft.domain.PlayerCamera.resolvePlayerCamera;
import caxecraft.domain.PlayerAgent.bind as bindPlayerAgent;
import caxecraft.domain.WaterCellCodec.isSolidCode as isSolidStorageCode;
import caxecraft.gameplay.AuthoredItemSlots;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.InventoryState;
import caxecraft.gameplay.ItemKind;
import caxecraft.gameplay.Mining.attempt as attemptMining;
import caxecraft.gameplay.Mining.itemForCollectableBlock;
import caxecraft.gameplay.MiningOutcome;
import caxecraft.gameplay.MiningResult;
import caxecraft.gameplay.Recovery.applyInventory as applyRecoveryInventory;
import caxecraft.gameplay.Recovery.applyVitals as applyRecoveryVitals;
import caxecraft.gameplay.Recovery.decide as decideRecovery;
import caxecraft.gameplay.RecoveryDecision;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowEventContext;
import caxecraft.scenario.CaxeFlow.FlowEventOccurrence;
import caxecraft.scenario.CaxeFlow.FlowEventPosition;
import caxecraft.scenario.CaxeFlowExecutor;
import caxecraft.domain.GameSessionFlowSnapshot.GameSessionFlowSnapshot;
import caxecraft.domain.GameSessionFlowSnapshot.TriggerActorSnapshot;
import caxecraft.domain.GameSessionFlowSnapshot.TriggerMembershipSnapshot;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventOccurrence;
import caxecraft.scenario.CaxeFlowEventRegistry.externalFlowEventCapacity as availableExternalFlowEvents;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickResult;
import caxecraft.scenario.CaxeFlowRuntime.FlowPosition;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioLimits;
import caxecraft.domain.Vitals.isDefeated as characterVitalsDefeated;
#if c
import c.CArray;
import c.ConstSpan;
import c.UInt8;
#end

/**
	All controller-neutral input required for one exact simulation tick.

	Input devices, artificial intelligence, cutscenes, and tests can all produce
	this same value. The session therefore knows what the character intends to do,
	not which controller produced that intent.
**/
typedef GameTickInput = {
	final intent:CharacterIntent;
	final damagePolicy:CharacterDamagePolicy;
	final waterUpdateBudget:Int;
}

/**
	The committed semantic result of one exact simulation tick.

	Rendering consumes `immersion` without rerunning physics. `committed` is false
	only when the requested character was missing or its stable identity changed;
	the application treats that as an ownership defect and exits cleanly.
**/
typedef GameTickResult = {
	/** Zero-based index of this committed step, or `-1` when the tick was rejected. */
	final tickIndex:Int;

	final character:Character;
	final immersion:Immersion;
	final drowningDamage:Int;
	final water:WaterTickResult;

	/** Authored-rule observations committed after mechanics, or null before a flow is installed. */
	final flow:Null<FlowTickResult>;

	final committed:Bool;
}

/**
	The post-state of one semantic command targeting the session's local character.

	`resolved` means the session found its bound character and either committed the
	calculated replacement or completed an intentional no-op. A false value is an
	ownership failure, so callers keep the returned pre-command snapshot and stop
	instead of guessing which state became authoritative.
**/
typedef LocalCharacterCommandResult = {
	final character:Character;
	final resolved:Bool;
}

/**
	Result of applying an already rate-limited damage amount to any character.

	`damageApplied` reports the committed health difference. `defeated` is true
	only for the impact that changed a living character into a defeated one, so
	content can request a drop once without examining mutable storage.
**/
typedef CharacterDamageResult = {
	final character:Character;
	final damageApplied:Int;
	final defeated:Bool;
	final resolved:Bool;
}

/** Closed provenance for damage that authored rules are allowed to observe. */
enum CharacterDamageCause {
	/** The locally controlled actor committed a hit with this validated item. */
	LocalPlayerItem(itemType:ContentId);

	/** One authored actor committed damage without an inventory item. */
	AuthoredActor(actor:ScenarioId);

	/** Terrain, fluid, a trap, or another actor-less mechanic committed damage. */
	Environment;
}

/** Named input for one authoritative damage transaction. */
typedef CharacterDamageCommand = {
	final target:EntityId;
	final amount:Int;
	final cause:CharacterDamageCause;
}

/**
	The result of advancing any stored character through shared simulation rules.

	`resolved` is false when the stable ID is missing or the calculated
	replacement cannot be committed. Player, non-player, cutscene, and test
	controllers can therefore use one movement/aquatics/vitals path without
	receiving direct access to `EntityStore`.
**/
typedef CharacterCommandStepResult = {
	final character:Character;
	final immersion:Immersion;
	final drowningDamage:Int;
	final resolved:Bool;
}

/**
	The atomic result of trying the selected inventory item's recovery behavior.

	The session commits health before it publishes the matching inventory value.
	If the character is unavailable, `resolved` is false and both returned values
	are unchanged. Ordinary decisions such as `HealthAlreadyFull` are successfully
	resolved no-ops, so presentation can explain them without treating them as an
	engine failure.
**/
typedef LocalRecoveryResult = {
	final decision:RecoveryDecision;
	final inventory:InventoryState;
	final character:Character;
	final resolved:Bool;
}

/**
	The result of collecting one authored aquatic-equipment item.

	`collected` is false for an invalid or already inactive item slot. `resolved`
	separately states whether the session still owned a valid local character, so a
	normal failed pickup cannot be confused with broken entity ownership.
**/
typedef AuthoredAquaticEquipmentResult = {
	final character:Character;
	final collected:Bool;
	final resolved:Bool;
}

/**
	The result of moving one authored world item into the player inventory.

	The operation collects the complete authored stack or leaves both owners
	unchanged. `resolved` is false only for invalid input. A full inventory is a
	valid no-op and returns zero in `collected`.
**/
typedef AuthoredInventoryItemResult = {
	final inventory:InventoryState;
	final collected:Int;
	final resolved:Bool;
}

/**
	Owns the mutable, controller-neutral state of one loaded game simulation.

	`EntityStore` and `WaterSimulation` are real child objects embedded directly
	inside this session by haxe.c. The session owns the world and authored-item
	flags; the water child owns the queue that must agree with its scheduler
	counters. The child objects have one stable lifetime and cannot escape as
	independent owned values. Embedding them needs no separate class allocation;
	`EntityStore`'s ordinary Haxe Array separately owns a managed resizable buffer.
	This is ordinary Haxe composition; the compiler selects the safe, readable C
	representation and its exact runtime support.

	A class is used instead of a record because the session is the stable mutable
	owner: callers must update and observe the same loaded simulation across many
	frames. Making it `final` documents that variation belongs in composed systems
	and content, not hidden session subclasses. Haxe.c emits one parent C struct
	with direct child storage and pointer-receiver methods.

	The C build uses compact `CArray` fields for the fixed world and water work
	buffers. Eval uses ordinary Haxe arrays for those fields as an independent
	behavior oracle. Haxe removes the inactive representation branch at compile
	time; simulation methods below are shared. Public readers receive a zero-copy
	`WorldView` that cannot write and is valid only for the direct call. World and
	item mutations remain session commands, so the application cannot bypass water
	scheduling or retain a mutable storage view.
**/
final class GameSession {
	/** All live character state, owned for exactly this session's lifetime. */
	final entities:EntityStore = new EntityStore();

	/** Controller runtime snapshots for authored non-player characters, in order. */
	var actorControllers:Array<ActorControllerState> = [];

	/** Typed observations emitted by the most recent authored-controller pass. */
	final actorControllerEvents:Array<ActorControllerEvent> = [];

	/** Human-control binding; it names the store entry and owns no character copy. */
	var localPlayer:PlayerAgent;

	/** Number of fixed simulation steps that this session has committed. */
	var completedTicks:Int = 0;

	/** Validated authored rules sharing this session's fixed simulation clock. */
	var flowExecutor:Null<CaxeFlowExecutor> = null;

	/** Known block storage bytes for the session-owned Flow content index. */
	var flowBlockStorageCodes:Array<Int> = [];

	/** Stable block IDs paired with `flowBlockStorageCodes` in byte order. */
	var flowBlockContentIds:Array<ContentId> = [];

	/** Runtime actor identities paired with their stable authored identities. */
	var authoredActorEntities:Array<EntityId> = [];

	/** Stable CAXEMAP identities in the same order as `authoredActorEntities`. */
	var authoredActorIds:Array<ScenarioId> = [];

	/** Validated content IDs in the same slot order as authored-item activity. */
	var authoredItemContentIds:Array<ContentId> = [];

	/** Stable authored IDs for generic interactables in deterministic load order. */
	var statefulObjectIds:Array<ScenarioId> = [];

	/** Interaction centers in integer thousandths of a block, stored as x/y/z triples. */
	var statefulObjectPositionsMilli:Array<Int> = [];

	/** Maximum interaction distance paired with each `statefulObjectIds` entry. */
	var statefulObjectRadiiMilli:Array<Int> = [];

	/** Width, height, depth, and cardinal yaw for each object's authored box. */
	var statefulObjectBoundsMilli:Array<Int> = [];

	/** First flattened state-mechanics slot for each generic object. */
	var statefulObjectStateStarts:Array<Int> = [];

	/** Number of flattened state-mechanics slots for each generic object. */
	var statefulObjectStateCounts:Array<Int> = [];

	/** Compatible CaxeFlow state IDs in object-major canonical order. */
	var statefulObjectCollisionStates:Array<ContentId> = [];

	/** One zero/passable or one/solid flag per flattened state slot. */
	var statefulObjectCollisionSolid:Array<Int> = [];

	/** Solid boxes rebuilt from the authoritative CaxeFlow state after each tick. */
	var activeStatefulCollision:Array<DynamicCollisionBox> = [];

	/** Stateful solids plus stationary actors that the local player cannot enter. */
	var activeLocalPlayerCollision:Array<DynamicCollisionBox> = [];

	/** Semantic events waiting for the next successfully committed fixed tick. */
	var pendingFlowEvents:Array<FlowEventOccurrence> = [];

	/** Stable CAXEMAP identity of the locally controlled player spawn. */
	var localPlayerAuthoredId:Null<ScenarioId> = null;

	/** Stable authored identities for trigger zones in deterministic map order. */
	var triggerZoneIds:Array<ScenarioId> = [];

	/** Integer x/y/z origin and width/height/depth for each half-open zone. */
	var triggerZoneBounds:Array<Int> = [];

	/** Stable runtime identities observed for zone crossings, local player first. */
	var triggerActorEntities:Array<EntityId> = [];

	/** Stable CAXEMAP identities paired with `triggerActorEntities`. */
	var triggerActorIds:Array<ScenarioId> = [];

	/** Last membership per zone/actor pair, in zone-major deterministic order. */
	var triggerZoneInside:Array<Int> = [];

	/** Previous integer-milliblock position per trigger actor. */
	var triggerActorPrevious:Array<Int> = [];

	/** One zero/uninitialized or one/initialized flag per trigger actor. */
	var triggerActorPreviousValid:Array<Int> = [];

	/** Deterministic water work state, shared by loading and fixed simulation. */
	final water:WaterSimulation = new WaterSimulation();

	#if c
	/** Compact voxel storage embedded directly in the generated C session struct. */
	final worldStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);

	/** Loaded authored-item activity flags, bounded by the engine capacity. */
	final authoredItemStorage:CArray<Int, AuthoredItemSlots> = CArray.zero(AuthoredItemSlots.CAPACITY);
	#else

	/** Eval's behavior-oracle carrier for the same fixed world semantics. */
	final worldStorage:Array<Int> = [];

	/** Eval's behavior-oracle carrier for authored-item activity flags. */
	final authoredItemStorage:Array<Int> = [];
	#end

	/** Construct zeroed storage before validated level content populates it. */
	public function new() {
		localPlayer = bindPlayerAgent(EntityId.invalid());
		#if !c
		var worldIndex = 0;
		while (worldIndex < World.VOLUME) {
			worldStorage.push(0);
			worldIndex++;
		}
		var itemIndex = 0;
		while (itemIndex < AuthoredItemSlots.CAPACITY) {
			authoredItemStorage.push(0);
			itemIndex++;
		}
		#end
		water.resetPending();
	}

	/**
		Attach the validated authored rule model before this candidate can publish.

		Only `RuntimeLevelLoader` receives this authority. The loader has already
		parsed and validated the complete scenario. Actor and item identities use the
		same stable slot order as the session so later events can report authored IDs.
		Every candidate owns a fresh session, so a second installation is an internal
		construction defect rather than recoverable content input.
	**/
	@:allow(caxecraft.content.RuntimeLevelLoader)
	private function installValidatedScenarioFlow(scenario:Scenario, registry:ScenarioContentRegistry, playerId:ScenarioId, actorEntities:Array<EntityId>,
			actorIds:Array<ScenarioId>, itemContentIds:Array<ContentId>, objectIds:Array<ScenarioId>, objectPositionsMilli:Array<Int>,
			objectRadiiMilli:Array<Int>, objectBoundsMilli:Array<Int>, objectStateStarts:Array<Int>, objectStateCounts:Array<Int>,
			objectCollisionStates:Array<ContentId>, objectCollisionSolid:Array<Int>, zoneIds:Array<ScenarioId>, zoneBounds:Array<Int>):Void {
		if (flowExecutor != null)
			throw "CaxeFlow is already installed for this GameSession";
		if (actorEntities.length != actorIds.length || actorEntities.length != actorControllers.length)
			throw "CaxeFlow actor bindings do not match this GameSession";
		for (index in 0...actorEntities.length)
			if (actorEntities[index] != actorControllers[index].characterId)
				throw "CaxeFlow actor binding order does not match this GameSession";
		if (itemContentIds.length > AuthoredItemSlots.CAPACITY)
			throw "CaxeFlow item bindings exceed this GameSession";
		if (objectPositionsMilli.length != objectIds.length * 3 || objectRadiiMilli.length != objectIds.length)
			throw "CaxeFlow stateful-object bindings do not match this GameSession";
		if (objectBoundsMilli.length != objectIds.length * 4
			|| objectStateStarts.length != objectIds.length
			|| objectStateCounts.length != objectIds.length
			|| objectCollisionStates.length != objectCollisionSolid.length)
			throw "CaxeFlow stateful-object collision bindings do not match this GameSession";
		for (radius in objectRadiiMilli)
			if (radius < 0)
				throw "CaxeFlow stateful-object interaction range is invalid";
		var expectedStateStart = 0;
		for (objectIndex in 0...objectIds.length) {
			final boundsOffset = objectIndex * 4;
			final yaw = objectBoundsMilli[boundsOffset + 3];
			if (objectBoundsMilli[boundsOffset] <= 0
				|| objectBoundsMilli[boundsOffset + 1] <= 0
				|| objectBoundsMilli[boundsOffset + 2] <= 0)
				throw "CaxeFlow stateful-object collision bounds are invalid";
			if (yaw != 0 && yaw != 90 && yaw != 180 && yaw != 270)
				throw "CaxeFlow stateful-object collision yaw is invalid";
			if (objectStateStarts[objectIndex] != expectedStateStart || objectStateCounts[objectIndex] <= 0)
				throw "CaxeFlow stateful-object collision state range is invalid";
			expectedStateStart += objectStateCounts[objectIndex];
			if (expectedStateStart > objectCollisionStates.length)
				throw "CaxeFlow stateful-object collision state range exceeds its values";
		}
		if (expectedStateStart != objectCollisionStates.length)
			throw "CaxeFlow stateful-object collision states are not completely owned";
		for (solid in objectCollisionSolid)
			if (solid != 0 && solid != 1)
				throw "CaxeFlow stateful-object collision flag is invalid";
		flowBlockStorageCodes.resize(0);
		flowBlockContentIds.resize(0);
		for (code in 0...256) {
			final blockType = registry.blockContentIdForStorageCode(code);
			if (blockType != null) {
				if (registry.blockStorageCode(blockType) != code)
					throw "CaxeFlow block content index is not reversible";
				flowBlockStorageCodes.push(code);
				flowBlockContentIds.push(blockType);
			}
		}
		if (zoneBounds.length != zoneIds.length * 6)
			throw "CaxeFlow trigger-zone bindings do not match this GameSession";
		for (zoneIndex in 0...zoneIds.length) {
			final offset = zoneIndex * 6;
			if (zoneBounds[offset + 3] <= 0 || zoneBounds[offset + 4] <= 0 || zoneBounds[offset + 5] <= 0)
				throw "CaxeFlow trigger-zone bounds are invalid";
		}
		authoredActorEntities = actorEntities.copy();
		authoredActorIds = actorIds.copy();
		localPlayerAuthoredId = playerId;
		authoredItemContentIds = itemContentIds.copy();
		statefulObjectIds = objectIds.copy();
		statefulObjectPositionsMilli = objectPositionsMilli.copy();
		statefulObjectRadiiMilli = objectRadiiMilli.copy();
		statefulObjectBoundsMilli = objectBoundsMilli.copy();
		statefulObjectStateStarts = objectStateStarts.copy();
		statefulObjectStateCounts = objectStateCounts.copy();
		statefulObjectCollisionStates = objectCollisionStates.copy();
		statefulObjectCollisionSolid = objectCollisionSolid.copy();
		triggerZoneIds = zoneIds.copy();
		triggerZoneBounds = zoneBounds.copy();
		triggerActorEntities.resize(0);
		triggerActorIds.resize(0);
		triggerActorEntities.push(localPlayer.characterId);
		triggerActorIds.push(playerId);
		for (index in 0...actorEntities.length) {
			triggerActorEntities.push(actorEntities[index]);
			triggerActorIds.push(actorIds[index]);
		}
		triggerZoneInside.resize(0);
		for (_ in 0...triggerZoneIds.length * triggerActorIds.length)
			triggerZoneInside.push(0);
		triggerActorPrevious.resize(0);
		triggerActorPreviousValid.resize(0);
		for (_ in triggerActorIds) {
			triggerActorPrevious.push(0);
			triggerActorPrevious.push(0);
			triggerActorPrevious.push(0);
			triggerActorPreviousValid.push(0);
		}
		flowExecutor = new CaxeFlowExecutor(scenario, registry);
		pendingFlowEvents.push(flowEventOccurrence(FlowEvent.LevelEntered(scenario.id)));
		refreshStatefulCollision();
	}

	/**
		Queue one committed semantic fact for authored rules on the next game tick.

		Input, artificial-intelligence, and mechanics owners call this only after the
		fact actually happened. A bounded false result leaves the queue unchanged, so
		the caller can stop instead of silently dropping campaign progression.
	**/
	public function queueFlowEvent(event:FlowEventOccurrence):Bool {
		final executor = flowExecutor;
		if (executor == null
			|| executor.fault() != null
			|| pendingFlowEvents.length >= externalFlowEventCapacity()
			|| !executor.acceptsRuntimeEvent(event))
			return false;
		pendingFlowEvents.push(event);
		return true;
	}

	/**
		Capture executor and spatial trigger state for the wider save-game owner.

		Character bodies and world cells are intentionally outside this value. The
		game-session persistence layer owns those facts and must restore them before
		advancing the first tick.
	**/
	public function caxeFlowSnapshot():Null<GameSessionFlowSnapshot> {
		final executor = flowExecutor;
		if (executor == null)
			return null;
		final actors:Array<TriggerActorSnapshot> = [];
		for (actorIndex in 0...triggerActorIds.length) {
			if (triggerActorPreviousValid[actorIndex] == 0)
				actors.push({id: triggerActorIds[actorIndex], previous: null});
			else
				actors.push({id: triggerActorIds[actorIndex], previous: readTriggerActorPosition(actorIndex)});
		}
		final memberships:Array<TriggerMembershipSnapshot> = [];
		for (zoneIndex in 0...triggerZoneIds.length)
			for (actorIndex in 0...triggerActorIds.length) {
				final index = zoneIndex * triggerActorIds.length + actorIndex;
				memberships.push({
					zone: triggerZoneIds[zoneIndex],
					actor: triggerActorIds[actorIndex],
					inside: triggerZoneInside[index] != 0
				});
			}
		return {
			executor: executor.snapshot(),
			pendingEvents: pendingFlowEvents.copy(),
			actors: actors,
			memberships: memberships
		};
	}

	/**
		Restore one validated Flow snapshot without inventing spatial transitions.

		Stable actor and zone IDs must exactly match this freshly loaded session. The
		executor restores last through its atomic candidate path; false leaves all
		game-session spatial arrays and queued events unchanged.
	**/
	public function restoreCaxeFlowSnapshot(snapshot:GameSessionFlowSnapshot):Bool {
		final executor = flowExecutor;
		if (executor == null
			|| snapshot.pendingEvents.length > externalFlowEventCapacity()
			|| snapshot.actors.length != triggerActorIds.length
			|| snapshot.memberships.length != triggerZoneIds.length * triggerActorIds.length)
			return false;
		for (actorIndex in 0...snapshot.actors.length)
			if (snapshot.actors[actorIndex].id.text() != triggerActorIds[actorIndex].text())
				return false;
		for (index in 0...snapshot.memberships.length) {
			final zoneIndex = Std.int(index / triggerActorIds.length);
			final actorIndex = index - zoneIndex * triggerActorIds.length;
			final value = snapshot.memberships[index];
			if (value.zone.text() != triggerZoneIds[zoneIndex].text() || value.actor.text() != triggerActorIds[actorIndex].text())
				return false;
		}
		for (event in snapshot.pendingEvents)
			if (!executor.acceptsRuntimeEvent(event))
				return false;
		if (!executor.restore(snapshot.executor))
			return false;
		final restoredPrevious:Array<Int> = [];
		final restoredPreviousValid:Array<Int> = [];
		for (actor in snapshot.actors) {
			final previous = actor.previous;
			if (previous == null) {
				restoredPrevious.push(0);
				restoredPrevious.push(0);
				restoredPrevious.push(0);
				restoredPreviousValid.push(0);
			} else {
				restoredPrevious.push(previous.xMilli);
				restoredPrevious.push(previous.yMilli);
				restoredPrevious.push(previous.zMilli);
				restoredPreviousValid.push(1);
			}
		}
		final restoredInside:Array<Int> = [for (membership in snapshot.memberships) membership.inside ? 1 : 0];
		pendingFlowEvents = snapshot.pendingEvents.copy();
		triggerActorPrevious = restoredPrevious;
		triggerActorPreviousValid = restoredPreviousValid;
		triggerZoneInside = restoredInside;
		refreshStatefulCollision();
		return true;
	}

	/**
		Reserve the spatial worst case before engine adapters queue other events.

		Validated scenarios always retain at least one slot for a non-spatial event.
		This makes every committed movement segment observable on its exact tick.
	**/
	function externalFlowEventCapacity():Int {
		return availableExternalFlowEvents(triggerZoneIds.length, triggerActorIds.length);
	}

	/**
		Commit one available actor interaction as a typed authored event.

		The session checks the same range rule that controls the interaction prompt.
		It then converts the numeric runtime identity to the matching CAXEMAP identity.
		A false result changes no rule state and means that no valid interaction was
		available or the bounded event queue could not accept the event.
	**/
	public function interactWithActor(id:EntityId):Bool {
		if (!actorInteractionAvailable(id))
			return false;
		for (index in 0...authoredActorEntities.length)
			if (authoredActorEntities[index] == id)
				return queueLocalActorEvent(FlowEvent.Interact(authoredActorIds[index]));
		return false;
	}

	/**
		Commit one nearby generic-object interaction as an authored event.

		CaxeFlow owns whether the object is active and what state it has. The session
		owns proximity to the current player. This command joins those two facts and
		queues the same typed `Interact` event used by other authored rules.
	**/
	public function interactWithStatefulObject(id:ScenarioId):Bool
		return statefulObjectInteractionAvailable(id) && queueLocalActorEvent(FlowEvent.Interact(id));

	/** Queue one player-owned event only after validated gameplay commits it. */
	function queueLocalActorEvent(source:FlowEvent):Bool {
		final actor = localPlayerAuthoredId;
		return actor != null && queueFlowEvent(flowEventOccurrence(source, ActorEventContext(actor)));
	}

	/**
		Validate one event batch before its matching gameplay transaction commits.

		A batch is all-or-nothing. This matters when one block lies in overlapping
		zones or one sword hit emits both use-item and entity-defeated events.
	**/
	function flowEventsCanCommit(events:Array<FlowEventOccurrence>):Bool {
		if (events.length == 0)
			return true;
		final executor = flowExecutor;
		if (executor == null || executor.fault() != null || pendingFlowEvents.length + events.length > externalFlowEventCapacity())
			return false;
		for (event in events)
			if (!executor.acceptsRuntimeEvent(event))
				return false;
		return true;
	}

	/** Append one already validated batch after its gameplay transaction commits. */
	function commitFlowEvents(events:Array<FlowEventOccurrence>):Void
		for (event in events)
			pendingFlowEvents.push(event);

	/** Build one actor-owned event, or reject a missing local authored identity. */
	function localActorOccurrence(source:FlowEvent):Null<FlowEventOccurrence> {
		if (flowExecutor == null)
			return null;
		final actor = localPlayerAuthoredId;
		return actor == null ? null : flowEventOccurrence(source, ActorEventContext(actor));
	}

	/**
		Build deterministic block-change events for every containing trigger zone.

		Only player-authoritative mine, remove, and place commands call this adapter.
		Water settling and content loading are system state changes, not player facts.
	**/
	function blockChangedOccurrences(coord:BlockCoord, kind:BlockKind):Null<Array<FlowEventOccurrence>> {
		if (flowExecutor == null)
			return [];
		final actor = localPlayerAuthoredId;
		if (actor == null)
			return null;
		final blockType = flowBlockContentId(World.kindCode(kind));
		if (blockType == null)
			return null;
		final events:Array<FlowEventOccurrence> = [];
		for (zoneIndex in 0...triggerZoneIds.length)
			if (triggerZoneContainsBlock(zoneIndex, coord))
				events.push(flowEventOccurrence(FlowEvent.BlockChanged(triggerZoneIds[zoneIndex], blockType), ActorEventContext(actor)));
		return events;
	}

	/** Resolve one compact world byte through the index copied during level load. */
	function flowBlockContentId(code:Int):Null<ContentId> {
		for (index in 0...flowBlockStorageCodes.length)
			if (flowBlockStorageCodes[index] == code)
				return flowBlockContentIds[index];
		return null;
	}

	/** Build a validated local item-use occurrence when Flow is installed. */
	function itemUseOccurrences(itemType:Null<ContentId>):Null<Array<FlowEventOccurrence>> {
		if (flowExecutor == null)
			return [];
		if (itemType == null)
			return null;
		final occurrence = localActorOccurrence(FlowEvent.UseItem(itemType));
		return occurrence == null ? null : [occurrence];
	}

	/** True when one active generic object is within its content-defined range. */
	public function statefulObjectInteractionAvailable(id:ScenarioId):Bool {
		final executor = flowExecutor;
		if (executor == null || !executor.objectActive(id))
			return false;
		final player = readLocalPlayer();
		if (!isValidCharacter(player))
			return false;
		for (index in 0...statefulObjectIds.length)
			if (statefulObjectIds[index] == id) {
				final offset = index * 3;
				final dx:Float = player.body.x * 1000.0 - statefulObjectPositionsMilli[offset];
				final dy:Float = player.body.y * 1000.0 - statefulObjectPositionsMilli[offset + 1];
				final dz:Float = player.body.z * 1000.0 - statefulObjectPositionsMilli[offset + 2];
				final radius:Float = statefulObjectRadiiMilli[index];
				if (radius <= 0.0)
					return false;
				return dx * dx + dy * dy + dz * dz <= radius * radius;
			}
		return false;
	}

	/** Observe the current CaxeFlow state without exposing its mutable owner. */
	public function statefulObjectState(id:ScenarioId):Null<ContentId> {
		final executor = flowExecutor;
		return executor == null ? null : executor.objectState(id);
	}

	/** Observe whether CaxeFlow currently admits an object to interaction and drawing. */
	public function statefulObjectIsActive(id:ScenarioId):Bool {
		final executor = flowExecutor;
		return executor != null && executor.objectActive(id);
	}

	/** Number of solid authored boxes active for the current committed flow state. */
	public inline function activeStatefulCollisionCount():Int
		return activeStatefulCollision.length;

	/**
	 * Resolve a presentation camera against the current committed collision set.
	 *
	 * This read cannot mutate simulation or replace the gameplay interaction ray.
	 * Stateful doors and mechanisms use the same active boxes as movement.
	 */
	public function playerCamera(mode:PlayerCameraMode, body:CharacterBody, lookX:Float, lookY:Float, lookZ:Float):PlayerCameraView
		return resolvePlayerCamera(worldView(), activeStatefulCollision, mode, body, lookX, lookY, lookZ);

	/**
		Install the one locally controlled character and bind human input to its ID.

		`EntityStore.put` completes before the binding changes, so a rejected
		character leaves the session unbound. The small `PlayerAgent` stores only the
		accepted identity; it never creates a second body, vitals, or aquatic state.
	**/
	public function bindLocalPlayer(character:Character):Bool {
		if (!entities.put(character))
			return false;
		localPlayer = bindPlayerAgent(character.id);
		return true;
	}

	/**
		Add one non-player or not-yet-controlled character to this simulation.

		Validated level composition supplies the stable identity and component
		snapshot. The session delegates duplicate and capacity checks to its sole
		`EntityStore`; no role name or content ID changes storage behavior.
	**/
	public inline function addCharacter(character:Character):Bool
		return entities.put(character);

	/** True only when the binding still names the character owned by this session. */
	public inline function hasLocalPlayer():Bool
		return localPlayer.characterId.isValid() && entities.contains(localPlayer.characterId);

	/** Read the committed local-character snapshot without exposing its store key. */
	public inline function readLocalPlayer():Character
		return entities.read(localPlayer.characterId);

	/**
		Read any committed character snapshot by stable identity.

		This remains a normal method rather than `inline` so split generated C
		keeps the `EntityStore` read inside `GameSession.c`. Callers receive the
		same immutable value either way, but preserving this function boundary
		makes the session's storage ownership visible to a C reader and prevents
		presentation code from growing a copied storage access.
	**/
	public function readCharacter(id:EntityId):Character
		return entities.read(id);

	/** Number of committed characters in deterministic insertion order. */
	public inline function characterCount():Int
		return entities.count();

	/**
		Publish copy-owned character snapshots for saves, tests, and schedulers.

		The returned Array may be changed by its caller without changing the live
		store. Its `Character` elements are immutable value snapshots, so this
		observation grants no simulation mutation authority.
	**/
	public inline function characterSnapshots():Array<Character>
		return entities.snapshots();

	/** Return copy-owned controller bindings in the same order as authored actors. */
	public function actorControllerSnapshots():Array<ActorControllerBinding> {
		final result:Array<ActorControllerBinding> = [];
		for (controller in actorControllers)
			result.push({characterId: controller.characterId, profile: controller.profile});
		return result;
	}

	/** Return copy-owned controller execution state in stable authored order. */
	public function actorControllerStateSnapshots():Array<ActorControllerState>
		return actorControllers.copy();

	/** Return copy-owned typed events from the most recent controller pass. */
	public function actorControllerEventSnapshots():Array<ActorControllerEvent>
		return actorControllerEvents.copy();

	/**
		Observe one actor's current stationary-interaction offer without advancing it.

		The lookup uses the session-owned controller and character snapshots plus the
		current local player. This lets input sampled before a fixed tick use the same
		typed range rule as the scheduler; presentation does not recreate a guide
		controller or gain access to mutable actor storage.
	**/
	public function actorInteractionAvailable(id:EntityId):Bool {
		final actor = readCharacter(id);
		final player = readLocalPlayer();
		for (controller in actorControllers)
			if (controller.characterId == id)
				return controllerInteractionAvailable(controller, actor, player);
		return false;
	}

	/**
		Atomically replace authored non-player characters and their controllers.

		Both input arrays are copied and cross-checked before `EntityStore` changes.
		After that preflight no remaining operation can reject. The synchronous
		commit invokes no callback and exposes no intermediate observation, so a
		caller receives either rejection with the old state or success with the
		complete new state.
	**/
	public function replaceAuthoredActors(characters:Array<Character>, controllers:Array<ActorControllerBinding>):Bool {
		if (!hasLocalPlayer() || characters.length != controllers.length)
			return false;
		final ownedCharacters = characters.copy();
		final ownedControllers = controllers.copy();
		final ownedStates:Array<ActorControllerState> = [];
		for (index in 0...ownedCharacters.length)
			if (ownedControllers[index].characterId != ownedCharacters[index].id)
				return false;
			else
				ownedStates.push(startActorController(ownedControllers[index], ownedCharacters[index]));
		if (!entities.replaceOthers(localPlayer.characterId, ownedCharacters))
			return false;
		while (actorControllers.length > 0)
			actorControllers.pop();
		for (state in ownedStates)
			actorControllers.push(state);
		clearActorControllerEvents();
		return true;
	}

	/**
		Advance every published actor controller once in stable authored order.

		All decisions observe the same local-player snapshot from the tick start.
		Each accepted intent then enters `stepCharacter`, so player, NPC, enemy,
		cutscene, and test movement share collision, water, breath, and vitals rules.
		The method stops at the first ownership/model/command failure; the returned
		processed count names the valid committed prefix.
	**/
	public function stepAuthoredActorControllers(tickNumber:Int, damagePolicy:CharacterDamagePolicy):ActorControllerTickResult {
		clearActorControllerEvents();
		final observedLocalPlayer = readLocalPlayer();
		var processed = 0;
		for (index in 0...actorControllers.length) {
			final state = actorControllers[index];
			final character = readCharacter(state.characterId);
			if (!isValidCharacter(character))
				return actorControllerTick(ControlledCharacterMissing(state.characterId), processed);
			final decision = planActorController(state, character, observedLocalPlayer, tickNumber);
			switch decision {
				case ControllerPlanRejected(error):
					return actorControllerTick(ControllerModelRejected(state.characterId, error), processed);
				case ControllerPlanned(next, intent, event):
					switch event {
						case LocalPlayerAttack(source):
							final attack = receiveLocalPlayerAttack();
							if (!attack.resolved) return actorControllerTick(LocalAttackCommandRejected(source), processed);
						case NoControllerEvent | InteractionAvailable(_) | DropRequested(_, _):
					}
					final step = stepCharacter(state.characterId, intent, damagePolicy);
					if (!step.resolved)
						return actorControllerTick(CharacterCommandRejected(state.characterId), processed);
					actorControllers[index] = next;
					switch event {
						case NoControllerEvent:
						case InteractionAvailable(_) | LocalPlayerAttack(_) | DropRequested(_, _):
							actorControllerEvents.push(event);
					}
					processed++;
			}
		}
		return actorControllerTick(ControllersAdvanced, processed);
	}

	/**
		Remove a non-player character by stable identity.

		The locally controlled character cannot disappear through this generic
		command because input and presentation still name it. A dedicated
		level-transition or player-rebinding operation must settle that ownership
		first.
	**/
	public function removeCharacter(id:EntityId):Bool
		return id != localPlayer.characterId && entities.remove(id);

	/**
		Advance any stored character against the current world and commit the result.

		This operation runs the same `Character.step` function as the local fixed
		tick. It intentionally does not advance water or the session clock: the
		caller is a deterministic controller/scheduler operating inside one fixed
		tick boundary. The later actor scheduler can call this for every admitted
		intent without creating player- and NPC-specific physics.
	**/
	public function stepCharacter(id:EntityId, intent:CharacterIntent, damagePolicy:CharacterDamagePolicy):CharacterCommandStepResult {
		#if c
		var readCells:WorldView = worldStorage.constSpan();
		#else
		var readCells:WorldView = WorldView.borrow(worldStorage);
		#end
		return stepStoredCharacter(readCells, entities.read(id), intent, damagePolicy);
	}

	/**
		Revive the bound character at one already validated placement.

		The application chooses when the return-to-spawn action is requested, but the
		session reads and replaces its own character. A later fixed tick therefore
		cannot overwrite an uncommitted application snapshot.
	**/
	public function reviveLocalPlayerAt(body:CharacterBody):LocalCharacterCommandResult {
		final original = readLocalPlayer();
		if (!isValidCharacter(original))
			return rejectedLocalCharacterCommand(original);
		return commitLocalCharacter(original, reviveCharacterAt(original, body));
	}

	/**
		Apply one confirmed hostile impact to the bound character.

		Shared `Character` and `Vitals` rules still decide safe-time and defeat. The
		session merely owns the read-calculate-commit sequence, making repeated fixed
		ticks observe the hit immediately instead of reviving an older snapshot.
	**/
	public function receiveLocalPlayerAttack():LocalCharacterCommandResult {
		final original = readLocalPlayer();
		if (!isValidCharacter(original))
			return rejectedLocalCharacterCommand(original);
		return commitLocalCharacter(original, applyCharacterAttack(original, true));
	}

	/**
		Apply one confirmed hostile impact to any character owned by this session.

		Player weapons, NPC combat, traps, and tests can share the same vitals rule
		without receiving `EntityStore` mutation authority. A missing identity
		returns `resolved == false`; an accepted attack commits through the same
		stable-ID replacement check used by fixed-tick movement.
	**/
	public function receiveCharacterAttack(id:EntityId):LocalCharacterCommandResult {
		final original = readCharacter(id);
		if (!isValidCharacter(original))
			return rejectedLocalCharacterCommand(original);
		final replacement = applyCharacterAttack(original, true);
		final resolved = entities.replace(id, replacement);
		return {character: resolved ? replacement : original, resolved: resolved};
	}

	/**
		Apply positive damage after a weapon or mechanic has admitted the impact.

		The caller owns rate limiting—for example the Copper Sword cooldown—while
		`GameSession` owns stable-ID lookup and commit. This keeps weapons reusable
		for players and non-player actors without exposing `EntityStore` or forcing
		the application to maintain a second health value.
	**/
	public function damageCharacter(command:CharacterDamageCommand):CharacterDamageResult {
		final original = readCharacter(command.target);
		if (!isValidCharacter(original) || command.amount <= 0)
			return {
				character: original,
				damageApplied: 0,
				defeated: false,
				resolved: false
			};
		final replacement = applyCharacterDamage(original, command.amount);
		final newlyDefeated = !characterVitalsDefeated(original.vitals) && characterVitalsDefeated(replacement.vitals);
		final defeatedAuthoredId = newlyDefeated ? authoredEnemyId(command.target) : null;
		final stagedEvents:Array<FlowEventOccurrence> = [];
		if (flowExecutor != null) {
			final causeActor = switch command.cause {
				case LocalPlayerItem(itemType):
					final use = localActorOccurrence(FlowEvent.UseItem(itemType));
					if (use == null)
						return rejectedCharacterDamage(original);
					stagedEvents.push(use);
					localPlayerAuthoredId;
				case AuthoredActor(actor): actor;
				case Environment: null;
			};
			if (defeatedAuthoredId != null) {
				final context = causeActor == null ? NoEventContext : ActorEventContext(causeActor);
				stagedEvents.push(flowEventOccurrence(FlowEvent.EntityDefeated(defeatedAuthoredId), context));
			}
		}
		if (!flowEventsCanCommit(stagedEvents))
			return {
				character: original,
				damageApplied: 0,
				defeated: false,
				resolved: false
			};
		final resolved = entities.replace(command.target, replacement);
		if (!resolved)
			return {
				character: original,
				damageApplied: 0,
				defeated: false,
				resolved: false
			};
		commitFlowEvents(stagedEvents);
		return {
			character: replacement,
			damageApplied: original.vitals.health - replacement.vitals.health,
			defeated: newlyDefeated,
			resolved: true
		};
	}

	/** Return one unchanged failed damage result without duplicating its shape. */
	function rejectedCharacterDamage(original:Character):CharacterDamageResult
		return {
			character: original,
			damageApplied: 0,
			defeated: false,
			resolved: false
		};

	/** True when one living actor's content-selected phase admits a melee hit. */
	public function characterAcceptsMeleeDamage(id:EntityId):Bool {
		final character = readCharacter(id);
		for (state in actorControllers)
			if (state.characterId == id)
				return controllerAcceptsMeleeDamage(state, character);
		return false;
	}

	/** Resolve a hostile runtime identity back to its authored CAXEMAP identity. */
	function authoredEnemyId(id:EntityId):Null<ScenarioId> {
		if (flowExecutor == null)
			return null;
		for (index in 0...actorControllers.length)
			if (actorControllers[index].characterId == id)
				return switch actorControllers[index].profile {
					case StationaryDialogue(_): null;
					case WanderChaseMelee(_) | TelegraphedCharge(_): authoredActorIds[index];
				};
		return null;
	}

	/**
		Resolve recovery against the bound character and caller-owned inventory.

		Inventory has not yet joined the runtime actor composition, so it crosses this
		boundary as an immutable value. The method calculates both halves through the
		shared `Recovery` rules, commits health first, and returns the new inventory
		only after that commit succeeds. This prevents consuming berries while leaving
		health unchanged.
	**/
	public function useSelectedRecovery(inventory:InventoryState, ?itemType:ContentId):LocalRecoveryResult {
		final original = readLocalPlayer();
		if (!isValidCharacter(original)) {
			return {
				decision: RecoveryDecision.NotRecoveryItem,
				inventory: inventory,
				character: original,
				resolved: false
			};
		}
		final decision = decideRecovery(inventory, original.vitals);
		if (decision != RecoveryDecision.UseBerries) {
			return {
				decision: decision,
				inventory: inventory,
				character: original,
				resolved: true
			};
		}
		final nextInventory = applyRecoveryInventory(decision, inventory);
		final nextCharacter = withCharacterVitals(original, applyRecoveryVitals(decision, original.vitals));
		final stagedEvents = itemUseOccurrences(itemType);
		if (stagedEvents == null || !flowEventsCanCommit(stagedEvents))
			return {
				decision: decision,
				inventory: inventory,
				character: original,
				resolved: false
			};
		final committed = commitLocalCharacter(original, nextCharacter);
		if (committed.resolved)
			commitFlowEvents(stagedEvents);
		return {
			decision: decision,
			inventory: committed.resolved ? nextInventory : inventory,
			character: committed.character,
			resolved: committed.resolved
		};
	}

	/**
		Return authoritative simulation time as a count of completed fixed steps.

		The application owns real frame time and its accumulator. This counter belongs
		to the session because saves, artificial intelligence, tests, and presentation
		must all observe the same deterministic time, independent of rendering speed.
	**/
	public inline function completedTickCount():Int
		return completedTicks;

	/**
		Publish one immutable snapshot for presentation and telemetry.

		The view copies committed value state and exposes no store, span, scheduler,
		or mutation method. A caller can therefore render it without acquiring any
		authority to change the simulation.
	**/
	public function view():GameView {
		final character = entities.read(localPlayer.characterId);
		return {
			valid: isValidCharacter(character),
			localPlayer: character,
			completedTicks: completedTicks
		};
	}

	/**
		Lend the owned voxel bytes to one direct read-only consumer.

		The C result is the address and length of `worldStorage`, not a copied world.
		Haxe.c ties that borrow to this exact session receiver and rejects storing,
		re-returning, or otherwise letting it outlive the call that consumes it.
		Eval returns the same session-owned array, whose public type is still used
		only by read operations in `WorldRead`.
	**/
	public function worldView():WorldView {
		#if c
		return worldStorage.constSpan();
		#else
		return WorldView.borrow(worldStorage);
		#end
	}

	#if c
	/**
		Lend authored-item activity flags to the native renderer for one call.

		The renderer receives `const int *` plus length in generated C. Gameplay
		changes a flag only through `collectAuthoredAquaticEquipment`, so drawing
		cannot accidentally collect or restore an item.
	**/
	public function authoredItemsView():ConstSpan<Int>
		return authoredItemStorage.constSpan();
	#end

	/** Number of water cells waiting for deterministic fixed-tick work. */
	public inline function pendingWaterWork():Int
		return water.pending();

	/**
		Clear the complete level storage before a loader writes validated content.

		This is a semantic reset, not C memory access exposed to content code. It also
		clears deterministic water scheduling so no work from an older level survives.
	**/
	public function resetEmptyWorld():Void {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		var index = 0;
		while (index < World.VOLUME) {
			WorldStorage.writeCode(cells, index, 0);
			index++;
		}
		water.resetPending();
	}

	/** Write one already validated terrain cell while a level is being assembled. */
	public function replaceTerrainDuringLoad(coord:BlockCoord, kind:BlockKind):Bool {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		return World.replace(cells, coord, kind);
	}

	/** Add one permanent water source while assembling or editing a level. */
	public function placeWaterSource(coord:BlockCoord):Bool {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		return water.placeSource(cells, coord);
	}

	/**
		Write one validated compact terrain run while assembling an unpublished level.

		The resolved content plan describes terrain as ascending runs. Keeping the
		short-lived storage view here lets the loader remain ordinary Haxe without
		gaining direct access to the session's C array. Failure returns `-1` and
		performs no write; success returns the first index after the run.
	**/
	public function writeTerrainRunDuringLoad(startIndex:Int, storageCode:Int, count:Int):Int {
		// Air and the complete registered terrain range are valid level data. Use
		// the shared storage rule so adding a material cannot leave this loader on
		// an older hand-written maximum; water remains owned by the fluid loader.
		if (startIndex < 0
			|| count <= 0
			|| startIndex > World.VOLUME - count
			|| (storageCode != 0 && !isSolidStorageCode(storageCode)))
			return -1;
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		final end = startIndex + count;
		var index = startIndex;
		while (index < end) {
			WorldStorage.writeCode(cells, index, storageCode);
			index++;
		}
		return end;
	}

	/** Add one checked, non-source water box while assembling a candidate level. */
	public function placeInitialWaterVolume(origin:BlockCoord, width:Int, height:Int, depth:Int):Bool {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		return water.placeInitialVolume(cells, origin, width, height, depth);
	}

	/** Mark one validated authored-item slot active in an unpublished level. */
	public function activateAuthoredItemDuringLoad(index:Int):Bool {
		if (index < 0 || index >= AuthoredItemSlots.CAPACITY)
			return false;
		authoredItemStorage[index] = 1;
		return true;
	}

	/** Observe one item flag without lending the mutable item buffer to a caller. */
	public function authoredItemIsActive(index:Int):Bool {
		if (index < 0 || index >= AuthoredItemSlots.CAPACITY)
			return false;
		return authoredItemStorage[index] != 0;
	}

	/**
		Collect one active authored item and adopt its validated aquatic capability.

		The caller resolves content data into `replacement`; it never receives the
		item buffer. The session first proves the item and character exist, then
		reserves room for its CaxeFlow event, commits the character profile, clears the
		item flag, and queues that event. Rejection leaves the character, item, and
		flow unchanged.
	**/
	public function collectAuthoredAquaticEquipment(index:Int, replacement:AquaticProfile):AuthoredAquaticEquipmentResult {
		final original = readLocalPlayer();
		if (!isValidCharacter(original)) {
			return {
				character: original,
				collected: false,
				resolved: false
			};
		}
		if (!authoredItemIsActive(index)) {
			return {
				character: original,
				collected: false,
				resolved: true
			};
		}
		if (!authoredItemCollectionEventAvailable(index)) {
			return {
				character: original,
				collected: false,
				resolved: false
			};
		}
		final committed = commitLocalCharacter(original, adoptCharacterProfile(original, replacement));
		if (committed.resolved) {
			authoredItemStorage[index] = 0;
			queueAuthoredItemCollected(index);
		}
		return {
			character: committed.character,
			collected: committed.resolved,
			resolved: committed.resolved
		};
	}

	/**
		Move one complete authored stack into a bounded inventory.

		The item flag changes only after the full quantity fits and its CaxeFlow event
		can be preserved. This rule prevents a world reward from disappearing when the
		inventory has insufficient space or progression cannot be queued.
	**/
	public function collectAuthoredInventoryItem(index:Int, inventory:InventoryState, kind:ItemKind, quantity:Int):AuthoredInventoryItemResult {
		if (index < 0 || index >= AuthoredItemSlots.CAPACITY || quantity <= 0 || quantity > Inventory.MAX_STACK) {
			return {
				inventory: inventory,
				collected: 0,
				resolved: false
			};
		}
		if (!authoredItemIsActive(index) || Inventory.acceptedAmount(inventory, kind, quantity) != quantity) {
			return {
				inventory: inventory,
				collected: 0,
				resolved: true
			};
		}
		if (!authoredItemCollectionEventAvailable(index)) {
			return {
				inventory: inventory,
				collected: 0,
				resolved: false
			};
		}
		final replacement = Inventory.collectItem(inventory, kind, quantity);
		authoredItemStorage[index] = 0;
		queueAuthoredItemCollected(index);
		return {
			inventory: replacement,
			collected: quantity,
			resolved: true
		};
	}

	/** True when a successful pickup can preserve its authored-rule event. */
	function authoredItemCollectionEventAvailable(index:Int):Bool {
		if (flowExecutor == null)
			return true;
		if (index < 0 || index >= authoredItemContentIds.length)
			return false;
		final occurrence = localActorOccurrence(FlowEvent.ItemCollected(authoredItemContentIds[index]));
		return occurrence != null && flowEventsCanCommit([occurrence]);
	}

	/** Queue the validated item identity after its matching transaction commits. */
	function queueAuthoredItemCollected(index:Int):Void {
		if (flowExecutor != null) {
			final occurrence = localActorOccurrence(FlowEvent.ItemCollected(authoredItemContentIds[index]));
			if (occurrence != null)
				commitFlowEvents([occurrence]);
		}
	}

	/**
		Attempt one Adventure mining transaction against the owned world.

		Inventory capacity is checked before terrain changes by `Mining.attempt`.
		Only a collected block schedules nearby water, so a rejected or full
		inventory leaves both world bytes and fluid work unchanged.
	**/
	public function mineTerrain(coord:BlockCoord, inventory:InventoryState):MiningResult {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		final item = itemForCollectableBlock(World.query(cells, coord));
		final stagedEvents:Array<FlowEventOccurrence> = if (item != null && Inventory.acceptedAmount(inventory, item, 1) == 1) {
			final candidate = blockChangedOccurrences(coord, BlockKind.Air);
			if (candidate == null || !flowEventsCanCommit(candidate))
				return {inventory: inventory, outcome: MiningOutcome.FlowEventUnavailable};
			candidate;
		} else [];
		final result = attemptMining(cells, coord, inventory);
		if (result.outcome == MiningOutcome.Collected) {
			water.terrainChanged(coord);
			commitFlowEvents(stagedEvents);
		}
		return result;
	}

	/**
		Remove one player-removable terrain cell and schedule water around it.

		`World.remove` preserves bedrock and authored scenery that does not yet have
		a matching item contract. Water is notified only after a successful edit.
	**/
	public function removeTerrain(coord:BlockCoord):Bool {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		final stagedEvents = blockChangedOccurrences(coord, BlockKind.Air);
		if (stagedEvents == null || !flowEventsCanCommit(stagedEvents) || !World.remove(cells, coord))
			return false;
		water.terrainChanged(coord);
		commitFlowEvents(stagedEvents);
		return true;
	}

	/** Place one validated block and atomically publish its optional item use. */
	public function placeTerrain(coord:BlockCoord, kind:BlockKind, ?usedItem:ContentId):Bool {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		final changed = blockChangedOccurrences(coord, kind);
		if (changed == null)
			return false;
		final stagedEvents = changed;
		if (usedItem != null) {
			final used = itemUseOccurrences(usedItem);
			if (used == null)
				return false;
			for (event in used)
				stagedEvents.push(event);
		}
		if (!flowEventsCanCommit(stagedEvents) || !water.placeTerrain(cells, coord, kind))
			return false;
		commitFlowEvents(stagedEvents);
		return true;
	}

	/** Stable whole-world summary for save checks and cross-target test evidence. */
	public function worldStateHash():Int {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		return World.stateHash(cells);
	}

	/**
		Validate the binding, then advance water, movement, aquatics, and health once.

		A missing local character rejects the entire tick before water changes. After
		that ownership check, the explicit order is deterministic gameplay behavior.
		Presentation and host input never enter this method, and the returned snapshot
		is committed before the caller can render it.
	**/
	public function tick(input:GameTickInput):GameTickResult {
		#if c
		var cells:WorldCells = worldStorage.span();
		var readCells:WorldView = worldStorage.constSpan();
		#else
		var cells:WorldCells = worldStorage;
		var readCells:WorldView = WorldView.borrow(worldStorage);
		#end
		final characterId = localPlayer.characterId;
		final original = entities.read(characterId);
		if (!isValidCharacter(original)) {
			return {
				tickIndex: -1,
				character: original,
				immersion: observeAquatics(readCells, original.body),
				drowningDamage: 0,
				water: {processed: 0, changed: 0, remaining: water.pending()},
				flow: null,
				committed: false
			};
		}

		final waterResult = water.tick(cells, input.waterUpdateBudget);
		final characterResult = stepStoredCharacter(readCells, original, input.intent, input.damagePolicy);
		final committed = characterResult.resolved;
		final tickIndex = committed ? completedTicks : -1;
		if (committed)
			completedTicks++;
		var flowResult:Null<FlowTickResult> = null;
		if (committed) {
			final executor = flowExecutor;
			if (executor != null) {
				queueTriggerZoneTransitions();
				flowResult = executor.runTick({events: pendingFlowEvents, positions: currentFlowPositions(executor)});
				pendingFlowEvents.resize(0);
				refreshStatefulCollision();
			}
		}
		return {
			tickIndex: tickIndex,
			character: characterResult.character,
			immersion: characterResult.immersion,
			drowningDamage: characterResult.drowningDamage,
			water: waterResult,
			flow: flowResult,
			committed: committed
		};
	}

	/**
		Queue trigger-zone crossings after every committed movement step.

		Zones are visited in map order and actors in stable load order, with the local
		player first. A character's body origin is sampled in integer milliblocks.
		The first inside sample emits an enter event, while a fast outside-to-outside
		segment emits one atomic enter/leave pair marked as swept. Disabled zones clear
		membership silently, so re-enabling a zone around an actor produces a fresh
		enter. If the bounded queue cannot accept a complete transition, observation
		state does not advance and the next committed tick retries it.
	**/
	function queueTriggerZoneTransitions():Void {
		final executor = flowExecutor;
		if (executor == null)
			return;
		final actorCount = triggerActorIds.length;
		final currentPositions:Array<FlowEventPosition> = [];
		final currentValid:Array<Int> = [];
		for (actorIndex in 0...actorCount) {
			final character = entities.read(triggerActorEntities[actorIndex]);
			currentValid.push(isValidCharacter(character)
				&& !characterVitalsDefeated(character.vitals)
				&& executor.objectActive(triggerActorIds[actorIndex]) ? 1 : 0);
			currentPositions.push(flowEventPosition(character));
		}
		final stagedEvents:Array<FlowEventOccurrence> = [];
		final nextInside = triggerZoneInside.copy();
		final nextPrevious = triggerActorPrevious.copy();
		final nextPreviousValid = triggerActorPreviousValid.copy();
		for (zoneIndex in 0...triggerZoneIds.length) {
			final zone = triggerZoneIds[zoneIndex];
			final active = executor.objectActive(zone);
			for (actorIndex in 0...actorCount) {
				final membershipIndex = zoneIndex * actorCount + actorIndex;
				if (currentValid[actorIndex] == 0) {
					nextInside[membershipIndex] = 0;
					nextPreviousValid[actorIndex] = 0;
					continue;
				}
				final current = currentPositions[actorIndex];
				if (!active) {
					nextInside[membershipIndex] = 0;
					writeTriggerActorPosition(nextPrevious, actorIndex, current);
					nextPreviousValid[actorIndex] = 1;
					continue;
				}

				final inside = triggerZoneContains(zoneIndex, current);
				final wasInside = triggerZoneInside[membershipIndex] != 0;
				final hadPrevious = triggerActorPreviousValid[actorIndex] != 0;
				final previous = hadPrevious ? readTriggerActorPosition(actorIndex) : current;
				var eventCount = 0;
				var swept = false;
				if (inside != wasInside)
					eventCount = 1;
				else if (hadPrevious && !inside && !wasInside && triggerZoneSegmentCrosses(zoneIndex, previous, current)) {
					eventCount = 2;
					swept = true;
				}
				final context = SpatialEventContext(triggerActorIds[actorIndex], previous, current, swept);
				if (eventCount == 2) {
					stagedEvents.push(flowEventOccurrence(FlowEvent.EnterZone(zone), context));
					stagedEvents.push(flowEventOccurrence(FlowEvent.LeaveZone(zone), context));
				} else if (eventCount == 1) {
					final source = inside ? FlowEvent.EnterZone(zone) : FlowEvent.LeaveZone(zone);
					stagedEvents.push(flowEventOccurrence(source, context));
				}
				nextInside[membershipIndex] = inside ? 1 : 0;
				writeTriggerActorPosition(nextPrevious, actorIndex, current);
				nextPreviousValid[actorIndex] = 1;
			}
		}
		if (pendingFlowEvents.length + stagedEvents.length > ScenarioLimits.MAX_EVENTS_PER_TICK)
			return;
		for (event in stagedEvents)
			pendingFlowEvents.push(event);
		triggerZoneInside = nextInside;
		triggerActorPrevious = nextPrevious;
		triggerActorPreviousValid = nextPreviousValid;
	}

	/** Publish current active actor positions for `NearObject` predicates. */
	function currentFlowPositions(executor:CaxeFlowExecutor):Array<FlowPosition> {
		final result:Array<FlowPosition> = [];
		for (actorIndex in 0...triggerActorIds.length) {
			final character = entities.read(triggerActorEntities[actorIndex]);
			if (isValidCharacter(character)
				&& !characterVitalsDefeated(character.vitals)
				&& executor.objectActive(triggerActorIds[actorIndex])) {
				final position = flowEventPosition(character);
				result.push({
					objectId: triggerActorIds[actorIndex],
					xMilli: position.xMilli,
					yMilli: position.yMilli,
					zMilli: position.zMilli
				});
			}
		}
		return result;
	}

	/** Convert one committed body origin to the event model's exact unit. */
	static function flowEventPosition(character:Character):FlowEventPosition
		return {
			xMilli: Std.int(character.body.x * 1000.0),
			yMilli: Std.int(character.body.y * 1000.0),
			zMilli: Std.int(character.body.z * 1000.0)
		};

	/** Read one previously committed actor sample from compact parallel storage. */
	function readTriggerActorPosition(actorIndex:Int):FlowEventPosition {
		final offset = actorIndex * 3;
		return {
			xMilli: triggerActorPrevious[offset],
			yMilli: triggerActorPrevious[offset + 1],
			zMilli: triggerActorPrevious[offset + 2]
		};
	}

	/** Commit one actor sample only after all corresponding events fit. */
	static function writeTriggerActorPosition(storage:Array<Int>, actorIndex:Int, value:FlowEventPosition):Void {
		final offset = actorIndex * 3;
		storage[offset] = value.xMilli;
		storage[offset + 1] = value.yMilli;
		storage[offset + 2] = value.zMilli;
	}

	/** Test one point against CAXEMAP's inclusive-minimum, exclusive-maximum box. */
	function triggerZoneContains(zoneIndex:Int, value:FlowEventPosition):Bool {
		final offset = zoneIndex * 6;
		final minimumX = triggerZoneBounds[offset] * 1000;
		final minimumY = triggerZoneBounds[offset + 1] * 1000;
		final minimumZ = triggerZoneBounds[offset + 2] * 1000;
		return value.xMilli >= minimumX
			&& value.xMilli < minimumX + triggerZoneBounds[offset + 3] * 1000
			&& value.yMilli >= minimumY
			&& value.yMilli < minimumY + triggerZoneBounds[offset + 4] * 1000
			&& value.zMilli >= minimumZ
			&& value.zMilli < minimumZ + triggerZoneBounds[offset + 5] * 1000;
	}

	/** True when one voxel coordinate lies inside a zone's half-open bounds. */
	function triggerZoneContainsBlock(zoneIndex:Int, coord:BlockCoord):Bool {
		final offset = zoneIndex * 6;
		return coord.x >= triggerZoneBounds[offset]
			&& coord.x < triggerZoneBounds[offset] + triggerZoneBounds[offset + 3]
			&& coord.y >= triggerZoneBounds[offset + 1]
			&& coord.y < triggerZoneBounds[offset + 1] + triggerZoneBounds[offset + 4]
			&& coord.z >= triggerZoneBounds[offset + 2]
			&& coord.z < triggerZoneBounds[offset + 2] + triggerZoneBounds[offset + 5];
	}

	/**
		Test whether a movement segment spends positive distance inside one zone.

		The slab interval uses the same half-open box as point membership. A segment
		that only touches an edge has equal entry and exit times and is rejected.
	**/
	function triggerZoneSegmentCrosses(zoneIndex:Int, previous:FlowEventPosition, current:FlowEventPosition):Bool {
		final offset = zoneIndex * 6;
		var enter = 0.0;
		var leave = 1.0;
		final x = intersectTriggerAxis(previous.xMilli, current.xMilli, triggerZoneBounds[offset] * 1000,
			(triggerZoneBounds[offset] + triggerZoneBounds[offset + 3]) * 1000, enter, leave);
		if (x == null)
			return false;
		enter = x.enter;
		leave = x.leave;
		final y = intersectTriggerAxis(previous.yMilli, current.yMilli, triggerZoneBounds[offset + 1] * 1000,
			(triggerZoneBounds[offset + 1] + triggerZoneBounds[offset + 4]) * 1000, enter, leave);
		if (y == null)
			return false;
		enter = y.enter;
		leave = y.leave;
		final z = intersectTriggerAxis(previous.zMilli, current.zMilli, triggerZoneBounds[offset + 2] * 1000,
			(triggerZoneBounds[offset + 2] + triggerZoneBounds[offset + 5]) * 1000, enter, leave);
		return z != null && z.enter < z.leave;
	}

	/** Narrow one segment interval against one half-open axis range. */
	static function intersectTriggerAxis(start:Int, finish:Int, minimum:Int, maximum:Int, enter:Float,
			leave:Float):Null<{final enter:Float; final leave:Float;}> {
		final delta = finish - start;
		if (delta == 0)
			return start < minimum || start >= maximum ? null : {enter: enter, leave: leave};
		final first = (minimum - start) / delta;
		final second = (maximum - start) / delta;
		final axisEnter = first < second ? first : second;
		final axisLeave = first < second ? second : first;
		final narrowedEnter = axisEnter > enter ? axisEnter : enter;
		final narrowedLeave = axisLeave < leave ? axisLeave : leave;
		return narrowedEnter < narrowedLeave ? {enter: narrowedEnter, leave: narrowedLeave} : null;
	}

	/**
		Calculate and commit one already-resolved character against a world view.

		The caller performs the ID lookup once. Reusing that snapshot here avoids a
		second linear store search inside the same fixed tick; replacement still
		rechecks the stable ID before it changes authoritative state.
	**/
	function stepStoredCharacter(readCells:WorldView, original:Character, intent:CharacterIntent,
			damagePolicy:CharacterDamagePolicy):CharacterCommandStepResult {
		if (!isValidCharacter(original)) {
			return {
				character: original,
				immersion: observeAquatics(readCells, original.body),
				drowningDamage: 0,
				resolved: false
			};
		}
		final local = original.id.storageCode() == localPlayer.characterId.storageCode();
		if (local)
			refreshLocalPlayerCollision();
		final collision = local ? activeLocalPlayerCollision : activeStatefulCollision;
		final result = advanceCharacterState(readCells, collision, original, intent, damagePolicy);
		final resolved = entities.replace(original.id, result.character);
		return {
			character: resolved ? result.character : original,
			immersion: result.immersion,
			drowningDamage: result.drowningDamage,
			resolved: resolved
		};
	}

	/** Rebuild collision from the same authoritative state read by presentation. */
	function refreshStatefulCollision():Void {
		activeStatefulCollision.resize(0);
		final executor = flowExecutor;
		if (executor != null) {
			for (objectIndex in 0...statefulObjectIds.length) {
				final objectId = statefulObjectIds[objectIndex];
				if (!executor.objectActive(objectId))
					continue;
				final currentState = executor.objectState(objectId);
				if (currentState == null)
					continue;
				final start = statefulObjectStateStarts[objectIndex];
				final end = start + statefulObjectStateCounts[objectIndex];
				var stateIndex = start;
				var solid = false;
				while (stateIndex < end) {
					if (statefulObjectCollisionStates[stateIndex] == currentState) {
						solid = statefulObjectCollisionSolid[stateIndex] != 0;
						stateIndex = end;
					} else
						stateIndex++;
				}
				if (!solid)
					continue;
				final positionOffset = objectIndex * 3;
				final boundsOffset = objectIndex * 4;
				var widthMilli = statefulObjectBoundsMilli[boundsOffset];
				final heightMilli = statefulObjectBoundsMilli[boundsOffset + 1];
				var depthMilli = statefulObjectBoundsMilli[boundsOffset + 2];
				final yaw = statefulObjectBoundsMilli[boundsOffset + 3];
				if (yaw == 90 || yaw == 270) {
					final swap = widthMilli;
					widthMilli = depthMilli;
					depthMilli = swap;
				}
				final centerX = statefulObjectPositionsMilli[positionOffset] / 1000.0;
				final minimumY = statefulObjectPositionsMilli[positionOffset + 1] / 1000.0;
				final centerZ = statefulObjectPositionsMilli[positionOffset + 2] / 1000.0;
				final halfWidth = widthMilli / 2000.0;
				final halfDepth = depthMilli / 2000.0;
				activeStatefulCollision.push({
					minimumX: centerX - halfWidth,
					maximumX: centerX + halfWidth,
					minimumY: minimumY,
					maximumY: minimumY + heightMilli / 1000.0,
					minimumZ: centerZ - halfDepth,
					maximumZ: centerZ + halfDepth
				});
			}
		}
	}

	/**
		Rebuild the local player's complete non-terrain collision set.

		Stateful objects already own their authored solid/passable behavior. A
		stationary dialogue actor adds its character body plus a small conversation
		margin, so the camera cannot enter or crop its billboard at the nearest legal
		position. Moving hostiles remain governed by combat and controller rules
		instead of becoming invisible walls.
	**/
	function refreshLocalPlayerCollision():Void {
		activeLocalPlayerCollision.resize(0);
		for (box in activeStatefulCollision)
			activeLocalPlayerCollision.push(box);
		for (controller in actorControllers)
			switch controller.profile {
				case StationaryDialogue(_):
					final actor = entities.read(controller.characterId);
					if (isValidCharacter(actor)) {
						final occupied = characterCollisionBox(actor.body);
						final personalSpace = 0.6;
						activeLocalPlayerCollision.push({
							minimumX: occupied.minimumX - personalSpace,
							maximumX: occupied.maximumX + personalSpace,
							minimumY: occupied.minimumY,
							maximumY: occupied.maximumY,
							minimumZ: occupied.minimumZ - personalSpace,
							maximumZ: occupied.maximumZ + personalSpace
						});
					}
				case WanderChaseMelee(_) | TelegraphedCharge(_):
			}
	}

	/**
		Commit one calculated local-character replacement through the stable ID.

		Every public semantic command uses this single private write point. A failed
		store check returns the original snapshot, so callers cannot accidentally
		publish an unowned replacement as though the session accepted it.
	**/
	function commitLocalCharacter(original:Character, replacement:Character):LocalCharacterCommandResult {
		final resolved = isValidCharacter(original) && entities.replace(localPlayer.characterId, replacement);
		return {
			character: resolved ? replacement : original,
			resolved: resolved
		};
	}

	/** Clear last-tick events without replacing the session-owned Array identity. */
	function clearActorControllerEvents():Void {
		while (actorControllerEvents.length > 0)
			actorControllerEvents.pop();
	}

	/** Build one compact controller-pass summary from the session event buffer. */
	function actorControllerTick(status:ActorControllerTickStatus, processed:Int):ActorControllerTickResult
		return {status: status, processed: processed, emittedEvents: actorControllerEvents.length};

	/** Build the fail-closed result shared by commands with no bound character. */
	static inline function rejectedLocalCharacterCommand(original:Character):LocalCharacterCommandResult
		return {character: original, resolved: false};
}
