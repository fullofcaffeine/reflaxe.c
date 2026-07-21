#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored water specification.
 * This fixture calls generated public functions; it does not implement any
 * water rule or patch the generated C program.
 */
int main(void)
{
	const int32_t check = hxc_caxecraft_qa_WaterProbe_selfCheck();
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_WaterProbe_trace());
	return check == INT32_C(0) ? 0 : 1;
}
