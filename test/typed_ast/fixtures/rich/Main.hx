import FixtureTypes.FixtureBox;
import FixtureTypes.FixtureCounter;
import FixtureTypes.FixturePoint;
import FixtureTypes.FixtureState;

class Main {
	static function main():Void {
		final box = new FixtureBox<Int>(3);
		var total = 0;
		for (value in [box.value, FixtureCounter.ofInt(4).toInt()]) {
			total += value;
		}
		final label = switch FixtureState.Ready(total) {
			case Idle: "idle";
			case Ready(value) if (value > 0): 'ready:$value';
			case Ready(_): "empty";
		};
		final point:FixturePoint = {x: total, y: label.length};
		if (point.x < point.y) {
			throw "unreachable fixture branch";
		}
	}
}
