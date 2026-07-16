#include "hxrt/string.h"

hxc_string_view hxc_string_view_from_cstr(const char *value) {
  hxc_string_view view;
  size_t length = 0u;
  if (value != NULL) {
    while (value[length] != '\0') {
      length++;
    }
  }
  view.data = (const uint8_t *)value;
  view.length = length;
  return view;
}

hxc_status hxc_string_copy(
  hxc_string_view source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
) {
  void *memory = NULL;
  uint8_t *data;
  size_t index;
  hxc_status status;

  if (out_string == NULL || (source.data == NULL && source.length != 0u)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (source.length == SIZE_MAX) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  status = hxc_alloc(
    allocator,
    source.length + 1u,
    HXC_ALIGNOF(uint8_t),
    &memory
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }

  data = (uint8_t *)memory;
  for (index = 0u; index < source.length; index++) {
    data[index] = source.data[index];
  }
  data[source.length] = UINT8_C(0);

  out_string->data = data;
  out_string->length = source.length;
  out_string->capacity = source.length + 1u;
  out_string->allocator = *allocator;
  return HXC_STATUS_OK;
}

void hxc_owned_string_dispose(hxc_owned_string *value) {
  hxc_allocator allocator;
  uint8_t *data;
  size_t capacity;
  if (value == NULL) {
    return;
  }

  allocator = value->allocator;
  data = value->data;
  capacity = value->capacity;
  value->data = NULL;
  value->length = 0u;
  value->capacity = 0u;
  value->allocator.context = NULL;
  value->allocator.allocate = NULL;
  value->allocator.reallocate = NULL;
  value->allocator.release = NULL;

  if (data != NULL && hxc_allocator_is_valid(&allocator)) {
    (void)hxc_free(&allocator, data, capacity, HXC_ALIGNOF(uint8_t));
  }
}
