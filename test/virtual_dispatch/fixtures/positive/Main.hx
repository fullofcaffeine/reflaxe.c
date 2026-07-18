class Main {
	static function main():Void {
		var leaf:BaseWorker = new LeafWorker(5);
		var middle:BaseWorker = new MiddleWorker(2);
		var exact = new FinalWorker(7);
		var derived = new LeafWorker(1);
		while (!(leaf.value(1) == 116 && middle.value(3) == 15 && leaf.finalValue(2) == 8 && leaf.pinnedValue(3) == 10 && exact.value(3) == 10
			&& derived.value(2) == 113)) {}
	}
}
