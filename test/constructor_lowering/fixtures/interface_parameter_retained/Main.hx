/** One interface retained by a longer-lived class instance. */
interface ScoreSource {
	/** Add an implementation-owned base to the caller's seed. */
	function score(seed:Int):Int;
}

/** Small ownership-free record stored directly in `RetainedScore`. */
private typedef ScoreSettings = {
	final offset:Int;
	final enabled:Bool;
}

/** Managed record whose Array owner must survive safe field replacement. */
private typedef ScoreDraft = {
	final values:Array<Int>;
}

/**
 * Concrete score state whose lifetime is hidden behind `ScoreSource`.
 *
 * This class becomes collector-managed because it can inhabit the retained
 * interface field below. Call-only interface fixtures remain stack allocated,
 * proving that retention—not merely `implements`—selects the collector.
 */
final class FixedScore implements ScoreSource {
	var base:Int;

	/** Stores the small piece of state used by later interface dispatch. */
	public function new(base:Int) {
		this.base = base;
	}

	/** Returns the caller's seed plus this implementation's stored base. */
	public function score(seed:Int):Int
		return base + seed;
}

/**
 * Concrete child retained through a nullable class field.
 *
 * This type does not implement `ScoreSource`, so it enters the collector graph
 * only because the already-managed `RetainedScore` stores it. That distinction
 * proves representation planning follows the real object graph instead of
 * marking every class in this fixture as managed.
 */
final class DirectScore {
	var base:Int;

	/**
	 * Stores the state read after the creating method has returned.
	 *
	 * The negative branch makes this constructor genuinely fallible. Normal
	 * fixture calls stay on the successful path while generated HxcIR must still
	 * carry the explicit uncaught-exception edge.
	 */
	public function new(base:Int) {
		if (base < 0)
			throw 99;
		this.base = base;
	}

	/** Returns the caller's seed plus this child's stored base. */
	public function score(seed:Int):Int
		return base + seed;
}

/**
 * Owns one interface value beyond the constructor call that supplied it.
 *
 * Generated C stores the interface's object/table pair by value. The object is
 * collector-owned, and this class's exact trace callback visits `source.object`
 * so later collection cannot leave the dispatch table pointing at dead storage.
 */
final class RetainedScore {
	final source:ScoreSource;
	final settings:ScoreSettings;
	var draft:ScoreDraft;
	var direct:Null<DirectScore> = null;

	/**
	 * Counts calls to `advance` while exposing only a public getter.
	 *
	 * In Haxe, `(default, null)` means callers may read this value but only this
	 * declaring class may write it. It is not a `final` field. The compiler must
	 * preserve that distinction even when `advance` is inlined into its caller.
	 */
	public var advances(default, null):Int = 0;

	/**
	 * Retains an interface past this constructor call.
	 *
	 * Unlike a call-only interface argument, this assignment requires the
	 * compiler to keep both this owner and the concrete `source` object alive.
	 */
	public function new(source:ScoreSource) {
		this.source = source;
		settings = {offset: 2, enabled: true};
		draft = {values: [40]};
	}

	/** Dispatches through the retained interface after construction has ended. */
	public function read(seed:Int):Int
		return source.score(seed);

	/** Read both direct-record and managed-record constructor fields. */
	public function readDraft():Int
		return settings.enabled ? draft.values[0] + settings.offset : -1;

	/**
	 * Replace the managed record through capture-before-release ownership.
	 *
	 * The new record becomes the field owner before the old Array is released,
	 * so aliasing and allocation failure cannot leave the field dangling.
	 */
	public function replaceDraft(value:Int):Void
		draft = {values: [value]};

	/** Store one caller-owned record after acquiring its nested Array owner. */
	function assignDraft(next:ScoreDraft):Void
		draft = next;

	/** Exercise an alias-preserving replacement without source-level `x = x`. */
	public function keepDraft():Void
		assignDraft(draft);

	/**
	 * Replace the optional child with one fresh collector-managed instance.
	 *
	 * The child outlives this method, so generated C must allocate stable traced
	 * storage rather than retain the address of an automatic local.
	 */
	public function installDirect(base:Int):Void
		direct = new DirectScore(base);

	/** Remove the graph edge without manually freeing collector-owned storage. */
	public function clearDirect():Void
		direct = null;

	/** Read the retained child, or `-1` when the nullable field is empty. */
	public function readDirect(seed:Int):Int
		return direct == null ? -1 : direct.score(seed);

	/**
	 * Mutates private-set storage through a method that Haxe may inline.
	 *
	 * This mirrors the cursor operation that Caxecraft used to expose the
	 * compiler bug: access control is checked before inlining, while the emitted
	 * C field must remain writable for this already-approved operation.
	 */
	public inline function advance():Void
		advances++;
}

/** Exercises delayed interface dispatch after deterministic collection pressure. */
final class Main {
	/** Builds the retained graph without exposing its concrete implementation. */
	static function build():RetainedScore
		return new RetainedScore(new FixedScore(40));

	/** Forces several collector cycles so a missing trace edge fails reliably. */
	static function forceCollectionPressure():Void {
		for (index in 0...40000)
			new FixedScore(index);
	}

	/** Keeps the process alive only when delayed interface dispatch returns 42. */
	static function main():Void {
		final value = build();
		value.advance();
		value.advance();
		value.installDirect(40);
		forceCollectionPressure();
		while (value.advances != 2 || value.read(2) != 42 || value.readDirect(2) != 42 || value.readDraft() != 42) {}
		value.keepDraft();
		value.replaceDraft(39);
		forceCollectionPressure();
		while (value.readDraft() != 41) {}
		value.installDirect(39);
		forceCollectionPressure();
		while (value.readDirect(3) != 42) {}
		value.clearDirect();
		forceCollectionPressure();
		while (value.readDirect(2) != -1) {}
	}
}
