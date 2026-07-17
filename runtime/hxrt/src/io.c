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
  if (value.byte_length > 0u &&
      fwrite(value.data, 1u, value.byte_length, stdout) != value.byte_length) {
    return HXC_STATUS_IO_ERROR;
  }
  if (fputc('\n', stdout) == EOF || fflush(stdout) == EOF) {
    return HXC_STATUS_IO_ERROR;
  }
  return HXC_STATUS_OK;
#endif
}
