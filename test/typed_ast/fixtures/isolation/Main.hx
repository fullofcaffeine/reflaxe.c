class Main {
	static function main():Void {
		final value = IsolationOnly.identity(7);
		if (value != 7) {
			throw "unreachable isolation fixture branch";
		}
	}
}

private class IsolationOnly {
	public static inline function identity(value:Int):Int {
		return value;
	}
}
