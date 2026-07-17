package;

enum Chain<T> {
	End(value:T);
	Link(value:T, next:Chain<T>);
}

class Main {
	static function identity(value:Chain<Int>):Chain<Int> {
		return value;
	}

	static function main():Void {
		var value:Chain<Int> = End(1);
		identity(value);
	}
}
