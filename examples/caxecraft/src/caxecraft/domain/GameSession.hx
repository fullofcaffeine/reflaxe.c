package caxecraft.domain;

import caxecraft.domain.Aquatics.observe as observeAquatics;
import caxecraft.domain.Character.isValid as isValidCharacter;
import caxecraft.domain.Character.step as stepCharacter;
import caxecraft.domain.PlayerAgent.bind as bindPlayerAgent;
import caxecraft.domain.WaterCellCodec.isSolidCode as isSolidStorageCode;
import caxecraft.gameplay.AuthoredItemSlots;
import caxecraft.gameplay.InventoryState;
import caxecraft.gameplay.Mining.attempt as attemptMining;
import caxecraft.gameplay.MiningOutcome;
import caxecraft.gameplay.MiningResult;
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
	final committed:Bool;
}

/**
	Owns the mutable, controller-neutral state of one loaded game simulation.

	`EntityStore` and `WaterSimulation` are real child objects embedded directly
	inside this session by haxe.c. The session owns the world and authored-item
	flags; the water child owns the queue that must agree with its scheduler
	counters. They have one stable lifetime, require no heap allocation, and cannot
	escape as independent owned values. This is ordinary Haxe composition; the
	compiler selects the safe, readable C representation.

	A class is used instead of a record because the session is the stable mutable
	owner: callers must update and observe the same loaded simulation across many
	frames. Making it `final` documents that variation belongs in composed systems
	and content, not hidden session subclasses. Haxe.c emits one parent C struct
	with direct child storage and pointer-receiver methods.

	The C build uses compact `CArray` fields. Eval uses ordinary Haxe arrays as an
	independent behavior oracle. Haxe removes the inactive representation branch
	at compile time; simulation methods below are shared. Public readers receive a
	zero-copy `WorldView` that cannot write and is valid only for the direct call.
	World and item mutations remain session commands, so the application cannot
	bypass water scheduling or retain a mutable storage view.
**/
final class GameSession {
	/** All live character state, owned for exactly this session's lifetime. */
	final entities:EntityStore = new EntityStore();

	/** Human-control binding; it names the store entry and owns no character copy. */
	var localPlayer:PlayerAgent;

	/** Number of fixed simulation steps that this session has committed. */
	var completedTicks:Int = 0;

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

	/** True only when the binding still names the character owned by this session. */
	public inline function hasLocalPlayer():Bool
		return localPlayer.characterId.isValid() && entities.contains(localPlayer.characterId);

	/** Read the committed local-character snapshot without exposing its store key. */
	public inline function readLocalPlayer():Character
		return entities.read(localPlayer.characterId);

	/**
		Commit a temporary application-level change to the local character.

		This migration seam retains the stable-ID check while interaction, recovery,
		and combat move into `tick`. Remove it once those systems return session-owned
		commands instead of allowing `CaxecraftApp` to construct a replacement snapshot.
	**/
	public inline function replaceLocalPlayer(character:Character):Bool
		return entities.replace(localPlayer.characterId, character);

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
		changes a flag only through `deactivateAuthoredItem`, so drawing cannot
		accidentally collect or restore an item.
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

		The generated content adapter describes terrain as ascending runs. Keeping the
		short-lived storage view here lets content remain ordinary Haxe without gaining
		direct access to the session's C array. Failure returns `-1` and performs no
		write; success returns the first index after the run.
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
		Deactivate one live authored item after its gameplay effect commits.

		Invalid or already-inactive slots leave the session unchanged. Returning
		`false` lets callers keep their inventory/equipment update atomic with the
		item disappearance.
	**/
	public function deactivateAuthoredItem(index:Int):Bool {
		if (!authoredItemIsActive(index))
			return false;
		authoredItemStorage[index] = 0;
		return true;
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
		final result = attemptMining(cells, coord, inventory);
		if (result.outcome == MiningOutcome.Collected)
			water.terrainChanged(coord);
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
		if (!World.remove(cells, coord))
			return false;
		water.terrainChanged(coord);
		return true;
	}

	/** Place one validated block and schedule nearby water recomputation. */
	public function placeTerrain(coord:BlockCoord, kind:BlockKind):Bool {
		#if c
		var cells:WorldCells = worldStorage.span();
		#else
		var cells:WorldCells = worldStorage;
		#end
		return water.placeTerrain(cells, coord, kind);
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
				committed: false
			};
		}

		final waterResult = water.tick(cells, input.waterUpdateBudget);
		final characterResult = stepCharacter(readCells, original, input.intent, input.damagePolicy);
		final committed = entities.replace(characterId, characterResult.character);
		final tickIndex = committed ? completedTicks : -1;
		if (committed)
			completedTicks++;
		return {
			tickIndex: tickIndex,
			character: characterResult.character,
			immersion: characterResult.immersion,
			drowningDamage: characterResult.drowningDamage,
			water: waterResult,
			committed: committed
		};
	}
}
