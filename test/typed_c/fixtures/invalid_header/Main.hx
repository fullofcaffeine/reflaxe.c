package;

@:c.layout(c.Layout.Struct)
@:c.header("../escape.h", c.Header.Public)
extern class InvalidHeader {
	public var value:c.Int32;
}

class Main {
	static var value:InvalidHeader;

	static function main():Void {}
}
