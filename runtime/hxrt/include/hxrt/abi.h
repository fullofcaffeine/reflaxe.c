#ifndef HXRT_ABI_H_INCLUDED
#define HXRT_ABI_H_INCLUDED

#include "hxrt/base.h"

#define HXC_RUNTIME_ABI_MAJOR 0u
#define HXC_RUNTIME_ABI_MINOR 4u
#define HXC_RUNTIME_ABI_PATCH 0u
#define HXC_RUNTIME_ABI_VERSION \
  ((HXC_RUNTIME_ABI_MAJOR << 24u) | (HXC_RUNTIME_ABI_MINOR << 12u) | HXC_RUNTIME_ABI_PATCH)

#if defined(__cplusplus)
extern "C" {
#endif

/** Provisional native-seed ABI version; stabilization remains E4.T11. */
HXC_API uint32_t hxc_runtime_abi_version(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_ABI_H_INCLUDED */
