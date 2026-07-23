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

class Main {
	static function maybe(value:ManagedRecord):Null<ManagedRecord>
		return value;

	static function choose(include:Bool, value:ManagedRecord):Null<ManagedRecord>
		return include ? value : null;

	static function byteLength(value:Null<ManagedRecord>):Int
		return value == null ? -1 : value.bytes.length;

	static function maybeChoice(value:ManagedChoice):Null<ManagedChoice>
		return value;

	static function chooseChoice(include:Bool, value:ManagedChoice):Null<ManagedChoice>
		return include ? value : null;

	static function choiceByteLength(value:Null<ManagedChoice>):Int
		return value == null ? -1 : switch value {
			case NoValues: 0;
			case HasValues(values): values.length;
		};

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
	}
}
