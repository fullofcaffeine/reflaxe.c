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

	public static function enumInstanceKey(haxePath:String, argumentKeys:Array<String>):String
		return 'haxe-enum-v1(${canonicalPart(haxePath)}${canonicalArray(argumentKeys)})';

	public static function canonicalArray(values:Array<String>):String
		return '${values.length}:${values.map(canonicalPart).join("")}';

	public static function canonicalPart(value:String):String
		return '${Bytes.ofString(value).length}:$value';
}
