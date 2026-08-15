/*
 * Implementation of compiler-selectable hosted feature `string-float`.
 *
 * Haxe Eval's Numeric.float_repres is the independent semantic oracle. It
 * tries 12, 15, and then 18 significant digits and accepts the first decimal
 * that parses back to the same binary64 value. This implementation follows
 * that observable rule while normalizing the host locale's decimal separator.
 */
#include "hxrt/string_float.h"

#if !defined(HXC_FREESTANDING)
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#if !defined(HXC_FREESTANDING)
enum { HXC_FLOAT_TEXT_CAPACITY = 64 };

static hxc_status hxc_float_text_copy(
  const char *text,
  size_t length,
  hxc_allocator allocator,
  hxc_string *out_string
) {
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_byte_view view;
  hxc_status status = hxc_string_buffer_init(&allocator, &buffer);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  view.data = (const uint8_t *)text;
  view.length = length;
  status = hxc_string_buffer_append_utf8_checked(&buffer, view);
  if (status == HXC_STATUS_OK) {
    status = hxc_string_buffer_finish_ref(&buffer, out_string);
  }
  if (status != HXC_STATUS_OK) {
    (void)hxc_string_buffer_dispose(&buffer);
  }
  return status;
}

static size_t hxc_float_normalize_decimal(char *text, size_t length) {
  const struct lconv *conventions = localeconv();
  const char *decimal = conventions == NULL ? NULL : conventions->decimal_point;
  size_t decimal_length;
  char *position;
  size_t suffix_length;
  if (decimal == NULL || decimal[0] == '\0' || (decimal[0] == '.' && decimal[1] == '\0')) {
    return length;
  }
  decimal_length = strlen(decimal);
  position = strstr(text, decimal);
  if (position == NULL) {
    return length;
  }
  suffix_length = length - (size_t)(position - text) - decimal_length;
  position[0] = '.';
  if (decimal_length != 1u) {
    memmove(position + 1, position + decimal_length, suffix_length + 1u);
    length = length - decimal_length + 1u;
  }
  return length;
}
#endif

hxc_status hxc_string_from_float64(
  double value,
  hxc_allocator allocator,
  hxc_string *out_string
) {
#if defined(HXC_FREESTANDING)
  (void)value;
  (void)allocator;
  (void)out_string;
  return HXC_STATUS_INVALID_ARGUMENT;
#else
  static const int precisions[3] = { 12, 15, 18 };
  char text[HXC_FLOAT_TEXT_CAPACITY];
  size_t length;
  size_t index;
  int written;
  if (isnan(value)) {
    return hxc_float_text_copy("nan", 3u, allocator, out_string);
  }
  if (isinf(value)) {
    return signbit(value)
      ? hxc_float_text_copy("neg_infinity", 12u, allocator, out_string)
      : hxc_float_text_copy("infinity", 8u, allocator, out_string);
  }
  for (index = 0u; index < 3u; index++) {
    char *end = NULL;
    double parsed;
    written = snprintf(text, sizeof(text), "%.*g", precisions[index], value);
    if (written < 0 || (size_t)written >= sizeof(text)) {
      return HXC_STATUS_SIZE_OVERFLOW;
    }
    parsed = strtod(text, &end);
    if (end != NULL && *end == '\0' && parsed == value) {
      length = hxc_float_normalize_decimal(text, (size_t)written);
      return hxc_float_text_copy(text, length, allocator, out_string);
    }
  }
  return HXC_STATUS_INVALID_ARGUMENT;
#endif
}
