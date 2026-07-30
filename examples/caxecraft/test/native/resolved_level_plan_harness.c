#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * This C file is justified twice and owns no level-resolution behavior.
 *
 * Technical necessity: an external native compiler must run the generated
 * lifecycle and observe exported Haxe scalars; generating that check through
 * haxe.c would compare the compiler with itself.
 *
 * Durable ownership value: the same tiny consumer proves that the plan's
 * public test envelope remains usable from ordinary strict C. Remove it if an
 * equally independent ABI consumer takes over both evidence jobs.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_ResolvedLevelPlanProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceScenario);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceWorldCells);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceTerrainRuns);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceTerrain);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceFluids);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceFluid);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceItems);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceItem);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceActors);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceActor);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceFlowBindings);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_traceFlow);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_tracePlayer);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ResolvedLevelPlanProbe_tracePresentation);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
