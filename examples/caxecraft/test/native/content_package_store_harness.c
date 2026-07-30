#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/*
 * Independent ABI observer for the Haxe-authored package store.
 *
 * Repository behavior—including fault injection and descriptor cleanup—stays
 * in Haxe. This handwritten C is retained for two separate reasons:
 *
 * 1. The Haxe POSIX declarations select exact carriers for header-owned scalar
 *    types. The native compiler must independently reject a host whose ABI
 *    widths or signedness do not match that selection.
 * 2. A non-haxe.c consumer must prove that the exported Haxe result envelope is
 *    ordinary C ABI data and that generated main returns normally.
 *
 * It contains no path, file, retry, cleanup, or expected-content algorithm.
 */
_Static_assert(sizeof(size_t) == sizeof(uint64_t),
               "the admitted POSIX package target requires 64-bit size_t");
_Static_assert(sizeof(ssize_t) == sizeof(int64_t),
               "the admitted POSIX package target requires 64-bit ssize_t");
_Static_assert(sizeof(ino_t) == sizeof(uint64_t),
               "the admitted POSIX package target requires 64-bit ino_t");
_Static_assert(((ino_t)-1 > (ino_t)0),
               "the admitted POSIX package target requires unsigned ino_t");
_Static_assert(sizeof(off_t) == sizeof(int64_t),
               "the admitted POSIX package target requires 64-bit off_t");
_Static_assert(((off_t)-1 < (off_t)0),
               "the admitted POSIX package target requires signed off_t");
_Static_assert(sizeof(time_t) == sizeof(intptr_t),
               "the admitted POSIX package target requires pointer-width time_t");
_Static_assert(((time_t)-1 < (time_t)0),
               "the admitted POSIX package target requires signed time_t");

#if defined(__APPLE__)
_Static_assert(sizeof(dev_t) == sizeof(int32_t),
               "the admitted Darwin package target requires 32-bit dev_t");
_Static_assert(((dev_t)-1 < (dev_t)0),
               "the admitted Darwin package target requires signed dev_t");
_Static_assert(sizeof(mode_t) == sizeof(uint16_t),
               "the admitted Darwin package target requires 16-bit mode_t");
#else
_Static_assert(sizeof(dev_t) == sizeof(uint64_t),
               "the admitted POSIX package target requires 64-bit dev_t");
_Static_assert(((dev_t)-1 > (dev_t)0),
               "the admitted POSIX package target requires unsigned dev_t");
_Static_assert(sizeof(mode_t) == sizeof(uint32_t),
               "the admitted POSIX package target requires 32-bit mode_t");
#endif
_Static_assert(((mode_t)-1 > (mode_t)0),
               "the admitted POSIX package target requires unsigned mode_t");

int hxc_generated_main(void);

int main(void)
{
	int generated_status = hxc_generated_main();
	int32_t check = hxc_caxecraft_qa_ContentPackageStoreProbe_observed;

	(void)printf("%" PRId32 "\n", check);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageStoreProbe_traceLength);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageStoreProbe_traceFirstByte);
	(void)printf("%" PRId32 "\n",
	             hxc_caxecraft_qa_ContentPackageStoreProbe_traceLastByte);
	return generated_status == 0
		&& check == INT32_C(0)
		&& hxc_caxecraft_qa_ContentPackageStoreProbe_traceAttempts
			== INT32_C(2)
		? 0
		: 1;
}
