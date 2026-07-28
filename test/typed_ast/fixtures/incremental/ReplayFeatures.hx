/**
	Provides shared semantic facts for incremental-backend invalidation tests.

	The tiny program combines virtual dispatch, a generic function, and a typed
	C import. Later test mutations change one owner at a time and compare the
	warm result with a fresh-process generated project. `Main.startupOffset`
	owns the catalog's mutable-static-initializer case; this module keeps `seed`
	inline so each mutation still has one clear semantic owner.
**/
class ReplayFeatures {
	/** A compile-time constant that keeps the runtime score easy to inspect. */
	public static inline final seed:Int = 2;

	/**
		Select one define-owned constant without carrying a runtime target check.

		The catalog later adds `replay_extra`; Haxe removes the inactive branch
		during typing, so the backend must see a different current typed program.
	**/
	#if replay_extra
	static inline final variant:Int = 1;
	#else
	static inline final variant:Int = 0;
	#end

	/** Return the same value through one reachable generic specialization. */
	static function identity<T>(value:T):T
		return value;

	/**
		Exercise the shared class/dispatch/import plan without exposing test state.

		The base-typed local forces virtual dispatch to remain observable. The C
		call is deliberately discarded: its imported symbol and header are compiler
		facts even though this fixture does not link the generated project.
	**/
	public static function score():Int {
		final worker:ReplayBase = new ReplayLeaf(seed);
		ReplayNative.magnitude(ReplayNative.zero);
		return switch identity(ReplayValue(worker.value(1))) {
			case ReplayValue(value): value + variant + ReplayDependency.value() + ReplayMacro.value();
		};
	}
}

/** One generic enum specialization owned by the shared-program plan. */
private enum ReplayBox<T> {
	ReplayValue(value:T);
}

/** Base instance used to make the dispatch table reachable. */
private class ReplayBase {
	public var offset:Int;

	/** Retain one per-instance value used by the virtual method. */
	public function new(offset:Int) {
		this.offset = offset;
	}

	/** Compute the base result; subclasses may refine it. */
	public function value(delta:Int):Int
		return offset + delta;
}

/** Concrete override selected through the base-typed local in `score`. */
private class ReplayLeaf extends ReplayBase {
	/** Forward construction to the base-owned offset field. */
	public function new(offset:Int) {
		super(offset);
	}

	/** Add one recognizable subclass contribution after the base call. */
	public override function value(delta:Int):Int
		return super.value(delta) + 10;
}
