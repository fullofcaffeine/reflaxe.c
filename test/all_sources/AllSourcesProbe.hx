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
import haxe.Timer;
import haxe.atomic.AtomicBool;
import haxe.atomic.AtomicInt;
import haxe.atomic.AtomicObject;

/** Exercises target-context implementations while the HXML includes every owned module. */
class AllSourcesProbe {
	static function main():Void {
		#if c
		// Keep the production branch focused on its stable first unsupported
		// boundary now that reachable instance methods are followed transitively.
		ProfileResolver.resolve();
		#else
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
		final timerTypeProbe:Null<Timer> = null;
		if (timerTypeProbe != null) {
			throw "the target-owned Timer override must remain type-visible";
		}
		// The haxe.atomic overrides are parsed only when referenced; keep them in
		// the exhaustive graph without constructing atomic storage under Eval.
		final atomicBoolProbe:Null<AtomicBool> = null;
		final atomicIntProbe:Null<AtomicInt> = null;
		final atomicObjectProbe:Null<AtomicObject<String>> = null;
		if (atomicBoolProbe != null || atomicIntProbe != null || atomicObjectProbe != null) {
			throw "the target-owned haxe.atomic overrides must remain type-visible";
		}

		final context = new CompilationContext(CProfile.Portable);
		new CCompiler(context);
		new CReflaxeCompiler();

		final unit = new CTranslationUnit();
		if (new CASTPrinter().printTranslationUnit(unit) != "\n") {
			throw "the empty C translation unit printer contract drifted";
		}

		Sys.println("all-sources: OK");
		#end
	}
}
