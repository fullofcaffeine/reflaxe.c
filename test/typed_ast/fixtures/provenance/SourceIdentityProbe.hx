import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.frontend.TypedAstNormalizer;
import reflaxe.c.frontend.TypedFunctionSourceProvenance;
import reflaxe.c.frontend.TypedFunctionSourceProvenance.TypedFunctionSourcePlan;
import reflaxe.c.lowering.HaxeSourceSpan.HaxeSourceSpanResolver;
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
		checkColdRanges(file);
		final owner = new TypedFunctionSourceProvenance();
		checkPosition(owner, file, 1, 1);
		// Removing the file proves that later functions reuse its captured identity.
		FileSystem.deleteFile(file);
		final retained = capture(owner, file, 3);

		final next = new TypedFunctionSourceProvenance();
		final missing = capture(next, file, 3);
		if (missing.positionOverrides.iterator().hasNext())
			throw new haxe.Exception("a new request reused a missing file");
		// A failed read must not hide a file which becomes readable later.
		File.saveContent(file, "different source contents");
		checkResolved(retained, 1);
		checkResolved(missing, 3);
		checkPosition(next, file, 3, 3);
		checkPosition(next, file, 7, 3);
		File.saveContent(file, "third source contents");
		final third = new TypedFunctionSourceProvenance();
		checkPosition(third, file, 5, 5);
		checkPosition(third, file, 7, 5);
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
				final plan = field.functionSourcePlan;
				if (plan == null || plan.expressionPositions.length == 0)
					throw new haxe.Exception("normalization lost function source positions");
				final resolver = new HaxeSourceSpanResolver("SourceIdentitySubject.hx", false, plan.positionOverrides);
				final expected = field.name == "first" ? "SourceIdentitySubject.hx:5:10-5:11" : "SourceIdentitySubject.hx:10:10-10:11";
				if (![for (position in plan.expressionPositions) resolver.resolve(position).display()].contains(expected))
					throw new haxe.Exception("normalization lost an authored literal range");
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
		checkResolved(capture(owner, file, offset), expected);
	}

	/** Exercise the real lowering resolver, with one-based independently known coordinates. */
	static function checkResolved(plan:TypedFunctionSourcePlan, expected:Int):Void {
		final resolver = new HaxeSourceSpanResolver("source.txt", false, plan.positionOverrides);
		final span = resolver.resolve(plan.expressionPositions[0]);
		if (span.display() != 'source.txt:1:${expected + 1}-1:${expected + 2}')
			throw new haxe.Exception("source identity returned incorrect effective positions");
	}

	/** Cold ranges remain validated; failed plans publish nothing, and foreign ranges stay owned by Haxe. */
	static function checkColdRanges(file:String):Void {
		final type = Context.getType("Int");
		function pos(min:Int, max:Int)
			return Context.makePosition({file: file, min: min, max: max});
		function value(min:Int, max:Int):TypedExpr
			return {expr: TConst(TInt(1)), t: type, pos: pos(min, max)};
		for (range in [{min: -1, max: 1}, {min: 3, max: 2}, {min: 0, max: 1000}]) {
			var rejected = false;
			try {
				new TypedFunctionSourceProvenance().plan("Ranges", 'invalid${range.min}:${range.max}', pos(0, 1), value(range.min, range.max));
			} catch (error:haxe.Exception) {
				if (!StringTools.contains(error.message, "retained an invalid source-position offset"))
					throw error;
				rejected = true;
			}
			if (!rejected)
				throw new haxe.Exception("cold invalid range accepted");
		}
		function block(left:Int, right:Int):TypedExpr
			return {expr: TBlock([value(left, left + 1), value(right, right + 1)]), t: type, pos: pos(0, 1)};
		final retry = new TypedFunctionSourceProvenance();
		var failed = false;
		try {
			retry.plan("Ranges", "retry", pos(0, 1), block(1, 1000));
		} catch (error:haxe.Exception) {
			if (!StringTools.contains(error.message, "retained an invalid source-position offset"))
				throw error;
			failed = true;
		}
		if (!failed)
			throw new haxe.Exception("partial invalid cold tree accepted");
		retry.plan("Ranges", "retry", pos(0, 1), block(3, 5));
		final recovered = retry.plan("Ranges", "retry", pos(0, 1), block(7, 9));
		final resolver = new HaxeSourceSpanResolver("source.txt", false, recovered.positionOverrides);
		final expected = ["source.txt:1:1-1:2", "source.txt:1:4-1:5", "source.txt:1:6-1:7"];
		for (index in 0...expected.length)
			if (resolver.resolve(recovered.expressionPositions[index]).display() != expected[index])
				throw new haxe.Exception("failed cold plan polluted later recovery");
		final duplicate = new TypedFunctionSourceProvenance();
		duplicate.plan("Ranges", "duplicate", pos(0, 1), block(2, 2));
		final repeated = duplicate.plan("Ranges", "duplicate", pos(0, 1), block(8, 8));
		final duplicateResolver = new HaxeSourceSpanResolver("source.txt", false, repeated.positionOverrides);
		for (index in 1...3)
			if (duplicateResolver.resolve(repeated.expressionPositions[index]).display() != "source.txt:1:3-1:4")
				throw new haxe.Exception("equal cold positions did not recover equally");
		final foreign = file + ".foreign";
		File.saveContent(foreign, "foreign source contents");
		final foreignOwner = new TypedFunctionSourceProvenance();
		function external(offset:Int):TypedExpr
			return {expr: TConst(TInt(1)), t: type, pos: Context.makePosition({file: foreign, min: offset, max: offset + 1})};
		foreignOwner.plan("Ranges", "foreign", pos(0, 1), external(2));
		final foreignPlan = foreignOwner.plan("Ranges", "foreign", pos(0, 1), external(5));
		checkResolved(foreignPlan, 5);
		var ownershipRejected = false;
		try {
			foreignOwner.plan("Ranges", "foreign", pos(0, 1), value(5, 6));
		} catch (error:haxe.Exception) {
			if (!StringTools.contains(error.message, "changed source-file ownership"))
				throw error;
			ownershipRejected = true;
		}
		if (!ownershipRejected)
			throw new haxe.Exception("warm source ownership changed");
		FileSystem.deleteFile(foreign);
	}
}
