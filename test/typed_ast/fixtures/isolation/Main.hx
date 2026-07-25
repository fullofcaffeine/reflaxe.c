class Main {
	static function main():Void {
		final value = IsolationOnly.identity(7);
		if (value != 7) {
			try {
				throw "unreachable isolation fixture branch";
			} catch (message:String) {
				trace(message);
			}
		}
	}
}

private class IsolationOnly {
	public static inline function identity(value:Int):Int {
		return value;
	}
}
