#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent ABI observer for the Haxe-authored ZIP package source.
 *
 * C is technically necessary because this lane must prove that a native
 * consumer can observe generated results without knowing Haxe layouts. Its
 * durable value is that ABI proof; ZIP records, CRC-32, path policy, package
 * semantics, and all expected values remain in Haxe and reviewed fixtures.
 */
int hxc_generated_main(void);

int main(void)
{
	int generated_status = hxc_generated_main();
	int32_t check = hxc_caxecraft_qa_ContentPackageZipSourceProbe_observed;

	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageZipSourceProbe_traceIdentity);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageZipSourceProbe_traceBytes);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageZipSourceProbe_traceRejections);
	return generated_status == 0 && check == INT32_C(0)
	       && hxc_caxecraft_qa_ContentPackageZipSourceProbe_traceIdentity
	              == INT32_C(105070)
	       && hxc_caxecraft_qa_ContentPackageZipSourceProbe_traceBytes
	              == INT32_C(42943)
	       && hxc_caxecraft_qa_ContentPackageZipSourceProbe_traceRejections
	              == INT32_C(36)
	           ? 0
	           : 1;
}
