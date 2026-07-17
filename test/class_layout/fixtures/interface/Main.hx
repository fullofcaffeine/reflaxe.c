interface Marker {
	public var value(default, null):Int;
}

class Main {
	static function isNull(value:Marker):Bool {
		return value == null;
	}

	static function main():Void {
		var value:Marker = null;
		while (!isNull(value)) {}
	}
}
