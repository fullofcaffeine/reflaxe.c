class EvaluationFixture {
	static var callFlag:Bool = false;
	static var shortCircuitIntact:Bool = true;
	static var ternaryIntact:Bool = true;
	static var counter:UInt = 0;
	static var switchCalls:UInt = 0;

	static function setCallFlag(value:Bool):Bool {
		callFlag = value;
		return value;
	}

	static function consumePair(first:Bool, second:Bool):Void {
		first;
		second;
	}

	static function spoilShortCircuit():Bool {
		shortCircuitIntact = false;
		return true;
	}

	static function keepTernary():UInt {
		return 7;
	}

	static function spoilTernary():UInt {
		ternaryIntact = false;
		return 8;
	}

	static function finish(value:UInt):UInt {
		return value;
	}

	static function switchSubject(value:Int):Int {
		switchCalls++;
		return value;
	}

	static function run():UInt {
		callFlag = false;
		consumePair(setCallFlag(true), setCallFlag(false));

		shortCircuitIntact = true;
		false
		&& spoilShortCircuit();
		true
		|| spoilShortCircuit();
		true
		&& setCallFlag(true);
		if (!callFlag)
			shortCircuitIntact = false;
		false
		|| setCallFlag(false);

		if (callFlag)
			shortCircuitIntact = false;

		ternaryIntact = true;
		var selected:UInt = ternaryIntact ? keepTernary() : spoilTernary();

		counter = 0;
		var old:UInt = counter++;
		var fresh:UInt = ++counter;
		old;
		fresh;
		selected;

		var assigned:UInt = 99;
		assigned = counter;
		var result:UInt = callFlag ? 91 : (shortCircuitIntact ? (ternaryIntact ? assigned : 93) : 94);

		var total:Int = 0;
		var outer:Int = 0;
		while (outer < 4) {
			outer++;
			if (outer == 2)
				continue;
			var inner:Int = 0;
			do {
				inner++;
				if (inner == 2 && outer == 3)
					break;
				switch (inner) {
					case 1:
						total += outer;
					case 2, 3:
						total += inner;
					default:
						total += 9;
				}
			} while (inner < 3);
		}

		var rangeTotal:Int = 0;
		var rangeEnd:Int = 3;
		for (index in 0...rangeEnd)
			rangeTotal += index;

		switchCalls = 0;
		var selectedBySwitch:UInt = switch (switchSubject(total)) {
			case 18: 2;
			case 19: 3;
			default: 95;
		};
		var controlIntact:Bool = true;
		if (total != 18)
			controlIntact = false;
		if (rangeTotal != 3)
			controlIntact = false;
		if (switchCalls != 1)
			controlIntact = false;
		if (controlIntact)
			result = selectedBySwitch;
		else
			result = 96;
		return finish(result);
	}
	static function main():Void {
		#if evaluation_order_oracle
		Sys.println(run());
		#else
		run();
		#end
	}
}
