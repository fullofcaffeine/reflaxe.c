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
		final nestedRecordCommandCount = countFirstEnabledRecord(records, true);
		final nestedEnvelopeCommandCount = countFirstScheduledCommands(envelopes, true);
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
			|| recordCopy.commands.length != 3 || nestedRecordCommandCount != 3 || nestedEnvelopeCommandCount != 3 || absent != null || present == null
			|| nullableLength(absent) != -1 || nullableLength(present) != 2) {}
	}

	/**
		Read an owned managed-record copy for one generated `for`-loop iteration.

		The outer Array read copies and owns the record until that iteration exits.
		A nested loop may borrow its `commands` field, while `continue`, normal
		iteration, and the early return must all release the record copy exactly
		once. Passing the field to `commandCount` proves the owned local—not the
		Array slot—keeps the nested Array alive across the call.
	**/
	static function countFirstEnabledRecord(records:Array<ManagedRecord>, enabled:Bool):Int {
		for (record in records) {
			if (!enabled)
				continue;
			var visited = 0;
			for (_ in record.commands)
				visited++;
			if (visited > 0)
				return commandCount(record.commands);
		}
		return 0;
	}

	/**
		Read an owned managed-enum copy through nested loop and switch control flow.

		Only `Schedule` owns an Array payload. The element destructor must inspect
		the active tag, preserve the projected Array while the nested loop and call
		borrow it, and release that payload on `continue`, early return, and normal
		iteration. `Idle` proves the inactive union arm is never released.
	**/
	static function countFirstScheduledCommands(envelopes:Array<ManagedEnvelope>, enabled:Bool):Int {
		for (envelope in envelopes) {
			if (!enabled)
				continue;
			var result = 0;
			switch envelope {
				case Schedule(commands):
					var visited = 0;
					for (_ in commands)
						visited++;
					if (visited > 0)
						result = commandCount(commands);
				case Idle:
			}
			if (result > 0)
				return result;
		}
		return 0;
	}

	/** Read one borrowed Array during a direct call without retaining a new alias. */
	static function commandCount(commands:Array<ManagedCommand>):Int
		return commands.length;

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
