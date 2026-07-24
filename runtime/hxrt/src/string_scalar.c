/*
 * Implementation of compiler-selectable feature `string-scalar`.
 *
 * These operations inspect immutable valid-UTF-8 views without allocation.
 * Checked helpers serve native contracts; hxc_string_char_at implements Haxe's
 * total, scalar-indexed character access and returns a borrow into the source.
 */
#include "hxrt/string_decode.h"

hxc_status hxc_utf8_validate(
  hxc_byte_view source,
  size_t *out_scalar_length
) {
  size_t byte_index = 0u;
  size_t scalar_length = 0u;
  hxc_utf8_step step;
  if (out_scalar_length == NULL || !hxc_byte_view_has_valid_shape(source)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  while (byte_index < source.length) {
    step = hxc_utf8_read(source.data + byte_index, source.length - byte_index);
    if (!step.valid) {
      return HXC_STATUS_INVALID_UTF8;
    }
    byte_index += step.consumed;
    scalar_length++;
  }
  *out_scalar_length = scalar_length;
  return HXC_STATUS_OK;
}

bool hxc_string_is_valid(hxc_string value) {
  hxc_byte_view view;
  size_t scalar_length;
  if (value.data == NULL) {
    return false;
  }
  if (value.has_trailing_nul
    && (value.data == NULL || value.data[value.byte_length] != UINT8_C(0))) {
    return false;
  }
  view.data = value.data;
  view.length = value.byte_length;
  return hxc_utf8_validate(view, &scalar_length) == HXC_STATUS_OK;
}

hxc_status hxc_string_scalar_length(
  hxc_string value,
  size_t *out_length
) {
  hxc_byte_view view;
  if (out_length == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(value)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  view.data = value.data;
  view.length = value.byte_length;
  return hxc_utf8_validate(view, out_length);
}

hxc_status hxc_string_scalar_at(
  hxc_string value,
  size_t scalar_index,
  uint32_t *out_scalar
) {
  size_t byte_index = 0u;
  size_t index = 0u;
  hxc_utf8_step step;
  if (out_scalar == NULL || !hxc_string_is_valid(value)) {
    return out_scalar == NULL
      ? HXC_STATUS_INVALID_ARGUMENT
      : HXC_STATUS_INVALID_UTF8;
  }
  while (byte_index < value.byte_length) {
    step = hxc_utf8_read(value.data + byte_index, value.byte_length - byte_index);
    if (index == scalar_index) {
      *out_scalar = step.scalar;
      return HXC_STATUS_OK;
    }
    byte_index += step.consumed;
    index++;
  }
  return HXC_STATUS_OUT_OF_RANGE;
}

hxc_status hxc_string_slice(
  hxc_string source,
  size_t scalar_start,
  size_t scalar_length,
  hxc_string *out_slice
) {
  size_t byte_index = 0u;
  size_t index = 0u;
  size_t slice_start;
  size_t consumed = 0u;
  hxc_utf8_step step;
  hxc_string result = HXC_STRING_EMPTY_INITIALIZER;
  if (out_slice == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(source)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  while (index < scalar_start && byte_index < source.byte_length) {
    step = hxc_utf8_read(
      source.data + byte_index,
      source.byte_length - byte_index
    );
    byte_index += step.consumed;
    index++;
  }
  if (index != scalar_start) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  slice_start = byte_index;
  while (consumed < scalar_length && byte_index < source.byte_length) {
    step = hxc_utf8_read(
      source.data + byte_index,
      source.byte_length - byte_index
    );
    byte_index += step.consumed;
    consumed++;
  }
  if (consumed != scalar_length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  result.data = source.data == NULL ? NULL : source.data + slice_start;
  result.byte_length = byte_index - slice_start;
  result.has_trailing_nul = source.has_trailing_nul
    && byte_index == source.byte_length;
  *out_slice = result;
  return HXC_STATUS_OK;
}

hxc_string hxc_string_char_at(
  hxc_string source,
  int32_t scalar_index
) {
  hxc_string result = HXC_STRING_EMPTY_INITIALIZER;
  if (scalar_index < 0) {
    return result;
  }
  if (hxc_string_slice(source, (size_t)scalar_index, 1u, &result)
    != HXC_STATUS_OK) {
    return result;
  }
  return result;
}

hxc_status hxc_string_compare(
  hxc_string left,
  hxc_string right,
  int32_t *out_order
) {
  size_t common;
  size_t index;
  if (out_order == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(left) || !hxc_string_is_valid(right)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  common = left.byte_length < right.byte_length
    ? left.byte_length
    : right.byte_length;
  for (index = 0u; index < common; index++) {
    if (left.data[index] < right.data[index]) {
      *out_order = -1;
      return HXC_STATUS_OK;
    }
    if (left.data[index] > right.data[index]) {
      *out_order = 1;
      return HXC_STATUS_OK;
    }
  }
  if (left.byte_length < right.byte_length) {
    *out_order = -1;
  } else if (left.byte_length > right.byte_length) {
    *out_order = 1;
  } else {
    *out_order = 0;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_string_hash(
  hxc_string value,
  uint32_t *out_hash
) {
  uint32_t hash = UINT32_C(2166136261);
  size_t index;
  if (out_hash == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(value)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  for (index = 0u; index < value.byte_length; index++) {
    hash ^= (uint32_t)value.data[index];
    hash *= UINT32_C(16777619);
  }
  *out_hash = hash;
  return HXC_STATUS_OK;
}
