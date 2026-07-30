/**
 * Second concrete implementation used to exercise both branches of a
 * child-interface to parent-interface table map.
 *
 * Keeping two reachable implementations matters: the generated C must inspect
 * the child's table and select the matching parent table, rather than relying
 * on one globally fixed table that happens to work for a single class.
 */
final class AlternateCounter implements CounterView {
	var base:Int;

	public function new(base:Int) {
		this.base = base;
	}

	public function read(delta:Int):Int
		return base + delta;

	public function doubled():Int
		return base * 2;
}
