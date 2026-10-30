package caxecraft.domain;

import caxecraft.gameplay.AuthoredItemSlots;
#if c
import c.CArray;
import c.UInt8;
#end

/**
	Selects only the storage representation inherited by `GameSession`.

	The session remains the direct lifetime owner and performs every domain
	operation. Narrow friend access lets that cohesive owner borrow the exact
	fields without exposing mutable storage to other modules. A later target can
	provide this same base type from a target-specific source root.
**/
class SessionStorageBase {
	#if c
	@:allow(caxecraft.domain.GameSession)
	private final worldStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
	@:allow(caxecraft.domain.GameSession)
	private final authoredItemStorage:CArray<Int, AuthoredItemSlots> = CArray.zero(AuthoredItemSlots.CAPACITY);
	#else
	@:allow(caxecraft.domain.GameSession)
	private final worldStorage:PortableWorldStorage = PortableWorldStorage.zero();
	@:allow(caxecraft.domain.GameSession)
	private final authoredItemStorage:PortableItemStorage = PortableItemStorage.zero();
	#end

	/** Construct the target-selected fields before the session uses them. */
	public function new() {}
}

#if !c
/** Gives the portable Array the same view operations as native `CArray`. */
private abstract PortableWorldStorage(Array<Int>) {
	/** Construct one initialized portable world. */
	public static function zero():PortableWorldStorage {
		final storage:Array<Int> = [];
		var index = 0;
		while (index < World.VOLUME) {
			storage.push(0);
			index++;
		}
		return new PortableWorldStorage(storage);
	}

	private inline function new(storage:Array<Int>)
		this = storage;

	/** Match the native mutable-view operation for shared session source. */
	public inline function span():WorldCells
		return this;

	/** Match the native read-only-view operation for shared session source. */
	public inline function constSpan():WorldView
		return WorldView.borrow(this);
}

/** Gives portable item flags native-style views and indexed access. */
private abstract PortableItemStorage(Array<Int>) {
	/** Construct initialized portable item flags. */
	public static function zero():PortableItemStorage {
		final storage:Array<Int> = [];
		var index = 0;
		while (index < AuthoredItemSlots.CAPACITY) {
			storage.push(0);
			index++;
		}
		return new PortableItemStorage(storage);
	}

	private inline function new(storage:Array<Int>)
		this = storage;

	/** Match the native read-only-view operation for shared session source. */
	public inline function constSpan():AuthoredItemView
		return AuthoredItemView.borrow(this);

	/** Read one validated item slot. */
	@:arrayAccess
	public inline function get(index:Int):Int
		return this[index];

	/** Store one validated item slot. */
	@:arrayAccess
	public inline function set(index:Int, value:Int):Int
		return this[index] = value;
}
#end
