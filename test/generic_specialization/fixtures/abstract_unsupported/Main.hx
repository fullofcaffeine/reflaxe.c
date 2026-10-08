/**
	A source-level name over a class reference, which this bounded generic
	specialization path does not admit.

	This fixture proves that an abstract cannot make an unsupported underlying
	type legal merely by hiding it behind a nominal name.
**/

/** A deliberately unsupported interface carrier for the generic boundary. */
interface UnsupportedCarrier {}

/** One concrete value that lets Haxe construct the interface-backed abstract. */
final class CarrierImpl implements UnsupportedCarrier {
	public function new() {}
}

/** A nominal name that must not hide the unsupported class-reference carrier. */
abstract UnsafeValue(UnsupportedCarrier) {
	public inline function new(value:UnsupportedCarrier)
		this = value;
}

/** The closed record that exposes the hidden unsupported carrier to planning. */
typedef UnsafeRecord = {
	final value:UnsafeValue;
}

/** Reaches the generic planner with an unsupported abstract carrier. */
class Main {
	static function identity<T>(value:T):T {
		return value;
	}

	static function main():Void {
		final value:UnsafeRecord = {
			value: new UnsafeValue(new CarrierImpl())
		};
		identity(value);
	}
}
