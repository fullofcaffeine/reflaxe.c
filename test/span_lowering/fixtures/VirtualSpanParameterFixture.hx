import c.CArray;
import c.ConstSpan;
import c.UInt8;

private class BaseSpanReader {
	public function new() {}

	public function read(values:ConstSpan<UInt8>, index:Int):UInt8 {
		return values[index];
	}
}

private class DerivedSpanReader extends BaseSpanReader {
	public function new() {
		super();
	}

	public override function read(values:ConstSpan<UInt8>, index:Int):UInt8 {
		return values[index];
	}
}

class VirtualSpanParameterFixture {
	static function main():Void {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		var view:ConstSpan<UInt8> = values.constSpan();
		var reader:BaseSpanReader = new DerivedSpanReader();
		reader.read(view, 0);
	}
}
