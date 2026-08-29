package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type.TypedExpr;
import haxe.macro.TypedExprTools;
import sys.io.File;

private typedef CachedTypedFunctionPosition = {
	final owned:Bool;
	final min:Int;
	final max:Int;
}

/**
	Keeps exact typed-function positions stable across Haxe server cache reuse.

	The pinned frontend can retain the same typed tree while changing positions
	for its outer function, a one-expression body, or a parenthesized child. This
	owner remembers only byte offsets from the first authoritative request. Its
	key contains the exact source bytes and canonical typed tree, so a source or
	semantic edit cannot reuse stale positions. Macro-generated positions from a
	different file remain compiler-owned and are never rewritten here.
**/
class TypedFunctionSourceProvenance {
	static inline final CACHE_SCHEMA = 1;
	static inline final MAX_CACHE_ENTRIES = 4096;

	#if (eval && macro)
	@:persistent
	#end
	static var cacheByContent:Map<String, Array<CachedTypedFunctionPosition>> = [];

	#if (eval && macro)
	@:persistent
	#end
	static var cacheInsertionOrder:Array<String> = [];

	/**
		Return current-request positions indexed by the compiler positions they replace.

		The traversal is structural and deterministic. If one cached compiler shape
		collapses distinct authored ranges onto the same current position, recovery
		fails instead of choosing a plausible but incorrect source span.
	**/
	public static function plan(declarationPath:String, fieldName:String, declarationPosition:Position, expression:TypedExpr):Map<String, Position> {
		final declarationInfo = Context.getPosInfos(declarationPosition);
		final bytes = try {
			File.getBytes(declarationInfo.file);
		} catch (_:haxe.Exception) {
			return [];
		}
		final positions:Array<Position> = [];
		function visit(value:TypedExpr):Void {
			positions.push(value.pos);
			TypedExprTools.iter(value, visit);
		}
		visit(expression);
		final key = [
			Std.string(CACHE_SCHEMA),
			declarationPath,
			fieldName,
			Sha256.make(bytes).toHex(),
			canonicalTypedExpressionText(expression)
		].join("\n");
		var cached = cacheByContent.get(key);
		if (cached == null) {
			cached = positions.map(position -> {
				final info = Context.getPosInfos(position);
				{owned: info.file == declarationInfo.file, min: info.min, max: info.max};
			});
			remember(key, cached);
		}
		if (cached.length != positions.length)
			throw new haxe.Exception('typed function `$declarationPath.$fieldName` changed source-position arity under one semantic cache key');

		final result:Map<String, Position> = [];
		for (index in 0...positions.length) {
			final current = positions[index];
			final currentInfo = Context.getPosInfos(current);
			final remembered = cached[index];
			if (remembered.owned != (currentInfo.file == declarationInfo.file))
				throw new haxe.Exception('typed function `$declarationPath.$fieldName` changed source-file ownership under Haxe server reuse');
			if (!remembered.owned)
				continue;
			if (remembered.min < 0 || remembered.max < remembered.min || remembered.max > bytes.length)
				throw new haxe.Exception('typed function `$declarationPath.$fieldName` retained an invalid source-position offset');
			final position = Context.makePosition({file: declarationInfo.file, min: remembered.min, max: remembered.max});
			final currentKey = exactPositionKey(current);
			final existing = result.get(currentKey);
			if (existing != null && exactPositionKey(existing) != exactPositionKey(position))
				throw new haxe.Exception('typed function `$declarationPath.$fieldName` collapsed distinct source positions under Haxe server reuse');
			result.set(currentKey, position);
		}
		return result;
	}

	/** Build the exact key used by request-local source-span resolvers. */
	public static function exactPositionKey(position:Position):String {
		final info = Context.getPosInfos(position);
		return info.file + "\n" + info.min + ":" + info.max;
	}

	static function remember(key:String, positions:Array<CachedTypedFunctionPosition>):Void {
		cacheByContent.set(key, positions);
		cacheInsertionOrder.push(key);
		while (cacheInsertionOrder.length > MAX_CACHE_ENTRIES) {
			final oldest = cacheInsertionOrder.shift();
			if (oldest != null)
				cacheByContent.remove(oldest);
		}
	}

	/** Remove process-local variable numbers without weakening typed structure. */
	@:noCompletion
	public static function canonicalTypedExpressionText(expression:TypedExpr):String {
		final variableIds:Map<String, Int> = [];
		var nextVariableId = 0;
		function stable(originalId:String):Int {
			var stableId = variableIds.get(originalId);
			if (stableId == null) {
				stableId = nextVariableId++;
				variableIds.set(originalId, stableId);
			}
			return stableId;
		}
		final angleMarker = ~/\[(Arg|Local|Var) ([^<\r\n]+)<([0-9]+)>/g;
		final angleCanonical = angleMarker.map(TypedExprTools.toString(expression, false), marker -> {
			return '[${marker.matched(1)} ${marker.matched(2)}<${stable(marker.matched(3))}>';
		});
		final parenthesizedMarker = ~/\[(Local|Var) ([^(\r\n]+)\(([0-9]+)\):/g;
		return parenthesizedMarker.map(angleCanonical, marker -> {
			return '[${marker.matched(1)} ${marker.matched(2)}(${stable(marker.matched(3))}):';
		});
	}
}
#else
class TypedFunctionSourceProvenance {}
#end
