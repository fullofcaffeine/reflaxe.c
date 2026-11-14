package haxe.atomic;

import c.Atomic;
import c.MemoryOrder;

/**
	Maps `AtomicInt` to one managed holder with embedded C11 atomic storage.

	The holder gives copied Haxe wrapper values shared identity. Its field remains
	private so all access keeps the selected atomic memory order.
**/
private final class AtomicIntData {
	public final value:Atomic<Int>;

	/** Initialize the holder before another thread can observe it. */
	public function new(initial:Int) {
		value = new Atomic(initial);
	}
}

/** Provides the pinned Haxe atomic-integer contract through C11 atomics. */
abstract AtomicInt(AtomicIntData) {
	/** Create one shared integer location initialized to `value`. */
	public inline function new(value:Int):Void {
		this = new AtomicIntData(value);
	}

	/** Add `value` atomically and return the prior value. */
	public inline function add(value:Int):Int {
		return this.value.fetchAdd(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Subtract `value` atomically and return the prior value. */
	public inline function sub(value:Int):Int {
		return this.value.fetchSub(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Apply bitwise AND atomically and return the prior value. */
	public inline function and(value:Int):Int {
		return this.value.fetchAnd(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Apply bitwise OR atomically and return the prior value. */
	public inline function or(value:Int):Int {
		return this.value.fetchOr(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Apply bitwise XOR atomically and return the prior value. */
	public inline function xor(value:Int):Int {
		return this.value.fetchXor(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Replace `expected` when present and return the observed prior value. */
	public inline function compareExchange(expected:Int, replacement:Int):Int {
		return this.value.compareExchange(expected, replacement, MemoryOrder.SequentiallyConsistent, MemoryOrder.SequentiallyConsistent);
	}

	/** Replace the integer atomically and return its prior value. */
	public inline function exchange(value:Int):Int {
		return this.value.exchange(value, MemoryOrder.SequentiallyConsistent);
	}

	/** Read the current integer with sequentially consistent ordering. */
	public inline function load():Int {
		return this.value.load(MemoryOrder.SequentiallyConsistent);
	}

	/** Store `value` atomically and return that stored value. */
	public inline function store(value:Int):Int {
		return this.value.store(value, MemoryOrder.SequentiallyConsistent);
	}
}
