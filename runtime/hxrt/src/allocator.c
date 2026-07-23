/*
 * Implementation of compiler-selectable dependency feature `alloc`.
 *
 * Selected Array, Bytes, and GC closures plus independent string evidence call
 * these checked ownership helpers. No source mention selects allocation by
 * itself: representation and lifetime planning must first prove that direct or
 * program-local storage is insufficient. Every live owner stores its allocator
 * identity, caller contexts must outlive allocations, and outputs publish only
 * after success. Hosted builds provide an aligned malloc adapter; freestanding
 * callers must supply callbacks. There is no hidden global or thread state and
 * every exposed record is private internal ABI.
 */
#include "hxrt/allocator.h"

#if !defined(HXC_FREESTANDING)
#include <stdlib.h>
#endif

static bool hxc_is_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

static hxc_status hxc_alignment_status(size_t alignment) {
  if (!hxc_is_power_of_two(alignment)) {
    return HXC_STATUS_UNSUPPORTED_ALIGNMENT;
  }
#if SIZE_MAX > UINTPTR_MAX
  if (alignment > (size_t)UINTPTR_MAX) {
    return HXC_STATUS_UNSUPPORTED_ALIGNMENT;
  }
#endif
  return HXC_STATUS_OK;
}

static bool hxc_pointer_is_aligned(const void *memory, size_t alignment) {
  return memory != NULL
    && ((uintptr_t)memory % (uintptr_t)alignment) == (uintptr_t)0u;
}

static void hxc_copy_bytes(void *destination, const void *source, size_t size) {
  /* Unsigned-character access is C's alias-safe representation-copy boundary. */
  unsigned char *output = (unsigned char *)destination;
  const unsigned char *input = (const unsigned char *)source;
  size_t index;
  for (index = 0u; index < size; index++) {
    output[index] = input[index];
  }
}

static void hxc_allocation_clear(hxc_allocation *allocation) {
  allocation->memory = NULL;
  allocation->size = 0u;
  allocation->alignment = 0u;
  allocation->allocator.context = NULL;
  allocation->allocator.allocate = NULL;
  allocation->allocator.reallocate = NULL;
  allocation->allocator.release = NULL;
}

static bool hxc_allocation_is_empty(const hxc_allocation *allocation) {
  return allocation->memory == NULL
    && allocation->size == 0u
    && allocation->alignment == 0u
    && allocation->allocator.context == NULL
    && allocation->allocator.allocate == NULL
    && allocation->allocator.reallocate == NULL
    && allocation->allocator.release == NULL;
}

#if !defined(HXC_FREESTANDING)
static void hxc_host_store_base(void *memory, void *base) {
  /* Use byte copies because the prefix is not guaranteed pointer-aligned. */
  unsigned char *prefix = (unsigned char *)memory - sizeof(base);
  hxc_copy_bytes(prefix, &base, sizeof(base));
}

static void *hxc_host_load_base(void *memory) {
  void *base = NULL;
  const unsigned char *prefix = (const unsigned char *)memory - sizeof(base);
  hxc_copy_bytes(&base, prefix, sizeof(base));
  return base;
}

static hxc_status hxc_host_allocate(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  uintptr_t candidate_address;
  size_t padding;
  size_t remainder;
  size_t total_size;
  void *base;
  void *memory;
  hxc_status status;

  (void)context;
  if (out_memory == NULL || size == 0u) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alignment_status(alignment);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  /* Overallocate for both the recoverable malloc base and alignment padding. */
  status = hxc_size_add(size, sizeof(base), &total_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_size_add(total_size, alignment - 1u, &total_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }

  base = malloc(total_size);
  if (base == NULL) {
    return HXC_STATUS_OUT_OF_MEMORY;
  }
  candidate_address = (uintptr_t)((unsigned char *)base + sizeof(base));
  remainder = (size_t)(candidate_address % (uintptr_t)alignment);
  padding = remainder == 0u ? 0u : alignment - remainder;
  memory = (unsigned char *)base + sizeof(base) + padding;
  /* The hidden prefix lets release recover the original malloc pointer. */
  hxc_host_store_base(memory, base);
  *out_memory = memory;
  return HXC_STATUS_OK;
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
  free(hxc_host_load_base(memory));
}

static hxc_status hxc_host_reallocate(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
) {
  const size_t copy_size = old_size < new_size ? old_size : new_size;
  void *resized = NULL;
  hxc_status status = hxc_host_allocate(
    context,
    new_size,
    alignment,
    &resized
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  /* Allocate first so failure leaves the original allocation live and intact. */
  hxc_copy_bytes(resized, memory, copy_size);
  hxc_host_release(context, memory, old_size, alignment);
  *out_memory = resized;
  return HXC_STATUS_OK;
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
    && allocator->release != NULL;
}

bool hxc_allocator_same_identity(
  const hxc_allocator *left,
  const hxc_allocator *right
) {
  return hxc_allocator_is_valid(left)
    && hxc_allocator_is_valid(right)
    && left->context == right->context
    && left->allocate == right->allocate
    && left->reallocate == right->reallocate
    && left->release == right->release;
}

hxc_status hxc_size_add(size_t left, size_t right, size_t *out_size) {
  if (out_size == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (right > SIZE_MAX - left) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  *out_size = left + right;
  return HXC_STATUS_OK;
}

hxc_status hxc_size_mul(size_t left, size_t right, size_t *out_size) {
  if (out_size == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (left != 0u && right > SIZE_MAX / left) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  *out_size = left * right;
  return HXC_STATUS_OK;
}

hxc_status hxc_alloc(
  const hxc_allocator *allocator,
  size_t size,
  size_t alignment,
  void **out_memory
) {
  void *memory = NULL;
  hxc_status status;
  if (out_memory == NULL || !hxc_allocator_is_valid(allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alignment_status(alignment);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (size == 0u) {
    /* Zero-sized ownership is canonical null storage and calls no allocator. */
    *out_memory = NULL;
    return HXC_STATUS_OK;
  }
  status = allocator->allocate(
    allocator->context,
    size,
    alignment,
    &memory
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  /* Treat a callback that violates its alignment contract as an internal error. */
  if (!hxc_pointer_is_aligned(memory, alignment)) {
    if (memory != NULL) {
      allocator->release(allocator->context, memory, size, alignment);
    }
    return HXC_STATUS_INTERNAL_ERROR;
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
  const size_t copy_size = old_size < new_size ? old_size : new_size;
  void *resized = NULL;
  hxc_status status;
  if (out_memory == NULL || !hxc_allocator_is_valid(allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alignment_status(alignment);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if ((memory == NULL && old_size != 0u)
    || (memory != NULL && old_size == 0u)
    || (memory != NULL && !hxc_pointer_is_aligned(memory, alignment))) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (new_size == 0u) {
    if (memory != NULL) {
      allocator->release(allocator->context, memory, old_size, alignment);
    }
    *out_memory = NULL;
    return HXC_STATUS_OK;
  }
  if (memory == NULL) {
    return hxc_alloc(allocator, new_size, alignment, out_memory);
  }

  if (allocator->reallocate != NULL) {
    status = allocator->reallocate(
      allocator->context,
      memory,
      old_size,
      new_size,
      alignment,
      &resized
    );
    if (status != HXC_STATUS_OK) {
      return status;
    }
    /* A successful callback transfers ownership, even if validation then fails. */
    if (!hxc_pointer_is_aligned(resized, alignment)) {
      if (resized != NULL) {
        allocator->release(
          allocator->context,
          resized,
          new_size,
          alignment
        );
      }
      return HXC_STATUS_INTERNAL_ERROR;
    }
  } else {
    /* The fallback preserves failure atomicity by releasing only after copy. */
    status = hxc_alloc(allocator, new_size, alignment, &resized);
    if (status != HXC_STATUS_OK) {
      return status;
    }
    hxc_copy_bytes(resized, memory, copy_size);
    allocator->release(allocator->context, memory, old_size, alignment);
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
  hxc_status status;
  if (!hxc_allocator_is_valid(allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alignment_status(alignment);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if ((memory == NULL && size != 0u)
    || (memory != NULL && size == 0u)
    || (memory != NULL && !hxc_pointer_is_aligned(memory, alignment))) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (memory != NULL) {
    allocator->release(allocator->context, memory, size, alignment);
  }
  return HXC_STATUS_OK;
}

bool hxc_allocation_is_valid(const hxc_allocation *allocation) {
  if (allocation == NULL
    || !hxc_allocator_is_valid(&allocation->allocator)
    || hxc_alignment_status(allocation->alignment) != HXC_STATUS_OK) {
    return false;
  }
  if (allocation->size == 0u) {
    return allocation->memory == NULL;
  }
  return hxc_pointer_is_aligned(allocation->memory, allocation->alignment);
}

hxc_status hxc_allocation_allocate(
  const hxc_allocator *allocator,
  size_t count,
  size_t element_size,
  size_t alignment,
  hxc_allocation *out_allocation
) {
  hxc_allocation allocation = HXC_ALLOCATION_INITIALIZER;
  hxc_status status;
  if (out_allocation == NULL || !hxc_allocation_is_empty(out_allocation)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_size_mul(count, element_size, &allocation.size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_alloc(
    allocator,
    allocation.size,
    alignment,
    &allocation.memory
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  allocation.alignment = alignment;
  allocation.allocator = *allocator;
  /* Publish the complete owner only after every fallible step has succeeded. */
  *out_allocation = allocation;
  return HXC_STATUS_OK;
}

hxc_status hxc_allocation_resize(
  hxc_allocation *allocation,
  size_t count,
  size_t element_size
) {
  size_t new_size;
  void *resized;
  hxc_status status;
  if (!hxc_allocation_is_valid(allocation)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_size_mul(count, element_size, &new_size);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (new_size == allocation->size) {
    return HXC_STATUS_OK;
  }
  resized = allocation->memory;
  status = hxc_realloc(
    &allocation->allocator,
    allocation->memory,
    allocation->size,
    new_size,
    allocation->alignment,
    &resized
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  /* hxc_realloc leaves the old owner untouched when it reports failure. */
  allocation->memory = resized;
  allocation->size = new_size;
  return HXC_STATUS_OK;
}

hxc_status hxc_allocation_move(
  hxc_allocation *source,
  hxc_allocation *out_allocation
) {
  if (source == NULL
    || out_allocation == NULL
    || source == out_allocation
    || !hxc_allocation_is_valid(source)
    || !hxc_allocation_is_empty(out_allocation)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  /* Clear the source immediately so ordinary struct copying cannot double-own. */
  *out_allocation = *source;
  hxc_allocation_clear(source);
  return HXC_STATUS_OK;
}

hxc_status hxc_allocation_dispose(hxc_allocation *allocation) {
  hxc_status status;
  if (allocation == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (hxc_allocation_is_empty(allocation)) {
    return HXC_STATUS_OK;
  }
  if (!hxc_allocation_is_valid(allocation)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_free(
    &allocation->allocator,
    allocation->memory,
    allocation->size,
    allocation->alignment
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  hxc_allocation_clear(allocation);
  return HXC_STATUS_OK;
}
