/** One collector-owned identity transported through a Dynamic exception. */
private final class Counter {
	/** Value observed after the exception crosses the generated call boundary. */
	public var value:Int;

	/** Create the managed payload used by the focused exception proof. */
	public function new(value:Int) {
		this.value = value;
	}
}

/**
 * Proves that a Dynamic exception crosses one generated function boundary.
 *
 * The throwing function has no local handler. The caller owns the contained
 * frame, catches the same Dynamic carrier, and reads its exact Int payload.
 */
class Main {
	/** Transfer one Dynamic value to the nearest caller-owned exception frame. */
	static function raiseFromCallee(value:Dynamic):Void {
		throw value;
	}

	/** Catch and narrow the payload after one real generated C call boundary. */
	static function catchAcrossCall(value:Int):Int {
		try {
			try {
				final values = [value, value + 1];
				values.push(value + 2);
				raiseFromCallee(values[0]);
			} catch (payload:Dynamic) {
				throw payload;
			}
		} catch (payload:Dynamic) {
			return cast payload;
		}
		return -1;
	}

	/** Keep one managed Dynamic payload rooted while its callee frame unwinds. */
	static function catchManagedAcrossCall(value:Int):Int {
		try {
			final payload:Dynamic = new Counter(value);
			raiseFromCallee(payload);
		} catch (payload:Dynamic) {
			final counter:Counter = cast payload;
			return counter.value;
		}
		return -1;
	}

	/** Release one managed String while an Int payload crosses the call. */
	static function catchWithStringCleanup(value:Int):Int {
		try {
			final text = Std.string(value);
			raiseFromCallee(value);
			return text.length;
		} catch (payload:Dynamic) {
			return cast payload;
		}
	}

	/** Leave a protected body normally after releasing its local owner. */
	static function returnFromTry(value:Int):Int {
		try {
			final values = [value];
			values.push(value + 1);
			return values[0];
		} catch (_:Dynamic) {
			return -1;
		}
	}

	/** Transfer an owner while discarding its now-obsolete unwind callback. */
	static function returnOwnerFromTry(value:Int):Array<Int> {
		try {
			final values = [value];
			values.push(value + 1);
			return values;
		} catch (_:Dynamic) {
			return [];
		}
	}

	/** Exit normally only when the transported payload remains exactly 7. */
	static function main():Void {
		final actual = catchAcrossCall(7);
		final returned = returnOwnerFromTry(11);
		if (actual != 7 || catchManagedAcrossCall(13) != 13 || catchWithStringCleanup(15) != 15 || returnFromTry(9) != 9 || returned.length != 2
			|| returned[1] != 12)
			throw actual;
	}
}
