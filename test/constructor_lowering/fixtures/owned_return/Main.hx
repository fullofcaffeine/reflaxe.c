class Child {
	public var value:Int = 7;

	public function new() {}
}

class Parent {
	public final child:Child = new Child();

	public function new() {}

	public function expose():Child {
		return child;
	}
}

class Main {
	static function main():Void {
		var parent = new Parent();
		var escaped = parent.expose();
		while (escaped.value != 7) {}
	}
}
