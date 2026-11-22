package caxecraft.benchmark;

/**
 * Typed access to the independent native correctness-test observer.
 *
 * This module imports a test ABI; it does not export generated Haxe or select
 * a runtime feature. Only instrumented correctness artifacts may link the
 * observer. Ordinary game and timing builds must leave this module unreachable.
 * Counts describe API attempts, including failures and realloc delegation;
 * they do not describe successful allocations or balanced root frames.
 */
@:c.layout(c.Layout.Enum)
@:c.name("caxecraft_test_interval_event")
@:c.include("interval_observer.h", c.IncludeKind.Local)
extern enum IntervalEvent {
	/** Entry to the checked allocation API, including zero-size calls. */
	@:c.name("CAXECRAFT_TEST_INTERVAL_ALLOC_ATTEMPT")
	AllocateAttempt;

	/** Entry to resize; its fallback can also enter the allocation API. */
	@:c.name("CAXECRAFT_TEST_INTERVAL_REALLOC_ATTEMPT")
	ReallocateAttempt;

	/** Collection entry, observed before validation and the clock callback. */
	@:c.name("CAXECRAFT_TEST_INTERVAL_COLLECT_ATTEMPT")
	CollectAttempt;

	/** Root-frame push entry; this includes rejected pushes. */
	@:c.name("CAXECRAFT_TEST_INTERVAL_ROOT_PUSH_ATTEMPT")
	RootPushAttempt;

	/** Root-frame pop entry; this includes rejected pops. */
	@:c.name("CAXECRAFT_TEST_INTERVAL_ROOT_POP_ATTEMPT")
	RootPopAttempt;
}

/**
 * Imports the fixed-storage, single-thread observer without recreating its layout.
 *
 * Begin/end remain direct native calls so generated-C analysis can identify
 * the complete observed region. Callers check valid() and completed() afterward;
 * ignoring a marker's Bool result must never make malformed evidence valid.
 */
@:c.include("interval_observer.h", c.IncludeKind.Local)
extern class IntervalObserver {
	/** Start a fresh independent run; an active reset invalidates that run. */
	@:c.name("caxecraft_test_interval_reset")
	public static function reset():Void;

	/** Open one non-nested observation region without evaluating render arguments. */
	@:c.name("caxecraft_test_interval_begin")
	public static function begin():Bool;

	/** Close the region; malformed evidence remains invalid until an inactive reset. */
	@:c.name("caxecraft_test_interval_end")
	public static function end():Bool;

	/** Read one exact counter without narrowing through Haxe Int or Float. */
	@:c.name("caxecraft_test_interval_count")
	public static function count(event:IntervalEvent):c.UInt64;

	/** Read the exact number of successfully closed intervals since reset. */
	@:c.name("caxecraft_test_interval_completed")
	public static function completed():c.UInt64;

	/** Reject unfinished intervals, overflow, nesting, or unbalanced markers. */
	@:c.name("caxecraft_test_interval_valid")
	public static function valid():Bool;
}
