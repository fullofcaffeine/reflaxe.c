package;

typedef Pair = {
	final left:Int;
	final right:Int;
}

enum Bad {
	PairValue(value:Pair);
}

class Main {
	static function main():Void {
		var value = PairValue({left: 1, right: 2});
		switch value {
			case PairValue(_):
		}
	}
}
