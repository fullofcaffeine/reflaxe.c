import caxecraft.benchmark.IntervalObserver;
import caxecraft.benchmark.IntervalObserver.IntervalEvent;

/**
 * Crosses the real Haxe-to-C import boundary with independent native observations.
 * Standalone Bool marker calls deliberately discard their results: the generated
 * AST must retain each direct call, while valid/completed checks reject bad runs.
 */
class Main {
	/** Prove exact event types, counters, allocation observation, and root bookkeeping. */
	static function main():Void {
		FixtureApi.setup();
		IntervalObserver.reset();
		IntervalObserver.begin();
		final allocated = FixtureApi.allocate();
		IntervalObserver.end();
		FixtureApi.require(allocated && IntervalObserver.valid());
		FixtureApi.require(IntervalObserver.completed() == FixtureApi.one);
		FixtureApi.require(IntervalObserver.count(AllocateAttempt) == FixtureApi.one);
		FixtureApi.require(IntervalObserver.count(ReallocateAttempt) == FixtureApi.zero);
		FixtureApi.require(IntervalObserver.count(CollectAttempt) == FixtureApi.zero);
		FixtureApi.require(IntervalObserver.count(RootPushAttempt) == FixtureApi.zero);
		FixtureApi.require(IntervalObserver.count(RootPopAttempt) == FixtureApi.zero);

		IntervalObserver.reset();
		IntervalObserver.begin();
		final rooted = FixtureApi.roots();
		IntervalObserver.end();
		FixtureApi.require(rooted && IntervalObserver.valid());
		FixtureApi.require(IntervalObserver.completed() == FixtureApi.one);
		FixtureApi.require(IntervalObserver.count(AllocateAttempt) == FixtureApi.zero);
		FixtureApi.require(IntervalObserver.count(ReallocateAttempt) == FixtureApi.zero);
		FixtureApi.require(IntervalObserver.count(CollectAttempt) == FixtureApi.zero);
		FixtureApi.require(IntervalObserver.count(RootPushAttempt) == FixtureApi.one);
		FixtureApi.require(IntervalObserver.count(RootPopAttempt) == FixtureApi.one);

		IntervalObserver.reset();
		IntervalObserver.begin();
		FixtureApi.wideCount();
		IntervalObserver.end();
		FixtureApi.require(IntervalObserver.valid());
		FixtureApi.require(IntervalObserver.completed() == FixtureApi.one);
		FixtureApi.require(IntervalObserver.count(RootPushAttempt) == FixtureApi.wide);
		FixtureApi.finish();
	}
}
