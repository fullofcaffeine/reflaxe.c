#include "hxrt/allocator.h"

#include <stddef.h>

void hxc_test_c_allocator_layout(size_t *values);

_Static_assert(offsetof(hxc_allocator, context) == 0u, "allocator context must lead");
_Static_assert(
  offsetof(hxc_allocator, context) < offsetof(hxc_allocator, allocate),
  "allocator fields must retain declaration order"
);
_Static_assert(
  offsetof(hxc_allocator, allocate) < offsetof(hxc_allocator, reallocate),
  "allocator fields must retain declaration order"
);
_Static_assert(
  offsetof(hxc_allocator, reallocate) < offsetof(hxc_allocator, release),
  "allocator fields must retain declaration order"
);
_Static_assert(offsetof(hxc_allocation, memory) == 0u, "allocation memory must lead");
_Static_assert(
  offsetof(hxc_allocation, memory) < offsetof(hxc_allocation, size),
  "allocation fields must retain declaration order"
);
_Static_assert(
  offsetof(hxc_allocation, size) < offsetof(hxc_allocation, alignment),
  "allocation fields must retain declaration order"
);
_Static_assert(
  offsetof(hxc_allocation, alignment) < offsetof(hxc_allocation, allocator),
  "allocation fields must retain declaration order"
);

void hxc_test_c_allocator_layout(size_t *values) {
  values[0] = sizeof(hxc_allocator);
  values[1] = HXC_ALIGNOF(hxc_allocator);
  values[2] = offsetof(hxc_allocator, context);
  values[3] = offsetof(hxc_allocator, allocate);
  values[4] = offsetof(hxc_allocator, reallocate);
  values[5] = offsetof(hxc_allocator, release);
  values[6] = sizeof(hxc_allocation);
  values[7] = HXC_ALIGNOF(hxc_allocation);
  values[8] = offsetof(hxc_allocation, memory);
  values[9] = offsetof(hxc_allocation, size);
  values[10] = offsetof(hxc_allocation, alignment);
  values[11] = offsetof(hxc_allocation, allocator);
}
