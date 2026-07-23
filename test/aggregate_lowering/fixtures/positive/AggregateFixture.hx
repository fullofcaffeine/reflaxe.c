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

/** A direct optional record nested inside another ordinary record. */
typedef OptionalEnvelope = {
	final point:Null<OrderB>;
}

enum abstract SwitchState(Int) {
	var Off = 0;
	var On = 1;
}

typedef SwitchRecord = {
	final state:SwitchState;
}

enum ActorPhase {
	Waiting;
	Moving(speed:Int);
}

typedef ActorRecord = {
	final phase:ActorPhase;
}

typedef FlowRecord = {
	final first:Int;
	final second:Int;
	final third:Int;
	final order:Int;
}

class AggregateFixture {
	static function identity(value:Int):Int {
		return value;
	}

	/** Create the branch that crosses an IR block while building a field. */
	static inline function checkedField(value:Int):Int {
		return value < 0 ? -value : value;
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

	static function noPoint():Null<OrderB>
		return null;

	static function somePoint(value:OrderB):Null<OrderB>
		return value;

	static function optionalEnvelope(value:Null<OrderB>):OptionalEnvelope
		return {point: value};

	static function optionalSum(value:Null<OrderB>):Int
		return value == null ? 0 : value.a + value.z;

	static function makeSwitch(state:SwitchState):SwitchRecord {
		return {state: state};
	}

	static function makeActor(phase:ActorPhase):ActorRecord {
		return {phase: phase};
	}

	/** Ordinary left-to-right Haxe fields must survive later field branches. */
	static function makeFlowRecord(first:Int, second:Int, third:Int):FlowRecord {
		var order = 0;
		return {
			first: {
				order = order * 10 + 1;
				checkedField(first);
			},
			second: {
				order = order * 10 + 2;
				checkedField(second);
			},
			third: {
				order = order * 10 + 3;
				checkedField(third);
			},
			order: order
		};
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

	static function actorSpeed(value:ActorRecord):Int {
		return switch value.phase {
			case Waiting: 0;
			case Moving(speed): speed;
		};
	}

	static function main():Void {
		var first:OrderA = make(3, 4);
		var copied:OrderB = copy(first);
		var nested:Envelope = envelope(copied);
		var absent = optionalEnvelope(noPoint());
		var present = optionalEnvelope(somePoint(copied));
		var flow = makeFlowRecord(-8, 9, -10);
		while (!(sum(copied) == 7
			&& localSum(5, 6) == 11
			&& envelopeSum(nested) == 7
			&& absent.point == null
			&& present.point != null
			&& optionalSum(absent.point) == 0
			&& optionalSum(present.point) == 7
			&& switchStateValue(SwitchState.On) == 7
			&& switchIsOn(makeSwitch(SwitchState.On))
			&& !switchIsOn(makeSwitch(SwitchState.Off))
			&& actorSpeed(makeActor(ActorPhase.Moving(5))) == 5
			&& actorSpeed(makeActor(ActorPhase.Waiting)) == 0
			&& flow.first == 8
			&& flow.second == 9
			&& flow.third == 10
			&& flow.order == 123)) {}
	}
}
