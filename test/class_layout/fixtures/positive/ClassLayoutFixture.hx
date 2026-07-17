class EmptyBase {}
class EmptyLeaf extends EmptyBase {}

class RootRecord {
	public var rootValue:Int;
}

class MiddleRecord extends RootRecord {
	public var enabled:Bool;
}

class LeafRecord extends MiddleRecord {
	public var score:Float;
	public var peer:RootRecord;
}

class ClassLayoutFixture {
	static function readRoot(value:RootRecord):Int {
		return value.rootValue;
	}

	static function readInherited(value:LeafRecord):Int {
		return value.rootValue;
	}

	static function writeInherited(value:LeafRecord, next:Int):Int {
		value.rootValue = next;
		return value.rootValue;
	}

	static function readScore(value:LeafRecord):Float {
		return value.score;
	}

	static function readPeer(value:LeafRecord):RootRecord {
		return value.peer;
	}

	static function writePeer(value:LeafRecord, peer:RootRecord):RootRecord {
		value.peer = peer;
		return value.peer;
	}

	static function asMiddle(value:LeafRecord):MiddleRecord {
		return value;
	}

	static function asRoot(value:LeafRecord):RootRecord {
		return value;
	}

	static function asEmptyBase(value:EmptyLeaf):EmptyBase {
		return value;
	}

	static function same(left:RootRecord, right:RootRecord):Bool {
		return left == right;
	}

	static function different(left:RootRecord, right:RootRecord):Bool {
		return left != right;
	}

	static function isNull(value:RootRecord):Bool {
		return value == null;
	}

	static function main():Void {
		var leaf:LeafRecord = null;
		var root:RootRecord = asRoot(leaf);
		var middle:MiddleRecord = asMiddle(leaf);
		var emptyLeaf:EmptyLeaf = null;
		var emptyBase:EmptyBase = asEmptyBase(emptyLeaf);
		while (!(root == null && middle == null && emptyBase == null && same(root, null) && !different(root, null) && isNull(root))) {}
	}
}
