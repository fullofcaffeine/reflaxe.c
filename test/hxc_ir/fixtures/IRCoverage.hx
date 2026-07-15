typedef CoverageRecord = {
	var x:Int;
}

enum CoverageOption {
	Some(value:Int);
	None;
}

interface CoverageInterface {
	function measure(value:Int):Int;
}

class CoverageObject {
	public function new() {}

	public function render(value:Int):Int
		return value;
}

class IRCoverage {
	public static function target():Void {}

	public static function throwValue(value:Dynamic):Void
		throw value;
}
