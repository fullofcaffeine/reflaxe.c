package;

@:c.layout(c.Layout.Struct)
@:c.pack(1)
extern class MetalPackedStruct {
	public var value:c.UInt32;
}

class Main {
	static var value:MetalPackedStruct;

	static function main():Void {}
}
