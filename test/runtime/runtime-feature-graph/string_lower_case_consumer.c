/* Independent C consumer for the isolated string-lower-case runtime package. */
#include "hxrt/string_lower_case.h"

#include <stdio.h>

int main(void) {
  const hxc_string source = HXC_STRING_LITERAL(
    "A\0\xC3\x84\xC4\xB0\xE1\xBA\x9E\xF0\x90\x90\x80"
  );
  const hxc_string expected = HXC_STRING_LITERAL(
    "a\0\xC3\xA4i\xC3\x9F\xF0\x90\x90\x80"
  );
  hxc_string lowered = HXC_STRING_INITIALIZER;
  int32_t order = 1;

  if (hxc_string_to_lower_case(
      source,
      hxc_default_allocator(),
      &lowered
    ) != HXC_STATUS_OK
    || hxc_string_compare(lowered, expected, &order) != HXC_STATUS_OK
    || order != 0
    || lowered.owner == NULL
    || hxc_string_release(&lowered) != HXC_STATUS_OK) {
    return 1;
  }
  (void)puts("runtime-feature-string-lower-case: OK");
  return 0;
}
