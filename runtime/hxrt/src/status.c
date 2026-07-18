/*
 * Implementation of native-seed-only feature `status-name`.
 *
 * Native smoke diagnostics call this total lookup. Generated Haxe does not
 * select it. Returned strings have static lifetime; there is no allocation,
 * failure, mutable state, thread state, or platform dependency.
 */
#include "hxrt/status_name.h"

const char *hxc_status_name(hxc_status status) {
  switch (status) {
    case HXC_STATUS_OK:
      return "HXC_STATUS_OK";
    case HXC_STATUS_INVALID_ARGUMENT:
      return "HXC_STATUS_INVALID_ARGUMENT";
    case HXC_STATUS_OUT_OF_MEMORY:
      return "HXC_STATUS_OUT_OF_MEMORY";
    case HXC_STATUS_SIZE_OVERFLOW:
      return "HXC_STATUS_SIZE_OVERFLOW";
    case HXC_STATUS_UNSUPPORTED_ALIGNMENT:
      return "HXC_STATUS_UNSUPPORTED_ALIGNMENT";
    case HXC_STATUS_DIVISION_BY_ZERO:
      return "HXC_STATUS_DIVISION_BY_ZERO";
    case HXC_STATUS_INVALID_UTF8:
      return "HXC_STATUS_INVALID_UTF8";
    case HXC_STATUS_OUT_OF_RANGE:
      return "HXC_STATUS_OUT_OF_RANGE";
    case HXC_STATUS_EMBEDDED_NUL:
      return "HXC_STATUS_EMBEDDED_NUL";
    case HXC_STATUS_BORROW_UNAVAILABLE:
      return "HXC_STATUS_BORROW_UNAVAILABLE";
    case HXC_STATUS_IO_ERROR:
      return "HXC_STATUS_IO_ERROR";
    case HXC_STATUS_INTERNAL_ERROR:
      return "HXC_STATUS_INTERNAL_ERROR";
    default:
      /* Preserve total diagnostics when an unknown future/foreign value arrives. */
      return "HXC_STATUS_UNKNOWN";
  }
}
