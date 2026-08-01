#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * This C file is justified twice and owns no JSON or content behavior.
 *
 * Technical necessity: an external strict C compiler must execute the emitted
 * lifecycle and observe Haxe-owned results; haxe.c cannot independently verify
 * its own output.
 *
 * Durable ownership value: this tiny consumer also proves that the parser's
 * QA envelope remains ordinary fixed-width C data. Remove it if another
 * independent consumer permanently owns both checks.
 */
int hxc_generated_main(void);

int main(void)
{
	const int generated_status = hxc_generated_main();
	const int32_t check = hxc_caxecraft_qa_ContentJsonProbe_observed;
	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n", hxc_caxecraft_qa_ContentJsonProbe_traceTree);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentJsonProbe_traceDuplicateLine);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentJsonProbe_traceUtf8Offset);
	return generated_status == 0 && check == INT32_C(0) ? 0 : 1;
}
