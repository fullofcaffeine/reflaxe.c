#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

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
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
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
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
