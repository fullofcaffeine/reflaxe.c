#include "mutable_text.h"

#include <stddef.h>

/*
 * This small function is intentionally handwritten C, not haxe.c output.
 * It is the independent foreign library that the generated fixture consumes:
 * compiling both sides through haxe.c would only prove that the compiler agrees
 * with itself about the ABI. Its checks make a wrong pointer or capacity visible
 * under ordinary execution and the sanitizer lane.
 */
bool fixture_replace_mutable_text(char *text, int32_t capacity) {
  static const char replacement[] = "Caxe!";
  size_t index;
  if (text == NULL
    || capacity < (int32_t)sizeof(replacement)
    || text[0] != 'H'
    || text[1] != 'a'
    || text[2] != 'x'
    || text[3] != 'e'
    || text[4] != '\0') {
    return false;
  }
  for (index = 0u; index < sizeof(replacement); index++) {
    text[index] = replacement[index];
  }
  return true;
}
