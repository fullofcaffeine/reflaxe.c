class DefaultReference {}

class DefaultRecord {
	public var defaultInt:Int;
	public var defaultBool:Bool;
	public var defaultFloat:Float;
	public var defaultReference:DefaultReference;
	public var observedInt:Int;
	public var observedBool:Bool;
	public var observedFloat:Float;

	public function new() {
		observedInt = defaultInt;
		observedBool = defaultBool;
		observedFloat = defaultFloat;
	}
}

class Main {
	static function main():Void {
		var value = new DefaultRecord();
		while (!(value.observedInt == 0 && value.observedBool == false && value.observedFloat == 0.0 && value.defaultReference == null)) {}
	}
}
