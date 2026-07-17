#include "hxrt/string.h"

#include <stdio.h>

int main(void) {
  hxc_allocator allocator = hxc_default_allocator();
  hxc_owned_string owned = HXC_OWNED_STRING_INITIALIZER;
  const hxc_string source = HXC_STRING_LITERAL("typed-runtime-plan");

  if (hxc_string_copy(source, &allocator, &owned) != HXC_STATUS_OK) {
    return 1;
  }
  if (owned.value.byte_length != 18u
    || owned.value.data[owned.value.byte_length] != UINT8_C(0)) {
    (void)hxc_owned_string_dispose(&owned);
    return 2;
  }
  if (hxc_owned_string_dispose(&owned) != HXC_STATUS_OK) {
    return 3;
  }
  (void)puts("runtime-feature-string: OK");
  return 0;
}
