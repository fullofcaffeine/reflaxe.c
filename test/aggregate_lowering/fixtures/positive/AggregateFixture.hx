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

enum abstract SwitchState(Int) {
	var Off = 0;
	var On = 1;
}

typedef SwitchRecord = {
	final state:SwitchState;
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

	static function makeSwitch(state:SwitchState):SwitchRecord {
		return {state: state};
	}

	static function switchIsOn(value:SwitchRecord):Bool {
		return value.state == SwitchState.On;
	}

	static function switchStateValue(value:SwitchState):Int {
		return switch value {
			case Off: 3;
			case On: 7;
			case _: -1;
		};
	}

	static function main():Void {
		var first:OrderA = make(3, 4);
		var copied:OrderB = copy(first);
		var nested:Envelope = envelope(copied);
		while (!(sum(copied) == 7
			&& localSum(5, 6) == 11
			&& envelopeSum(nested) == 7
			&& switchStateValue(SwitchState.On) == 7
			&& switchIsOn(makeSwitch(SwitchState.On))
			&& !switchIsOn(makeSwitch(SwitchState.Off)))) {}
	}
}
