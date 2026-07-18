class CycleA {
	public function new() {
		var peer = new CycleB();
		while (peer == null) {}
	}
}

class CycleB {
	public function new() {
		var peer = new CycleA();
		while (peer == null) {}
	}
}

class Main {
	static function main():Void {
		var value = new CycleA();
		while (value == null) {}
	}
}
