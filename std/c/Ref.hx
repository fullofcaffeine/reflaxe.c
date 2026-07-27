package c;

/**
	Non-null mutable pointer contract with a caller-proven lifetime.

	`Ref.to(value)` borrows one addressable Haxe variable or field for exactly one
	direct imported-C call. The C function may read or replace that value before
	it returns, but it may not retain the pointer. haxe.c rejects temporaries,
	storage in another value, returns, and indirect calls so this small surface
	does not pretend to be a general borrow checker.
**/
@:coreType
extern abstract Ref<T> {
	/**
		Borrow the address of mutable storage for one direct imported-C call.

		This is a compiler intrinsic: it creates no wrapper and emits C's `&`
		operator at the call site. The argument must be an assignable local or
		field whose exact carrier matches `T`.
	**/
	public static function to<T>(value:T):Ref<T>;
}
