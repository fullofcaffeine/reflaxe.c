package;

enum Mode {
	Off;
	On;
}

class Main {
	static function value(mode:Mode):Int {
		switch mode {
			case Off:
				return 0;
		}
		return 1;
	}

	static function main():Void {
		while (value(On) != 1) {}
	}
}
