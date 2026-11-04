package haxe.atomic;

import c.Atomic;
import c.MemoryOrder;

/** Owns one shared atomic managed reference for `AtomicObject`. */
private final class AtomicObjectData<T:{}> {
	public final value:Atomic<T>;

	/** Initialize the holder before another thread can observe it. */
	public function new(initial:T) {
		value = new Atomic(initial);
	}
}

/**
	Provides the pinned atomic-reference contract through C11 atomics.

	The collector traces the field through an atomic load. Atomic storage protects
	only the reference itself; it does not make fields of the referenced object
	thread-safe.
**/
abstract AtomicObject<T:{}>(AtomicObjectData<T>) {
	/** Create one shared reference location initialized to `value`. */
	public inline function new(value:T):Void {
		this = new AtomicObjectData(value);
	}

	/** Replace the identical expected reference and return the observed prior reference. */
	public inline function compareExchange(expected:T, replacement:T):T {
		return this.value.compareExchange(expected, replacement, MemoryOrder.SequentiallyConsistent, MemoryOrder.SequentiallyConsistent);
	}

	/** Replace the reference atomically and return its prior value. */
	public inline function exchange(value:T):T {
		return this.value.exchange(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Read and root the current reference with sequentially consistent ordering. */
	public inline function load():T {
		return this.value.load(MemoryOrder.SequentiallyConsistent);
	}

	/** Store `value` atomically and return that stored reference. */
	public inline function store(value:T):T {
		return this.value.store(value, MemoryOrder.SequentiallyConsistent);
	}
}
