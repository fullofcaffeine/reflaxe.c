package;

@:c.layout(c.Layout.Struct)
extern class LeftNode {
	public var right:RightNode;
}

@:c.layout(c.Layout.Struct)
extern class RightNode {
	public var left:LeftNode;
}

class Main {
	static var value:LeftNode;

	static function main():Void {}
}
