#include "hxrt/bytes.h"

#include <stdio.h>

int main(void) {
  hxc_bytes_ref *bytes = NULL;
  int32_t value = 0;

  if (hxc_bytes_ref_create_zeroed(hxc_default_allocator(), 2, &bytes) != HXC_STATUS_OK
    || hxc_bytes_ref_set(bytes, 0, 0x141) != HXC_STATUS_OK
    || hxc_bytes_ref_get(bytes, 0, &value) != HXC_STATUS_OK
    || value != 0x41
    || hxc_bytes_ref_release(bytes) != HXC_STATUS_OK) {
    return 1;
  }
  (void)puts("runtime-feature-bytes: OK");
  return 0;
}
