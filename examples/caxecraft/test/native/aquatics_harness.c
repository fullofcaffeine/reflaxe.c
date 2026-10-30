#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored aquatic specification.
 * This fixture prints generated scalar results and contains no swimming rule.
 * The probe's session uses collector-managed CaxeFlow state, so the generated
 * main must initialize the runtime before the probe runs; the probe stores its
 * results in statics that this observer reads afterwards.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_AquaticsProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_AquaticsProbe_traceObserved);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
