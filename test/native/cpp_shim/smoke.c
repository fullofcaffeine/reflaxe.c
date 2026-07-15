#include "counter_shim.h"

#include <stdio.h>

int main(void) {
  hxc_fixture_counter *counter = hxc_fixture_counter_create(40);
  int32_t value = 0;
  if (counter == NULL) {
    return 1;
  }
  if (!hxc_fixture_counter_add(counter, 2)) {
    hxc_fixture_counter_destroy(counter);
    return 1;
  }
  if (!hxc_fixture_counter_value(counter, &value) || value != 42) {
    hxc_fixture_counter_destroy(counter);
    return 1;
  }
  hxc_fixture_counter_destroy(counter);
  (void)puts("cpp-shim-smoke: OK");
  return 0;
}
