package c;

/**
	Owns one non-copyable C11 atomic storage location.

	Embed this value in an owner, then use its methods through that addressable
	field. Ordinary Haxe assignment must not copy the storage. The standard
	`haxe.atomic` wrappers use managed holders so wrapper assignment still shares
	one location.
**/
@:coreType
extern abstract Atomic<T> {
	/** Initialize one previously uninitialized atomic storage location. */
	public function new(value:T):Void;

	/** Read the current value with the selected memory order. */
	public function load(order:MemoryOrder):T;

	/** Store a value and return that stored value. */
	public function store(value:T, order:MemoryOrder):T;

	/** Replace the value and return the prior value. */
	public function exchange(value:T, order:MemoryOrder):T;

	/** Compare by value, replace on equality, and return the observed prior value. */
	public function compareExchange(expected:T, replacement:T, success:MemoryOrder, failure:MemoryOrder):T;

	/** Add an integer operand and return the prior value. */
	public function fetchAdd(value:T, order:MemoryOrder):T;

	/** Subtract an integer operand and return the prior value. */
	public function fetchSub(value:T, order:MemoryOrder):T;

	/** Apply bitwise AND and return the prior integer value. */
	public function fetchAnd(value:T, order:MemoryOrder):T;

	/** Apply bitwise OR and return the prior integer value. */
	public function fetchOr(value:T, order:MemoryOrder):T;

	/** Apply bitwise XOR and return the prior integer value. */
	public function fetchXor(value:T, order:MemoryOrder):T;
}
