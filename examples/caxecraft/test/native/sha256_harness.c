#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * This native observer owns only the generated entry point and scalar ABI.
 * SHA-256 behavior and test inputs remain in ordinary Haxe; Python hashlib
 * independently checks the printed trace.
 */
int hxc_generated_main(void);

int main(void)
{
	int generated_status = hxc_generated_main();
	int32_t check = hxc_caxecraft_qa_Sha256Probe_observed;

	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_Sha256Probe_digestTrace);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_Sha256Probe_digestCases);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
