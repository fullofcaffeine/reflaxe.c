class Base {}
class Leaf extends Base {}

class Main {
	static function downcast(value:Base):Leaf {
		return Std.downcast(value, Leaf);
	}

	static function main():Void {
		var value:Base = null;
		var leaf:Leaf = downcast(value);
		while (leaf != null) {}
	}
}
