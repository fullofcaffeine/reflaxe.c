#include "hxrt/allocator.h"

#include <stdio.h>

int main(void) {
  hxc_allocator allocator = hxc_default_allocator();
  hxc_allocation allocation = HXC_ALLOCATION_INITIALIZER;

  if (hxc_allocation_allocate(
      &allocator,
      2u,
      4u,
      HXC_ALIGNOF(max_align_t),
      &allocation
    ) != HXC_STATUS_OK) {
    return 1;
  }
  if (!hxc_allocation_is_valid(&allocation)
    || !hxc_allocator_same_identity(&allocator, &allocation.allocator)) {
    return 2;
  }
  if (hxc_allocation_resize(&allocation, 4u, 4u) != HXC_STATUS_OK) {
    return 3;
  }
  if (hxc_allocation_dispose(&allocation) != HXC_STATUS_OK) {
    return 4;
  }
  (void)puts("runtime-feature-alloc: OK");
  return 0;
}
