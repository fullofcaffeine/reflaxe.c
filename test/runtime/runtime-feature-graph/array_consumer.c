#include "hxrt/array.h"

#include <stdio.h>

int main(void) {
  hxc_allocator allocator = hxc_default_allocator();
  hxc_array_element_ops elements;
  hxc_array array = HXC_ARRAY_INITIALIZER;
  int32_t value = 42;
  const void *stored = NULL;

  elements.size = sizeof(value);
  elements.alignment = HXC_ALIGNOF(int32_t);
  elements.context = NULL;
  elements.copy = NULL;
  elements.assign = NULL;
  elements.destroy = NULL;
  if (hxc_array_init(&allocator, elements, &array) != HXC_STATUS_OK) {
    return 1;
  }
  if (hxc_array_push_copy(&array, &value) != HXC_STATUS_OK
    || hxc_array_at_const(&array, 0u, &stored) != HXC_STATUS_OK
    || *(const int32_t *)stored != value) {
    (void)hxc_array_dispose(&array);
    return 2;
  }
  if (hxc_array_dispose(&array) != HXC_STATUS_OK) {
    return 3;
  }
  (void)puts("runtime-feature-array: OK");
  return 0;
}
