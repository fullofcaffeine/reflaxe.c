package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type.ModuleType;
import reflaxe.c.emit.GeneratedFile;

/** Whole-program lowering boundary. Semantic lowering is intentionally fail-closed at M0. */
class CCompiler {
	final context:CompilationContext;

	public function new(context:CompilationContext) {
		this.context = context;
	}

	public function compileModules(modules:Array<ModuleType>):Array<GeneratedFile> {
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
