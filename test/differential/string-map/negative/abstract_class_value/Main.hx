/**
	Proves that a nominal abstract cannot hide unsupported object ownership.

	A String-backed nominal abstract is safe because String already has an exact
	immutable retain/release carrier. Wrapping a mutable class does not remove its
	identity, escape, or collector requirements, so this neighboring abstract must
	remain rejected before C output.
**/

/** Mutable identity value deliberately outside admitted direct map storage. */
private final class Box {
	public var value:Int;

	/** Create one identity-bearing value for the rejected abstract wrapper. */
	public function new(value:Int)
		this.value = value;
}

/** Nominal syntax must preserve, not erase, the wrapped class ownership. */
private abstract BoxHandle(Box) {
	/** Wrap one class reference without changing its runtime ownership. */
	public inline function new(value:Box)
		this = value;
}

/** Keeps the unsupported custom-abstract diagnostic isolated from valid Strings. */
final class Main {
	static function main():Void {
		final values:Map<String, BoxHandle> = [];
		values.set("box", new BoxHandle(new Box(7)));
	}
}
