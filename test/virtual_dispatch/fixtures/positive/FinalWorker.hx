final class FinalWorker {
	public var seed:Int;

	public function new(seed:Int) {
		this.seed = seed;
	}

	public function value(delta:Int):Int {
		return seed + delta;
	}
}
