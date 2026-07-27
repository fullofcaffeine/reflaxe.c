import c.CArray;
import c.ConstSpan;
import c.UInt8;

/**
	Proves that a static function cannot claim receiver-owned span lifetime.

	The admitted read-only return contract is tied to one exact live instance.
	A static function has no such receiver, so even a locally valid `CArray`
	borrow must fail before generated C invents an owner for its pointer.
**/
class StaticConstSpanReturnFixture {
	static function escape():ConstSpan<UInt8> {
		var values:CArray<UInt8, Length4> = CArray.zero(4);
		return values.constSpan();
	}

	static function main():Void {
		escape();
	}
}
