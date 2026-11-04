/** Concrete source shapes for Array classification, without runtime behavior. */
typedef Ints = Array<Int>;

/** A typedef must retain the header-owned representation of its target. */
typedef ImportedAlias = imported.Array;

/** Only the active constructor owns the nested Array payload. */
enum Payload {
	Empty;
	Values(values:Array<Int>);
}

/** An ordinary class element requires a collector-managed reference. */
class Item {
	public var value:Int;

	public function new() {
		value = 0;
	}
}

/** Typed fields expose distinct element and wrapper shapes to the macro probe. */
class ArrayClassificationFixture {
	public var ints:Array<Int>;
	public var bools:Array<Bool>;
	public var nested:Array<Array<Int>>;
	public var strings:Array<String>;
	public var bytes:Array<haxe.io.Bytes>;
	public var enums:Array<Payload>;
	public var classes:Array<Item>;
	public var functions:Array<Int->Int>;
	public var alias:Ints;
	public var imported:imported.Array;
	public var importedAlias:ImportedAlias;
	public var namesake:other.Array;

	/** All checks run in the macro; the Eval entry point has no side effects. */
	public static function main():Void {}
}
