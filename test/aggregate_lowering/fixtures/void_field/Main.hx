package;

typedef BadRecord = {
	final value:Void;
}

class Main {
	static function make():BadRecord {
		throw "unreachable";
	}

	static function main():Void {
		make();
	}
}
