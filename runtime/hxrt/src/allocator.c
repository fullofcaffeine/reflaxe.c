#include "hxrt/allocator.h"

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
