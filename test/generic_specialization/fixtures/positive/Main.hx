typedef AliasInt = Int;

enum GenericBox<T> {
	Empty;
	Value(value:T);
}

class Main {
	static function identity<T>(value:T):T {
		return value;
	}

	static function choose<A, B>(first:A, second:B):A {
		identity(second);
		return first;
	}

	static function forward<T>(value:T):T {
		return identity(value);
	}

	static function recursive<T>(value:T):T {
		return recursive(value);
	}

	static function guarded<T>(takeRecursive:Bool, value:T):T {
		if (takeRecursive) {
			return recursive(value);
		}
		return forward(value);
	}

	static function unbox(value:GenericBox<Int>):Int {
		return switch value {
			case Empty: 0;
			case Value(inner): inner;
		};
	}

	static function main():Void {
		final first:Int = identity(7);
		final alias:AliasInt = identity(8);
		final decimal:Float = identity(2.5);
		final chosen:Int = choose(3, decimal);
		final flag:Bool = identity(true);
		final unsignedSeed:UInt = 5;
		final bits:UInt = identity(unsignedSeed);
		final selected:Int = guarded(false, first + alias + chosen);
		final boxed:GenericBox<Int> = Value(selected);
		final echoed:GenericBox<Int> = identity(boxed);
		final nested:GenericBox<GenericBox<Int>> = Value(boxed);
		identity(nested);
		final result = unbox(echoed);
		if (flag && result == 0) {
			identity(bits);
			identity(decimal);
		}
	}
}
