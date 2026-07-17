class Main {
	static function identity<T>(value:T):T {
		return value;
	}

	static function main():Void {
		identity(7);
	}
}
