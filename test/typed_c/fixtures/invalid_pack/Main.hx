package;

@:c.layout(c.Layout.Struct)
@:c.pack(3)
extern class InvalidPack {
	public var value:c.UInt32;
}

class Main {
	static var value:InvalidPack;

	static function main():Void {}
}
