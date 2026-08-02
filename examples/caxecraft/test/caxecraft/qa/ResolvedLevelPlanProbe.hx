package caxecraft.qa;

import caxecraft.content.ActorContentResolver.ActorContentKind;
import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.LevelContentResolver;
import caxecraft.content.LevelContentResolver.FluidContentResolution;
import caxecraft.content.LevelContentResolver.ItemContentResolution;
import caxecraft.content.LevelContentResolver.TerrainContentResolution;
import caxecraft.content.ResolvedLevelPlan;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanError;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanResult;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.domain.World;
import caxecraft.qa.ResolvedLevelFixture.readFirstPlayableScenario;
import caxecraft.qa.ResolvedLevelFixture.resolveFirstPlayable;
import caxecraft.qa.FocusedContentFixture.FocusedContentRegistry;
import caxecraft.qa.FocusedContentFixture.standardAquaticProfile;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioWorld.ScenarioFluid;

/**
 * Proves the private level-construction boundary on Eval and generated C.
 *
 * The probe parses the same embedded first-playable CAXEMAP as the native codec
 * test, validates it against the built-in registry, and resolves a complete
 * plan. Haxe owns every semantic assertion. The native harness only reports the
 * exported scalar trace, so it cannot accidentally become a second resolver.
 */
/** Complete Haxe-authored assertion result observed by the native harness. */
var observed:Int = 0;

/** Stable scenario identity digest. */
var traceScenario:Int = 0;

/** Number of terrain cells covered by the plan. */
var traceWorldCells:Int = 0;

/** Number of compact terrain runs. */
var traceTerrainRuns:Int = 0;

/** Ordered terrain semantic digest. */
var traceTerrain:Int = 0;

/** Number of initial fluid declarations. */
var traceFluids:Int = 0;

/** Ordered fluid semantic digest. */
var traceFluid:Int = 0;

/** Number of authored item placements. */
var traceItems:Int = 0;

/** Ordered item semantic digest. */
var traceItem:Int = 0;

/** Number of fully resolved authored actors. */
var traceActors:Int = 0;

/** Ordered actor mechanics and identity digest. */
var traceActor:Int = 0;

/** Number of retained stable CaxeFlow bindings. */
var traceFlowBindings:Int = 0;

/** Ordered CaxeFlow binding digest. */
var traceFlow:Int = 0;

/** Local-player construction digest. */
var tracePlayer:Int = 0;

/** Separate presentation/read-model digest. */
var tracePresentation:Int = 0;

/**
 * Run the shared assertions and expose the same trace on both hosts.
 *
 * Eval prints directly. Generated C stores fields so the small independent C
 * consumer can run the generated lifecycle before printing them.
 */
function main():Void {
	final status = selfCheck();
	#if c
	observed = status;
	#else
	Sys.println(status);
	Sys.println(traceScenario);
	Sys.println(traceWorldCells);
	Sys.println(traceTerrainRuns);
	Sys.println(traceTerrain);
	Sys.println(traceFluids);
	Sys.println(traceFluid);
	Sys.println(traceItems);
	Sys.println(traceItem);
	Sys.println(traceActors);
	Sys.println(traceActor);
	Sys.println(traceFlowBindings);
	Sys.println(traceFlow);
	Sys.println(tracePlayer);
	Sys.println(tracePresentation);
	#end
}

/**
 * Return zero when resolution, ownership, and negative contracts all hold.
 *
 * Nonzero values identify a stable assertion stage without relying on host
 * exception text or generated symbol spelling.
 */
function selfCheck():Int {
	final registry = new FocusedContentRegistry();
	final scenario = readFirstPlayableScenario();
	if (scenario == null)
		return 1;
	final resolved = resolveFirstPlayable(scenario, registry);
	final values = switch resolved {
		case LevelPlanResolved(plan, presentation): {plan: plan, presentation: presentation};
		case LevelPlanRejected(_): return 2;
	};

	final terrain = values.plan.terrainRuns();
	final fluids = values.plan.fluids();
	final items = values.plan.items();
	final actors = values.plan.actors();
	final player = values.plan.player();
	final presentationFluids = values.presentation.fluidRequests();
	if (terrain.length == 0
		|| terrain[0].storage.value() != 4
		|| terrain[0].count != 32
		|| fluids.length != 2
		|| items.length != 1
		|| items[0].authoredId.text() != "item.tideweave"
		|| items[0].storage.value() != 8
		|| actors.length != 2
		|| actors[0].authoredId.text() != "enemy.mossling"
		|| actors[1].authoredId.text() != "guide.nia"
		|| player.authoredId.text() != "player.start"
		|| player.transform.xMilli != 16500
		|| presentationFluids.length != 2
		|| presentationFluids[0].cellIndex != 5)
		return 3;

	final before = values.plan.semanticTrace(values.presentation);
	terrain.pop();
	fluids.pop();
	items.pop();
	actors.pop();
	presentationFluids.pop();
	scenario.objects.pop();
	final after = values.plan.semanticTrace(values.presentation);
	if (!sameTrace(before, after)
		|| values.plan.terrainRuns().length == terrain.length
		|| values.plan.fluids().length != 2
		|| values.plan.items().length != 1
		|| values.plan.actors().length != 2
		|| values.presentation.fluidRequests().length != 2)
		return 4;

	final freshScenario = readFirstPlayableScenario();
	if (freshScenario == null)
		return 5;
	final interleaved = switch resolveFirstPlayable(withInterleavedFluidIds(freshScenario), registry) {
		case LevelPlanResolved(plan, presentation): {construction: plan.fluids(), presentation: presentation.fluidRequests()};
		case LevelPlanRejected(_): return 6;
	};
	if (interleaved.construction.length != 2
		|| interleaved.presentation.length != 2
		|| interleaved.construction[0].authoredId.text() != "water.z-volume"
		|| interleaved.presentation[0].authoredId.text() != "water.z-volume"
		|| interleaved.construction[1].authoredId.text() != "water.a-source"
		|| interleaved.presentation[1].authoredId.text() != "water.a-source")
		return 7;
	switch resolveFirstPlayable(freshScenario, new RejectingLevelRegistry(MissingTerrain)) {
		case LevelPlanRejected(UnknownTerrain(_, id)) if (id.text() == "caxecraft:grass"):
		case _:
			return 8;
	}
	switch resolveFirstPlayable(freshScenario, new RejectingLevelRegistry(WrongActorKind)) {
		case LevelPlanRejected(ActorResolutionRejected(PlacedActorKindMismatch(authored, content, EnemyContent, NpcContent)))
			if (authored.text() == "enemy.mossling" && content.text() == "caxecraft:mossling"):
		case _:
			return 9;
	}
	switch resolveFirstPlayable(withTooManyItems(freshScenario), registry) {
		case LevelPlanRejected(ItemCapacityExceeded(257, maximum)) if (maximum == 256):
		case _:
			return 10;
	}
	final invalidOptions:LevelPlayerOptions = {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH + 1,
		aquaticProfile: standardAquaticProfile()
	};
	switch ResolvedLevelPlan.resolve(freshScenario, registry, invalidOptions) {
		case LevelPlanRejected(InvalidPlayerOptions):
		case _:
			return 11;
	}

	traceScenario = before.scenarioDigest;
	traceWorldCells = before.worldCells;
	traceTerrainRuns = before.terrainRuns;
	traceTerrain = before.terrainDigest;
	traceFluids = before.fluids;
	traceFluid = before.fluidDigest;
	traceItems = before.items;
	traceItem = before.itemDigest;
	traceActors = before.actors;
	traceActor = before.actorDigest;
	traceFlowBindings = before.flowBindings;
	traceFlow = before.flowDigest;
	tracePlayer = before.playerDigest;
	tracePresentation = before.presentationDigest;
	return before.worldCells == World.VOLUME && before.fluids == 2 && before.items == 1 && before.actors == 2 ? 0 : 12;
}

/** Compare every scalar in the target-neutral semantic trace. */
function sameTrace(left:caxecraft.content.ResolvedLevelPlan.ResolvedLevelSemanticTrace,
		right:caxecraft.content.ResolvedLevelPlan.ResolvedLevelSemanticTrace):Bool
	return left.scenarioDigest == right.scenarioDigest
		&& left.worldCells == right.worldCells
		&& left.terrainRuns == right.terrainRuns
		&& left.terrainDigest == right.terrainDigest
		&& left.fluids == right.fluids
		&& left.fluidDigest == right.fluidDigest
		&& left.items == right.items
		&& left.itemDigest == right.itemDigest
		&& left.actors == right.actors
		&& left.actorDigest == right.actorDigest
		&& left.flowBindings == right.flowBindings
		&& left.flowDigest == right.flowDigest
		&& left.playerDigest == right.playerDigest
		&& left.presentationDigest == right.presentationDigest;

/**
 * Rename the fixture fluids so lexical ID order conflicts with placement order.
 *
 * Volumes must be constructed before sources because a source may intentionally
 * overlap its initial pool. Presentation must follow that same order even when
 * a source's ID sorts first, otherwise a valid plan is rejected at publication.
 */
function withInterleavedFluidIds(source:Scenario):Scenario {
	final fluids:Array<ScenarioFluid> = [];
	for (fluid in source.world.fluids)
		fluids.push({
			id: new ScenarioId(switch fluid.placement {
				case InitialVolume(_): "water.z-volume";
				case Source(_): "water.a-source";
			}),
			fluidType: fluid.fluidType,
			placement: fluid.placement
		});
	return {
		formatVersion: source.formatVersion,
		requiredFeatures: source.requiredFeatures.copy(),
		optionalFeatures: source.optionalFeatures.copy(),
		id: source.id,
		assetPack: source.assetPack,
		messages: source.messages,
		title: source.title,
		mode: source.mode,
		world: {
			size: source.world.size,
			palette: source.world.palette.copy(),
			chunks: source.world.chunks.copy(),
			fluids: fluids
		},
		objects: source.objects.copy(),
		story: source.story,
		flow: source.flow,
		extensions: source.extensions.copy()
	};
}

/**
 * Build an otherwise valid engine candidate that exceeds only item capacity.
 *
 * The base scenario has already passed the complete parser and validator. This
 * copy preserves its actors and player spawn, replaces the one item with 257
 * unique, individually valid item placements, and leaves the source untouched.
 */
function withTooManyItems(source:Scenario):Scenario {
	final objects:Array<ScenarioObject> = [];
	for (object in source.objects)
		switch object.placement {
			case Item(_, _, _):
			case _:
				objects.push(object);
		}
	for (index in 0...257)
		objects.push({
			id: new ScenarioId("item.capacity-" + index),
			tags: [],
			placement: ObjectPlacement.Item(new ContentId("caxecraft:tideweave-suit"), 1, {
				xMilli: 4500,
				yMilli: 5000,
				zMilli: 4500,
				yawDegrees: 0
			})
		});
	return {
		formatVersion: source.formatVersion,
		requiredFeatures: source.requiredFeatures.copy(),
		optionalFeatures: source.optionalFeatures.copy(),
		id: source.id,
		assetPack: source.assetPack,
		messages: source.messages,
		title: source.title,
		mode: source.mode,
		world: source.world,
		objects: objects,
		story: source.story,
		flow: source.flow,
		extensions: source.extensions.copy()
	};
}

/** Exact fault selected for one otherwise real content-registry wrapper. */
private enum RegistryRejection {
	MissingTerrain;
	WrongActorKind;
}

/**
 * Injects one resolver failure while delegating every other real pack fact.
 *
 * A wrapper is used instead of a hand-built fake registry so the negative case
 * cannot accidentally drift from the built-in content definitions unrelated to
 * the fault under test.
 */
private final class RejectingLevelRegistry implements LevelContentResolver {
	final rejection:RegistryRejection;
	final base:FocusedContentRegistry;

	public function new(rejection:RegistryRejection) {
		this.rejection = rejection;
		base = new FocusedContentRegistry();
	}

	public function resolveTerrain(id:ContentId):TerrainContentResolution {
		if (rejection == MissingTerrain && id.text() == "caxecraft:grass")
			return UnknownTerrainContent;
		return base.resolveTerrain(id);
	}

	public function resolveFluid(id:ContentId):FluidContentResolution
		return base.resolveFluid(id);

	public function resolveItem(id:ContentId):ItemContentResolution
		return base.resolveItem(id);

	public function resolveNpc(id:ContentId):ActorContentResolution
		return base.resolveNpc(id);

	public function resolveEnemy(id:ContentId):ActorContentResolution {
		if (rejection == WrongActorKind && id.text() == "caxecraft:mossling")
			return WrongActorContentKind(NpcContent);
		return base.resolveEnemy(id);
	}
}
