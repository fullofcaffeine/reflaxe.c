#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored session specification.
 * Assembly and validation remain in shared Haxe; this fixture prints scalars.
 */
int main(void)
{
	const int32_t check = hxc_caxecraft_qa_GameSessionProbe_selfCheck();
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_GameSessionProbe_trace());
	return check == INT32_C(0) ? 0 : 1;
}
