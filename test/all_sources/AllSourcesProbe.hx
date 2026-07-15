import reflaxe.c.BuildDetection;
import reflaxe.c.CCompiler;
import reflaxe.c.CProfile;
import reflaxe.c.CReflaxeCompiler;
import reflaxe.c.CompilationContext;
import reflaxe.c.CompilerBootstrap;
import reflaxe.c.CompilerInit;
import reflaxe.c.ProfileResolver;
import reflaxe.c.TargetPlatform;
import reflaxe.c.ast.CAST.CTranslationUnit;
import reflaxe.c.ast.CASTPrinter;

/** Exercises target-context implementations while the HXML includes every owned module. */
class AllSourcesProbe {
	static function main():Void {
		CompilerBootstrap.Start();
		CompilerInit.Start();
		c.Init.init();
		TargetPlatform.configure();

		if (BuildDetection.isCBuild()) {
			throw "the all-source Eval type-check must remain a non-C build";
		}
		if (ProfileResolver.resolve() != CProfile.Portable) {
			throw "the non-macro profile default drifted";
		}
		if (!TargetPlatform.environmentSupportsSys("hosted") || TargetPlatform.environmentSupportsSys("freestanding")) {
			throw "the shared environment capability predicate drifted";
		}

		final context = new CompilationContext(CProfile.Portable);
		new CCompiler(context);
		new CReflaxeCompiler();

		final unit = new CTranslationUnit();
		if (new CASTPrinter().printTranslationUnit(unit) != "\n") {
			throw "the empty C translation unit printer contract drifted";
		}

		Sys.println("all-sources: OK");
	}
}
