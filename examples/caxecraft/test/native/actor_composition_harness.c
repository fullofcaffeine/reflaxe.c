#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored actor-plan specification.
 * Content resolution and assertions stay in shared Haxe; this C consumer only
 * prints the exported scalar result from the generated project.
 */
int main(void)
{
	const int32_t check = hxc_caxecraft_qa_ActorCompositionProbe_selfCheck();
	(void)printf("%" PRId32 "\n", check);
	return check == INT32_C(0) ? 0 : 1;
}
