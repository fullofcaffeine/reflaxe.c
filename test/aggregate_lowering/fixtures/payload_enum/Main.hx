package;

enum ActorPhase {
	Waiting;
	Moving(values:Array<Int>);
}

typedef ActorRecord = {
	final phase:ActorPhase;
}

class Main {
	static function isWaiting(value:ActorRecord):Bool {
		return switch value.phase {
			case Waiting: true;
			case Moving(_): false;
		};
	}

	static function main():Void {
		while (!isWaiting({phase: Waiting})) {}
	}
}
