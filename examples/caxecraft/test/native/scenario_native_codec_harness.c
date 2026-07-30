#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the shared CAXEMAP codec. The renamed
 * generated entry point owns collector startup and shutdown exactly as the
 * shipped executable does. Parsing and assertions stay in Haxe; this consumer
 * only reads and prints the resulting scalar.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_ScenarioNativeCodecProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceBytes);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceWidth);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceHeight);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceDepth);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_tracePalette);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceFluids);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceObjects);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceDialogues);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ScenarioNativeCodecProbe_traceObjectives);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
