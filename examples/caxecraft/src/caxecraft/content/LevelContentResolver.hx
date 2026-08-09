package caxecraft.content;

import caxecraft.content.ActorContentResolver;
import caxecraft.scenario.ContentId;

/**
 * Turns validated content names into compact engine construction values.
 *
 * CaxeMap keeps readable IDs such as `caxecraft:grass`. A selected content
 * registry implements this boundary to resolve those IDs into nominal storage
 * values and one closed fluid simulation choice before a level can mutate a
 * session. The editor and authoring format continue to use names; only the
 * private runtime construction plan receives these compact values.
 */
/**
 * Compact terrain byte admitted by the selected content registry.
 *
 * An abstract is used instead of `Int` so a block code cannot be confused with
 * an item code, count, or array index. It remains a direct integer in generated
 * C. The registry validates the value before construction; the level resolver
 * checks the engine-supported range again before accepting it.
 */
abstract TerrainStorageCode(Int) {
	private inline function new(value:Int)
		this = value;

	/** Wrap one code after a typed content registry has validated its definition. */
	public static inline function fromValidated(value:Int):TerrainStorageCode
		return new TerrainStorageCode(value);

	/** Reveal the scalar only at the session/world storage boundary. */
	public inline function value():Int
		return this;
}

/**
 * Pack-local item code admitted by the selected content registry.
 *
 * This nominal scalar prevents item identities from being mixed with terrain
 * bytes or slot indices while retaining a direct C integer representation.
 */
abstract ItemStorageCode(Int) {
	private inline function new(value:Int)
		this = value;

	/** Wrap one code after a typed content registry has validated its definition. */
	public static inline function fromValidated(value:Int):ItemStorageCode
		return new ItemStorageCode(value);

	/** Reveal the scalar only at the item/session storage boundary. */
	public inline function value():Int
		return this;
}

/**
 * Closed fluid simulators understood by the current engine.
 *
 * A later pack may select another admitted simulator, but content can never
 * smuggle an arbitrary callback or native function through this value.
 */
enum LevelFluidSimulation {
	/** Use Caxecraft's deterministic bounded voxel-water simulation. */
	BoundedWater;
}

/** Result of resolving one block name into engine terrain storage. */
enum TerrainContentResolution {
	/** The content exists and supplies one nominal compact world code. */
	TerrainContentResolved(code:TerrainStorageCode);

	/** The selected pack contains no block definition for this ID. */
	UnknownTerrainContent;
}

/** Result of resolving one fluid name into simulation and presentation facts. */
enum FluidContentResolution {
	/** The content exists and both runtime and visual facts are complete. */
	FluidContentResolved(simulation:LevelFluidSimulation, presentationCell:Int);

	/** The selected pack contains no fluid definition for this ID. */
	UnknownFluidContent;
}

/** Result of resolving one item name into pack-local runtime storage. */
enum ItemContentResolution {
	/** The content exists and supplies one nominal item code. */
	ItemContentResolved(code:ItemStorageCode);

	/** The selected pack contains no item definition for this ID. */
	UnknownItemContent;
}

/** Result of resolving one character-like content ID into an entity-atlas cell. */
enum ActorPresentationResolution {
	/** The content exists and names one validated cell in the admitted actor atlas. */
	ActorPresentationResolved(cellIndex:Int);

	/** No NPC or enemy definition in the selected pack owns this ID. */
	UnknownActorPresentation;
}

/** Closed collision behavior selected by one authored object state. */
enum StatefulObjectCollisionProfile {
	/** Characters may move through the object's authored box in this state. */
	StatefulObjectPassable;

	/** The object's authored box blocks character movement in this state. */
	StatefulObjectSolid;
}

/** One closed state mechanic retained for later CaxeFlow state changes. */
typedef StatefulObjectStateMechanics = {
	/** Stable state identity used by CaxeFlow. */
	final state:ContentId;

	/** Whether this state contributes the object's collision box. */
	final collision:StatefulObjectCollisionProfile;

	/** Whether presentation draws this state. */
	final visible:Bool;
}

/** Milliblock dimensions of one box centered on the placement's X/Z position. */
typedef StatefulObjectBounds = {
	/** Full extent along the object's local X axis. */
	final widthMilli:Int;

	/** Full extent above the placement's world Y position. */
	final heightMilli:Int;

	/** Full extent along the object's local Z axis. */
	final depthMilli:Int;
}

/** Result of resolving one interactive-object profile in one authored state. */
enum StatefulObjectContentResolution {
	/** Complete mechanics and initial presentation agree for this object profile. */
	StatefulObjectContentResolved(interactionRadiusMilli:Int, bounds:StatefulObjectBounds, states:Array<StatefulObjectStateMechanics>,
		presentationAsset:String, presentationCell:Int);

	/** The object is absent or does not admit the requested state. */
	UnknownStatefulObjectContent;
}

/**
 * Typed content operations needed to resolve one complete level.
 *
 * This interface extends actor resolution because one selected registry must
 * answer every construction question consistently. It deliberately exposes no
 * file access, parser, renderer handle, or mutable session operation.
 */
interface LevelContentResolver extends ActorContentResolver {
	/** Resolve one validated block ID into compact terrain storage. */
	function resolveTerrain(id:ContentId):TerrainContentResolution;

	/** Resolve one validated fluid ID into simulation and presentation choices. */
	function resolveFluid(id:ContentId):FluidContentResolution;

	/** Resolve one validated item ID into pack-local runtime storage. */
	function resolveItem(id:ContentId):ItemContentResolution;

	/** Resolve one validated NPC or enemy ID into its actor presentation cell. */
	function resolveActorPresentation(id:ContentId):ActorPresentationResolution;

	/** Resolve one stateful object and its initial state into bounded runtime facts. */
	function resolveStatefulObject(id:ContentId, state:ContentId):StatefulObjectContentResolution;
}
