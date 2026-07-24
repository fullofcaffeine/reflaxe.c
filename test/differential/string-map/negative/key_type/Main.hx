/** Keeps the unsupported-key diagnostic isolated from the positive Map fixture. */

/** Identity-bearing key deliberately outside StringMap and bounded IntMap. */
private final class Key {
	public final value:Int;

	/** Give each key an observable field without defining target hash semantics. */
	public function new(value:Int)
		this.value = value;
}

/** Another key family must not be mistaken for StringMap or bounded IntMap. */
final class Main {
	static function main():Void {
		final values:Map<Key, Bool> = [];
		values.set(new Key(1), true);
	}
}
