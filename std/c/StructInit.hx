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
	public static function make<Fields, Result>(fields:Fields):Result;
}
