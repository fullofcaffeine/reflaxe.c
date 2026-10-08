package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import reflaxe.c.CContentDigest.sha256Hex;
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

/** Source identity shared by functions captured in one normalization request. */
private typedef FunctionSourceIdentity = {
	final digest:String;
	final length:Int;
}

/**
	Carries one request's already-computed function identity inputs into lowering.

	Frontend provenance must inspect the complete typed tree to repair positions
	that Haxe's server can reuse. Body replay needs the same canonical tree text
	and expression order. Keeping those request-local values together avoids a
	second print and traversal without making them persistent cache authority.
**/
typedef TypedFunctionSourcePlan = {
	/** The stable typed-tree text that both provenance and replay identity use. */
	final canonicalTypedExpressionText:String;

	/** The typed tree's positions in deterministic traversal order for this request. */
	final expressionPositions:Array<Position>;

	/** Maps reused compiler positions to the matching positions in current source bytes. */
	final positionOverrides:Map<String, Position>;
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
	final sourcesByFile:Map<String, FunctionSourceIdentity> = [];

	/**
		Own one request's file identities, separate from the persistent position cache.

		An instance bounds reuse to one normalization pass. A new pass reads current
		bytes again, including edits received by the same compilation server.
	**/
	public function new() {}

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
	public function plan(declarationPath:String, fieldName:String, declarationPosition:Position, expression:TypedExpr):TypedFunctionSourcePlan {
		final positions:Array<Position> = [];
		function visit(value:TypedExpr):Void {
			positions.push(value.pos);
			TypedExprTools.iter(value, visit);
		}
		visit(expression);
		final canonicalText = canonicalTypedExpressionText(expression);
		final declarationInfo = Context.getPosInfos(declarationPosition);
		final source = sourceIdentity(declarationInfo.file);
		if (source == null) {
			return {
				canonicalTypedExpressionText: canonicalText,
				expressionPositions: positions,
				positionOverrides: []
			};
		}
		final key = [
			Std.string(CACHE_SCHEMA),
			declarationPath,
			fieldName,
			source.digest,
			canonicalText
		].join("\n");
		final previous = cacheByContent.get(key);
		final hasPrevious = previous != null;
		final cached:Array<CachedTypedFunctionPosition> = previous == null ? [] : previous;
		if (hasPrevious && cached.length != positions.length)
			throw new haxe.Exception('typed function `$declarationPath.$fieldName` changed source-position arity under one semantic cache key');

		final result:Map<String, Position> = [];
		for (index in 0...positions.length) {
			final current = positions[index];
			final currentInfo = Context.getPosInfos(current);
			final remembered:CachedTypedFunctionPosition = if (hasPrevious) cached[index] else {
				owned: currentInfo.file == declarationInfo.file,
				min: currentInfo.min,
				max: currentInfo.max
			};
			if (!hasPrevious)
				cached.push(remembered);
			if (remembered.owned != (currentInfo.file == declarationInfo.file))
				throw new haxe.Exception('typed function `$declarationPath.$fieldName` changed source-file ownership under Haxe server reuse');
			if (!remembered.owned)
				continue;
			if (remembered.min < 0 || remembered.max < remembered.min || remembered.max > source.length)
				throw new haxe.Exception('typed function `$declarationPath.$fieldName` retained an invalid source-position offset');
			// Preserve the current compiler value when its exact range already matches.
			// Only a restored warm-server range needs a new Position allocation.
			final position = currentInfo.min == remembered.min && currentInfo.max == remembered.max ? current : Context.makePosition({
				file: declarationInfo.file,
				min: remembered.min,
				max: remembered.max
			});
			final currentKey = currentInfo.file + "\n" + currentInfo.min + ":" + currentInfo.max;
			final existing = result.get(currentKey);
			if (existing != null) {
				final existingInfo = Context.getPosInfos(existing);
				if (existingInfo.file != declarationInfo.file || existingInfo.min != remembered.min || existingInfo.max != remembered.max)
					throw new haxe.Exception('typed function `$declarationPath.$fieldName` collapsed distinct source positions under Haxe server reuse');
			}
			result.set(currentKey, position);
		}
		// Publish only after every range and collision check succeeds.
		if (!hasPrevious)
			remember(key, cached);
		return {
			canonicalTypedExpressionText: canonicalText,
			expressionPositions: positions,
			positionOverrides: result
		};
	}

	/** Hash each readable file once; failed reads remain retryable within the request. */
	function sourceIdentity(file:String):Null<FunctionSourceIdentity> {
		final existing = sourcesByFile.get(file);
		if (existing != null)
			return existing;
		final bytes = try {
			File.getBytes(file);
		} catch (_:haxe.Exception) {
			return null;
		}
		final source:FunctionSourceIdentity = {digest: sha256Hex(bytes), length: bytes.length};
		sourcesByFile.set(file, source);
		return source;
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
