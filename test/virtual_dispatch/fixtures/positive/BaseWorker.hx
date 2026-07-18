class BaseWorker {
	public var seed:Int;

	public function new(seed:Int) {
		this.seed = seed;
	}

	public function value(delta:Int):Int {
		return seed + delta;
	}

	public final function finalValue(delta:Int):Int {
		return privateValue(delta) + 1;
	}

	@:nonVirtual
	public function pinnedValue(delta:Int):Int {
		return seed + delta + 2;
	}

	private function privateValue(delta:Int):Int {
		return seed + delta;
	}
}
