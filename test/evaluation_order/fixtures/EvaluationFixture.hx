class EvaluationFixture {
	static var callFlag:Bool = false;
	static var shortCircuitIntact:Bool = true;
	static var ternaryIntact:Bool = true;
	static var counter:UInt = 0;

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

	static function run():UInt {
		callFlag = false;
		consumePair(setCallFlag(true), setCallFlag(false));

		shortCircuitIntact = true;
		false && spoilShortCircuit();
		true || spoilShortCircuit();

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
