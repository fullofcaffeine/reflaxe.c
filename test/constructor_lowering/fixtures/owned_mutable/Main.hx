class Child {
	public function new() {}
}

class Parent {
	public var child:Child = new Child();

	public function new() {}
}

class Main {
	static function main():Void {
		var parent = new Parent();
		while (parent == null) {}
	}
}
