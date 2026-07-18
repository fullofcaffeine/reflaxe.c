import c.CArray;
import c.Span;
import c.UInt8;

class StoredSpanGlobalFixture {
	static var borrowed:Span<UInt8>;

	static function remember(values:Span<UInt8>):Void {
		borrowed = values;
	}

	static function main():Void {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		var view:Span<UInt8> = values.span();
		remember(view);
	}
}
