import c.CArray;
import c.Span;

extern class SpanLookalike {
	public static function span<T, N>(values:CArray<T, N>):Span<T>;
}

class LookalikeFixture {
	static function main():Void {
		var values:CArray<Int, Length4> = [1, 2, 3, 4];
		var view:Span<Int> = SpanLookalike.span(values);
		view[0];
	}
}
