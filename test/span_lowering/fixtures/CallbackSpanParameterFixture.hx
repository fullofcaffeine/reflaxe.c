import c.CArray;
import c.ConstSpan;
import c.UInt8;

class CallbackSpanParameterFixture {
	static function read(values:ConstSpan<UInt8>, index:Int):UInt8 {
		return values[index];
	}

	static function main():Void {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		var view:ConstSpan<UInt8> = values.constSpan();
		var callback:(ConstSpan<UInt8>, Int) -> UInt8 = read;
		callback(view, 0);
	}
}
