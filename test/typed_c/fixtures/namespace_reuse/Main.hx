package;

@:c.layout(c.Layout.Struct)
@:c.name("shared_name")
extern class SharedTag {
	@:c.name("value")
	public var value:c.Int32;
}

@:c.layout(c.Layout.Struct)
@:c.name("second_tag")
extern class SecondTag {
	@:c.name("value")
	public var value:c.Int32;
}

extern class SharedApi {
	@:c.name("shared_name")
	public static function sharedName():c.Int32;
}

class Main {
	static var first:SharedTag;
	static var second:SecondTag;

	static function main():Void {
		SharedApi.sharedName();
	}
}
