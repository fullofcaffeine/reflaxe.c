import c.CArray;
import c.Span;
import c.UInt8;

private class StoredSpanHolder {
	public var borrowed:Span<UInt8>;

	public function new(borrowed:Span<UInt8>) {
		this.borrowed = borrowed;
	}
}

class StoredSpanFieldFixture {
	static function main():Void {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		var view:Span<UInt8> = values.span();
		new StoredSpanHolder(view);
	}
}
