#include "hxrt/allocator.h"

#include <stdio.h>

int main(void) {
  hxc_allocator allocator = hxc_default_allocator();
  void *memory = NULL;

  if (hxc_runtime_abi_version() != HXC_RUNTIME_ABI_VERSION) {
    return 1;
  }
  if (hxc_alloc(&allocator, 8u, HXC_ALIGNOF(max_align_t), &memory) != HXC_STATUS_OK) {
    return 2;
  }
  if (hxc_free(&allocator, memory, 8u, HXC_ALIGNOF(max_align_t)) != HXC_STATUS_OK) {
    return 3;
  }
  (void)puts("runtime-feature-alloc: OK");
  return 0;
}
