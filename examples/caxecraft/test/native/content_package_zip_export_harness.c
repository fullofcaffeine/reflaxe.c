#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored canonical ZIP exporter.
 *
 * C is necessary only to prove a non-Haxe caller can observe the generated
 * result envelope. Package selection, receipts, CRC-32, ZIP records, and every
 * expected semantic value remain owned by Haxe and independently checked data.
 */
int hxc_generated_main(void);

int main(void)
{
	int generated_status = hxc_generated_main();
	int32_t check = hxc_caxecraft_qa_ContentPackageZipExportProbe_observed;

	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageZipExportProbe_traceIdentity);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageZipExportProbe_tracePayloadBytes);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageZipExportProbe_traceArchiveBytes);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageZipExportProbe_traceArchiveFingerprint);
	return generated_status == 0 && check == INT32_C(0)
	       && hxc_caxecraft_qa_ContentPackageZipExportProbe_traceIdentity
	              > INT32_C(0)
	       && hxc_caxecraft_qa_ContentPackageZipExportProbe_tracePayloadBytes
	              > INT32_C(0)
	       && hxc_caxecraft_qa_ContentPackageZipExportProbe_traceArchiveBytes
	              > hxc_caxecraft_qa_ContentPackageZipExportProbe_tracePayloadBytes
	           ? 0
	           : 1;
}
