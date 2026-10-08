/**
	Proves one enum-switch arm can return while grouped arms continue a loop.

	The payload arm exits the function immediately. The three valid tags share an
	empty continuing arm, so their generated switch exit must belong to the loop
	body exactly once rather than being consumed by more than one case region.
**/

/** Closed cell states used by the loop's statement switch. */
private enum CellState {
	Empty;
	Solid;
	Water;
	InvalidStorage(value:Int);
}

/** Executes the grouped-arm and early-return paths under Eval and generated C. */
final class Main {
	/** Reject the first invalid payload and otherwise continue through every cell. */
	static function valid(cells:Array<CellState>):Bool {
		var index = 0;
		while (index < cells.length) {
			switch cells[index] {
				case InvalidStorage(_):
					return false;
				case Empty | Solid | Water:
			}
			index++;
		}
		return true;
	}

	/** Run both paths so Eval and native C must agree on the observable result. */
	static function main():Void {
		if (!valid([Empty, Solid, Water]) || valid([Water, InvalidStorage(7), Empty]))
			throw "statement enum switch changed its early-return behavior";
	}
}
