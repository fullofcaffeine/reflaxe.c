class Child {
	public function new(shouldFail:Bool) {
		if (shouldFail)
			throw 1;
	}
}

class Parent {
	public final child:Child = new Child(false);

	public function new() {}
}

class Main {
	static function main():Void {
		var parent = new Parent();
		while (parent == null) {}
	}
}
