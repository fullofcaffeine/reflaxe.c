#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored aquatic specification.
 * This fixture prints generated scalar results and contains no swimming rule.
 */
int main(void)
{
	const int32_t check = hxc_caxecraft_qa_AquaticsProbe_selfCheck();
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_AquaticsProbe_trace());
	return check == INT32_C(0) ? 0 : 1;
}
