package reflaxe.c.lowering;

import haxe.io.Bytes;

/** Target-neutral identity and hard bounds for the generic specialization report. */
class CGenericSpecializationContract {
	public static inline final REPORT_SCHEMA_VERSION = 1;
	public static inline final REPORT_ALGORITHM = "hxc-generic-specialization-v1";
	public static inline final REPORT_STATUS = "analyzed-closed-specializations";
	public static inline final KEY_ENCODING = "length-prefixed-utf8-full-semantic-key";
	public static inline final COMPACT_NAME_DIGEST = "sha256-with-full-key-collision-check";
	public static inline final CODE_SIZE_ATTRIBUTION = "function definitions are isolated; enum bytes conservatively include the dependency-closed emitted enum block";
	public static inline final MAX_FUNCTION_SPECIALIZATIONS = 64;
	public static inline final MAX_TYPE_SPECIALIZATIONS = 64;
	public static inline final MAX_ESTIMATED_SPECIALIZATION_C_BYTES = 524288;

	public static function functionKey(baseFunctionId:String, argumentKeys:Array<String>):String
		return 'generic-function-v1(${canonicalPart(baseFunctionId)}${canonicalArray(argumentKeys)})';

	public static function enumArgumentKey(haxePath:String, argumentKeys:Array<String>):String
		return 'enum(${canonicalPart(haxePath)}${canonicalArray(argumentKeys)})';

	/** Preserve each nested element identity instead of collapsing every Array to one key. */
	public static function arrayArgumentKey(elementKey:String):String
		return 'array(${canonicalPart(elementKey)})';

	public static function stringArgumentKey():String
		return "string(utf8-scalar-indexed)";

	/**
		Keep a Haxe abstract's nominal identity beside its proven storage carrier.

		Two abstracts can use the same C representation while remaining different
		Haxe types. Including both the declaration path and closed type arguments
		prevents specialization sharing from erasing that source-level distinction;
		the carrier key proves that later lowering already understands the stored
		value.
	**/
	public static function transparentAbstractArgumentKey(haxePath:String, argumentKeys:Array<String>, carrierKey:String):String
		return 'transparent-abstract(${canonicalPart(haxePath)}${canonicalArray(argumentKeys)}${canonicalPart(carrierKey)})';

	/** Preserve a closed inner identity when Haxe's `Null<T>` wrapper is inferred. */
	public static function nullableArgumentKey(valueKey:String):String
		return 'nullable(${canonicalPart(valueKey)})';

	public static function recordArgumentKey(fieldKeys:Array<String>):String
		return 'closed-record(${canonicalArray(fieldKeys)})';

	public static function recordFieldKey(name:String, access:String, typeKey:String):String
		return '${canonicalPart(name)}${canonicalPart(access)}${canonicalPart(typeKey)}';

	public static function enumInstanceKey(haxePath:String, argumentKeys:Array<String>):String
		return 'haxe-enum-v1(${canonicalPart(haxePath)}${canonicalArray(argumentKeys)})';

	public static function canonicalArray(values:Array<String>):String
		return '${values.length}:${values.map(canonicalPart).join("")}';

	public static function canonicalPart(value:String):String
		return '${Bytes.ofString(value).length}:$value';
}
