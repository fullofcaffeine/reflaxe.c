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
}
