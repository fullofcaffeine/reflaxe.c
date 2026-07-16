#ifndef HXRT_STATUS_H_INCLUDED
#define HXRT_STATUS_H_INCLUDED

#include "hxrt/abi.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum hxc_status {
  HXC_STATUS_OK = 0,
  HXC_STATUS_INVALID_ARGUMENT = 1,
  HXC_STATUS_OUT_OF_MEMORY = 2,
  HXC_STATUS_SIZE_OVERFLOW = 3,
  HXC_STATUS_UNSUPPORTED_ALIGNMENT = 4,
  HXC_STATUS_DIVISION_BY_ZERO = 5,
  HXC_STATUS_INTERNAL_ERROR = 255
} hxc_status;

/** Stable symbolic status name; never returns null. */
HXC_API const char *hxc_status_name(hxc_status status);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STATUS_H_INCLUDED */
