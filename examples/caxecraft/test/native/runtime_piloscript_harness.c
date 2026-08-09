#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * The generated program owns every parser and observation check. This small
 * C consumer proves only that an external native caller can read the result.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t observed = hxc_caxecraft_qa_RuntimePilotScriptProbe_observed;
	(void)printf("%" PRId32 "\n", observed);
	return generated_status == 0 && observed == INT32_C(0) ? 0 : 1;
}
