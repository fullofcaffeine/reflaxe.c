package;

typedef IdentityRecord = {
	var value:Int;
}

class Main {
	static function same(left:IdentityRecord, right:IdentityRecord):Bool {
		return left == right;
	}

	static function main():Void {
		var left:IdentityRecord = {value: 1};
		var right:IdentityRecord = {value: 1};
		while (!same(left, right)) {}
	}
}
