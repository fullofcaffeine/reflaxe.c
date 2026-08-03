#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent observer for the Haxe-authored package proof.
 *
 * C is technically necessary here because this lane must prove that a native
 * consumer can read the generated globals without knowing Haxe layouts. Its
 * durable value is that ABI observation; package parsing, path handling,
 * hashing, and expected values remain entirely in Haxe and authored data.
 */
int hxc_generated_main(void);

int main(void)
{
	int generated_status = hxc_generated_main();
	int32_t check = hxc_caxecraft_qa_ContentPackageManifestProbe_observed;

	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageManifestProbe_traceIdentity);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageManifestProbe_traceBytes);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageManifestProbe_traceKinds);
	return generated_status == 0 && check == INT32_C(0)
	       && hxc_caxecraft_qa_ContentPackageManifestProbe_traceIdentity
	              == INT32_C(105150)
	       && hxc_caxecraft_qa_ContentPackageManifestProbe_traceBytes
	              == INT32_C(9486319)
	       && hxc_caxecraft_qa_ContentPackageManifestProbe_traceKinds
	              == INT32_C(127)
	           ? 0
	           : 1;
}
