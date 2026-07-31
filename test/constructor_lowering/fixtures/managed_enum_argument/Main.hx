/**
	Exercises fresh managed enum values passed directly to class constructors.

	Each enum value may own a String or Array. The constructor copies that value
	into its field, so the caller must keep the temporary alive through the call
	and then destroy its own copy. Returning the class through another enum makes
	the object escape its helper and therefore exercises collector-managed class
	construction rather than automatic stack storage.
**/

/** One closed value whose active payload may own managed runtime storage. */
private enum ManagedChoice {
	Empty;
	Text(value:String);
	Values(values:Array<Int>);
}

/** Stores one complete enum value for reads after its constructing helper ends. */
private final class ManagedEnumOwner {
	final choice:ManagedChoice;

	/** Retains an independent field copy of the caller's enum value. */
	public function new(choice:ManagedChoice) {
		this.choice = choice;
	}

	/** Makes each retained tag and payload observable after construction. */
	public function score():Int {
		return switch choice {
			case Empty: 0;
			case Text(value): value.length;
			case Values(values): values[0] + values[1];
		};
	}
}

/** Returning this wrapper makes its class payload escape the factory. */
private enum ManagedOwnerResult {
	ManagedOwnerReady(owner:ManagedEnumOwner);
}

/** Runs the ownership checks without relying on console or file output. */
final class Main {
	/** Pass a fresh fieldless variant into a collector-managed class. */
	static function buildEmpty():ManagedOwnerResult
		return ManagedOwnerReady(new ManagedEnumOwner(Empty));

	/** Pass a fresh String-payload variant into a collector-managed class. */
	static function buildText():ManagedOwnerResult
		return ManagedOwnerReady(new ManagedEnumOwner(Text("managed")));

	/** Pass a fresh Array-payload variant into a collector-managed class. */
	static function buildValues():ManagedOwnerResult
		return ManagedOwnerReady(new ManagedEnumOwner(Values([40, 2])));

	/** Project one completed owner after its factory has released all temporaries. */
	static function read(result:ManagedOwnerResult):Int {
		return switch result {
			case ManagedOwnerReady(owner): owner.score();
		};
	}

	/** Keeps running only when a tag, payload, root, or cleanup is wrong. */
	static function main():Void {
		final empty = buildEmpty();
		final text = buildText();
		final values = buildValues();
		for (index in 0...40000)
			new ManagedEnumOwner(Values([index, 1]));
		while (read(empty) != 0 || read(text) != 7 || read(values) != 42) {}
	}
}
