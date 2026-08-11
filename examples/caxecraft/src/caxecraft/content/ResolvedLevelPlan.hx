package caxecraft.content;

import caxecraft.content.ActorCompositionPlanner.ActorCompositionError;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnPlan;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnRole;
import caxecraft.content.ActorCompositionPlanner.planActorComposition;
import caxecraft.content.LevelContentResolver.FluidContentResolution;
import caxecraft.content.LevelContentResolver.ActorPresentationResolution;
import caxecraft.content.LevelContentResolver.ItemContentResolution;
import caxecraft.content.LevelContentResolver.ItemStorageCode;
import caxecraft.content.LevelContentResolver.LevelFluidSimulation;
import caxecraft.content.LevelContentResolver.TerrainContentResolution;
import caxecraft.content.LevelContentResolver.TerrainStorageCode;
import caxecraft.content.LevelContentResolver.StatefulObjectContentResolution;
import caxecraft.content.LevelContentResolver.StatefulObjectBounds;
import caxecraft.content.LevelContentResolver.StatefulObjectCollisionProfile;
import caxecraft.content.LevelContentResolver.StatefulObjectStateMechanics;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.domain.AquaticProfile;
import caxecraft.domain.BlockKind;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.domain.WaterCellCodec.isSolidCode;
import caxecraft.domain.World;
import caxecraft.gameplay.AuthoredItemSlots;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioWorld.ScenarioFluidPlacement;
import haxe.io.Bytes;

/**
 * Resolves one validated authored level into private construction facts.
 *
 * The editor and package reader produce a `Scenario`, whose values still use
 * readable content IDs and mutable Arrays. `ResolvedLevelPlan.resolve` checks
 * the selected engine/content limits, resolves every runtime representation,
 * copies all retained values, and returns one construction plan plus a separate
 * presentation plan. Neither value has a parser, writer, filename, or serialized
 * schema: CAXEMAP remains the only editable level format.
 */
/** Runtime mechanics chosen by the application for the level's local player. */
typedef LevelPlayerOptions = {
	/** Positive session identity reserved for the local player. */
	final entityId:EntityId;

	/** Initial health admitted by the current local-player vitals model. */
	final initialHealth:Int;

	/** Fully validated movement and survival mechanics for this load request. */
	final aquaticProfile:AquaticProfile;
}

/** One compact ascending-index terrain run owned by the construction plan. */
typedef ResolvedTerrainRun = {
	/** Nominal block storage value; it cannot be confused with an item code. */
	final storage:TerrainStorageCode;

	/** Positive number of consecutive world cells using this value. */
	final count:Int;
}

/** Fully resolved initial fluid placement without renderer-owned facts. */
typedef ResolvedFluid = {
	/** Stable authored identity retained for diagnostics, saves, and bindings. */
	final authoredId:ScenarioId;

	/** Logical content identity retained for package-generation provenance. */
	final contentId:ContentId;

	/** Closed engine simulation selected by the content registry. */
	final simulation:LevelFluidSimulation;

	/** Validated source point or finite initial volume. */
	final placement:ResolvedFluidPlacement;
}

/** Closed runtime shapes for one initial fluid declaration. */
enum ResolvedFluidPlacement {
	/** Fill a checked finite volume once, then let simulation evolve it. */
	InitialVolume(bounds:VoxelBounds);

	/** Place one permanent deterministic fluid source. */
	Source(point:VoxelPoint);
}

/** Fully resolved authored item placement owned by the construction plan. */
typedef ResolvedItem = {
	/** Stable authored identity retained across runtime storage changes. */
	final authoredId:ScenarioId;

	/** Logical content identity retained for presentation and save bindings. */
	final contentId:ContentId;

	/** Nominal pack-local item code selected by the content registry. */
	final storage:ItemStorageCode;

	/** Positive authored stack quantity already checked by scenario validation. */
	final quantity:Int;

	/** Copied placement; mutating the source scenario cannot change this plan. */
	final transform:ScenarioTransform;
}

/** Fully resolved generic interactable owned by one loaded level. */
typedef ResolvedStatefulObject = {
	/** Stable authored identity used by CaxeFlow interaction and state changes. */
	final authoredId:ScenarioId;

	/** Content profile that supplies range and state visuals. */
	final contentId:ContentId;

	/** Initial state selected by the validated CAXEMAP placement. */
	final initialState:ContentId;

	/** Maximum semantic interaction distance in integer thousandths of a block. */
	final interactionRadiusMilli:Int;

	/** Box dimensions resolved before a mutable session can exist. */
	final bounds:StatefulObjectBounds;

	/** Complete closed mechanics for every state this profile admits. */
	final states:Array<StatefulObjectStateMechanics>;

	/** Copied world transform used for proximity and presentation. */
	final transform:ScenarioTransform;
}

/** Fully resolved local-player construction facts. */
typedef ResolvedPlayer = {
	/** Stable authored ID of the CAXEMAP player-spawn object. */
	final authoredId:ScenarioId;

	/** Positive runtime identity selected through `LevelPlayerOptions`. */
	final entityId:EntityId;

	/** Copied spawn transform retained independently from the source scenario. */
	final transform:ScenarioTransform;

	/** Initial health selected for this candidate generation. */
	final initialHealth:Int;

	/** Copied player mechanics selected for this candidate generation. */
	final aquaticProfile:AquaticProfile;
}

/**
 * Stable CaxeFlow declaration categories needed by generation bindings.
 *
 * Executable predicates and actions remain in the validated `Scenario` owned
 * by the future content generation. The construction plan retains only stable
 * IDs and a deterministic digest, avoiding a second CaxeFlow representation.
 */
enum ResolvedFlowBindingKind {
	FlowVariable;
	FlowSequence;
	FlowRule;
}

/** One stable validated CaxeFlow identity retained for runtime binding. */
typedef ResolvedFlowBinding = {
	final id:ScenarioId;
	final kind:ResolvedFlowBindingKind;
}

/** Fluid presentation request kept outside simulation construction facts. */
typedef ResolvedFluidPresentation = {
	final authoredId:ScenarioId;
	final contentId:ContentId;
	final cellIndex:Int;
}

/** Item presentation request kept outside session item storage facts. */
typedef ResolvedItemPresentation = {
	final authoredId:ScenarioId;
	final contentId:ContentId;
}

/** Actor presentation request kept outside character mechanics. */
typedef ResolvedActorPresentation = {
	final authoredId:ScenarioId;
	final contentId:ContentId;
	final role:CharacterSpawnRole;

	/** Manifest-validated atlas selected by the actor's content profile. */
	final asset:String;

	/** Manifest-validated cell in that atlas. */
	final cellIndex:Int;
}

/** Visual request for one stateful interactable's initial state. */
typedef ResolvedStatefulObjectPresentation = {
	final authoredId:ScenarioId;
	final contentId:ContentId;
	final state:ContentId;
	final asset:String;
	final cellIndex:Int;

	/** Whether the initial state contributes a visual. */
	final visible:Bool;
}

/**
 * Compact deterministic evidence for one fully resolved level.
 *
 * Counts make the trace readable while wrapping 32-bit digests cover the exact
 * ordered construction and presentation facts. The trace contains no pointer,
 * allocation order, host path, or generated-C name, so Eval and native builds
 * can compare the same semantics directly.
 */
typedef ResolvedLevelSemanticTrace = {
	final scenarioDigest:Int;
	final worldCells:Int;
	final terrainRuns:Int;
	final terrainDigest:Int;
	final fluids:Int;
	final fluidDigest:Int;
	final items:Int;
	final itemDigest:Int;
	final statefulObjects:Int;
	final statefulObjectDigest:Int;
	final actors:Int;
	final actorDigest:Int;
	final flowBindings:Int;
	final flowDigest:Int;
	final playerDigest:Int;
	final presentationDigest:Int;
}

/** Exact reason no private construction plan can be produced. */
enum ResolvedLevelPlanError {
	/** The validated document targets dimensions unsupported by this engine build. */
	WorldSizeMismatch(width:Int, height:Int, depth:Int);

	/** The selected content registry cannot resolve one palette block. */
	UnknownTerrain(authoredPaletteCode:Int, contentId:ContentId);

	/** The registry returned a terrain byte unsupported by world storage. */
	UnsupportedTerrainStorage(contentId:ContentId, storageCode:Int);

	/** Chunk runs did not produce exactly one complete fixed-world image. */
	IncompleteTerrain;

	/** The selected content registry cannot resolve one authored fluid. */
	UnknownFluid(authoredId:ScenarioId, contentId:ContentId);

	/** Fluid presentation was absent even though simulation resolution succeeded. */
	InvalidFluidPresentation(authoredId:ScenarioId, contentId:ContentId, cellIndex:Int);

	/** The level contains more authored items than the fixed session storage. */
	ItemCapacityExceeded(count:Int, maximum:Int);

	/** The selected content registry cannot resolve one authored item. */
	UnknownItem(authoredId:ScenarioId, contentId:ContentId);

	/** The registry returned an invalid pack-local item code. */
	InvalidItemStorage(authoredId:ScenarioId, contentId:ContentId, storageCode:Int);

	/** One object profile/state pair did not resolve into complete runtime facts. */
	StatefulObjectResolutionRejected(authoredId:ScenarioId, contentId:ContentId, state:ContentId);

	/** A solid authored box uses a yaw that this axis-aligned collision slice excludes. */
	UnsupportedStatefulObjectCollisionYaw(authoredId:ScenarioId, yawDegrees:Int);

	/** Generic actor identity, content-kind, capacity, or mechanics planning failed. */
	ActorResolutionRejected(error:ActorCompositionError);

	/** Actor mechanics resolved, but its independently owned visual did not. */
	ActorPresentationRejected(authoredId:ScenarioId, contentId:ContentId);

	/** The document has no unique player-spawn construction fact. */
	PlayerSpawnCountMismatch(count:Int);

	/** Application-selected local-player mechanics are not safe for construction. */
	InvalidPlayerOptions;
}

/** A complete construction/read-model pair, or one fail-closed rejection. */
enum ResolvedLevelPlanResult {
	/** Both values are complete, privately owned, and safe to retain together. */
	LevelPlanResolved(plan:ResolvedLevelPlan, presentation:ResolvedLevelPresentationPlan);

	/** No plan or partially usable prefix escapes this result. */
	LevelPlanRejected(error:ResolvedLevelPlanError);
}

/**
 * Presentation requests resolved beside, but not inside, session construction.
 *
 * This class owns private Arrays and returns defensive copies. Presentation may
 * mutate those returned Arrays without changing the candidate plan. Its private
 * constructor is granted only to `ResolvedLevelPlan`, the cohesive resolver
 * that creates construction and presentation facts in one all-or-nothing pass.
 */
@:allow(caxecraft.content.ResolvedLevelPlan)
final class ResolvedLevelPresentationPlan {
	final fluids:Array<ResolvedFluidPresentation>;
	final items:Array<ResolvedItemPresentation>;
	final actors:Array<ResolvedActorPresentation>;
	final statefulObjects:Array<ResolvedStatefulObjectPresentation>;

	private function new(fluids:Array<ResolvedFluidPresentation>, items:Array<ResolvedItemPresentation>, actors:Array<ResolvedActorPresentation>,
			statefulObjects:Array<ResolvedStatefulObjectPresentation>) {
		this.fluids = fluids.copy();
		this.items = items.copy();
		this.actors = actors.copy();
		this.statefulObjects = statefulObjects.copy();
	}

	/** Return a caller-owned copy of all fluid presentation requests. */
	public function fluidRequests():Array<ResolvedFluidPresentation>
		return fluids.copy();

	/** Return a caller-owned copy of all item presentation requests. */
	public function itemRequests():Array<ResolvedItemPresentation>
		return items.copy();

	/** Return a caller-owned copy of all actor presentation requests. */
	public function actorRequests():Array<ResolvedActorPresentation>
		return actors.copy();

	/** Return a caller-owned copy of all stateful-object visual requests. */
	public function statefulObjectRequests():Array<ResolvedStatefulObjectPresentation>
		return statefulObjects.copy();
}

private typedef TerrainPlan = {
	final runs:Array<ResolvedTerrainRun>;
}

private typedef FluidPlans = {
	final construction:Array<ResolvedFluid>;
	final presentation:Array<ResolvedFluidPresentation>;
}

/** One fluid read-model request paired with its construction-order group. */
private typedef PendingFluidPresentation = {
	final placementRank:Int;
	final value:ResolvedFluidPresentation;
}

private typedef ItemPlans = {
	final construction:Array<ResolvedItem>;
	final presentation:Array<ResolvedItemPresentation>;
}

private typedef StatefulObjectPlans = {
	final construction:Array<ResolvedStatefulObject>;
	final presentation:Array<ResolvedStatefulObjectPresentation>;
}

private enum TerrainPlanResult {
	TerrainPlanned(value:TerrainPlan);
	TerrainRejected(error:ResolvedLevelPlanError);
}

private enum FluidPlanResult {
	FluidsPlanned(value:FluidPlans);
	FluidsRejected(error:ResolvedLevelPlanError);
}

private enum ItemPlanResult {
	ItemsPlanned(value:ItemPlans);
	ItemsRejected(error:ResolvedLevelPlanError);
}

private enum StatefulObjectPlanResult {
	StatefulObjectsPlanned(value:StatefulObjectPlans);
	StatefulObjectsRejected(error:ResolvedLevelPlanError);
}

private enum PlayerPlanResult {
	PlayerPlanned(value:ResolvedPlayer);
	PlayerRejected(error:ResolvedLevelPlanError);
}

/**
 * Complete private engine construction plan for one validated CAXEMAP.
 *
 * A class is appropriate because this value owns several related Arrays for one
 * candidate lifetime and must protect them from caller mutation. A structural
 * record would expose those Arrays directly. The constructor is private and
 * `resolve` is the sole factory, so no caller can assemble a plausible partial
 * plan from unrelated values.
 */
final class ResolvedLevelPlan {
	final resolvedScenarioId:ScenarioId;
	final resolvedTerrain:Array<ResolvedTerrainRun>;
	final resolvedFluids:Array<ResolvedFluid>;
	final resolvedItems:Array<ResolvedItem>;
	final resolvedStatefulObjects:Array<ResolvedStatefulObject>;
	final resolvedActors:Array<CharacterSpawnPlan>;
	final resolvedPlayer:ResolvedPlayer;
	final resolvedFlow:Array<ResolvedFlowBinding>;
	final resolvedFlowDigest:Int;
	final resolvedPresentationDigest:Int;

	private function new(scenarioId:ScenarioId, terrain:Array<ResolvedTerrainRun>, fluids:Array<ResolvedFluid>, items:Array<ResolvedItem>,
			statefulObjects:Array<ResolvedStatefulObject>, actors:Array<CharacterSpawnPlan>, player:ResolvedPlayer, flow:Array<ResolvedFlowBinding>,
			flowDigest:Int, presentationDigest:Int) {
		resolvedScenarioId = scenarioId;
		resolvedTerrain = terrain.copy();
		resolvedFluids = fluids.copy();
		resolvedItems = items.copy();
		resolvedStatefulObjects = copyStatefulObjects(statefulObjects);
		resolvedActors = copyActors(actors);
		resolvedPlayer = copyPlayer(player);
		resolvedFlow = flow.copy();
		resolvedFlowDigest = flowDigest;
		resolvedPresentationDigest = presentationDigest;
	}

	/**
	 * Resolve one complete candidate without mutating a live or unpublished session.
	 *
	 * `scenario` must already have passed `ScenarioValidator`; the resolver still
	 * checks engine dimensions, compact storage, capacity, selected-registry actor
	 * kinds, player options, and complete terrain coverage. Every retained value is
	 * copied before success escapes.
	 */
	public static function resolve(scenario:Scenario, registry:LevelContentResolver, playerOptions:LevelPlayerOptions):ResolvedLevelPlanResult {
		if (!World.admitsAuthoredSize(scenario.world.size.width, scenario.world.size.height, scenario.world.size.depth))
			return LevelPlanRejected(WorldSizeMismatch(scenario.world.size.width, scenario.world.size.height, scenario.world.size.depth));
		if (!validPlayerOptions(playerOptions))
			return LevelPlanRejected(InvalidPlayerOptions);

		final terrain = switch resolveTerrain(scenario, registry) {
			case TerrainPlanned(value): value;
			case TerrainRejected(error): return LevelPlanRejected(error);
		};
		final fluids = switch resolveFluids(scenario, registry) {
			case FluidsPlanned(value): value;
			case FluidsRejected(error): return LevelPlanRejected(error);
		};
		final items = switch resolveItems(scenario, registry) {
			case ItemsPlanned(value): value;
			case ItemsRejected(error): return LevelPlanRejected(error);
		};
		final statefulObjects = switch resolveStatefulObjects(scenario, registry) {
			case StatefulObjectsPlanned(value): value;
			case StatefulObjectsRejected(error): return LevelPlanRejected(error);
		};
		final actors = switch planActorComposition(scenario.objects, registry) {
			case ActorCompositionPlanned(plans): plans;
			case ActorCompositionRejected(error): return LevelPlanRejected(ActorResolutionRejected(error));
		};
		final player = switch resolvePlayer(scenario, playerOptions) {
			case PlayerPlanned(value): value;
			case PlayerRejected(error): return LevelPlanRejected(error);
		};
		final flow = resolveFlowBindings(scenario);
		final actorPresentation:Array<ResolvedActorPresentation> = [];
		for (actor in actors) {
			final resolvedPresentation = switch registry.resolveActorPresentation(actor.contentId) {
				case ActorPresentationResolved(asset, cell) if (asset.length > 0 && cell >= 0): {asset: asset, cell: cell};
				case ActorPresentationResolved(_, _) | UnknownActorPresentation:
					return LevelPlanRejected(ActorPresentationRejected(actor.authoredId, actor.contentId));
			};
			actorPresentation.push({
				authoredId: actor.authoredId,
				contentId: actor.contentId,
				role: copyRole(actor.role),
				asset: resolvedPresentation.asset,
				cellIndex: resolvedPresentation.cell
			});
		}
		final presentation = new ResolvedLevelPresentationPlan(fluids.presentation, items.presentation, actorPresentation, statefulObjects.presentation);
		final plan = new ResolvedLevelPlan(scenario.id, terrain.runs, fluids.construction, items.construction, statefulObjects.construction, actors, player,
			flow, digestFlowBindings(flow), digestPresentation(presentation));
		return LevelPlanResolved(plan, presentation);
	}

	/** Stable scenario identity retained for generation metadata and saves. */
	public inline function scenarioId():ScenarioId
		return resolvedScenarioId;

	/** Return a caller-owned copy of ascending-index terrain runs. */
	public function terrainRuns():Array<ResolvedTerrainRun>
		return resolvedTerrain.copy();

	/** Return a caller-owned copy of initial fluid construction facts. */
	public function fluids():Array<ResolvedFluid>
		return resolvedFluids.copy();

	/** Return a caller-owned copy of authored item construction facts. */
	public function items():Array<ResolvedItem>
		return resolvedItems.copy();

	/** Return a caller-owned copy of generic interactable construction facts. */
	public function statefulObjects():Array<ResolvedStatefulObject>
		return copyStatefulObjects(resolvedStatefulObjects);

	/** Return a caller-owned, deeply copied actor construction plan. */
	public function actors():Array<CharacterSpawnPlan>
		return copyActors(resolvedActors);

	/** Return copied local-player construction facts. */
	public function player():ResolvedPlayer
		return copyPlayer(resolvedPlayer);

	/** Return a caller-owned copy of stable CaxeFlow declaration bindings. */
	public function flowBindings():Array<ResolvedFlowBinding>
		return resolvedFlow.copy();

	/**
	 * Verify that a presentation plan came from this exact resolution result.
	 *
	 * The construction and presentation owners remain separate so render facts do
	 * not enter simulation storage. Their deterministic digest prevents a caller
	 * from accidentally pairing plans produced by different content registries.
	 */
	public inline function acceptsPresentation(presentation:ResolvedLevelPresentationPlan):Bool
		return digestPresentation(presentation) == resolvedPresentationDigest;

	/**
	 * Build deterministic target-neutral evidence for this construction pair.
	 *
	 * The presentation plan is explicit because renderer/resource facts are not
	 * part of the session construction plan. Passing a different presentation
	 * value therefore changes only `presentationDigest`.
	 */
	public function semanticTrace(presentation:ResolvedLevelPresentationPlan):ResolvedLevelSemanticTrace {
		var terrainDigest = hashStart();
		var worldCells = 0;
		for (run in resolvedTerrain) {
			terrainDigest = hashInt(terrainDigest, run.storage.value());
			terrainDigest = hashInt(terrainDigest, run.count);
			worldCells += run.count;
		}
		var fluidDigest = hashStart();
		for (fluid in resolvedFluids) {
			fluidDigest = hashText(fluidDigest, fluid.authoredId.text());
			fluidDigest = hashText(fluidDigest, fluid.contentId.text());
			fluidDigest = hashInt(fluidDigest, switch fluid.simulation {
				case BoundedWater: 1;
			});
			switch fluid.placement {
				case InitialVolume(bounds):
					fluidDigest = hashInt(fluidDigest, 1);
					fluidDigest = hashPoint(fluidDigest, bounds.origin);
					fluidDigest = hashInt(fluidDigest, bounds.size.width);
					fluidDigest = hashInt(fluidDigest, bounds.size.height);
					fluidDigest = hashInt(fluidDigest, bounds.size.depth);
				case Source(point):
					fluidDigest = hashInt(fluidDigest, 2);
					fluidDigest = hashPoint(fluidDigest, point);
			}
		}
		var itemDigest = hashStart();
		for (item in resolvedItems) {
			itemDigest = hashText(itemDigest, item.authoredId.text());
			itemDigest = hashText(itemDigest, item.contentId.text());
			itemDigest = hashInt(itemDigest, item.storage.value());
			itemDigest = hashInt(itemDigest, item.quantity);
			itemDigest = hashTransform(itemDigest, item.transform);
		}
		var actorDigest = hashStart();
		for (actor in resolvedActors)
			actorDigest = hashActor(actorDigest, actor);
		var statefulObjectDigest = hashStart();
		for (object in resolvedStatefulObjects) {
			statefulObjectDigest = hashText(statefulObjectDigest, object.authoredId.text());
			statefulObjectDigest = hashText(statefulObjectDigest, object.contentId.text());
			statefulObjectDigest = hashText(statefulObjectDigest, object.initialState.text());
			statefulObjectDigest = hashInt(statefulObjectDigest, object.interactionRadiusMilli);
			statefulObjectDigest = hashInt(statefulObjectDigest, object.bounds.widthMilli);
			statefulObjectDigest = hashInt(statefulObjectDigest, object.bounds.heightMilli);
			statefulObjectDigest = hashInt(statefulObjectDigest, object.bounds.depthMilli);
			for (state in object.states) {
				statefulObjectDigest = hashText(statefulObjectDigest, state.state.text());
				statefulObjectDigest = hashInt(statefulObjectDigest, switch state.collision {
					case StatefulObjectPassable: 0;
					case StatefulObjectSolid: 1;
				});
				statefulObjectDigest = hashInt(statefulObjectDigest, state.visible ? 1 : 0);
			}
			statefulObjectDigest = hashTransform(statefulObjectDigest, object.transform);
		}
		return {
			scenarioDigest: hashText(hashStart(), resolvedScenarioId.text()),
			worldCells: worldCells,
			terrainRuns: resolvedTerrain.length,
			terrainDigest: terrainDigest,
			fluids: resolvedFluids.length,
			fluidDigest: fluidDigest,
			items: resolvedItems.length,
			itemDigest: itemDigest,
			statefulObjects: resolvedStatefulObjects.length,
			statefulObjectDigest: statefulObjectDigest,
			actors: resolvedActors.length,
			actorDigest: actorDigest,
			flowBindings: resolvedFlow.length,
			flowDigest: resolvedFlowDigest,
			playerDigest: hashPlayer(hashStart(), resolvedPlayer),
			presentationDigest: digestPresentation(presentation)
		};
	}

	static function resolveTerrain(scenario:Scenario, registry:LevelContentResolver):TerrainPlanResult {
		final paletteCodes:Array<Int> = [];
		final paletteStorage:Array<TerrainStorageCode> = [];
		for (entry in scenario.world.palette) {
			final storage = switch registry.resolveTerrain(entry.blockType) {
				case TerrainContentResolved(code): code;
				case UnknownTerrainContent:
					return TerrainRejected(UnknownTerrain(entry.code, entry.blockType));
			};
			final value = storage.value();
			if (value < 0 || value > 255 || (value != 0 && !isSolidCode(value)))
				return TerrainRejected(UnsupportedTerrainStorage(entry.blockType, value));
			paletteCodes.push(entry.code);
			paletteStorage.push(storage);
		}

		final authoredWidth = scenario.world.size.width;
		final cells:Array<Int> = [];
		var physicalIndex = 0;
		while (physicalIndex < World.VOLUME) {
			final x = physicalIndex % World.WIDTH;
			if (x < authoredWidth)
				cells.push(-1);
			else if (x == authoredWidth)
				cells.push(World.kindCode(BlockKind.Bedrock));
			else
				cells.push(World.kindCode(BlockKind.Air));
			physicalIndex++;
		}
		for (chunk in scenario.world.chunks) {
			var localIndex = 0;
			for (run in chunk.runs) {
				var storageIndex = -1;
				for (index in 0...paletteCodes.length)
					if (paletteCodes[index] == run.paletteCode) {
						storageIndex = index;
						break;
					}
				if (storageIndex < 0)
					return TerrainRejected(IncompleteTerrain);
				var remaining = run.count;
				while (remaining > 0) {
					final localX = localIndex % chunk.size.width;
					final localY = Std.int(localIndex / chunk.size.width) % chunk.size.height;
					final localZ = Std.int(localIndex / (chunk.size.width * chunk.size.height));
					final worldIndex = chunk.origin.x + localX + World.WIDTH * (chunk.origin.y + localY + World.HEIGHT * (chunk.origin.z + localZ));
					if (worldIndex < 0 || worldIndex >= World.VOLUME || cells[worldIndex] >= 0)
						return TerrainRejected(IncompleteTerrain);
					cells[worldIndex] = paletteStorage[storageIndex].value();
					localIndex++;
					remaining--;
				}
			}
		}
		for (value in cells)
			if (value < 0)
				return TerrainRejected(IncompleteTerrain);

		final runs:Array<ResolvedTerrainRun> = [];
		var current = cells[0];
		var count = 1;
		for (index in 1...cells.length) {
			if (cells[index] == current) {
				count++;
			} else {
				runs.push({storage: TerrainStorageCode.fromValidated(current), count: count});
				current = cells[index];
				count = 1;
			}
		}
		runs.push({storage: TerrainStorageCode.fromValidated(current), count: count});
		return TerrainPlanned({runs: runs});
	}

	static function resolveFluids(scenario:Scenario, registry:LevelContentResolver):FluidPlanResult {
		final construction:Array<ResolvedFluid> = [];
		final pendingPresentation:Array<PendingFluidPresentation> = [];
		for (fluid in scenario.world.fluids) {
			final resolved = switch registry.resolveFluid(fluid.fluidType) {
				case FluidContentResolved(simulation, cell):
					if (cell < 0)
						return FluidsRejected(InvalidFluidPresentation(fluid.id, fluid.fluidType, cell));
					{simulation: simulation, cell: cell};
				case UnknownFluidContent:
					return FluidsRejected(UnknownFluid(fluid.id, fluid.fluidType));
			};
			final placement:ResolvedFluidPlacement = switch fluid.placement {
				case InitialVolume(bounds): InitialVolume(copyBounds(bounds));
				case Source(point): Source(copyPoint(point));
			};
			final placementRank = switch placement {
				case InitialVolume(_): 0;
				case Source(_): 1;
			};
			construction.push({
				authoredId: fluid.id,
				contentId: fluid.fluidType,
				simulation: resolved.simulation,
				placement: placement
			});
			pendingPresentation.push({
				placementRank: placementRank,
				value: {
					authoredId: fluid.id,
					contentId: fluid.fluidType,
					cellIndex: resolved.cell
				}
			});
		}
		construction.sort(compareFluids);
		pendingPresentation.sort((left,
				right) ->
				left.placementRank == right.placementRank ? compareText(left.value.authoredId.text(), right.value.authoredId.text()) : left.placementRank
				- right.placementRank);
		final presentation:Array<ResolvedFluidPresentation> = [];
		for (entry in pendingPresentation)
			presentation.push(entry.value);
		return FluidsPlanned({construction: construction, presentation: presentation});
	}

	static function resolveItems(scenario:Scenario, registry:LevelContentResolver):ItemPlanResult {
		var count = 0;
		for (object in scenario.objects)
			switch object.placement {
				case Item(_, _, _):
					count++;
				case _:
			}
		if (count > AuthoredItemSlots.CAPACITY)
			return ItemsRejected(ItemCapacityExceeded(count, AuthoredItemSlots.CAPACITY));

		final construction:Array<ResolvedItem> = [];
		final presentation:Array<ResolvedItemPresentation> = [];
		for (object in scenario.objects)
			switch object.placement {
				case Item(itemType, quantity, transform):
					final storage = switch registry.resolveItem(itemType) {
						case ItemContentResolved(code): code;
						case UnknownItemContent:
							return ItemsRejected(UnknownItem(object.id, itemType));
					};
					if (storage.value() < 0)
						return ItemsRejected(InvalidItemStorage(object.id, itemType, storage.value()));
					construction.push({
						authoredId: object.id,
						contentId: itemType,
						storage: storage,
						quantity: quantity,
						transform: copyTransform(transform)
					});
					presentation.push({authoredId: object.id, contentId: itemType});
				case _:
			}
		construction.sort((left, right) -> compareText(left.authoredId.text(), right.authoredId.text()));
		presentation.sort((left, right) -> compareText(left.authoredId.text(), right.authoredId.text()));
		return ItemsPlanned({construction: construction, presentation: presentation});
	}

	/** Resolve each generic interactable without giving the level planner campaign knowledge. */
	static function resolveStatefulObjects(scenario:Scenario, registry:LevelContentResolver):StatefulObjectPlanResult {
		final construction:Array<ResolvedStatefulObject> = [];
		final presentation:Array<ResolvedStatefulObjectPresentation> = [];
		for (object in scenario.objects)
			switch object.placement {
				case StatefulObject(contentId, initialState, transform):
					final resolved = switch registry.resolveStatefulObject(contentId, initialState) {
						case StatefulObjectContentResolved(interactionRadiusMilli, bounds, states, presentationAsset, presentationCell)
							if (interactionRadiusMilli >= 0 && presentationAsset.length > 0 && presentationCell >= 0):
							{
								interactionRadiusMilli: interactionRadiusMilli,
								bounds: copyStatefulObjectBounds(bounds),
								states: copyStatefulObjectStates(states),
								presentationAsset: presentationAsset,
								presentationCell: presentationCell
							};
						case StatefulObjectContentResolved(_, _, _, _, _) | UnknownStatefulObjectContent:
							return StatefulObjectsRejected(StatefulObjectResolutionRejected(object.id, contentId, initialState));
					};
					var initialVisible = false;
					var foundInitialState = false;
					var hasSolidState = false;
					for (state in resolved.states)
						switch state.collision {
							case StatefulObjectPassable:
							case StatefulObjectSolid: hasSolidState = true;
						}
					for (state in resolved.states)
						if (state.state == initialState) {
							initialVisible = state.visible;
							foundInitialState = true;
						}
					if (!foundInitialState)
						return StatefulObjectsRejected(StatefulObjectResolutionRejected(object.id, contentId, initialState));
					if (hasSolidState && transform.yawDegrees % 90 != 0)
						return StatefulObjectsRejected(UnsupportedStatefulObjectCollisionYaw(object.id, transform.yawDegrees));
					construction.push({
						authoredId: object.id,
						contentId: contentId,
						initialState: initialState,
						interactionRadiusMilli: resolved.interactionRadiusMilli,
						bounds: resolved.bounds,
						states: resolved.states,
						transform: copyTransform(transform)
					});
					presentation.push({
						authoredId: object.id,
						contentId: contentId,
						state: initialState,
						asset: resolved.presentationAsset,
						cellIndex: resolved.presentationCell,
						visible: initialVisible
					});
				case _:
			}
		construction.sort((left, right) -> compareText(left.authoredId.text(), right.authoredId.text()));
		presentation.sort((left, right) -> compareText(left.authoredId.text(), right.authoredId.text()));
		return StatefulObjectsPlanned({construction: construction, presentation: presentation});
	}

	static function resolvePlayer(scenario:Scenario, options:LevelPlayerOptions):PlayerPlanResult {
		var count = 0;
		var authoredId = new ScenarioId("invalid");
		var transform:ScenarioTransform = {
			xMilli: 0,
			yMilli: 0,
			zMilli: 0,
			yawDegrees: 0
		};
		for (object in scenario.objects)
			switch object.placement {
				case PlayerSpawn(value):
					count++;
					authoredId = object.id;
					transform = value;
				case _:
			}
		if (count != 1)
			return PlayerRejected(PlayerSpawnCountMismatch(count));
		return PlayerPlanned({
			authoredId: authoredId,
			entityId: options.entityId,
			transform: copyTransform(transform),
			initialHealth: options.initialHealth,
			aquaticProfile: copyAquatic(options.aquaticProfile)
		});
	}

	static function resolveFlowBindings(scenario:Scenario):Array<ResolvedFlowBinding> {
		final result:Array<ResolvedFlowBinding> = [];
		for (variable in scenario.flow.variables)
			result.push({id: variable.id, kind: FlowVariable});
		for (sequence in scenario.flow.sequences)
			result.push({id: sequence.id, kind: FlowSequence});
		for (rule in scenario.flow.rules)
			result.push({id: rule.id, kind: FlowRule});
		return result;
	}

	static function validPlayerOptions(options:LevelPlayerOptions):Bool {
		final aquatic = options.aquaticProfile;
		return options.entityId.isValid() && options.initialHealth > 0 && options.initialHealth <= MAX_HEALTH && aquatic.maximumBreathTicks > 0
			&& aquatic.breathRecoveryPerTick > 0 && aquatic.horizontalControl >= 0.0 && aquatic.horizontalControl <= 1.0
			&& aquatic.ascentAcceleration >= 0.0 && aquatic.descentAcceleration >= 0.0 && aquatic.buoyancyAcceleration >= 0.0 && aquatic.dragPerTick >= 0.0
			&& aquatic.dragPerTick < 1.0 && aquatic.drowningIntervalTicks > 0;
	}

	static function compareFluids(left:ResolvedFluid, right:ResolvedFluid):Int {
		final leftKind = switch left.placement {
			case InitialVolume(_): 0;
			case Source(_): 1;
		};
		final rightKind = switch right.placement {
			case InitialVolume(_): 0;
			case Source(_): 1;
		};
		return leftKind == rightKind ? compareText(left.authoredId.text(), right.authoredId.text()) : leftKind - rightKind;
	}

	static function compareText(left:String, right:String):Int {
		/*
			Content IDs use the same locale-independent UTF-8 byte order as the
			canonical CAXEMAP writer. Haxe's generic String comparison does not
			state that persistence rule, so make the bytes being ordered explicit.
		 */
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final shared = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...shared) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}

	/** Copy one stateful box so no registry-owned record escapes resolution. */
	static function copyStatefulObjectBounds(value:StatefulObjectBounds):StatefulObjectBounds
		return {widthMilli: value.widthMilli, heightMilli: value.heightMilli, depthMilli: value.depthMilli};

	/** Copy closed state mechanics without sharing the registry's array owner. */
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

	/** Deep-copy generic object mechanics retained by an immutable plan. */
	static function copyStatefulObjects(values:Array<ResolvedStatefulObject>):Array<ResolvedStatefulObject> {
		final result:Array<ResolvedStatefulObject> = [];
		for (value in values)
			result.push({
				authoredId: value.authoredId,
				contentId: value.contentId,
				initialState: value.initialState,
				interactionRadiusMilli: value.interactionRadiusMilli,
				bounds: copyStatefulObjectBounds(value.bounds),
				states: copyStatefulObjectStates(value.states),
				transform: copyTransform(value.transform)
			});
		return result;
	}

	static function copyActors(values:Array<CharacterSpawnPlan>):Array<CharacterSpawnPlan> {
		final result:Array<CharacterSpawnPlan> = [];
		for (value in values)
			result.push({
				authoredId: value.authoredId,
				entityId: value.entityId,
				contentId: value.contentId,
				transform: copyTransform(value.transform),
				maximumHealth: value.maximumHealth,
				aquaticProfile: copyAquatic(value.aquaticProfile),
				controller: copyController(value.controller),
				role: copyRole(value.role)
			});
		return result;
	}

	static function copyPlayer(value:ResolvedPlayer):ResolvedPlayer
		return {
			authoredId: value.authoredId,
			entityId: value.entityId,
			transform: copyTransform(value.transform),
			initialHealth: value.initialHealth,
			aquaticProfile: copyAquatic(value.aquaticProfile)
		};

	static function copyTransform(value:ScenarioTransform):ScenarioTransform
		return {
			xMilli: value.xMilli,
			yMilli: value.yMilli,
			zMilli: value.zMilli,
			yawDegrees: value.yawDegrees
		};

	static function copyPoint(value:VoxelPoint):VoxelPoint
		return {x: value.x, y: value.y, z: value.z};

	static function copyBounds(value:VoxelBounds):VoxelBounds
		return {
			origin: copyPoint(value.origin),
			size: {
				width: value.size.width,
				height: value.size.height,
				depth: value.size.depth
			}
		};

	static function copyAquatic(value:AquaticProfile):AquaticProfile
		return {
			maximumBreathTicks: value.maximumBreathTicks,
			breathRecoveryPerTick: value.breathRecoveryPerTick,
			horizontalControl: value.horizontalControl,
			ascentAcceleration: value.ascentAcceleration,
			descentAcceleration: value.descentAcceleration,
			buoyancyAcceleration: value.buoyancyAcceleration,
			dragPerTick: value.dragPerTick,
			drowningIntervalTicks: value.drowningIntervalTicks,
			underwaterMining: value.underwaterMining,
			coldProtection: value.coldProtection
		};

	static function copyController(value:ActorControllerProfile):ActorControllerProfile
		return switch value {
			case StationaryDialogue(radius): StationaryDialogue(radius);
			case WanderChaseMelee(profile):
				WanderChaseMelee({
					noticeRadiusMilli: profile.noticeRadiusMilli,
					strikeRadiusMilli: profile.strikeRadiusMilli,
					attackRadiusMilli: profile.attackRadiusMilli,
					windupTicks: profile.windupTicks,
					recoveryTicks: profile.recoveryTicks,
					stepMilli: profile.stepMilli,
					drop: profile.drop
				});
			case TelegraphedCharge(profile):
				TelegraphedCharge({
					noticeRadiusMilli: profile.noticeRadiusMilli,
					strikeRadiusMilli: profile.strikeRadiusMilli,
					attackRadiusMilli: profile.attackRadiusMilli,
					windupTicks: profile.windupTicks,
					recoveryTicks: profile.recoveryTicks,
					stepMilli: profile.stepMilli,
					drop: profile.drop
				});
		};

	static function copyRole(value:CharacterSpawnRole):CharacterSpawnRole
		return switch value {
			case DialogueNpc(dialogue): DialogueNpc(dialogue);
			case EnemyActor: EnemyActor;
		};

	static inline function hashStart():Int
		return -2128831035;

	static inline function hashInt(hash:Int, value:Int):Int
		return (hash ^ value) * 16777619;

	static function hashText(hash:Int, value:String):Int {
		var result = hashInt(hash, value.length);
		for (index in 0...value.length)
			result = hashInt(result, value.charCodeAt(index));
		return result;
	}

	static function hashPoint(hash:Int, point:VoxelPoint):Int {
		var result = hashInt(hash, point.x);
		result = hashInt(result, point.y);
		return hashInt(result, point.z);
	}

	static function hashTransform(hash:Int, transform:ScenarioTransform):Int {
		var result = hashInt(hash, transform.xMilli);
		result = hashInt(result, transform.yMilli);
		result = hashInt(result, transform.zMilli);
		return hashInt(result, transform.yawDegrees);
	}

	static function hashAquatic(hash:Int, value:AquaticProfile):Int {
		var result = hashInt(hash, value.maximumBreathTicks);
		result = hashInt(result, value.breathRecoveryPerTick);
		result = hashInt(result, Std.int(value.horizontalControl * 1000000.0));
		result = hashInt(result, Std.int(value.ascentAcceleration * 1000000.0));
		result = hashInt(result, Std.int(value.descentAcceleration * 1000000.0));
		result = hashInt(result, Std.int(value.buoyancyAcceleration * 1000000.0));
		result = hashInt(result, Std.int(value.dragPerTick * 1000000.0));
		result = hashInt(result, value.drowningIntervalTicks);
		result = hashInt(result, value.underwaterMining ? 1 : 0);
		return hashInt(result, value.coldProtection ? 1 : 0);
	}

	static function hashController(hash:Int, value:ActorControllerProfile):Int
		return switch value {
			case StationaryDialogue(radius):
				hashInt(hashInt(hash, 1), radius);
			case WanderChaseMelee(profile):
				var result = hashInt(hash, 2);
				result = hashInt(result, profile.noticeRadiusMilli);
				result = hashInt(result, profile.strikeRadiusMilli);
				result = hashInt(result, profile.attackRadiusMilli);
				result = hashInt(result, profile.windupTicks);
				result = hashInt(result, profile.recoveryTicks);
				result = hashInt(result, profile.stepMilli);
				hashText(result, profile.drop.text());
			case TelegraphedCharge(profile):
				var result = hashInt(hash, 3);
				result = hashInt(result, profile.noticeRadiusMilli);
				result = hashInt(result, profile.strikeRadiusMilli);
				result = hashInt(result, profile.attackRadiusMilli);
				result = hashInt(result, profile.windupTicks);
				result = hashInt(result, profile.recoveryTicks);
				result = hashInt(result, profile.stepMilli);
				hashText(result, profile.drop.text());
		};

	static function hashActor(hash:Int, value:CharacterSpawnPlan):Int {
		var result = hashText(hash, value.authoredId.text());
		result = hashInt(result, value.entityId.storageCode());
		result = hashText(result, value.contentId.text());
		result = hashTransform(result, value.transform);
		result = hashInt(result, value.maximumHealth);
		result = hashAquatic(result, value.aquaticProfile);
		result = hashController(result, value.controller);
		return switch value.role {
			case DialogueNpc(dialogue): hashText(hashInt(result, 1), dialogue.text());
			case EnemyActor: hashInt(result, 2);
		};
	}

	static function hashPlayer(hash:Int, value:ResolvedPlayer):Int {
		var result = hashText(hash, value.authoredId.text());
		result = hashInt(result, value.entityId.storageCode());
		result = hashTransform(result, value.transform);
		result = hashInt(result, value.initialHealth);
		return hashAquatic(result, value.aquaticProfile);
	}

	static function digestFlowBindings(values:Array<ResolvedFlowBinding>):Int {
		var result = hashStart();
		for (value in values) {
			result = hashInt(result, switch value.kind {
				case FlowVariable: 1;
				case FlowSequence: 2;
				case FlowRule: 3;
			});
			result = hashText(result, value.id.text());
		}
		return result;
	}

	static function digestPresentation(value:ResolvedLevelPresentationPlan):Int {
		var result = hashStart();
		for (fluid in value.fluidRequests()) {
			result = hashText(result, fluid.authoredId.text());
			result = hashText(result, fluid.contentId.text());
			result = hashInt(result, fluid.cellIndex);
		}
		for (item in value.itemRequests()) {
			result = hashText(result, item.authoredId.text());
			result = hashText(result, item.contentId.text());
		}
		for (actor in value.actorRequests()) {
			result = hashText(result, actor.authoredId.text());
			result = hashText(result, actor.contentId.text());
			result = hashText(result, actor.asset);
			result = hashInt(result, actor.cellIndex);
			result = switch actor.role {
				case DialogueNpc(dialogue): hashText(hashInt(result, 1), dialogue.text());
				case EnemyActor: hashInt(result, 2);
			}
		}
		for (object in value.statefulObjectRequests()) {
			result = hashText(result, object.authoredId.text());
			result = hashText(result, object.contentId.text());
			result = hashText(result, object.state.text());
			result = hashText(result, object.asset);
			result = hashInt(result, object.cellIndex);
			result = hashInt(result, object.visible ? 1 : 0);
		}
		return result;
	}
}
