/**
 * Inline child that owns one independently reference-counted Array.
 *
 * The possible throw makes the constructor's failure edge real. On success the
 * child remains physically inside its parent; on failure the partial parent is
 * responsible for releasing the Array exactly once.
 */
final class Child {
	final values:Array<Int> = [41];

	public function new(shouldFail:Bool) {
		if (shouldFail)
			throw 1;
	}

	public function score(delta:Int):Int
		return values[0] + delta;
}

/** Collector-managed parent whose child remains an inline C subobject. */
final class Parent {
	public final child:Child = new Child(false);

	public function new() {}

	public function score(delta:Int):Int
		return child.score(delta);
}

/**
 * Retains `Parent` beyond `install`, forcing stable collector storage.
 *
 * This wrapper makes the fixture prove both recursive descriptor cleanup and
 * the fallible inline-child construction edge that Caxecraft exposed.
 */
final class Holder {
	var parent:Null<Parent> = null;

	public function new() {}

	public function install():Void
		parent = new Parent();

	public function score(delta:Int):Int
		return parent == null ? -1 : parent.score(delta);
}

/**
 * Smallest cycle-capable class graph: one object retains its own identity.
 *
 * The constructor's first assignment publishes `this` only into its own
 * collector-managed field. Exact tracing must then keep that self-edge valid
 * across collection without adding reference counting or a special C case.
 */
final class SelfReference {
	public final peer:SelfReference;

	public function new() {
		peer = this;
	}

	public function pointsToSelf():Bool
		return peer == this;
}

final class Main {
	static function pressure():Void {
		for (index in 0...40000)
			new Holder();
	}

	static function main():Void {
		final holder = new Holder();
		final selfReference = new SelfReference();
		holder.install();
		pressure();
		while (holder.score(1) != 42 || !selfReference.pointsToSelf()) {}
	}
}
