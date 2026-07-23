/** Small concrete implementation used to prove multiple interface tables without a runtime. */
final class Counter implements CounterView implements ResetView {
	var base:Int;

	public function new(base:Int) {
		this.base = base;
	}

	public function read(delta:Int):Int
		return base + delta;

	public function doubled():Int
		return base * 2;

	public function resetValue():Int
		return 0;
}
