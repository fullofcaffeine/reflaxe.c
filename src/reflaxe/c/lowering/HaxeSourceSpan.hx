package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Path;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.PositionTools;
import reflaxe.c.ir.HxcSourceSpan;

/** Converts compiler-native positions into stable one-based HxcIR ranges. */
class HaxeSourceSpan {
	public static function fromPosition(position:Position, fallbackPath:String):HxcSourceSpan {
		final location = PositionTools.toLocation(position);
		final file = normalizedSourcePath(Context.getPosInfos(position).file, fallbackPath);
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
#else
class HaxeSourceSpan {}
#end
