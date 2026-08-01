#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Observe only the Haxe-owned runtime-generation result envelope.
 *
 * This independent C consumer proves the generated lifecycle and scalar ABI;
 * it deliberately owns no JSON parsing, hashing, schema, or gameplay oracle.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_RuntimeContentGenerationProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_RuntimeContentGenerationProbe_traceGenerationId);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_RuntimeContentGenerationProbe_tracePack);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_RuntimeContentGenerationProbe_traceUi);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_RuntimeContentGenerationProbe_traceWorldState);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_RuntimeContentGenerationProbe_traceSourceBytes);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
