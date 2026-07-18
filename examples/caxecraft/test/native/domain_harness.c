#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static void print_trace(int32_t value)
{
	(void)printf("%" PRId32 "\n", value);
}

int main(void)
{
	print_trace(hxc_method_caxecraft_qa_DomainProbe_selfCheck());
	print_trace(hxc_method_caxecraft_domain_CaxecraftTrace_terrainTrace());
	print_trace(hxc_method_caxecraft_domain_CaxecraftTrace_editTrace());
	print_trace(hxc_method_caxecraft_domain_CaxecraftTrace_rayTrace());
	print_trace(hxc_method_caxecraft_domain_CaxecraftTrace_collisionTrace());
	print_trace(hxc_method_caxecraft_domain_CaxecraftTrace_runTrace());
	for (int32_t seed = INT32_C(0); seed < INT32_C(32); ++seed) {
		print_trace(hxc_method_caxecraft_domain_CaxecraftTrace_propertyTrace(seed));
	}
	return 0;
}
