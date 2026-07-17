package;

enum Chain<T> {
	End(value:T);
	Link(value:T, next:Chain<T>);
}

class Main {
	static function make():Chain<Int> {
		return End(1);
	}

	static function main():Void {
		var value = make();
		switch value {
			case End(_):
			case Link(_, _):
		}
	}
}
