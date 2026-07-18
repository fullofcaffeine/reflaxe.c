import c.CArray;
import c.Span;
import c.UInt8;

class EscapingSpanFixture {
	static function escape():Span<UInt8> {
		var values:CArray<UInt8, GridVolume> = CArray.zero(32 * 16 * 32);
		return values.span();
	}

	static function main():Void {
		escape();
	}
}
