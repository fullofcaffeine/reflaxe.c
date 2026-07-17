typedef OrderA = {
	var z:Int;
	var a:Int;
}

typedef OrderB = {
	var a:Int;
	var z:Int;
}

typedef Envelope = {
	var point:OrderB;
	var enabled:Bool;
}

class AggregateFixture {
	static function identity(value:Int):Int {
		return value;
	}

	static function make(left:Int, right:Int):OrderA {
		return {z: identity(left), a: identity(right)};
	}

	static function copy(value:OrderA):OrderB {
		var copied:OrderB = value;
		return copied;
	}

	static function sum(value:OrderB):Int {
		return value.a + value.z;
	}

	static function localSum(left:Int, right:Int):Int {
		var value:OrderA = make(left, right);
		return value.a + value.z;
	}

	static function envelope(value:OrderA):Envelope {
		return {point: value, enabled: true};
	}

	static function envelopeSum(value:Envelope):Int {
		return value.enabled ? value.point.a + value.point.z : 0;
	}

	static function main():Void {
		var first:OrderA = make(3, 4);
		var copied:OrderB = copy(first);
		var nested:Envelope = envelope(copied);
		while (!(sum(copied) == 7 && localSum(5, 6) == 11 && envelopeSum(nested) == 7)) {}
	}
}
