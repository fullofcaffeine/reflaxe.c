#include "text_observer.h"

#include <stddef.h>
#include <stdint.h>

static bool bytes_equal(const char *actual, const uint8_t *expected, size_t length) {
  size_t index;
  if (actual == NULL) {
    return false;
  }
  for (index = 0u; index < length; index++) {
    if ((uint8_t)actual[index] != expected[index]) {
      return false;
    }
  }
  return actual[length] == '\0';
}

bool fixture_text_matches(const char *text, int32_t case_id) {
  static const uint8_t empty[] = {0u};
  static const uint8_t ascii[] = {'H', 'a', 'x', 'e', 0u};
  static const uint8_t unicode[] = {'A', 0xC3u, 0xA9u, 0xF0u, 0x9Fu, 0x98u, 0x80u, 0u};
  static const uint8_t suffix[] = {0xC3u, 0xA9u, 0xF0u, 0x9Fu, 0x98u, 0x80u, 0u};
  switch (case_id) {
    case 0:
      return bytes_equal(text, empty, sizeof(empty) - 1u);
    case 1:
      return bytes_equal(text, ascii, sizeof(ascii) - 1u);
    case 2:
      return bytes_equal(text, unicode, sizeof(unicode) - 1u);
    case 3:
      return bytes_equal(text, suffix, sizeof(suffix) - 1u);
    default:
      return false;
  }
}
