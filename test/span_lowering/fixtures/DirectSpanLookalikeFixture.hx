import LookalikeFixture.SpanLookalike;
import c.CArray;
import c.Span;

/** An unrelated method named span cannot borrow an array by using the intrinsic's name. */
class DirectSpanLookalikeFixture {
	static function first(values:Span<Int>):Int {
		return values[0];
	}

	static function main():Void {
		final values:CArray<Int, Length4> = [1, 2, 3, 4];
		first(SpanLookalike.span(values));
	}
}
