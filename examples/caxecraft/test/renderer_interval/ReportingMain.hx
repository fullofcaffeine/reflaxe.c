import caxecraft.benchmark.IntervalObserver;
import caxecraft.benchmark.TerrainIntervalEvidence.TerrainIntervalPhase;
import caxecraft.benchmark.TerrainIntervalEvidence.writeTerrainIntervalSnapshot;

/**
 * Exercises the real cumulative report across warmup and one measured interval.
 *
 * Warmup includes one allocation and a synthetic root count above 32 bits. The
 * measured interval contains only real native root bookkeeping. Formatting and
 * printing happen outside all intervals; the host checks exact decimal values
 * and their difference, using the same parser as renderer evidence.
 */
class ReportingMain {
	/** Keep one observer run intact so reporting cannot hide sticky state by reset. */
	static function main():Void {
		FixtureApi.setup();
		IntervalObserver.reset();
		IntervalObserver.begin();
		final allocated = FixtureApi.allocate();
		IntervalObserver.end();
		FixtureApi.require(allocated);

		IntervalObserver.begin();
		FixtureApi.wideCount();
		IntervalObserver.end();
		writeTerrainIntervalSnapshot(Warmup, 2, 0);

		IntervalObserver.begin();
		final rooted = FixtureApi.roots();
		IntervalObserver.end();
		FixtureApi.require(rooted);
		writeTerrainIntervalSnapshot(Complete, 3, 0);
		FixtureApi.finish();
	}
}
