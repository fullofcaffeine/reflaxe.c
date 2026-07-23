package;

/** A natural private Haxe enum with required and explicitly nullable class cases. */
enum SessionEvent {
	Closed;
	Opened(session:Session);
	MaybeOpened(session:Null<Session>);
}

/** By-value composition proves that tracing follows a record field into its enum. */
typedef SessionEnvelope = {
	final event:SessionEvent;
}

/**
	Exercise class references through every ordinary enum/container boundary.

	The pressure function allocates enough short-lived objects to force collection
	after the producer function's root frame has gone away. At that point the only
	owner of the important Session can be the caller's enum root or a traced Array.
**/
final class Main {
	static function makeEvent(value:Int):SessionEvent
		return Opened(new Session(value));

	static function copyEvent(value:SessionEvent):SessionEvent
		return value;

	static function makeEnvelope(value:Int):SessionEnvelope
		return {event: makeEvent(value)};

	static function makeEventArray(value:Int):Array<SessionEvent>
		return [makeEvent(value), Closed, MaybeOpened(null)];

	static function makeEnvelopeArray(value:Int):Array<SessionEnvelope>
		return [makeEnvelope(value)];

	static function forceCollectionPressure():Void {
		for (index in 0...40000)
			new Session(index);
	}

	static function read(value:SessionEvent):Int
		return switch value {
			case Closed: -1;
			case Opened(session): session.value;
			case MaybeOpened(null): -2;
			case MaybeOpened(session): session.value;
		};

	static function main():Void {
		var direct = copyEvent(makeEvent(7));
		var assigned:SessionEvent = Closed;
		assigned = direct;
		final nullable = MaybeOpened(null);
		final envelope = makeEnvelope(11);
		final events = makeEventArray(13);
		final envelopes = makeEnvelopeArray(17);

		forceCollectionPressure();

		switch assigned {
			case Opened(session):
				session.add(5);
			case Closed | MaybeOpened(_):
		}
		while (read(direct) != 12 || read(assigned) != 12 || read(nullable) != -2 || read(envelope.event) != 11 || read(events[0]) != 13
			|| read(events[1]) != -1 || read(events[2]) != -2 || read(envelopes[0].event) != 17) {}
	}
}
