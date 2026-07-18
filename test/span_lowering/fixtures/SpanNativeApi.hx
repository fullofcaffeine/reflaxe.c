import c.ConstSpan;
import c.UInt8;

@:c.include("span_native.h", c.IncludeKind.Local)
extern class SpanNativeApi {
	@:c.name("span_native_read")
	public static function read(values:ConstSpan<UInt8>, index:Int):UInt8;
}
