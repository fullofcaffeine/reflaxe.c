/**
 * Keeps all four dynamic bounds failures reachable in one generated program.
 *
 * The small native harness selects one branch and records which branch ran.
 * This lets the test compile Haxe once per profile/build pair, then prove each
 * local/parameter and upper/negative abort independently.
 */
class BoundsMatrixFixture {
	static function main():Void {
		final selected = SpanBoundsHarness.span_bounds_case();
		if (selected == 0) {
			SpanBoundsHarness.span_bounds_mark(0);
			UpperBoundsFixture.main();
		} else if (selected == 1) {
			SpanBoundsHarness.span_bounds_mark(1);
			NegativeBoundsFixture.main();
		} else if (selected == 2) {
			SpanBoundsHarness.span_bounds_mark(2);
			ParameterUpperBoundsFixture.main();
		} else {
			SpanBoundsHarness.span_bounds_mark(3);
			ParameterNegativeBoundsFixture.main();
		}
	}
}
