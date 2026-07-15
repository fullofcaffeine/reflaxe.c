package;

@:c.layout(c.Layout.Opaque)
extern class OpaqueStorage {}

@:c.layout(c.Layout.Struct)
extern class InvalidContainer {
	public var storage:OpaqueStorage;
}

class Main {
	static var value:InvalidContainer;

	static function main():Void {}
}
