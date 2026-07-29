/**
 * One class returned by a factory only after ordinary input validation.
 *
 * The caller keeps this object after `create` returns, so generated C needs
 * stable collector storage. The guard must not force the constructor into an
 * automatic local whose address would dangle at the return boundary.
 */
final class ValidatedValue {
	var stored:Int;

	/** Store the validated value read after the factory call has ended. */
	public function new(value:Int)
		stored = value;

	/** Return the state retained by this instance. */
	public inline function read():Int
		return stored;
}

/**
 * Owns one child that may be returned independently from the parent.
 *
 * Because `expose` publishes the child, whole-program planning represents this
 * field as a traced managed reference rather than an inline C subobject.
 */
final class ValueContainer {
	public final child:ValidatedValue = new ValidatedValue(9);

	/** Construct the container after its field initializer has made the child. */
	public function new() {}

	/** Return the same child while allowing this container to end. */
	public function expose():ValidatedValue
		return child;
}

/** Exercises the general validated-factory shape exposed by Raygui text state. */
final class Main {
	/**
	 * Reject invalid input before constructing the successful result.
	 *
	 * `Null<ValidatedValue>` is Haxe's normal fallible factory spelling here:
	 * invalid input returns no object, while a valid result may outlive this
	 * function.
	 */
	static function create(value:Int):Null<ValidatedValue> {
		if (value <= 0)
			return null;
		final result = new ValidatedValue(value);
		return result;
	}

	/**
	 * Pass an existing class reference through a helper without changing identity.
	 *
	 * This is the non-owning neighbor of `create`: it proves that selecting
	 * stable storage for the returned class also makes a parameter safe to return
	 * after this helper's stack frame has ended.
	 */
	static function forward(value:ValidatedValue):ValidatedValue
		return value;

	/** Keep running only when both rejected and retained paths agree with Eval. */
	static function main():Void {
		final missing = create(0);
		final retained = create(42);
		final forwarded = forward(new ValidatedValue(7));
		final exposed = new ValueContainer().expose();
		for (index in 0...40000)
			new ValidatedValue(index + 1);
		while (missing != null || retained == null || retained.read() != 42 || forwarded.read() != 7 || exposed.read() != 9) {}
	}
}
