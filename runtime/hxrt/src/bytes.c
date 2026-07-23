/*
 * Implementation of the runtime feature `bytes`.
 *
 * The compiler selects this source only for ordinary Haxe Bytes operations.
 * It owns fixed-length binary storage and reference counts, depends on the
 * registered allocator/status contracts, and carries no hidden global state.
 */
#include "hxrt/bytes.h"

static void hxc_bytes_zero(uint8_t *bytes, size_t length) {
  size_t index;
  for (index = 0u; index < length; index++) {
    bytes[index] = UINT8_C(0);
  }
}

static void hxc_bytes_copy_forward(
  uint8_t *destination,
  const uint8_t *source,
  size_t length
) {
  size_t index;
  for (index = 0u; index < length; index++) {
    destination[index] = source[index];
  }
}

static void hxc_bytes_copy_backward(
  uint8_t *destination,
  const uint8_t *source,
  size_t length
) {
  size_t index = length;
  while (index != 0u) {
    index--;
    destination[index] = source[index];
  }
}

static hxc_status hxc_bytes_checked_range(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t length,
  size_t *out_position,
  size_t *out_length
) {
  size_t start;
  size_t count;
  if (!hxc_bytes_ref_is_valid(bytes)
    || out_position == NULL
    || out_length == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (position < 0 || length < 0) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  start = (size_t)position;
  count = (size_t)length;
  if (start > bytes->length || count > bytes->length - start) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  *out_position = start;
  *out_length = count;
  return HXC_STATUS_OK;
}

static hxc_status hxc_bytes_create_storage(
  hxc_allocator allocator,
  size_t length,
  hxc_bytes_ref **out_bytes
) {
  hxc_bytes_ref *bytes = NULL;
  hxc_status status;
  if (out_bytes == NULL
    || *out_bytes != NULL
    || length > (size_t)INT32_MAX
    || !hxc_allocator_is_valid(&allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alloc(
    &allocator,
    sizeof(hxc_bytes_ref),
    HXC_ALIGNOF(hxc_bytes_ref),
    (void **)&bytes
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  bytes->references = 1u;
  bytes->storage = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  bytes->length = length;
  bytes->allocator = allocator;
  status = hxc_allocation_allocate(
    &allocator,
    length,
    sizeof(uint8_t),
    HXC_ALIGNOF(uint8_t),
    &bytes->storage
  );
  if (status != HXC_STATUS_OK) {
    (void)hxc_free(
      &allocator,
      bytes,
      sizeof(hxc_bytes_ref),
      HXC_ALIGNOF(hxc_bytes_ref)
    );
    return status;
  }
  *out_bytes = bytes;
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_create_zeroed(
  hxc_allocator allocator,
  int32_t length,
  hxc_bytes_ref **out_bytes
) {
  hxc_status status;
  if (length < 0) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  status = hxc_bytes_create_storage(allocator, (size_t)length, out_bytes);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if ((*out_bytes)->length != 0u) {
    hxc_bytes_zero((uint8_t *)(*out_bytes)->storage.memory, (*out_bytes)->length);
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_create_copy(
  hxc_allocator allocator,
  const uint8_t *source,
  size_t length,
  hxc_bytes_ref **out_bytes
) {
  hxc_status status;
  if ((source == NULL && length != 0u) || length > (size_t)INT32_MAX) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_bytes_create_storage(allocator, length, out_bytes);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (length != 0u) {
    hxc_bytes_copy_forward(
      (uint8_t *)(*out_bytes)->storage.memory,
      source,
      length
    );
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_create_utf8_copy(
  hxc_allocator allocator,
  hxc_string source,
  hxc_bytes_ref **out_bytes
) {
  if ((source.data == NULL && source.byte_length != 0u)
    || source.byte_length > (size_t)INT32_MAX) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  return hxc_bytes_ref_create_copy(
    allocator,
    source.data,
    source.byte_length,
    out_bytes
  );
}

bool hxc_bytes_ref_is_valid(const hxc_bytes_ref *bytes) {
  return bytes != NULL
    && bytes->references > 0u
    && bytes->length <= (size_t)INT32_MAX
    && hxc_allocator_is_valid(&bytes->allocator)
    && hxc_allocation_is_valid(&bytes->storage)
    && bytes->storage.size == bytes->length
    && bytes->storage.alignment == HXC_ALIGNOF(uint8_t)
    && hxc_allocator_same_identity(&bytes->allocator, &bytes->storage.allocator);
}

hxc_status hxc_bytes_ref_retain(hxc_bytes_ref *bytes) {
  if (!hxc_bytes_ref_is_valid(bytes)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (bytes->references == SIZE_MAX) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  bytes->references++;
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_release(hxc_bytes_ref *bytes) {
  hxc_allocator allocator;
  hxc_status status;
  if (!hxc_bytes_ref_is_valid(bytes)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  bytes->references--;
  if (bytes->references != 0u) {
    return HXC_STATUS_OK;
  }
  allocator = bytes->allocator;
  status = hxc_allocation_dispose(&bytes->storage);
  if (status != HXC_STATUS_OK) {
    bytes->references = 1u;
    return status;
  }
  bytes->length = 0u;
  bytes->allocator = (hxc_allocator){ NULL, NULL, NULL, NULL };
  return hxc_free(
    &allocator,
    bytes,
    sizeof(hxc_bytes_ref),
    HXC_ALIGNOF(hxc_bytes_ref)
  );
}

hxc_status hxc_bytes_ref_length(
  const hxc_bytes_ref *bytes,
  int32_t *out_length
) {
  if (out_length == NULL || !hxc_bytes_ref_is_valid(bytes)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_length = (int32_t)bytes->length;
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_get(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t *out_value
) {
  if (out_value == NULL || !hxc_bytes_ref_is_valid(bytes)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (position < 0 || (size_t)position >= bytes->length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  *out_value = (int32_t)((const uint8_t *)bytes->storage.memory)[position];
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_set(
  hxc_bytes_ref *bytes,
  int32_t position,
  int32_t value
) {
  if (!hxc_bytes_ref_is_valid(bytes)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (position < 0 || (size_t)position >= bytes->length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  ((uint8_t *)bytes->storage.memory)[position] = (uint8_t)((uint32_t)value & UINT32_C(255));
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_sub(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t length,
  hxc_bytes_ref **out_bytes
) {
  size_t start;
  size_t count;
  const uint8_t *source;
  const hxc_status range_status = hxc_bytes_checked_range(bytes, position, length, &start, &count);
  if (range_status != HXC_STATUS_OK)
    return range_status;
  source = count == 0u ? NULL : (const uint8_t *)bytes->storage.memory + start;
  return hxc_bytes_ref_create_copy(bytes->allocator, source, count, out_bytes);
}

hxc_status hxc_bytes_ref_blit(
  hxc_bytes_ref *destination,
  int32_t destination_position,
  const hxc_bytes_ref *source,
  int32_t source_position,
  int32_t length
) {
  size_t destination_start;
  size_t destination_count;
  size_t source_start;
  size_t source_count;
  uint8_t *destination_bytes;
  const uint8_t *source_bytes;
  hxc_status range_status = hxc_bytes_checked_range(
      destination,
      destination_position,
      length,
      &destination_start,
      &destination_count
    );
  if (range_status != HXC_STATUS_OK)
    return range_status;
  range_status = hxc_bytes_checked_range(
      source,
      source_position,
      length,
      &source_start,
      &source_count
    );
  if (range_status != HXC_STATUS_OK)
    return range_status;
  if (destination_count == 0u || source_count == 0u) {
    return HXC_STATUS_OK;
  }
  destination_bytes = (uint8_t *)destination->storage.memory + destination_start;
  source_bytes = (const uint8_t *)source->storage.memory + source_start;
  if (destination == source
    && destination_start > source_start
    && destination_start - source_start < source_count) {
    hxc_bytes_copy_backward(destination_bytes, source_bytes, source_count);
  } else {
    hxc_bytes_copy_forward(destination_bytes, source_bytes, source_count);
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_fill(
  hxc_bytes_ref *bytes,
  int32_t position,
  int32_t length,
  int32_t value
) {
  size_t start;
  size_t count;
  size_t index;
  uint8_t *destination;
  const hxc_status range_status = hxc_bytes_checked_range(bytes, position, length, &start, &count);
  if (range_status != HXC_STATUS_OK)
    return range_status;
  destination = count == 0u ? NULL : (uint8_t *)bytes->storage.memory + start;
  for (index = 0u; index < count; index++) {
    destination[index] = (uint8_t)((uint32_t)value & UINT32_C(255));
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_bytes_ref_compare(
  const hxc_bytes_ref *left,
  const hxc_bytes_ref *right,
  int32_t *out_order
) {
  size_t index;
  const uint8_t *left_bytes;
  const uint8_t *right_bytes;
  if (out_order == NULL
    || !hxc_bytes_ref_is_valid(left)
    || !hxc_bytes_ref_is_valid(right)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  left_bytes = (const uint8_t *)left->storage.memory;
  right_bytes = (const uint8_t *)right->storage.memory;
  for (index = 0u; index < (left->length < right->length ? left->length : right->length); index++) {
    if (left_bytes[index] != right_bytes[index]) {
      *out_order = left_bytes[index] < right_bytes[index] ? -1 : 1;
      return HXC_STATUS_OK;
    }
  }
  *out_order = left->length < right->length ? -1 : left->length > right->length ? 1 : 0;
  return HXC_STATUS_OK;
}
