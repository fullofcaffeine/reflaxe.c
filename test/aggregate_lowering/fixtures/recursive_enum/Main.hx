package;

enum Chain {
	End;
	Link(next:Chain);
}

typedef ChainRecord = {
	final chain:Chain;
}

final class Main {
	static function isEnd(value:ChainRecord):Bool {
		return switch value.chain {
			case End: false;
			case Link(_): true;
		};
	}

	static function main():Void {
		while (isEnd({chain: End})) {}
	}
}
