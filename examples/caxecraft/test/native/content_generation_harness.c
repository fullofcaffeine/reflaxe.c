#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * This C file is justified twice and owns no content-generation behavior.
 *
 * Technical necessity: an external native compiler must provide the generated
 * collector lifecycle, force collection after Haxe roots leave scope, and
 * observe exported Haxe scalars; generating that check through haxe.c would
 * compare the compiler with itself.
 *
 * Durable ownership value: this tiny consumer independently proves that
 * repeated retired generation graphs are reclaimable and that the result
 * envelope remains usable from strict C. Remove it if an equally independent
 * collector/ABI consumer takes over both evidence jobs.
 */
void hxc_caxecraft_qa_ContentGenerationProbe_main(void);

int main(void)
{
	struct hxc_gc_stats stats = HXC_GC_STATS_INITIALIZER;
	const struct hxc_gc_config config = {
		hxc_default_allocator(),
		65536U,
		NULL,
		NULL
	};
	if (hxc_gc_init(&config, &hxc_program_gc) != HXC_STATUS_OK ||
	    hxc_gc_thread_register(&hxc_program_gc, &hxc_program_gc_thread) != HXC_STATUS_OK) {
		return 1;
	}
	hxc_caxecraft_qa_ContentGenerationProbe_main();
	if (hxc_gc_collect(&hxc_program_gc) != HXC_STATUS_OK ||
	    hxc_gc_get_stats(&hxc_program_gc, &stats) != HXC_STATUS_OK) {
		return 1;
	}
	const int32_t check = hxc_caxecraft_qa_ContentGenerationProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentGenerationProbe_finalGenerationId);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentGenerationProbe_finalPublicationCount);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentGenerationProbe_finalTraceDigest);
	const int bounded = stats.collection_count > UINT64_C(0) &&
	                    stats.pressure_collection_count > UINT64_C(0) &&
	                    stats.reclaimed_object_count > UINT64_C(0) &&
	                    stats.peak_object_count < stats.allocation_count &&
	                    stats.current_object_count == 0U;
	if (hxc_gc_thread_unregister(&hxc_program_gc_thread) != HXC_STATUS_OK ||
	    hxc_gc_dispose(&hxc_program_gc) != HXC_STATUS_OK) {
		return 1;
	}
	return bounded && check == INT32_C(0) ? 0 : 1;
}
