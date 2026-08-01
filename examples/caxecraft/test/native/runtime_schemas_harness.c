#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent runtime observer for the Haxe-authored schema tracer.
 *
 * Technical necessity: a strict external C compiler must execute the emitted
 * lifecycle and observe the Haxe-owned result envelope.
 *
 * Durable ownership value: this consumer proves that schema results cross the
 * generated program boundary as ordinary scalar C data. It owns no JSON,
 * content, localization, filesystem, ABI-width, or platform expectation.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_RuntimeSchemasProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeSchemasProbe_tracePack);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeSchemasProbe_traceUi);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_RuntimeSchemasProbe_traceDiagnosticLine);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
