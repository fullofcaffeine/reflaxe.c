import c.CArray;
import c.ConstSpan;
import c.UInt8;

class NativeSpanParameterFixture {
	static function main():Void {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		var view:ConstSpan<UInt8> = values.constSpan();
		SpanNativeApi.read(view, 0);
	}
}
