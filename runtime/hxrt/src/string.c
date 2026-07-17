#include "hxrt/string.h"

typedef struct hxc_utf8_step {
  size_t consumed;
  uint32_t scalar;
  bool valid;
} hxc_utf8_step;

static const uint8_t hxc_empty_string_storage[1] = { UINT8_C(0) };

static bool hxc_byte_view_has_valid_shape(hxc_byte_view source) {
  return source.data != NULL || source.length == 0u;
}

static bool hxc_is_continuation(uint8_t value) {
  return value >= UINT8_C(0x80) && value <= UINT8_C(0xBF);
}

static hxc_utf8_step hxc_invalid_step(size_t consumed) {
  hxc_utf8_step step;
  step.consumed = consumed;
  step.scalar = UINT32_C(0xFFFD);
  step.valid = false;
  return step;
}

static hxc_utf8_step hxc_valid_step(size_t consumed, uint32_t scalar) {
  hxc_utf8_step step;
  step.consumed = consumed;
  step.scalar = scalar;
  step.valid = true;
  return step;
}

static hxc_utf8_step hxc_utf8_read(const uint8_t *data, size_t length) {
  const uint8_t first = data[0];
  uint8_t second;
  uint8_t third;
  uint8_t fourth;
  uint32_t scalar;

  if (first <= UINT8_C(0x7F)) {
    return hxc_valid_step(1u, (uint32_t)first);
  }

  if (first >= UINT8_C(0xC2) && first <= UINT8_C(0xDF)) {
    if (length < 2u || !hxc_is_continuation(data[1])) {
      return hxc_invalid_step(1u);
    }
    second = data[1];
    scalar = ((uint32_t)(first & UINT8_C(0x1F)) << 6u)
      | (uint32_t)(second & UINT8_C(0x3F));
    return hxc_valid_step(2u, scalar);
  }

  if (first >= UINT8_C(0xE0) && first <= UINT8_C(0xEF)) {
    if (length < 2u) {
      return hxc_invalid_step(1u);
    }
    second = data[1];
    if ((first == UINT8_C(0xE0)
        && (second < UINT8_C(0xA0) || second > UINT8_C(0xBF)))
      || (first == UINT8_C(0xED)
        && (second < UINT8_C(0x80) || second > UINT8_C(0x9F)))
      || (first != UINT8_C(0xE0) && first != UINT8_C(0xED)
        && !hxc_is_continuation(second))) {
      return hxc_invalid_step(1u);
    }
    if (length < 3u || !hxc_is_continuation(data[2])) {
      return hxc_invalid_step(2u);
    }
    third = data[2];
    scalar = ((uint32_t)(first & UINT8_C(0x0F)) << 12u)
      | ((uint32_t)(second & UINT8_C(0x3F)) << 6u)
      | (uint32_t)(third & UINT8_C(0x3F));
    return hxc_valid_step(3u, scalar);
  }

  if (first >= UINT8_C(0xF0) && first <= UINT8_C(0xF4)) {
    if (length < 2u) {
      return hxc_invalid_step(1u);
    }
    second = data[1];
    if ((first == UINT8_C(0xF0)
        && (second < UINT8_C(0x90) || second > UINT8_C(0xBF)))
      || (first == UINT8_C(0xF4)
        && (second < UINT8_C(0x80) || second > UINT8_C(0x8F)))
      || (first != UINT8_C(0xF0) && first != UINT8_C(0xF4)
        && !hxc_is_continuation(second))) {
      return hxc_invalid_step(1u);
    }
    if (length < 3u || !hxc_is_continuation(data[2])) {
      return hxc_invalid_step(2u);
    }
    if (length < 4u || !hxc_is_continuation(data[3])) {
      return hxc_invalid_step(3u);
    }
    third = data[2];
    fourth = data[3];
    scalar = ((uint32_t)(first & UINT8_C(0x07)) << 18u)
      | ((uint32_t)(second & UINT8_C(0x3F)) << 12u)
      | ((uint32_t)(third & UINT8_C(0x3F)) << 6u)
      | (uint32_t)(fourth & UINT8_C(0x3F));
    return hxc_valid_step(4u, scalar);
  }

  return hxc_invalid_step(1u);
}

static size_t hxc_utf8_encode(uint32_t scalar, uint8_t output[4]) {
  if ((scalar >= UINT32_C(0xD800) && scalar <= UINT32_C(0xDFFF))
    || scalar > UINT32_C(0x10FFFF)) {
    scalar = UINT32_C(0xFFFD);
  }
  if (scalar <= UINT32_C(0x7F)) {
    output[0] = (uint8_t)scalar;
    return 1u;
  }
  if (scalar <= UINT32_C(0x7FF)) {
    output[0] = (uint8_t)(UINT32_C(0xC0) | (scalar >> 6u));
    output[1] = (uint8_t)(UINT32_C(0x80) | (scalar & UINT32_C(0x3F)));
    return 2u;
  }
  if (scalar <= UINT32_C(0xFFFF)) {
    output[0] = (uint8_t)(UINT32_C(0xE0) | (scalar >> 12u));
    output[1] = (uint8_t)(UINT32_C(0x80) | ((scalar >> 6u) & UINT32_C(0x3F)));
    output[2] = (uint8_t)(UINT32_C(0x80) | (scalar & UINT32_C(0x3F)));
    return 3u;
  }
  output[0] = (uint8_t)(UINT32_C(0xF0) | (scalar >> 18u));
  output[1] = (uint8_t)(UINT32_C(0x80) | ((scalar >> 12u) & UINT32_C(0x3F)));
  output[2] = (uint8_t)(UINT32_C(0x80) | ((scalar >> 6u) & UINT32_C(0x3F)));
  output[3] = (uint8_t)(UINT32_C(0x80) | (scalar & UINT32_C(0x3F)));
  return 4u;
}

static void hxc_copy_bytes(uint8_t *destination, const uint8_t *source, size_t length) {
  size_t index;
  for (index = 0u; index < length; index++) {
    destination[index] = source[index];
  }
}

static bool hxc_owned_string_slot_is_empty(const hxc_owned_string *value) {
  return value != NULL
    && value->value.data == NULL
    && value->value.byte_length == 0u
    && !value->value.has_trailing_nul
    && value->storage.memory == NULL
    && value->storage.size == 0u
    && value->storage.alignment == 0u
    && value->storage.allocator.context == NULL
    && value->storage.allocator.allocate == NULL
    && value->storage.allocator.reallocate == NULL
    && value->storage.allocator.release == NULL;
}

static void hxc_owned_string_clear(hxc_owned_string *value) {
  value->value.data = NULL;
  value->value.byte_length = 0u;
  value->value.has_trailing_nul = false;
  value->storage.memory = NULL;
  value->storage.size = 0u;
  value->storage.alignment = 0u;
  value->storage.allocator.context = NULL;
  value->storage.allocator.allocate = NULL;
  value->storage.allocator.reallocate = NULL;
  value->storage.allocator.release = NULL;
}

static bool hxc_owned_string_is_valid(const hxc_owned_string *value) {
  size_t required;
  if (value == NULL || !hxc_allocation_is_valid(&value->storage)) {
    return false;
  }
  if (value->storage.size == 0u) {
    return value->value.data == hxc_empty_string_storage
      && value->value.byte_length == 0u
      && value->value.has_trailing_nul
      && hxc_string_is_valid(value->value);
  }
  if (hxc_size_add(value->value.byte_length, 1u, &required) != HXC_STATUS_OK) {
    return false;
  }
  return value->value.data == (const uint8_t *)value->storage.memory
    && value->value.has_trailing_nul
    && value->storage.size >= required
    && value->value.data[value->value.byte_length] == UINT8_C(0)
    && hxc_string_is_valid(value->value);
}

static hxc_status hxc_owned_string_from_valid_bytes(
  const uint8_t *data,
  size_t length,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
) {
  hxc_owned_string value = HXC_OWNED_STRING_INITIALIZER;
  size_t allocation_size;
  uint8_t *destination;
  hxc_status status;

  if (!hxc_owned_string_slot_is_empty(out_string)
    || (data == NULL && length != 0u)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (length == 0u) {
    status = hxc_allocation_allocate(
      allocator,
      0u,
      1u,
      HXC_ALIGNOF(uint8_t),
      &value.storage
    );
    if (status != HXC_STATUS_OK) {
      return status;
    }
    value.value.data = hxc_empty_string_storage;
    value.value.byte_length = 0u;
    value.value.has_trailing_nul = true;
    *out_string = value;
    return HXC_STATUS_OK;
  }

  status = hxc_size_add(length, 1u, &allocation_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_allocate(
    allocator,
    allocation_size,
    1u,
    HXC_ALIGNOF(uint8_t),
    &value.storage
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  destination = (uint8_t *)value.storage.memory;
  hxc_copy_bytes(destination, data, length);
  destination[length] = UINT8_C(0);
  value.value.data = destination;
  value.value.byte_length = length;
  value.value.has_trailing_nul = true;
  *out_string = value;
  return HXC_STATUS_OK;
}

static bool hxc_string_contains_nul(hxc_string value) {
  size_t index;
  for (index = 0u; index < value.byte_length; index++) {
    if (value.data[index] == UINT8_C(0)) {
      return true;
    }
  }
  return false;
}

static bool hxc_string_buffer_slot_is_empty(const hxc_string_buffer *buffer) {
  return buffer != NULL
    && buffer->byte_length == 0u
    && buffer->storage.memory == NULL
    && buffer->storage.size == 0u
    && buffer->storage.alignment == 0u
    && buffer->storage.allocator.context == NULL
    && buffer->storage.allocator.allocate == NULL
    && buffer->storage.allocator.reallocate == NULL
    && buffer->storage.allocator.release == NULL;
}

static bool hxc_string_buffer_is_valid(const hxc_string_buffer *buffer) {
  hxc_byte_view view;
  size_t required;
  size_t scalar_length;
  if (buffer == NULL || !hxc_allocation_is_valid(&buffer->storage)) {
    return false;
  }
  if (buffer->storage.size == 0u) {
    return buffer->byte_length == 0u && buffer->storage.memory == NULL;
  }
  if (hxc_size_add(buffer->byte_length, 1u, &required) != HXC_STATUS_OK
    || required > buffer->storage.size) {
    return false;
  }
  view.data = (const uint8_t *)buffer->storage.memory;
  view.length = buffer->byte_length;
  return view.data[view.length] == UINT8_C(0)
    && hxc_utf8_validate(view, &scalar_length) == HXC_STATUS_OK;
}

static hxc_status hxc_string_buffer_reserve(
  hxc_string_buffer *buffer,
  size_t appended_length
) {
  size_t required_length;
  size_t required_storage;
  size_t new_size;
  hxc_status status;

  status = hxc_size_add(buffer->byte_length, appended_length, &required_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_size_add(required_length, 1u, &required_storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (required_storage <= buffer->storage.size) {
    return HXC_STATUS_OK;
  }

  new_size = buffer->storage.size == 0u ? 16u : buffer->storage.size;
  while (new_size < required_storage) {
    if (new_size > SIZE_MAX / 2u) {
      new_size = required_storage;
      break;
    }
    new_size *= 2u;
  }
  status = hxc_allocation_resize(&buffer->storage, new_size, 1u);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  ((uint8_t *)buffer->storage.memory)[buffer->byte_length] = UINT8_C(0);
  return HXC_STATUS_OK;
}

static hxc_status hxc_string_buffer_alias_offset(
  const hxc_string_buffer *buffer,
  hxc_byte_view source,
  bool *out_alias,
  size_t *out_offset
) {
  const uint8_t *data;
  size_t index;
  if (out_alias == NULL || out_offset == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_alias = false;
  *out_offset = 0u;
  if (buffer->storage.size == 0u || source.data == NULL) {
    return HXC_STATUS_OK;
  }
  data = (const uint8_t *)buffer->storage.memory;
  for (index = 0u; index <= buffer->byte_length; index++) {
    if (source.data == data + index) {
      if (source.length > buffer->byte_length - index) {
        return HXC_STATUS_INVALID_ARGUMENT;
      }
      *out_alias = true;
      *out_offset = index;
      return HXC_STATUS_OK;
    }
  }
  return HXC_STATUS_OK;
}

static bool hxc_borrowed_cstring_slot_is_empty(
  const hxc_borrowed_cstring *value
) {
  return value != NULL
    && value->data == NULL
    && value->byte_length == 0u
    && value->owner == NULL;
}

static bool hxc_owned_cstring_slot_is_empty(const hxc_owned_cstring *value) {
  return value != NULL
    && value->data == NULL
    && value->byte_length == 0u
    && value->storage.memory == NULL
    && value->storage.size == 0u
    && value->storage.alignment == 0u
    && value->storage.allocator.context == NULL
    && value->storage.allocator.allocate == NULL
    && value->storage.allocator.reallocate == NULL
    && value->storage.allocator.release == NULL;
}

static void hxc_owned_cstring_clear(hxc_owned_cstring *value) {
  value->data = NULL;
  value->byte_length = 0u;
  value->storage.memory = NULL;
  value->storage.size = 0u;
  value->storage.alignment = 0u;
  value->storage.allocator.context = NULL;
  value->storage.allocator.allocate = NULL;
  value->storage.allocator.reallocate = NULL;
  value->storage.allocator.release = NULL;
}

hxc_status hxc_byte_view_from_cstring(
  const char *value,
  hxc_byte_view *out_view
) {
  hxc_byte_view view = HXC_BYTE_VIEW_INITIALIZER;
  if (value == NULL || out_view == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  while (value[view.length] != '\0') {
    view.length++;
  }
  view.data = (const uint8_t *)value;
  *out_view = view;
  return HXC_STATUS_OK;
}

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
  if (value.data == NULL && value.byte_length != 0u) {
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

hxc_status hxc_string_from_utf8_checked(
  hxc_byte_view source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
) {
  size_t scalar_length;
  hxc_status status;
  if (!hxc_owned_string_slot_is_empty(out_string)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_utf8_validate(source, &scalar_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  return hxc_owned_string_from_valid_bytes(
    source.data,
    source.length,
    allocator,
    out_string
  );
}

hxc_status hxc_string_from_utf8_lossy(
  hxc_byte_view source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
) {
  hxc_owned_string value = HXC_OWNED_STRING_INITIALIZER;
  size_t input_index = 0u;
  size_t output_index = 0u;
  size_t output_length = 0u;
  size_t contribution;
  size_t allocation_size;
  uint8_t replacement[4];
  uint8_t *destination;
  hxc_utf8_step step;
  hxc_status status;

  if (!hxc_byte_view_has_valid_shape(source)
    || !hxc_owned_string_slot_is_empty(out_string)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  while (input_index < source.length) {
    step = hxc_utf8_read(source.data + input_index, source.length - input_index);
    contribution = step.valid ? step.consumed : 3u;
    status = hxc_size_add(output_length, contribution, &output_length);
    if (status != HXC_STATUS_OK) {
      return status;
    }
    input_index += step.consumed;
  }
  if (output_length == 0u) {
    return hxc_owned_string_from_valid_bytes(
      NULL,
      0u,
      allocator,
      out_string
    );
  }

  status = hxc_size_add(output_length, 1u, &allocation_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_allocate(
    allocator,
    allocation_size,
    1u,
    HXC_ALIGNOF(uint8_t),
    &value.storage
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  destination = (uint8_t *)value.storage.memory;
  (void)hxc_utf8_encode(UINT32_C(0xFFFD), replacement);
  input_index = 0u;
  while (input_index < source.length) {
    step = hxc_utf8_read(source.data + input_index, source.length - input_index);
    if (step.valid) {
      hxc_copy_bytes(destination + output_index, source.data + input_index, step.consumed);
      output_index += step.consumed;
    } else {
      hxc_copy_bytes(destination + output_index, replacement, 3u);
      output_index += 3u;
    }
    input_index += step.consumed;
  }
  destination[output_length] = UINT8_C(0);
  value.value.data = destination;
  value.value.byte_length = output_length;
  value.value.has_trailing_nul = true;
  *out_string = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_copy(
  hxc_string source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
) {
  if (!hxc_string_is_valid(source)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  return hxc_owned_string_from_valid_bytes(
    source.data,
    source.byte_length,
    allocator,
    out_string
  );
}

hxc_status hxc_string_concat(
  hxc_string left,
  hxc_string right,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
) {
  hxc_owned_string value = HXC_OWNED_STRING_INITIALIZER;
  size_t combined_length;
  size_t allocation_size;
  uint8_t *destination;
  hxc_status status;
  if (!hxc_owned_string_slot_is_empty(out_string)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(left) || !hxc_string_is_valid(right)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  status = hxc_size_add(left.byte_length, right.byte_length, &combined_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (combined_length == 0u) {
    return hxc_owned_string_from_valid_bytes(NULL, 0u, allocator, out_string);
  }
  status = hxc_size_add(combined_length, 1u, &allocation_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_allocate(
    allocator,
    allocation_size,
    1u,
    HXC_ALIGNOF(uint8_t),
    &value.storage
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  destination = (uint8_t *)value.storage.memory;
  hxc_copy_bytes(destination, left.data, left.byte_length);
  hxc_copy_bytes(destination + left.byte_length, right.data, right.byte_length);
  destination[combined_length] = UINT8_C(0);
  value.value.data = destination;
  value.value.byte_length = combined_length;
  value.value.has_trailing_nul = true;
  *out_string = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_owned_string_dispose(hxc_owned_string *value) {
  hxc_status status;
  if (value == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (hxc_owned_string_slot_is_empty(value)) {
    return HXC_STATUS_OK;
  }
  if (!hxc_owned_string_is_valid(value)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_allocation_dispose(&value->storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  hxc_owned_string_clear(value);
  return HXC_STATUS_OK;
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
    return out_scalar == NULL ? HXC_STATUS_INVALID_ARGUMENT : HXC_STATUS_INVALID_UTF8;
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
  hxc_string result = HXC_STRING_INITIALIZER;
  if (out_slice == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(source)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  while (index < scalar_start && byte_index < source.byte_length) {
    step = hxc_utf8_read(source.data + byte_index, source.byte_length - byte_index);
    byte_index += step.consumed;
    index++;
  }
  if (index != scalar_start) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  slice_start = byte_index;
  while (consumed < scalar_length && byte_index < source.byte_length) {
    step = hxc_utf8_read(source.data + byte_index, source.byte_length - byte_index);
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

hxc_status hxc_string_buffer_init(
  const hxc_allocator *allocator,
  hxc_string_buffer *out_buffer
) {
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_status status;
  if (!hxc_string_buffer_slot_is_empty(out_buffer)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_allocation_allocate(
    allocator,
    0u,
    1u,
    HXC_ALIGNOF(uint8_t),
    &buffer.storage
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *out_buffer = buffer;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_buffer_view(
  const hxc_string_buffer *buffer,
  hxc_string *out_view
) {
  hxc_string view = HXC_STRING_INITIALIZER;
  if (out_view == NULL || !hxc_string_buffer_is_valid(buffer)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  view.data = buffer->storage.size == 0u
    ? hxc_empty_string_storage
    : (const uint8_t *)buffer->storage.memory;
  view.byte_length = buffer->byte_length;
  view.has_trailing_nul = true;
  *out_view = view;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_buffer_append_utf8_checked(
  hxc_string_buffer *buffer,
  hxc_byte_view source
) {
  size_t scalar_length;
  size_t alias_offset;
  size_t destination_offset;
  const uint8_t *input;
  uint8_t *destination;
  bool alias;
  hxc_status status;
  if (!hxc_string_buffer_is_valid(buffer)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_utf8_validate(source, &scalar_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (source.length == 0u) {
    return HXC_STATUS_OK;
  }
  status = hxc_string_buffer_alias_offset(
    buffer,
    source,
    &alias,
    &alias_offset
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  destination_offset = buffer->byte_length;
  status = hxc_string_buffer_reserve(buffer, source.length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  destination = (uint8_t *)buffer->storage.memory;
  input = alias ? destination + alias_offset : source.data;
  hxc_copy_bytes(destination + destination_offset, input, source.length);
  buffer->byte_length = destination_offset + source.length;
  destination[buffer->byte_length] = UINT8_C(0);
  return HXC_STATUS_OK;
}

hxc_status hxc_string_buffer_append_scalar(
  hxc_string_buffer *buffer,
  uint32_t scalar
) {
  uint8_t encoded[4];
  hxc_byte_view view;
  view.length = hxc_utf8_encode(scalar, encoded);
  view.data = encoded;
  return hxc_string_buffer_append_utf8_checked(buffer, view);
}

hxc_status hxc_string_buffer_finish(
  hxc_string_buffer *buffer,
  hxc_owned_string *out_string
) {
  hxc_owned_string value = HXC_OWNED_STRING_INITIALIZER;
  hxc_status status;
  if (!hxc_string_buffer_is_valid(buffer)
    || !hxc_owned_string_slot_is_empty(out_string)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_allocation_move(&buffer->storage, &value.storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  value.value.data = value.storage.size == 0u
    ? hxc_empty_string_storage
    : (const uint8_t *)value.storage.memory;
  value.value.byte_length = buffer->byte_length;
  value.value.has_trailing_nul = true;
  buffer->byte_length = 0u;
  *out_string = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_buffer_dispose(hxc_string_buffer *buffer) {
  hxc_status status;
  if (buffer == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (hxc_string_buffer_slot_is_empty(buffer)) {
    return HXC_STATUS_OK;
  }
  if (!hxc_string_buffer_is_valid(buffer)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_allocation_dispose(&buffer->storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  buffer->byte_length = 0u;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_borrow_cstring(
  const hxc_string *source,
  hxc_borrowed_cstring *out_cstring
) {
  hxc_borrowed_cstring value = HXC_BORROWED_CSTRING_INITIALIZER;
  if (source == NULL || !hxc_borrowed_cstring_slot_is_empty(out_cstring)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(*source)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  if (hxc_string_contains_nul(*source)) {
    return HXC_STATUS_EMBEDDED_NUL;
  }
  if (!source->has_trailing_nul) {
    return HXC_STATUS_BORROW_UNAVAILABLE;
  }
  value.data = (const char *)source->data;
  value.byte_length = source->byte_length;
  value.owner = source;
  *out_cstring = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_to_cstring_owned(
  const hxc_string *source,
  const hxc_allocator *allocator,
  hxc_owned_cstring *out_cstring
) {
  hxc_owned_cstring value = HXC_OWNED_CSTRING_INITIALIZER;
  size_t allocation_size;
  uint8_t *destination;
  hxc_status status;
  if (source == NULL || !hxc_owned_cstring_slot_is_empty(out_cstring)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(*source)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  if (hxc_string_contains_nul(*source)) {
    return HXC_STATUS_EMBEDDED_NUL;
  }
  status = hxc_size_add(source->byte_length, 1u, &allocation_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_allocate(
    allocator,
    allocation_size,
    1u,
    HXC_ALIGNOF(uint8_t),
    &value.storage
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  destination = (uint8_t *)value.storage.memory;
  hxc_copy_bytes(destination, source->data, source->byte_length);
  destination[source->byte_length] = UINT8_C(0);
  value.data = (char *)value.storage.memory;
  value.byte_length = source->byte_length;
  *out_cstring = value;
  return HXC_STATUS_OK;
}

hxc_status hxc_owned_cstring_dispose(hxc_owned_cstring *value) {
  size_t required;
  hxc_status status;
  if (value == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (hxc_owned_cstring_slot_is_empty(value)) {
    return HXC_STATUS_OK;
  }
  if (hxc_size_add(value->byte_length, 1u, &required) != HXC_STATUS_OK
    || !hxc_allocation_is_valid(&value->storage)
    || value->data != (char *)value->storage.memory
    || value->storage.size != required
    || value->data[value->byte_length] != '\0') {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_allocation_dispose(&value->storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  hxc_owned_cstring_clear(value);
  return HXC_STATUS_OK;
}
