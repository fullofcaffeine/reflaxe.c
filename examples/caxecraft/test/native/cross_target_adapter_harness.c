#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the three Haxe-authored adapter strategies.
 * The harness reports only their shared result; it owns no normalization rule.
 */
int main(void)
{
	const int32_t check = hxc_caxecraft_adapterprobe_AdapterArchitectureProbe_selfCheck();
	(void)printf("%" PRId32 "\n", check);
	return check == INT32_C(0) ? 0 : 1;
}
