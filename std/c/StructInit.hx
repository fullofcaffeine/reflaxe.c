package c;

/**
 * Builds one header-owned C struct from a closed Haxe object literal.
 *
 * `Fields` preserves the literal's typed field set while `Result` is inferred
 * from the surrounding Haxe return or assignment type. The C target validates
 * that every imported struct field appears exactly once with the exact ABI
 * type, preserves field-expression evaluation order, and emits a C compound
 * literal. This operation allocates nothing, selects no `hxrt` feature, and is
 * portable only to Haxe targets that provide the same explicit intrinsic.
 *
 * This is not a cast and cannot reinterpret an arbitrary value. Pass a direct
 * object literal and give the result a header-owned `@:c.layout(Struct)` type.
 */
@:coreType
extern abstract StructInit {
	/**
	 * Create a completely zero-initialized header-owned C struct.
	 *
	 * The result type is inferred from the surrounding assignment or return.
	 * haxe.c emits C's structural `(struct T){0}` initializer, which initializes
	 * padding-independent field values without pretending that an out-parameter
	 * may read uninitialized storage.
	 */
	public static function zero<Result>():Result;

	/** Construct every field explicitly from one direct object literal. */
	public static function make<Fields, Result>(fields:Fields):Result;
}
