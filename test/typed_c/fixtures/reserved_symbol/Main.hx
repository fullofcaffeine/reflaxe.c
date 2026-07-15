package;

@:c.layout(c.Layout.Opaque)
@:c.name("hxc_private_handle")
extern class ReservedHandle {}

class Main {
	static var value:ReservedHandle;

	static function main():Void {}
}
