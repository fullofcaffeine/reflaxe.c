#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored terrain-cache rules.
 * It prints the shared result only; chunk discovery and edits remain in Haxe.
 */
int main(void)
{
	const int32_t check = hxc_caxecraft_qa_TerrainChunkProbe_selfCheck();
	(void)printf("%" PRId32 "\n", check);
	return check == INT32_C(0) ? 0 : 1;
}
