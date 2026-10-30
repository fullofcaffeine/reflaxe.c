#include "hxc/program.h"
#include "hxrt/gc.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This C file is justified twice and owns no content-loading behavior.
 *
 * Technical necessity: an external native compiler must run the generated
 * lifecycle and observe exported Haxe scalars; generating that check through
 * haxe.c would compare the compiler with itself.
 *
 * Durable ownership value: the same tiny consumer proves that the runtime
 * loader's public test envelope remains ordinary strict C ABI data. Remove it
 * if an equally independent consumer takes over both evidence jobs.
 */
void hxc_caxecraft_qa_RuntimeLevelLoaderProbe_main(void);

/*
 * Opt-in phase evidence belongs to this independent observer, not the Haxe
 * result envelope. Flushing stderr preserves the active operation on timeout;
 * "probe main" can itself include automatic collections.
 */
static void report_phase(bool enabled, const char *phase)
{
	if (enabled) {
		(void)fprintf(stderr, "native test phase: %s\n", phase);
		(void)fflush(stderr);
	}
}

int main(void)
{
	const char *progress = getenv("HXC_TEST_NATIVE_PROGRESS");
	const bool report_progress = progress != NULL && strcmp(progress, "1") == 0;
	struct hxc_gc_stats stats = HXC_GC_STATS_INITIALIZER;
	const struct hxc_gc_config config = {
		hxc_default_allocator(),
		65536U,
		NULL,
		NULL
	};
	report_phase(report_progress, "initialization");
	if (hxc_gc_init(&config, &hxc_program_gc) != HXC_STATUS_OK ||
	    hxc_gc_thread_register(&hxc_program_gc, &hxc_program_gc_thread) != HXC_STATUS_OK) {
		return 1;
	}
	report_phase(report_progress, "probe main");
	hxc_caxecraft_qa_RuntimeLevelLoaderProbe_main();
	report_phase(report_progress, "final collection");
	if (hxc_gc_collect(&hxc_program_gc) != HXC_STATUS_OK) {
		return 1;
	}
	report_phase(report_progress, "collector statistics");
	if (hxc_gc_get_stats(&hxc_program_gc, &stats) != HXC_STATUS_OK) {
		return 1;
	}
	const int32_t check = hxc_caxecraft_qa_RuntimeLevelLoaderProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeLevelLoaderProbe_traceInputHash);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeLevelLoaderProbe_traceByteLength);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeLevelLoaderProbe_traceGenerationId);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeLevelLoaderProbe_traceWorldState);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeLevelLoaderProbe_traceAuthored);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeLevelLoaderProbe_traceActorMechanics);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeLevelLoaderProbe_traceAuthority);
	const int lifecycle_reclaimed = stats.collection_count > UINT64_C(0) &&
	                               stats.reclaimed_object_count > UINT64_C(0) &&
	                               stats.peak_object_count < stats.allocation_count &&
	                               stats.current_object_count == 0U;
	report_phase(report_progress, "teardown");
	if (hxc_gc_thread_unregister(&hxc_program_gc_thread) != HXC_STATUS_OK ||
	    hxc_gc_dispose(&hxc_program_gc) != HXC_STATUS_OK) {
		return 1;
	}
	report_phase(report_progress, "complete");
	return lifecycle_reclaimed && check == INT32_C(0) ? 0 : 1;
}
