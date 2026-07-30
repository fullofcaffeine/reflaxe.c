package c;

/**
	Non-null mutable pointer contract with a caller-proven lifetime.

	`Ref.to(value)` borrows one addressable Haxe variable, field, or fixed-array
	element for exactly one direct imported-C call. The admitted value is an exact
	scalar carrier or a complete header-owned struct. The C function may read or
	replace that value before it returns, but it may not retain the pointer.
	haxe.c rejects temporaries, Haxe-owned aggregates, storage in another value,
	returns, and indirect calls so this small surface does not pretend to be a
	general borrow checker.
**/
@:coreType
extern abstract Ref<T> {
	/**
		Borrow the address of mutable storage for one direct imported-C call.

		This is a compiler intrinsic: it creates no wrapper and emits C's `&`
		operator at the call site. The argument must be assignable storage whose
		exact carrier matches `T`. Imported structs must already be initialized;
		this operation borrows storage and does not create uninitialized C memory.
	**/
	public static function to<T>(value:T):Ref<T>;
}
