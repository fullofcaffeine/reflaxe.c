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

	/**
		Exercise one receiver before a branch, inside it, and after its join.

		The entry check dominates both later reads, so generated C needs one guard.
	**/
	static function sumAcrossBranch(value:LeafRecord, addScore:Bool):Float {
		var total:Float = value.rootValue;
		if (addScore)
			total += value.score;
		return total + value.rootValue;
	}

	/**
		Prove that checks made on separate branch arms do not escape their join.

		Each nullable parameter has a different immutable HxcIR identity. The final
		reads therefore keep checks for both values because neither branch-local
		proof ran on every route to the join.
	**/
	static function branchProofDoesNotEscape(value:RootRecord, other:RootRecord, useFirst:Bool):Int {
		var result = 0;
		if (useFirst)
			result = value.rootValue;
		else
			result = other.rootValue;
		return result + value.rootValue + other.rootValue;
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
