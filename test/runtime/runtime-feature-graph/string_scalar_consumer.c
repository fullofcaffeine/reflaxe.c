/*
 * Independent C consumer for the isolated string-scalar runtime package.
 *
 * This file is intentionally C rather than generated Haxe: it proves that the
 * packaged runtime slice has a complete standalone C contract. The neighboring
 * string-char-at differential suite separately proves that ordinary Haxe reaches
 * this same API through haxe.c, so the compiler is not being used as its own
 * reference implementation here.
 */
#include "hxrt/string_scalar.h"

#include <stdio.h>

int main(void) {
  const hxc_string source = HXC_STRING_LITERAL("A\xF0\x9F\x99\x82Z");
  const hxc_string emoji = hxc_string_char_at(source, 1);
  const hxc_string missing = hxc_string_char_at(source, -1);

  if (emoji.byte_length != 4u
    || emoji.data == NULL
    || emoji.data[0] != UINT8_C(0xF0)
    || missing.byte_length != 0u) {
    return 1;
  }
  (void)puts("runtime-feature-string-scalar: OK");
  return 0;
}
