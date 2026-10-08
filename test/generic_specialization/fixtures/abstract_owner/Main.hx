/**
	A generic abstract whose implementation functions retain the owner's `T`.

	This is the target-neutral shape used by standard-library abstractions such
	as `haxe.ds.Vector<T>`; the C target must close the generated implementation
	owner without replacing the authored abstraction.
**/
abstract FirstBox<T>(Array<T>) {
	public function new(value:T) {
		this = [value];
	}

	public function first():T {
		return this[0];
	}

	public function copyData():Array<T> {
		return this.copy();
	}
}

/** Runs the focused generic abstract-implementation owner proof. */
class Main {
	static function main():Void {
		final value = new FirstBox<Int>(17);
		final copied = value.copyData();
		if (value.first() != 17 || copied.length != 1 || copied[0] != 17)
			throw "closed abstract owner specialization failed";
	}
}
