/*
 * Implementation of compiler-selectable hosted feature `io`.
 *
 * Generated sys-println-literal/trace-literal calls and the native package
 * harness use hxc_io_println. The function borrows its private hxc_string for
 * one call, allocates nothing, and maps write/flush failures to hxc_status. The
 * planner admits it only for hosted programs; no runtime-owned thread state is
 * introduced beyond the host C stdout stream.
 */
#include "hxrt/io.h"

#if !defined(HXC_FREESTANDING)
#include <stdio.h>
#endif

hxc_status hxc_io_println(hxc_string value) {
  if (value.data == NULL && value.byte_length != 0u) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }

#if defined(HXC_FREESTANDING)
  (void)value;
  return HXC_STATUS_IO_ERROR;
#else
  /* fwrite uses the semantic byte length; puts/fputs would truncate at NUL. */
  if (value.byte_length > 0u &&
      fwrite(value.data, 1u, value.byte_length, stdout) != value.byte_length) {
    return HXC_STATUS_IO_ERROR;
  }
  /* Success includes newline and flush so deferred stream errors are observed. */
  if (fputc('\n', stdout) == EOF || fflush(stdout) == EOF) {
    return HXC_STATUS_IO_ERROR;
  }
  return HXC_STATUS_OK;
#endif
}
