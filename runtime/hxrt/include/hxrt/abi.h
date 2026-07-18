/*
 * hxrt feature: runtime-abi (native-seed-only).
 *
 * The independent C/C++ smoke harness calls this query to compare the linked
 * runtime with the header contract. Generated Haxe cannot select it; generated
 * runtime-using C checks HXC_RUNTIME_ABI_MAJOR from base.h instead. The query is
 * pure, cannot fail, allocates nothing, and touches no mutable or thread state.
 */
#ifndef HXRT_ABI_H_INCLUDED
#define HXRT_ABI_H_INCLUDED

#include "hxrt/base.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Query the versioned internal ABI implemented by this runtime build. */
HXC_API uint32_t hxc_runtime_abi_version(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_ABI_H_INCLUDED */
