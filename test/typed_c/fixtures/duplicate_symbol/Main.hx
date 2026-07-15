package;

@:c.layout(c.Layout.Opaque)
@:c.name("same_symbol")
extern class FirstHandle {}

@:c.layout(c.Layout.Opaque)
@:c.name("same_symbol")
extern class SecondHandle {}

class Main {
	static var first:FirstHandle;
	static var second:SecondHandle;

	static function main():Void {}
}
