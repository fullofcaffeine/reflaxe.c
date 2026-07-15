package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.Json;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.frontend.TypedAstInventory;
import reflaxe.c.frontend.TypedProgramInput;

/** Whole-program lowering boundary. Semantic lowering is intentionally fail-closed at M0. */
class CCompiler {
	final context:CompilationContext;

	public function new(context:CompilationContext) {
		this.context = context;
	}

	public function compileModules(program:TypedProgramInput):Array<GeneratedFile> {
		if (context.typedProgram != program) {
			Context.fatalError("HXC9000: whole-program compiler received an input outside its per-build CompilationContext", compilationPosition());
		}
		if (Context.defined(TypedAstInventory.REPORT_DEFINE)) {
			Sys.println(TypedAstInventory.REPORT_PREFIX + Json.stringify(TypedAstInventory.snapshot(program)));
		}
		Context.fatalError("HXC1000: reflaxe.c reached its unimplemented whole-program lowering boundary; no C was emitted.", compilationPosition());
		return [];
	}

	static function compilationPosition():Position {
		final configuration = Compiler.getConfiguration();
		if (configuration != null && configuration.mainClass != null) {
			final mainClass = configuration.mainClass;
			final relativePath = mainClass.pack.concat([mainClass.name]).join("/") + ".hx";
			try {
				final file = Context.resolvePath(relativePath);
				return Context.makePosition({file: file, min: 0, max: 0});
			} catch (_:Dynamic) {}
		}
		return Context.currentPos();
	}
}
#else
class CCompiler {
	public function new(context:CompilationContext) {}
}
#end
