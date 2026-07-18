class LeafWorker extends MiddleWorker {
	public function new(seed:Int) {
		super(seed);
	}

	public override function value(delta:Int):Int {
		return super.value(delta) + 100;
	}
}
