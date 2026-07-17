#ifndef HXRT_BASE_H_INCLUDED
#define HXRT_BASE_H_INCLUDED

/* Provisional internal runtime foundation; this is not a stable public ABI. */

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Internal ABI changes are compatible only within one major version. */
#define HXC_RUNTIME_ABI_MAJOR 0u
#define HXC_RUNTIME_ABI_MINOR 5u
#define HXC_RUNTIME_ABI_PATCH 0u
#define HXC_RUNTIME_ABI_VERSION \
  ((HXC_RUNTIME_ABI_MAJOR << 24u) | (HXC_RUNTIME_ABI_MINOR << 12u) | HXC_RUNTIME_ABI_PATCH)

#if defined(_WIN32) || defined(__CYGWIN__)
  #if defined(HXC_RUNTIME_BUILD_SHARED)
    #define HXC_API __declspec(dllexport)
  #elif defined(HXC_RUNTIME_USE_SHARED)
    #define HXC_API __declspec(dllimport)
  #else
    #define HXC_API
  #endif
#elif defined(__GNUC__) || defined(__clang__)
  #define HXC_API __attribute__((visibility("default")))
#else
  #define HXC_API
#endif

#if defined(__cplusplus)
  #define HXC_ALIGNOF(T) alignof(T)
#else
  #define HXC_ALIGNOF(T) _Alignof(T)
#endif

#endif /* HXRT_BASE_H_INCLUDED */
