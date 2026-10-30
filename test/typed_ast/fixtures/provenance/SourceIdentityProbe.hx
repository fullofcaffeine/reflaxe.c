import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.frontend.TypedAstNormalizer;
import reflaxe.c.frontend.TypedFunctionSourceProvenance;
import sys.FileSystem;
import sys.io.File;

/** Checks source snapshots without loading the full compiler or emitting target code. */
class SourceIdentityProbe {
	/** Delay typed API use until initialization macros have completed. */
	public static function run():Void {
		Context.onAfterInitMacros(check);
	}

	/** File changes distinguish request-local reuse from stale process-wide reuse. */
	static function check():Void {
		final root = Sys.getEnv("HXC_PROVENANCE_PROBE_ROOT");
		if (root == null)
			throw new haxe.Exception("missing isolated probe directory");
		final file = root + "/source.txt";
		File.saveContent(file, "first source contents");
		checkCollapsedPositions(file);
		final owner = new TypedFunctionSourceProvenance();
		checkPosition(owner, file, 1, 1);
		// Removing the file proves that later functions reuse its captured identity.
		FileSystem.deleteFile(file);
		checkPosition(owner, file, 3, 1);

		final next = new TypedFunctionSourceProvenance();
		final missing = capture(next, file, 3);
		if (missing.positionOverrides.iterator().hasNext())
			throw new haxe.Exception("a new request reused a missing file");
		// A failed read must not hide a file which becomes readable later.
		File.saveContent(file, "different source contents");
		checkPosition(next, file, 3, 3);
		File.saveContent(file, "third source contents");
		checkPosition(new TypedFunctionSourceProvenance(), file, 5, 5);
		FileSystem.deleteFile(file);
		final declaration = switch Context.getType("SourceIdentitySubject") {
			case TInst(reference, _): TClassDecl(reference);
			case _: throw new haxe.Exception("missing typed source identity subject");
		};
		for (_ in 0...2) {
			final program = TypedAstNormalizer.normalize([declaration], declaration, null);
			final fields = program.declarations[0].fields;
			if (fields.length != 2)
				throw new haxe.Exception("normalization lost a source function");
			for (field in fields) {
				if (field.functionSourcePlan == null || !field.functionSourcePlan.positionOverrides.iterator().hasNext())
					throw new haxe.Exception("normalization lost function source positions");
			}
		}
		Sys.println("HXC_SOURCE_IDENTITY_OK");
	}

	/** Distinct remembered ranges must not collapse onto one warm compiler position. */
	static function checkCollapsedPositions(file:String):Void {
		final owner = new TypedFunctionSourceProvenance();
		function position(offset:Int)
			return Context.makePosition({file: file, min: offset, max: offset + 1});
		function expression(left:Int, right:Int):TypedExpr {
			final type = Context.getType("Int");
			return {
				expr: TBlock([
					{expr: TConst(TInt(1)), t: type, pos: position(left)},
					{expr: TConst(TInt(2)), t: type, pos: position(right)}
				]),
				t: type,
				pos: position(0)
			};
		}
		owner.plan("Fixture", "collision", position(0), expression(1, 3));
		var rejected = false;
		try {
			owner.plan("Fixture", "collision", position(0), expression(5, 5));
		} catch (error:haxe.Exception) {
			if (!StringTools.contains(error.message, "collapsed distinct source positions"))
				throw error;
			rejected = true;
		}
		if (!rejected)
			throw new haxe.Exception("source provenance accepted conflicting warm positions");
	}

	/** Use a synthetic typed value so this test owns its exact expected offsets. */
	static function capture(owner:TypedFunctionSourceProvenance, file:String, offset:Int) {
		final position = Context.makePosition({file: file, min: offset, max: offset + 1});
		final expression:TypedExpr = {expr: TConst(TInt(1)), t: Context.getType("Int"), pos: position};
		return owner.plan("Fixture", "value", position, expression);
	}

	/** An unchanged identity recovers old offsets; changed bytes admit current offsets. */
	static function checkPosition(owner:TypedFunctionSourceProvenance, file:String, offset:Int, expected:Int):Void {
		final plan = capture(owner, file, offset);
		final positions = [for (position in plan.positionOverrides) Context.getPosInfos(position)];
		if (positions.length != 1 || positions[0].file != file || positions[0].min != expected || positions[0].max != expected + 1)
			throw new haxe.Exception("source identity returned incorrect positions");
	}
}
