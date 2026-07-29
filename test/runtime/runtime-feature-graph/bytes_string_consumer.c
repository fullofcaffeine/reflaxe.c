/*
 * Native consumer for the independently selectable `bytes-string` feature.
 *
 * The ordinary Bytes differential suite owns the detailed language semantics.
 * This smaller program proves that the runtime graph packages enough of Bytes
 * and String to decode one UTF-8 range, without relying on the full umbrella.
 */
#include "hxrt/bytes_string.h"

#include <stdio.h>

int main(void) {
  hxc_bytes_ref *bytes = NULL;
  const hxc_string source = HXC_STRING_LITERAL("A\xC3\xA9");
  hxc_string decoded = HXC_STRING_INITIALIZER;

  if (hxc_bytes_ref_create_utf8_copy(
      hxc_default_allocator(),
      source,
      &bytes
    ) != HXC_STATUS_OK
    || hxc_bytes_ref_get_string_utf8(
      bytes,
      1,
      2,
      hxc_default_allocator(),
      &decoded
    ) != HXC_STATUS_OK
    || !hxc_string_is_valid(decoded)
    || decoded.byte_length != 2u
    || decoded.data[0] != (uint8_t)0xC3u
    || decoded.data[1] != (uint8_t)0xA9u
    || hxc_string_release(&decoded) != HXC_STATUS_OK
    || hxc_bytes_ref_release(bytes) != HXC_STATUS_OK) {
    return 1;
  }
  (void)puts("runtime-feature-bytes-string: OK");
  return 0;
}
