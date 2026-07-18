/*
 * hxrt feature: status (compiler-selectable, dependency-only).
 *
 * Fallible runtime C boundaries share this closed value vocabulary. The hosted
 * io slice and native allocator/string/array seeds include it transitively; no
 * source operation selects status by itself. It stores no last error, allocates
 * nothing, has no lifetime or thread effects, and is internal ABI rather than
 * an application-export contract.
 */
#ifndef HXRT_STATUS_H_INCLUDED
#define HXRT_STATUS_H_INCLUDED

#include "hxrt/base.h"

typedef enum hxc_status {
  HXC_STATUS_OK = 0,
  HXC_STATUS_INVALID_ARGUMENT = 1,
  HXC_STATUS_OUT_OF_MEMORY = 2,
  HXC_STATUS_SIZE_OVERFLOW = 3,
  HXC_STATUS_UNSUPPORTED_ALIGNMENT = 4,
  HXC_STATUS_DIVISION_BY_ZERO = 5,
  HXC_STATUS_INVALID_UTF8 = 6,
  HXC_STATUS_OUT_OF_RANGE = 7,
  HXC_STATUS_EMBEDDED_NUL = 8,
  HXC_STATUS_BORROW_UNAVAILABLE = 9,
  HXC_STATUS_IO_ERROR = 10,
  HXC_STATUS_INTERNAL_ERROR = 255
} hxc_status;

#endif /* HXRT_STATUS_H_INCLUDED */
