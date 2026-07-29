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

/**
	A String identifier plus a managed Array field, matching editor catalog data.

	Copying this record through one comprehension makes two ownership operations
	share the `value.id` source span. That is intentional regression pressure for
	runtime-reason reconciliation, not a reason to split natural Haxe source.
**/
typedef CatalogRecord = {
	final id:String;
	final commands:Array<ManagedCommand>;
}

/**
	Reads an Array through an ordinary instance method without keeping it.

	The instance gives the fixture a direct-method call boundary distinct from a
	static function. Its parameter is borrowed only while `length` runs; the
	caller remains responsible for a fresh Array literal passed to it.
**/
final class FreshArrayReader {
	public function new() {}

	/** Return the length of one caller-owned Array without retaining it. */
	public function length(values:Array<Int>):Int
		return values.length;
}

/** Ordinary-Haxe executable for typed managed-record Array elements. */
final class Main {
	static var joinSeparatorEvaluations:Int = 0;
	static var sortComparatorCalls:Int = 0;

	/** Return one separator while proving `Array.join` evaluates it once. */
	static function observedSeparator(value:String):String {
		joinSeparatorEvaluations += 1;
		return value;
	}

	/** Build one fresh managed String so Array insertion must transfer ownership. */
	static function fromCode(code:Int):String
		return String.fromCharCode(code);

	static function main():Void {
		final values:Array<Int> = [10, 20];
		final freshStaticLength = borrowedLength([2, 3, 5]);
		final freshInstanceLength = new FreshArrayReader().length([7, 11]);
		final clearedIntegers = [31, 32, 33];
		final clearedIntegersAlias = clearedIntegers;
		clearedIntegers.resize(0);
		final firstConditionalValue = conditionalIndex(values, true);
		final secondConditionalValue = conditionalIndex(values, false);
		final emptyCopy = ([] : Array<Int>).copy();
		final valuesCopy = values.copy();
		valuesCopy[0] = 99;
		valuesCopy.push(30);
		final returnedCopy = copyAndAppend(values);
		// Build this through the ordinary managed Array operations so the test
		// cannot be reduced to a constant expression before haxe.c sees it.
		final labels:Array<String> = [];
		labels.push("ready");
		labels.push("café");
		labels.push("a\u0000b");
		final conditionalLabels:Array<String> = [];
		pushConditional(conditionalLabels, true);
		pushConditional(conditionalLabels, false);
		final labelsCopy = labels.copy();
		labelsCopy.push(fromCode(0x1F680));
		final catalogCopies = copyCatalogs([{id: "en", commands: [Number(41)]}]);
		final emptySorted:Array<Int> = [];
		emptySorted.sort((left, right) -> left - right);
		final singletonSorted = [7];
		singletonSorted.sort((left, right) -> left - right);
		final alreadySorted = [1, 2, 3, 4];
		alreadySorted.sort((left, right) -> left - right);
		final reverseSorted = [4, 3, 2, 1];
		final reverseAlias = reverseSorted;
		sortComparatorCalls = 0;
		reverseSorted.sort((left, right) -> {
			sortComparatorCalls += 1;
			return left - right;
		});
		final duplicateSorted = [3, 1, 3, 2, 1];
		duplicateSorted.sort((left, right) -> left - right);
		final customSorted = [1, 4, 2, 3];
		customSorted.sort((left, right) -> right - left);
		labelsCopy.sort((left, right) -> left.length - right.length);
		joinSeparatorEvaluations = 0;
		final joined = labels.join(observedSeparator("|"));
		final emptyJoined = ([] : Array<String>).join("");
		final singletonJoined = ["solo"].join("ignored");
		final alias = values;
		final history = new History();
		final absentHistory = history.takeNewest();
		final before = Bytes.alloc(1);
		final after = Bytes.alloc(1);
		before.set(0, 7);
		after.set(0, 9);
		final mixedLiteral = ["literal", labels[0], fromCode(0x1F600), labels[1].substring(1)];
		final scalarCodes = [65, 0x1F600];
		final freshComprehension = [for (code in scalarCodes) fromCode(code)];
		final borrowedComprehension = [for (label in labels) label];
		alias.push(12);
		var sum = 0;
		for (value in values)
			sum += value;

		history.add(sum, before, after, {minimum: 5, maximum: 15});
		final poppedHistory = history.takeNewest();
		if (poppedHistory != null)
			history.restore(poppedHistory);

		final poppedIntegers = [4, 5];
		final poppedIntegersAlias = poppedIntegers;
		final poppedFive = poppedIntegers.pop();
		final poppedFour = poppedIntegers.pop();
		final poppedEmpty = poppedIntegers.pop();
		final shiftedIntegers = [6, 7, 8];
		final shiftedIntegersAlias = shiftedIntegers;
		final shiftedSix = shiftedIntegers.shift();
		final shiftedSeven = shiftedIntegers.shift();
		final shiftedEight = shiftedIntegers.shift();
		final shiftedEmpty = shiftedIntegers.shift();

		final row:Array<Int> = [1];
		final rows:Array<Array<Int>> = [row];
		final rowsCopy = rows.copy();
		rowsCopy[0].push(2);
		rowsCopy.push([3]);
		final nestedArrayLength = nestedCopy(rows, true);
		final arguments:Array<ManagedCommand> = [Number(7)];
		final scheduled = makeSchedule(arguments);
		final copied = scheduled;
		final returnedRecord = returnedManagedRecord();
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
		final shiftedRecords:Array<ManagedRecord> = [{commands: [Number(29)]}, {commands: [Number(31), Number(37)]}];
		final shiftedRecordsAlias = shiftedRecords;
		final firstShiftedRecord = shiftedRecords.shift();
		final secondShiftedRecord = shiftedRecords.shift();
		final absentShiftedRecord = shiftedRecords.shift();
		final clearedRecords:Array<ManagedRecord> = [{commands: [Number(17)]}, {commands: [Number(19), Number(23)]}];
		final clearedRecordsAlias = clearedRecords;
		clearedRecords.resize(0);
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
		while (values.length != 3
			|| freshStaticLength != 3
			|| freshInstanceLength != 2
			|| clearedIntegersAlias.length != 0
			|| clearedRecordsAlias.length != 0
			|| firstConditionalValue != 10
			|| secondConditionalValue != 20
			|| emptyCopy.length != 0
			|| valuesCopy.length != 3
			|| valuesCopy[0] != 99
			|| values[0] != 10
			|| returnedCopy.length != 3
			|| returnedCopy[2] != 77
			|| labels.length != 3
			|| conditionalLabels.length != 2
			|| conditionalLabels[0] != "narrator"
			|| conditionalLabels[1] != "speaker"
			|| labelsCopy.length != 4
			|| labelsCopy[0] != "🚀"
			|| labelsCopy[1] != "a\u0000b"
			|| labelsCopy[2] != "café"
			|| labelsCopy[3] != "ready"
			|| labels[0] != "ready"
			|| catalogCopies.length != 1
			|| catalogCopies[0].id != "en"
			|| catalogCopies[0].commands.length != 1
			|| emptySorted.length != 0
			|| singletonSorted[0] != 7
			|| alreadySorted[0] != 1
			|| alreadySorted[3] != 4
			|| reverseAlias[0] != 1
			|| reverseAlias[3] != 4
			|| sortComparatorCalls <= 0
			|| duplicateSorted[0] != 1
			|| duplicateSorted[1] != 1
			|| duplicateSorted[2] != 2
			|| duplicateSorted[3] != 3
			|| duplicateSorted[4] != 3
			|| customSorted[0] != 4
			|| customSorted[1] != 3
			|| customSorted[2] != 2
			|| customSorted[3] != 1
			|| values[2] != 12
			|| joined != "ready|café|a\u0000b"
			|| emptyJoined != ""
			|| singletonJoined != "solo"
			|| mixedLiteral.length != 4
			|| mixedLiteral[0] != "literal"
			|| mixedLiteral[1] != "ready"
			|| mixedLiteral[2] != "😀"
			|| mixedLiteral[3] != "afé"
			|| freshComprehension.length != 2
			|| freshComprehension[0] != "A"
			|| freshComprehension[1] != "😀"
			|| borrowedComprehension.length != 3
			|| borrowedComprehension[0] != "ready"
			|| borrowedComprehension[1] != "café"
			|| borrowedComprehension[2] != "a\u0000b"
			|| joinSeparatorEvaluations != 1
			|| sum != 42
			|| history.depth() != 1
			|| history.lastRevision() != 42
			|| history.lastAfterByte() != 11
			|| history.lastMinimum() != 5
			|| poppedHistory == null
			|| poppedHistory.revision != 42
			|| poppedHistory.after.get(0) != 11
			|| absentHistory != null
			|| poppedFive != 5
			|| poppedFour != 4
			|| poppedEmpty != null
			|| poppedIntegersAlias.length != 0
			|| shiftedSix != 6
			|| shiftedSeven != 7
			|| shiftedEight != 8
			|| shiftedEmpty != null
			|| shiftedIntegersAlias.length != 0
			|| managedPayloadLength != 3
			|| recordCopy.commands.length != 3
			|| firstShiftedRecord == null
			|| firstShiftedRecord.commands.length != 1
			|| secondShiftedRecord == null
			|| secondShiftedRecord.commands.length != 2
			|| absentShiftedRecord != null
			|| shiftedRecordsAlias.length != 0
			|| nestedRecordCommandCount != 3
			|| nestedEnvelopeCommandCount != 3
			|| returnedRecord.commands.length != 1
			|| absent != null
			|| present == null
			|| nullableLength(absent) != -1
			|| nullableLength(present) != 2
			|| rows.length != 1
			|| rowsCopy.length != 2
			|| row.length != 2
			|| nestedArrayLength != 2) {}
	}

	/**
		Read an Array element after a conditional chooses its index.

		Haxe evaluates `values` before it evaluates the conditional index. The
		conditional creates separate compiler control-flow blocks, so haxe.c must
		save the already evaluated Array reference and reload it where those
		branches meet. The saved reference is only a short-lived borrow: this
		operation does not retain or allocate another Array.
	**/
	static function conditionalIndex(values:Array<Int>, first:Bool):Int
		return values[first ? 0 : 1];

	/**
		Push a managed String chosen by a conditional into an existing Array.

		Haxe evaluates `target` before choosing the argument. Because that choice
		creates separate HxcIR blocks, the compiler saves the borrowed Array
		reference and reloads it at the join before calling `push`. The Array slot
		then receives its own retained String owner in the ordinary way.
	**/
	static function pushConditional(target:Array<String>, narrator:Bool):Void
		target.push(narrator ? "narrator" : "speaker");

	/**
		Return a fresh shallow copy after changing only its outer Array.

		This proves `Array.copy()` can cross a normal return boundary as one owned
		Array. Appending to the result must not change the borrowed source.
	**/
	static function copyAndAppend(values:Array<Int>):Array<Int> {
		final copied = values.copy();
		copied.push(77);
		return copied;
	}

	/**
		Copy catalog values through ordinary Haxe record construction.

		The `id` field read both obtains a managed String value and initializes an
		owned record field. HxcIR keeps both retain operations even though Haxe maps
		them to the same characters; the runtime planner must preserve both
		descriptions instead of treating a source position as a unique operation.
	**/
	static function copyCatalogs(values:Array<CatalogRecord>):Array<CatalogRecord>
		return [
			for (value in values)
				{id: value.id, commands: [for (command in value.commands) command]}
		];

	/**
		Keep a copied inner Array alive while a nested branch borrows it.

		This used to fail closed because the compiler had no path-scoped owner for
		the copied element. It is positive evidence now: entering the branch owns
		`selected` until either the early return or the normal branch exit.
	**/
	static function nestedCopy(rows:Array<Array<Int>>, enabled:Bool):Int {
		if (enabled && rows.length > 0) {
			final selected = rows[0];
			return selected.length;
		}
		return 0;
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

	/**
		Read a fresh Array literal through a direct static call.

		The callee borrows `values` only for this call. Because the literal has no
		Haxe local to own it, haxe.c must create one hidden caller-owned temporary,
		pass a borrow here, and release that owner exactly once after use.
	**/
	static function borrowedLength(values:Array<Int>):Int
		return values.length;

	/**
	 * Return a managed record copied from an Array whose local owners then end.
	 *
	 * Checked indexing already creates one independent record owner. The return
	 * moves that owner to the caller; it must not destroy the nested `commands`
	 * Array during this function's cleanup or retain a redundant second copy.
	 */
	static function returnedManagedRecord():ManagedRecord {
		final commands:Array<ManagedCommand> = [Number(13)];
		final records:Array<ManagedRecord> = [{commands: commands}];
		return records[0];
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
