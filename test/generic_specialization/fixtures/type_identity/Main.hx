#if macro
import haxe.macro.Context;
import haxe.macro.Expr;
import reflaxe.c.CProfile;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeCanonicalizer;
#end

typedef NestedToken = {
	final text:String;
	final kind:Int;
}

typedef ReorderedNestedToken = {
	final kind:Int;
	final text:String;
}

typedef NestedRecords = Array<{
	final tokens:Array<NestedToken>;
	final line:Int;
}>;

typedef ReorderedNestedRecords = Array<{
	final line:Int;
	final tokens:Array<ReorderedNestedToken>;
}>;

typedef NestedIntegers = Array<Array<Int>>;

/** Compile-time regression for closed generic type identity before C lowering. */
class TypeIdentityMacro {
	public static macro function verify():Expr {
		final canonicalizer = new CGenericTypeCanonicalizer(CProfile.Portable);
		final position = Context.currentPos();
		final reject = (failurePosition, node) -> Context.fatalError('unexpected generic identity rejection: $node', failurePosition);
		final records = canonicalizer.normalize(Context.getType("NestedRecords"), position, reject, "records");
		final reordered = canonicalizer.normalize(Context.getType("ReorderedNestedRecords"), position, reject, "reordered");
		final integers = canonicalizer.normalize(Context.getType("NestedIntegers"), position, reject, "integers");
		if (records.key != reordered.key)
			Context.fatalError("closed record identity depends on source field order", position);
		if (records.representation != "managed-array"
			|| records.key.indexOf("array(") == -1
			|| records.key.indexOf("closed-record(") == -1)
			Context.fatalError("nested record Array lost its structural managed identity", position);
		if (integers.key == records.key || integers.key.indexOf("array(") == integers.key.lastIndexOf("array("))
			Context.fatalError("nested Array element identities collapsed", position);
		return macro null;
	}
}

class Main {
	static function main():Void
		TypeIdentityMacro.verify();
}
