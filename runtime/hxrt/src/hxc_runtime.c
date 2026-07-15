#include "hxc_runtime.h"

#if !defined(HXC_FREESTANDING)
#include <stdlib.h>
#endif

static bool hxc_is_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

#if !defined(HXC_FREESTANDING)
static void *hxc_host_allocate(void *context, size_t size, size_t alignment) {
  (void)context;
  if (alignment > HXC_ALIGNOF(max_align_t)) {
    return NULL;
  }
  return malloc(size == 0u ? 1u : size);
}

static void *hxc_host_reallocate(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment
) {
  (void)context;
  (void)old_size;
  if (alignment > HXC_ALIGNOF(max_align_t)) {
    return NULL;
  }
  if (new_size == 0u) {
    free(memory);
    return NULL;
  }
  return realloc(memory, new_size);
}

static void hxc_host_release(
  void *context,
  void *memory,
  size_t size,
  size_t alignment
) {
  (void)context;
  (void)size;
  (void)alignment;
  free(memory);
}
#endif

uint32_t hxc_runtime_abi_version(void) {
  return HXC_RUNTIME_ABI_VERSION;
}

const char *hxc_status_name(hxc_status status) {
  switch (status) {
    case HXC_STATUS_OK:
      return "HXC_STATUS_OK";
    case HXC_STATUS_INVALID_ARGUMENT:
      return "HXC_STATUS_INVALID_ARGUMENT";
    case HXC_STATUS_OUT_OF_MEMORY:
      return "HXC_STATUS_OUT_OF_MEMORY";
    case HXC_STATUS_SIZE_OVERFLOW:
      return "HXC_STATUS_SIZE_OVERFLOW";
    case HXC_STATUS_UNSUPPORTED_ALIGNMENT:
      return "HXC_STATUS_UNSUPPORTED_ALIGNMENT";
    case HXC_STATUS_DIVISION_BY_ZERO:
      return "HXC_STATUS_DIVISION_BY_ZERO";
    case HXC_STATUS_INTERNAL_ERROR:
      return "HXC_STATUS_INTERNAL_ERROR";
    default:
      return "HXC_STATUS_UNKNOWN";
  }
}

hxc_allocator hxc_default_allocator(void) {
  hxc_allocator allocator;
  allocator.context = NULL;
#if defined(HXC_FREESTANDING)
  allocator.allocate = NULL;
  allocator.reallocate = NULL;
  allocator.release = NULL;
#else
  allocator.allocate = hxc_host_allocate;
  allocator.reallocate = hxc_host_reallocate;
  allocator.release = hxc_host_release;
#endif
  return allocator;
}

bool hxc_allocator_is_valid(const hxc_allocator *allocator) {
  return allocator != NULL
    && allocator->allocate != NULL
    && allocator->reallocate != NULL
    && allocator->release != NULL;
}

hxc_status hxc_alloc(
  const hxc_allocator *allocator,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  void *memory;
  if (out_memory == NULL || !hxc_allocator_is_valid(allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_memory = NULL;
  if (!hxc_is_power_of_two(alignment)) {
    return HXC_STATUS_UNSUPPORTED_ALIGNMENT;
  }
  memory = allocator->allocate(allocator->context, size, alignment);
  if (memory == NULL && size != 0u) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  *out_memory = memory;
  return HXC_STATUS_OK;
}

hxc_status hxc_realloc(
  const hxc_allocator *allocator,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
) {
  void *resized;
  if (out_memory == NULL || !hxc_allocator_is_valid(allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_memory = NULL;
  if (!hxc_is_power_of_two(alignment)) {
    return HXC_STATUS_UNSUPPORTED_ALIGNMENT;
  }
  resized = allocator->reallocate(
    allocator->context,
    memory,
    old_size,
    new_size,
    alignment
  );
  if (resized == NULL && new_size != 0u) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  *out_memory = resized;
  return HXC_STATUS_OK;
}

hxc_status hxc_free(
  const hxc_allocator *allocator,
  void *memory,
  size_t size,
  size_t alignment
) {
  if (!hxc_allocator_is_valid(allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_is_power_of_two(alignment)) {
    return HXC_STATUS_UNSUPPORTED_ALIGNMENT;
  }
  if (memory != NULL) {
    allocator->release(allocator->context, memory, size, alignment);
  }
  return HXC_STATUS_OK;
}

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
