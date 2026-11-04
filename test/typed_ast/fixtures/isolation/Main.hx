/** Keeps a small exception-bearing module independent of the rich server request. */
class Main {
	/** Retain try/catch inventory using the supported scalar exception contract. */
	static function main():Void {
		final value = IsolationOnly.identity(7);
		if (value != 7) {
			try {
				throw value;
			} catch (number:Int) {
				if (number != 7)
					throw number;
			}
		}
	}
}

private class IsolationOnly {
	public static inline function identity(value:Int):Int {
		return value;
	}
}
