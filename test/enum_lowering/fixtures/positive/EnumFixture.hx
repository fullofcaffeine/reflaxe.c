enum Mode {
	Off;
	On;
}

enum Option<T> {
	None;
	Some(value:T);
}

enum Chain<T> {
	End(value:T);
	Link(value:T, next:Chain<T>);
}

class EnumFixture {
	static function identity(value:Int):Int {
		return value;
	}

	static function modeValue(value:Mode):Int {
		return switch value {
			case Off: 0;
			case On: 1;
		};
	}

	static function optionValue(value:Option<Int>):Int {
		return switch value {
			case None: 0;
			case Some(payload): payload;
		};
	}

	static function guardedValue(value:Option<Int>):Int {
		return switch value {
			case Some(payload) if (payload > 4): payload;
			case Some(payload): payload + 1;
			case None: -1;
		};
	}

	static function boolOptionValue(value:Option<Bool>):Int {
		return switch value {
			case None: -1;
			case Some(payload): payload ? 1 : 0;
		};
	}

	static function recursiveLocal():Int {
		var tail:Chain<Int> = End(2);
		var head:Chain<Int> = Link(1, tail);
		return switch head {
			case End(value): value;
			case Link(value, next):
				switch next {
					case End(last): value + last;
					case Link(_, _): 0;
				};
		};
	}

	static function main():Void {
		var mode = On;
		var present:Option<Int> = Some(identity(7));
		var absent:Option<Int> = None;
		var truth:Option<Bool> = Some(true);
		while (!(modeValue(mode) == 1 && optionValue(present) == 7 && optionValue(absent) == 0 && guardedValue(present) == 7 && boolOptionValue(truth) == 1
			&& recursiveLocal() == 3)) {}
	}
}
