#if macro
import haxe.macro.Context;
import haxe.macro.Expr;
import reflaxe.c.CProfile;
import reflaxe.c.lowering.CBodyEmissionError;
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

/** First nominal view used to prove that an admitted carrier is not the identity. */
abstract LogicalPath(String) {}

/** A distinct nominal view over the same String carrier as `LogicalPath`. */
abstract ResourcePath(String) {}

/** A record whose path must retain the first nominal identity in its key. */
typedef LogicalAsset = {
	final path:LogicalPath;
}

/** The same record shape with a different nominal path type. */
typedef ResourceAsset = {
	final path:ResourcePath;
}

/** The same field name with no nominal abstract around its String carrier. */
typedef PlainAsset = {
	final path:String;
}

/** Compile-time regression for closed generic type identity before C lowering. */
class TypeIdentityMacro {
	public static macro function verify():Expr {
		final canonicalizer = new CGenericTypeCanonicalizer(CProfile.Portable);
		final position = Context.currentPos();
		final reject = (failurePosition, node) -> Context.fatalError('unexpected generic identity rejection: $node', failurePosition);
		final records = canonicalizer.normalize(Context.getType("NestedRecords"), position, reject, "records");
		final reordered = canonicalizer.normalize(Context.getType("ReorderedNestedRecords"), position, reject, "reordered");
		final integers = canonicalizer.normalize(Context.getType("NestedIntegers"), position, reject, "integers");
		final logical = canonicalizer.normalize(Context.getType("LogicalAsset"), position, reject, "logical");
		final resource = canonicalizer.normalize(Context.getType("ResourceAsset"), position, reject, "resource");
		final plain = canonicalizer.normalize(Context.getType("PlainAsset"), position, reject, "plain");
		final unsigned = canonicalizer.normalize(Context.getType("UInt"), position, reject, "unsigned");
		if (records.key != reordered.key)
			Context.fatalError("closed record identity depends on source field order", position);
		if (records.representation != "managed-array"
			|| records.key.indexOf("array(") == -1
			|| records.key.indexOf("closed-record(") == -1)
			Context.fatalError("nested record Array lost its structural managed identity", position);
		if (integers.key == records.key || integers.key.indexOf("array(") == integers.key.lastIndexOf("array("))
			Context.fatalError("nested Array element identities collapsed", position);
		if (logical.key == resource.key
			|| logical.key == plain.key
			|| logical.key.indexOf("transparent-abstract(") == -1
			|| logical.key.indexOf("LogicalPath") == -1
			|| logical.key.indexOf("string(utf8-scalar-indexed)") == -1)
			Context.fatalError("nominal String abstracts lost their identity or proven carrier", position);
		if (unsigned.key != "u32" || unsigned.displayName != "UInt")
			Context.fatalError("the transparent-abstract rule replaced UInt's dedicated unsigned contract", position);
		var topLevelRejection:Null<String> = null;
		try {
			canonicalizer.normalize(Context.getType("LogicalPath"), position, (_, node) -> topLevelRejection = node, "top-level");
			Context.fatalError("a transparent abstract escaped the bounded record-field admission", position);
		} catch (_:CBodyEmissionError) {}
		if (topLevelRejection == null
			|| topLevelRejection.indexOf("top-level:abstract `LogicalPath` is not an admitted primitive or native pointer contract") == -1)
			Context.fatalError("top-level transparent abstract rejection drifted", position);
		return macro null;
	}
}

class Main {
	static function main():Void
		TypeIdentityMacro.verify();
}
