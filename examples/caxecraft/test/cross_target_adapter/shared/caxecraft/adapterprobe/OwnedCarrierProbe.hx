package caxecraft.adapterprobe;

import caxecraft.domain.WorldCells;
import caxecraft.domain.World;
import caxecraft.domain.WorldStorage;
import caxecraft.domain.WorldView;
import caxecraft.domain.WorldViewStorage.readCode;
import caxecraft.domain.WorldVolume;
#if c
import c.CArray;
import c.UInt8;
#end

/**
	Proves that a class directly owns the selected carrier behind an abstract.

	The returned view borrows from the exact owner receiver. This mirrors the
	`GameSession` boundary without pulling the complete game session into the
	focused architecture probe.
**/
final class OwnedCarrierProbe extends OwnedCarrierStorageBase {
	/** Construct one zeroed owner. */
	public function new()
		super();

	/** Write one code through the shared mutable-view contract. */
	public function writeFirst(value:Int):Void {
		final cells:WorldCells = world.span();
		WorldStorage.writeCode(cells, 0, value);
	}

	/** Lend one read-only view whose lifetime is tied to this receiver. */
	public function view():WorldView
		return world.constSpan();

	/** Read one code through the shared read-only contract. */
	public static function readFirst(view:WorldView):Int
		return readCode(view, 0);
}

/** Selects only the carrier inherited by the focused direct owner. */
class OwnedCarrierStorageBase {
	#if c
	@:allow(caxecraft.adapterprobe.OwnedCarrierProbe)
	private final world:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
	#else
	@:allow(caxecraft.adapterprobe.OwnedCarrierProbe)
	private final world:PortableProbeStorage = PortableProbeStorage.zero();
	#end

	/** Construct the target-selected carrier. */
	public function new() {}
}

#if !c
/** Gives an Array the two view operations used by the shared owner. */
private abstract PortableProbeStorage(Array<Int>) {
	/** Construct one initialized portable world. */
	public static function zero():PortableProbeStorage {
		final storage:Array<Int> = [];
		var index = 0;
		while (index < World.VOLUME) {
			storage.push(0);
			index++;
		}
		return new PortableProbeStorage(storage);
	}

	private inline function new(storage:Array<Int>)
		this = storage;

	/** Match the native mutable-view operation. */
	public inline function span():WorldCells
		return this;

	/** Match the native read-only-view operation. */
	public inline function constSpan():WorldView
		return WorldView.borrow(this);
}
#end
