package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import reflaxe.ReflectCompiler;

/**
	Describes the frontend facts available for one incremental compiler request.

	Haxe's compilation server can restore unchanged classes without typing their
	bodies again. Reflaxe observes which class build macros reran, while enums,
	typedefs, and abstracts remain conservatively unknown. This report turns that
	bounded observation into deterministic paths for profiling; it does not cache
	`TypedExpr`, approve backend reuse, or survive the request.
**/
typedef IncrementalInputInventorySnapshot = {
	/** Version of this diagnostic JSON contract. */
	final schemaVersion:Int;

	/**
		`cold-complete` means there was no earlier server request.
		`server-partial` means class rebuild evidence came from Haxe's cache hook.
	**/
	final mode:String;

	/** Every class Haxe says it rebuilt, including any outside the target input. */
	final haxeRebuiltClassPaths:Array<String>;

	/** Rebuilt classes that are present in this target's normalized program. */
	final rebuiltClassDeclarations:Array<String>;

	/**
		Classes Haxe restored rather than rebuilding.

		This is frontend evidence only. Whole-program layout, dispatch, naming,
		runtime, and ABI decisions can still require their backend work to rerun.
	**/
	final frontendReusedClassDeclarations:Array<String>;

	/**
		Enums, typedefs, and abstracts that must still be treated as changed.

		Haxe's class build-macro hook cannot report whether these declarations
		were rebuilt, so omitting them from an invalidation key would be unsafe.
	**/
	final conservativelyReconsideredNonClassDeclarations:Array<String>;

	/** Rebuilt class paths not present in the normalized target input. */
	final unmatchedHaxeRebuiltClassPaths:Array<String>;
}

/**
	Builds an opt-in, path-stable inventory of Haxe frontend cache evidence.

	The compiler emits this snapshot before semantic lowering when
	`reflaxe_c_incremental_input_report` is defined. Keeping it separate from the
	backend makes the boundary honest: a class cache hit is one input to a future
	invalidation proof, not permission to reuse HxcIR or generated C by itself.
**/
class IncrementalInputInventory {
	/** Enables the diagnostic report without changing generated artifacts. */
	public static inline final REPORT_DEFINE = "reflaxe_c_incremental_input_report";

	/** Prefix that lets tooling distinguish this JSON record from diagnostics. */
	public static inline final REPORT_PREFIX = "HXC_INCREMENTAL_INPUT=";

	/**
		Returns deterministic frontend rebuild evidence for `program`.

		The returned arrays contain logical Haxe declaration paths and never host
		filesystem paths. A cold request lists every class as rebuilt. A server
		request partitions classes using Reflaxe's copied rebuild-path inventory
		and keeps every non-class declaration in the conservative set.
	**/
	public static function snapshot(program:TypedProgramInput):IncrementalInputInventorySnapshot {
		final reported = ReflectCompiler.getRebuiltClassPaths();
		final cold = reported == null;
		final haxeRebuiltClassPaths = reported == null ? [] : reported;
		final rebuiltSet:Map<String, Bool> = [];
		for (path in haxeRebuiltClassPaths) {
			rebuiltSet.set(path, true);
		}

		final classPaths:Array<String> = [];
		final nonClassPaths:Array<String> = [];
		final programClassSet:Map<String, Bool> = [];
		for (declaration in program.declarations) {
			if (declaration.kind == "class") {
				classPaths.push(declaration.path);
				programClassSet.set(declaration.path, true);
			} else {
				nonClassPaths.push(declaration.path);
			}
		}
		classPaths.sort(compareStrings);
		nonClassPaths.sort(compareStrings);

		final rebuiltClasses:Array<String> = [];
		final reusedClasses:Array<String> = [];
		for (path in classPaths) {
			if (cold || rebuiltSet.exists(path)) {
				rebuiltClasses.push(path);
			} else {
				reusedClasses.push(path);
			}
		}
		final unmatched:Array<String> = [];
		for (path in haxeRebuiltClassPaths) {
			if (!programClassSet.exists(path)) {
				unmatched.push(path);
			}
		}
		unmatched.sort(compareStrings);

		return {
			schemaVersion: 1,
			mode: cold ? "cold-complete" : "server-partial",
			haxeRebuiltClassPaths: haxeRebuiltClassPaths.copy(),
			rebuiltClassDeclarations: rebuiltClasses,
			frontendReusedClassDeclarations: reusedClasses,
			conservativelyReconsideredNonClassDeclarations: nonClassPaths,
			unmatchedHaxeRebuiltClassPaths: unmatched
		};
	}

	static function compareStrings(left:String, right:String):Int {
		return left < right ? -1 : left > right ? 1 : 0;
	}
}
#end
