package;

@:c.include("vendor.h", c.IncludeKind.System)
@:c.link("-lfoo;rm")
extern class UnsafeBuildFact {}

class Main {
	static var value:UnsafeBuildFact;

	static function main():Void {}
}
