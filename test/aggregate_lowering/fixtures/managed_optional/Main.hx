import haxe.io.Bytes;

/** A managed record used to prove present and absent tagged `Null` ownership. */
private typedef ManagedRecord = {
	final bytes:Bytes;
}

/** Managed enum payload proving that optional lifetime is not record-specific. */
private enum ManagedChoice {
	NoValues;
	HasValues(values:Array<Int>);
}

/** A recursive managed enum matching the Caxecraft parser's record payload. */
private enum RecursiveChoice {
	Done;
	Many(values:Array<RecursiveChoice>);
	Negated(value:RecursiveChoice);
}

/** One parser-style cursor result returned through a direct optional record. */
private typedef ParsedChoice = {
	final value:RecursiveChoice;
	final next:Int;
}

/** A parser-style reason whose String payload is created at runtime. */
private enum TextProblem {
	NoTextProblem;
	DuplicateText(name:String);
}

/** A diagnostic record that owns the managed payload inside `TextProblem`. */
private typedef TextDiagnostic = {
	final line:Int;
	final problem:TextProblem;
}

/**
 * Public parser result whose rejection arm owns the diagnostic record.
 *
 * Preparing this outer enum discovers `TextProblem` while an enum is already
 * being prepared. The nested String lifetime must settle before a nullable
 * diagnostic field asks whether the complete record is managed.
 */
private enum TextReadResult {
	TextReady;
	TextRejected(diagnostic:TextDiagnostic);
}

/**
 * Owns one parser-style nullable managed record across ordinary method calls.
 *
 * This is the natural class-field shape exposed by Caxecraft's bounded JSON
 * cursor. The class starts absent, may publish one complete diagnostic record,
 * may replace it, and may clear it without leaking the nested managed enum.
 */
private final class DiagnosticCursor {
	var failure:Null<ParsedChoice> = null;

	public function new() {}

	public function replace(value:Null<ParsedChoice>):Void
		failure = value;

	public function nextOr(fallback:Int):Int
		return failure == null ? fallback : failure.next;

	public function clear():Void
		failure = null;
}

/**
 * Stores the exact String-payload diagnostic shape used by bounded parsers.
 *
 * `StringBuf.toString()` creates owned text. The enum, record, optional, and
 * class field must all preserve that ownership when the value is replaced or
 * cleared.
 */
private final class TextDiagnosticCursor {
	var failure:Null<TextDiagnostic> = null;

	public function new() {}

	public function replace(value:Null<TextDiagnostic>):Void
		failure = value;

	public function lineOr(fallback:Int):Int
		return failure == null ? fallback : failure.line;

	public function clear():Void
		failure = null;

	public function result():TextReadResult
		return failure == null ? TextReady : TextRejected(failure);
}

class Main {
	static function maybe(value:ManagedRecord):Null<ManagedRecord>
		return value;

	static function choose(include:Bool, value:ManagedRecord):Null<ManagedRecord>
		return include ? value : null;

	static function byteLength(value:Null<ManagedRecord>):Int
		return value == null ? -1 : value.bytes.length;

	/**
	 * Return either one owned optional record or absence for discard testing.
	 *
	 * The caller intentionally ignores both results. Generated C must still
	 * destroy a present Bytes owner and safely do nothing for the absent value.
	 */
	static function discardCandidate(include:Bool, value:ManagedRecord):Null<ManagedRecord>
		return include ? value : null;

	static function maybeChoice(value:ManagedChoice):Null<ManagedChoice>
		return value;

	static function chooseChoice(include:Bool, value:ManagedChoice):Null<ManagedChoice>
		return include ? value : null;

	static function choiceByteLength(value:Null<ManagedChoice>):Int
		return value == null ? -1 : switch value {
			case NoValues: 0;
			case HasValues(values): values.length;
		};

	/** Construct the declared optional payload directly from an object literal. */
	static function parsedChoice(value:Null<RecursiveChoice>, next:Int):Null<ParsedChoice> {
		if (value == null)
			return null;
		return {value: value, next: next};
	}

	/** Build one owned String through the pinned core `StringBuf` API. */
	static function builtText(code:Int):String {
		final output = new StringBuf();
		output.add("field-");
		output.addChar(code);
		return output.toString();
	}

	static function main():Void {
		final value:ManagedRecord = {bytes: Bytes.alloc(1)};
		final absent:Null<ManagedRecord> = null;
		var selected:Null<ManagedRecord> = absent;
		selected = choose(true, value);
		final alias = selected;
		selected = alias;
		while (maybe(value) == null || selected == null || byteLength(selected) != 1) {}
		selected = null;
		while (selected != null) {}
		discardCandidate(true, value);
		discardCandidate(false, value);

		final choice = HasValues([1, 2]);
		var selectedChoice:Null<ManagedChoice> = null;
		selectedChoice = chooseChoice(true, choice);
		final choiceAlias = selectedChoice;
		selectedChoice = choiceAlias;
		while (maybeChoice(choice) == null || selectedChoice == null || choiceByteLength(selectedChoice) != 2) {}
		selectedChoice = NoValues;
		while (choiceByteLength(selectedChoice) != 0) {}
		selectedChoice = null;
		while (selectedChoice != null) {}

		final parsed = parsedChoice(Done, 7);
		while (parsed == null || parsed.next != 7) {}

		final cursor = new DiagnosticCursor();
		while (cursor.nextOr(-1) != -1) {}
		cursor.replace(parsedChoice(Many([Done]), 8));
		while (cursor.nextOr(-1) != 8) {}
		cursor.replace(parsedChoice(Negated(Done), 9));
		while (cursor.nextOr(-1) != 9) {}
		cursor.clear();
		while (cursor.nextOr(-1) != -1) {}

		final textCursor = new TextDiagnosticCursor();
		while (textCursor.lineOr(-1) != -1) {}
		textCursor.replace({line: 10, problem: DuplicateText(builtText(65))});
		while (textCursor.lineOr(-1) != 10) {}
		switch textCursor.result() {
			case TextRejected(diagnostic) if (diagnostic.line == 10):
			case _:
				while (true) {}
		}
		textCursor.replace({line: 11, problem: NoTextProblem});
		while (textCursor.lineOr(-1) != 11) {}
		textCursor.clear();
		while (textCursor.lineOr(-1) != -1) {}
	}
}
