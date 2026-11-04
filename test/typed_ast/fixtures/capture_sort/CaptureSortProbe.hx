import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.frontend.TypedAstNormalizer;

/** Count real declaration decoding while independently checking normalized order. */
class CaptureSortProbe {
	/** Capture real declarations twice to bound bridge reads and preserve ordering. */
	public static function run():Void {
		Context.onAfterInitMacros(() -> {
			var reads = 0;
			final modules:Array<ModuleType> = [];
			final expected = [for (i in 0...32) "Item" + StringTools.lpad(Std.string(i), "0", 2)];
			for (i in 0...32) {
				final name = expected[(i * 13) % 32];
				switch Context.getType("SortFixture." + name) {
					case TInst(reference, _):
						final counted:Ref<ClassType> = {
							get: () -> {
								reads++;
								return reference.get();
							},
							toString: () -> reference.toString()
						};
						modules.push(TClassDecl(counted));
					case _: throw "fixture is not a class";
				}
			}
			final program = TypedAstNormalizer.normalize(modules, null, null);
			final readCount = reads;
			final names = [for (declaration in program.declarations) declaration.name];
			if (names.join(",") != expected.join(",") || program.declarations.length != 32 || program.expressionRoots.length != 0)
				throw "normalized inventory changed";
			final rawNames = [for (raw in program.rawModules) TypedAstNormalizer.baseType(raw).name];
			if (rawNames.join(",") != expected.join(","))
				throw "raw module order changed";
			if (readCount > 96)
				throw "module capture exceeded three declaration reads per input";
			modules.reverse();
			final beforeSecond = reads;
			final second = TypedAstNormalizer.normalize(modules, null, null);
			if (reads - beforeSecond > 96 || reads - beforeSecond < 32)
				throw "repeated capture must read current declarations with a linear bound";
			if ([for (declaration in second.declarations) declaration.name].join(",") != expected.join(","))
				throw "reversing input changed normalized order";
			if ([for (raw in second.rawModules) TypedAstNormalizer.baseType(raw).name].join(",") != expected.join(","))
				throw "reversing input changed raw module order";
			Sys.println("HXC_CAPTURE_SORT_OK");
		});
	}
}
