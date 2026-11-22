package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Type.ClassType;
import haxe.macro.Type.Ref;

/**
	Request-local answers to class-hierarchy questions asked during lowering.

	Dispatch planning repeatedly asks two questions about Haxe's typed classes:
	which class or interface declares a field, and whether a class implements an
	interface. Answering walks the class graph through macro `Ref.get()` calls,
	which decode compiler data and rebuild dotted path strings on every visit.
	A Caxecraft build asks these questions hundreds of thousands of times, yet
	within one compilation the answers never change. This module remembers each
	answer for the rest of the request.

	`beginRequest` starts an empty memo (or none, when disabled) at the start of
	every compilation, and `endRequest` drops it when the compilation completes
	or fails, so no answer survives a compiler-server request; the next request
	recomputes everything from its own typed program. With no active request
	(direct structural tools and tests), every query runs its uncached
	computation.

	Keys are qualified class paths plus a field name or a target interface path,
	never object identity or iteration order, so a hit returns exactly what the
	computation returned earlier in the same request.

	A static memo with an explicit lifecycle follows the existing request caches
	(`CBodyControlFlowPlanCache`, `GeneratedFileDigestCache`). An instance owned
	by `CompilationContext` would avoid statics, but the queries are reached from
	static helpers used by several lowering owners, so threading an instance
	through all of them would be a much larger change for the same lifetime.
**/
class CClassHierarchyMemo {
	/** Define that disables the memo so a build can compare cached and uncached cost. */
	public static inline final DISABLE_DEFINE = "reflaxe_c_test_disable_class_hierarchy_memo";

	static var declaringClasses:Null<Map<String, Ref<ClassType>>> = null;
	static var interfaceImplementations:Null<Map<String, Bool>> = null;

	/**
		Start one compilation: an empty memo when `enabled`, otherwise none.

		Always replacing the previous state means a request can never read
		answers left by an earlier request that failed before `endRequest`.
	**/
	public static function beginRequest(enabled:Bool):Void {
		declaringClasses = enabled ? [] : null;
		interfaceImplementations = enabled ? [] : null;
	}

	/** Drop the current request's answers. Later queries compute uncached. */
	public static function endRequest():Void {
		declaringClasses = null;
		interfaceImplementations = null;
	}

	/**
		Return the remembered declaring class for `key`, computing it once.

		`key` must name the receiver class, the field name, and the field's
		declaration position. The position keeps overload variants apart: they
		share a name but are not listed among their class's fields, so they can
		resolve to a different class than the main field.
	**/
	public static function declaringClass(key:String, compute:() -> Ref<ClassType>):Ref<ClassType> {
		final memo = declaringClasses;
		if (memo == null)
			return compute();
		final known = memo.get(key);
		if (known != null)
			return known;
		final owner = compute();
		memo.set(key, owner);
		return owner;
	}

	/** Return whether the class in `key` implements its target interface, computing once. */
	public static function implementsInterface(key:String, compute:() -> Bool):Bool {
		final memo = interfaceImplementations;
		if (memo == null)
			return compute();
		final known = memo.get(key);
		if (known != null)
			return known;
		final value = compute();
		memo.set(key, value);
		return value;
	}
}
#end
