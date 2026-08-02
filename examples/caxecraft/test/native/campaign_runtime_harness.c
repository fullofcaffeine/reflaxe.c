#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Observe only the Haxe-owned campaign result envelope.
 *
 * This independent C consumer proves the generated lifecycle and scalar ABI;
 * it owns no JSON parsing, hashing, CAXEMAP semantics, or publication oracle.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_CampaignRuntimeProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_CampaignRuntimeProbe_traceManifest);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_CampaignRuntimeProbe_traceGeneration);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_CampaignRuntimeProbe_traceWorldState);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_CampaignRuntimeProbe_traceScenario);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_CampaignRuntimeProbe_traceAuthored);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
