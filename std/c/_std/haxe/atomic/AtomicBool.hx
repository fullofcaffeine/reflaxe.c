package haxe.atomic;

import c.Atomic;
import c.MemoryOrder;

/** Owns one shared C11 boolean storage location for `AtomicBool`. */
private final class AtomicBoolData {
	public final value:Atomic<Bool>;

	/** Initialize the holder before another thread can observe it. */
	public function new(initial:Bool) {
		value = new Atomic(initial);
	}
}

/** Provides the pinned Haxe atomic-boolean contract through C11 atomics. */
abstract AtomicBool(AtomicBoolData) {
	/** Create one shared Boolean location initialized to `value`. */
	public inline function new(value:Bool):Void {
		this = new AtomicBoolData(value);
	}

	/** Replace `expected` when present and return the observed prior value. */
	public inline function compareExchange(expected:Bool, replacement:Bool):Bool {
		return this.value.compareExchange(expected, replacement, MemoryOrder.SequentiallyConsistent, MemoryOrder.SequentiallyConsistent);
	}

	/** Replace the Boolean atomically and return its prior value. */
	public inline function exchange(value:Bool):Bool {
		return this.value.exchange(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Read the current Boolean with sequentially consistent ordering. */
	public inline function load():Bool {
		return this.value.load(MemoryOrder.SequentiallyConsistent);
	}

	/** Store `value` atomically and return that stored value. */
	public inline function store(value:Bool):Bool {
		return this.value.store(value, MemoryOrder.SequentiallyConsistent);
	}
}
