@:typedAstFixture("primary")
class FixtureTypes {
	public static final answer:Int = 42;
}

@:typedAstFixture("interface")
interface FixtureMarker {
	public function marker():String;
}

@:typedAstFixture("class")
class FixtureBox<T> implements FixtureMarker {
	public final value:T;

	public function new(value:T) {
		this.value = value;
	}

	public function marker():String {
		return "box";
	}
}

@:typedAstFixture("extern")
extern class FixtureNativeClock {
	public static function now():Float;
}

@:typedAstFixture("enum")
enum FixtureState {
	Idle;
	Ready(value:Int);
}

@:typedAstFixture("typedef")
typedef FixturePoint = {
	final x:Int;
	final y:Int;
}

@:typedAstFixture("abstract")
abstract FixtureCounter(Int) from Int to Int {
	public inline static function ofInt(value:Int):FixtureCounter {
		return new FixtureCounter(value);
	}

	inline function new(value:Int) {
		this = value;
	}

	public inline function toInt():Int {
		return this;
	}
}
