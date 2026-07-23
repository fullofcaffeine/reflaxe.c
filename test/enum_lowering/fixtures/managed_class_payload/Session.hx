/** Mutable identity used to prove that an enum carries one class reference, not a copy. */
final class Session {
	public var value:Int;
	public final samples:Array<Int> = [];

	public function new(value:Int) {
		this.value = value;
		samples.push(value);
	}

	public function add(delta:Int):Void
		value += delta;
}
