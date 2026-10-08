import haxe.ds.ObjectMap;

/** Reject inherited custom formatting until exact instance dispatch is owned. */
final class Main {
	static function main():Void {
		final map = new ObjectMap<NamedKey, Int>();
		map.set(new NamedKey(), 1);
		map.toString();
	}
}

/** Own the custom method inherited by the final key below. */
class NamedKeyBase {
	public function new() {}

	/** Return the deliberate spelling that requires a real method call. */
	public function toString():String {
		return "named";
	}
}

/** Close the runtime identity while preserving the inherited custom method. */
final class NamedKey extends NamedKeyBase {
	public function new() {
		super();
	}
}
