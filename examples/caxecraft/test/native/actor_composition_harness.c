#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored actor-plan specification.
 * Content resolution and assertions stay in shared Haxe; this C consumer only
 * prints the exported scalar result from the generated project.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_ActorCompositionProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
