import haxe.io.Bytes;

/** Plain nested element used to prove that the payload Array remains specialized. */
enum ManagedCommand {
	Number(value:Int);
}

/** Tagged value whose active constructor may own one managed Array reference. */
enum ManagedEnvelope {
	Idle;
	Schedule(arguments:Array<ManagedCommand>);
}

/** Closed record used to exercise an Array owner nested inside another Array element. */
typedef ManagedRecord = {
	final commands:Array<ManagedCommand>;
}

/** Ordinary-Haxe executable for typed managed-record Array elements. */
final class Main {
	static function main():Void {
		final values:Array<Int> = [10, 20];
		// Build this through the ordinary managed Array operations so the test
		// cannot be reduced to a constant expression before haxe.c sees it.
		final labels:Array<String> = [];
		labels.push("ready");
		final alias = values;
		final history = new History();
		final before = Bytes.alloc(1);
		final after = Bytes.alloc(1);
		before.set(0, 7);
		after.set(0, 9);
		alias.push(12);
		var sum = 0;
		for (value in values)
			sum += value;

		history.add(sum, before, after, {minimum: 5, maximum: 15});

		final arguments:Array<ManagedCommand> = [Number(7)];
		final scheduled = makeSchedule(arguments);
		final copied = scheduled;
		final envelopes:Array<ManagedEnvelope> = [];
		envelopes.push(Idle);
		envelopes.push(copied);
		switch envelopes[1] {
			case Schedule(projected):
				projected.push(Number(9));
			case Idle:
		}
		var managedPayloadLength = 0;
		final records:Array<ManagedRecord> = [];
		records.push({commands: arguments});
		final recordCopy = records[0];
		recordCopy.commands.push(Number(11));
		switch scheduled {
			case Schedule(projected):
				managedPayloadLength = projected.length;
			case Idle:
		}
		// Array insertion copies the record but preserves Bytes identity. A mutation
		// through the original alias must therefore be visible through history.
		after.set(0, 11);
		final absent = maybeValues(false);
		final present = maybeValues(true);
		while (values.length != 3 || labels.length != 1 || labels[0] != "ready" || values[2] != 12 || sum != 42 || history.depth() != 1
			|| history.lastRevision() != 42 || history.lastAfterByte() != 11 || history.lastMinimum() != 5 || managedPayloadLength != 3
			|| recordCopy.commands.length != 3 || absent != null || present == null || nullableLength(absent) != -1 || nullableLength(present) != 2) {}
	}

	/** Return transfers the newly constructed enum owner to the caller. */
	static function makeSchedule(arguments:Array<ManagedCommand>):ManagedEnvelope
		return Schedule(arguments);

	/**
		Return either an absent Array or one newly owned Array through the same pointer.

		The explicit `Null<Array<Int>>` source type must not add a tagged record
		around the managed Array reference. Returning the local also proves that its
		one reference-counted owner moves to the caller instead of being released at
		this function's cleanup edge.
	**/
	static function maybeValues(present:Bool):Null<Array<Int>> {
		if (!present)
			return null;
		final values:Array<Int> = [3, 4];
		return values;
	}

	/** Read a nullable Array parameter only after an ordinary Haxe null check. */
	static function nullableLength(values:Null<Array<Int>>):Int {
		if (values == null)
			return -1;
		return values.length;
	}
}
