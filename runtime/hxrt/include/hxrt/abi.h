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
