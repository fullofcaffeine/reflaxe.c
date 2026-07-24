typedef AliasInt = Int;

typedef Marker = {
	final code:Int;
}

enum GenericBox<T> {
	Empty;
	Value(value:T);
}

/**
 * Proves that a direct instance call receives the same bounded specialization
 * as a static call. The class is `final`, so the compiler knows the exact
 * method body; an overridable generic method needs a separate dispatch-table
 * design and remains fail-closed.
 */
final class GenericCursor {
	public function new() {}

	public function echo<T>(value:T):T {
		return value;
	}

	public function empty<T>():GenericBox<T> {
		return Empty;
	}

	public function emptyAt<T>(marker:Null<Marker>):GenericBox<T> {
		return Empty;
	}
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
		final cursor = new GenericCursor();
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
		final methodInt:Int = cursor.echo(11);
		final methodBool:Bool = cursor.echo(false);
		final emptyInt:GenericBox<Int> = cursor.empty();
		final emptyFloat:GenericBox<Float> = cursor.empty();
		final marker:Marker = {code: 1};
		final emptyAtInt:GenericBox<Int> = cursor.emptyAt(marker);
		final result = unbox(echoed);
		if (flag && methodBool && result + methodInt == 0) {
			identity(bits);
			identity(decimal);
			identity(emptyInt);
			identity(emptyFloat);
			identity(emptyAtInt);
		}
	}
}
