package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Path;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.PositionTools;
import reflaxe.c.ir.HxcSourceSpan;

/**
	Converts a Haxe compiler position into the stable source range stored in HxcIR.

	Haxe keeps positions in a compiler-owned format. This boundary asks Haxe for
	the corresponding one-based line and column range, then replaces checkout-
	specific filenames with the logical source path already owned by the target.
	The result keeps diagnostics and generated `#line` mappings deterministic
	across machines.
**/
class HaxeSourceSpan {
	/** Convert one position without retaining state between calls. */
	public static function fromPosition(position:Position, fallbackPath:String):HxcSourceSpan {
		final info = Context.getPosInfos(position);
		return fromPositionInfo(position, info.file, fallbackPath);
	}

	/**
		Convert a position whose compiler filename was already read by a resolver.

		`HaxeSourceSpanResolver` uses this seam so its exact-position cache does not
		call `Context.getPosInfos` twice on a miss. Callers that do not own a
		request-local resolver should use `fromPosition`.
	**/
	@:noCompletion
	public static function fromPositionInfo(position:Position, rawPath:String, fallbackPath:String):HxcSourceSpan {
		final location = PositionTools.toLocation(position);
		final file = normalizedSourcePath(rawPath, fallbackPath);
		// Haxe 5 exposes compiler-native one-based coordinates here; the range
		// end is already exclusive, so no LSP-style re-basing is appropriate.
		return new HxcSourceSpan(file, location.range.start.line, location.range.start.character, location.range.end.line, location.range.end.character);
	}

	static function normalizedSourcePath(rawPath:String, fallbackPath:String):String {
		// The target-owned logical coordinate is independent of caller CWD and
		// checkout root. Compiler-native paths are only a fallback for adapters
		// that have not yet supplied such a coordinate.
		final fallback = normalize(fallbackPath);
		if (HxcSourceSpan.isNormalizedFile(fallback)) {
			return fallback;
		}
		final normalized = normalize(rawPath);
		final cwd = normalize(Sys.getCwd());
		final relative = if (normalized == cwd) "" else if (StringTools.startsWith(normalized, cwd + "/")) normalized.substr(cwd.length + 1) else normalized;
		if (HxcSourceSpan.isNormalizedFile(relative)) {
			return relative;
		}
		throw new CBodyEmissionError('cannot normalize Haxe source position `$rawPath` or logical fallback `$fallbackPath`');
	}

	static function normalize(value:String):String {
		var result = StringTools.replace(Path.normalize(value), "\\", "/");
		while (result.length > 1 && StringTools.endsWith(result, "/")) {
			result = result.substr(0, result.length - 1);
		}
		return result;
	}
}

/**
	Reuses exact Haxe-position conversions while one function body is lowered.

	One typed Haxe expression can expand into several HxcIR instructions, and
	each instruction keeps a source range for diagnostics. Re-asking Haxe to
	convert the same compiler position does not add information. This resolver
	keys the result by Haxe's exact filename and byte-offset pair, so only truly
	identical compiler positions share a range; different files or offsets are
	still converted independently.

	The cache lives for one `FunctionBuilder`, which gives it a clear lifetime and
	prevents source positions from leaking between compiler requests. Its counters
	run only when the opt-in compiler profiler is active.
**/
@:noCompletion
class HaxeSourceSpanResolver {
	final fallbackPath:String;
	final collectWork:Bool;
	final spansByExactPosition:Map<String, HxcSourceSpan> = [];

	/** Number of conversions requested while profiling was enabled. */
	public var requestCount(default, null) = 0;

	/** Number of distinct positions that required Haxe coordinate conversion. */
	public var computationCount(default, null) = 0;

	/** Number of requests served from an earlier exact-position result. */
	public var cacheHitCount(default, null) = 0;

	/** CPU seconds spent looking up and converting positions. */
	public var cpuSeconds(default, null) = 0.0;

	/** Create a resolver for one logical source owner. */
	public function new(fallbackPath:String, collectWork:Bool) {
		this.fallbackPath = fallbackPath;
		this.collectWork = collectWork;
	}

	/**
		Return the stable range for `position`.

		The key includes the raw compiler filename as well as both byte offsets.
		This is slightly more conservative than comparing offsets alone, but it
		keeps macro-generated or inlined positions from different files distinct.
	**/
	public function resolve(position:Position):HxcSourceSpan {
		final started = collectWork ? Sys.cpuTime() : 0.0;
		final info = Context.getPosInfos(position);
		final key = info.file + "\n" + info.min + ":" + info.max;
		final cached = spansByExactPosition.get(key);
		final result = if (cached != null) {
			if (collectWork)
				cacheHitCount++;
			cached;
		} else {
			if (collectWork)
				computationCount++;
			final computed = HaxeSourceSpan.fromPositionInfo(position, info.file, fallbackPath);
			spansByExactPosition.set(key, computed);
			computed;
		};
		if (collectWork) {
			requestCount++;
			cpuSeconds += Sys.cpuTime() - started;
		}
		return result;
	}
}
#else
class HaxeSourceSpan {}
class HaxeSourceSpanResolver {}
#end
