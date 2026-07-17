class Main {
	static function identity<T>(value:T):T {
		return value;
	}

	static function main():Void {
		final dynamicValue:Dynamic = 7;
		identity(dynamicValue);
	}
}
