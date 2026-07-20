#include "hxc/program.h"
#include "method_symbols.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static void print_trace(int32_t value)
{
	(void)printf("%" PRId32 "\n", value);
}

int main(void)
{
	print_trace(CAXECRAFT_SELF_CHECK());
	print_trace(CAXECRAFT_TERRAIN_TRACE());
	print_trace(CAXECRAFT_EDIT_TRACE());
	print_trace(CAXECRAFT_RAY_TRACE());
	print_trace(CAXECRAFT_COLLISION_TRACE());
	print_trace(CAXECRAFT_RUN_TRACE());
	for (int32_t seed = INT32_C(0); seed < INT32_C(32); ++seed) {
		print_trace(CAXECRAFT_PROPERTY_TRACE(seed));
	}
	return 0;
}
