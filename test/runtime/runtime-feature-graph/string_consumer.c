#include "hxrt/string.h"

#include <stdio.h>

int main(void) {
  hxc_allocator allocator = hxc_default_allocator();
  hxc_owned_string owned = {0};
  const hxc_string_view source = hxc_string_view_from_cstr("typed-runtime-plan");

  if (hxc_string_copy(source, &allocator, &owned) != HXC_STATUS_OK) {
    return 1;
  }
  if (owned.length != 18u || owned.data[owned.length] != UINT8_C(0)) {
    hxc_owned_string_dispose(&owned);
    return 2;
  }
  hxc_owned_string_dispose(&owned);
  (void)puts("runtime-feature-string: OK");
  return 0;
}
