/*
 * hxrt feature: alloc (compiler-selectable dependency).
 *
 * Array, Bytes, object-collector, and independent string evidence call this API
 * for checked sizes, aligned storage, allocator identity, failure-atomic
 * resize, and move-only owners. The compiler selects it only through a proven
 * runtime-sized owner; known bounded storage remains direct C or a program-local
 * specialization. Caller allocator contexts must
 * outlive every owned block. Failures use hxc_status and preserve output owners;
 * there is no hidden global or thread state. All layouts are internal ABI and
 * forbidden in application exports.
 */
#ifndef HXRT_ALLOCATOR_H_INCLUDED
#define HXRT_ALLOCATOR_H_INCLUDED

#include "hxrt/status.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Allocate one non-empty block.
 *
 * On success the callback writes a non-null, suitably aligned pointer to
 * `out_memory`; that block must not alias any other live allocation. On failure
 * it leaves `out_memory` unchanged. The runtime never calls this callback for
 * a zero-size request.
 */
typedef hxc_status (*hxc_allocate_fn)(
  void *context,
  size_t size,
  size_t alignment,
  void **out_memory
);

/**
 * Replace one non-empty block while preserving failure atomicity.
 *
 * A failure leaves `memory` allocated and leaves `out_memory` unchanged. A
 * success transfers ownership to the returned non-null block. This callback is
 * optional; `old_size` must be the allocation's exact current size, and the
 * runtime falls back to allocate/copy/release when the callback is null.
 */
typedef hxc_status (*hxc_reallocate_fn)(
  void *context,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
);

/** Release one non-null block with its original size and alignment. */
typedef void (*hxc_release_fn)(void *context, void *memory, size_t size, size_t alignment);

typedef struct hxc_allocator {
  void *context;
  hxc_allocate_fn allocate;
  hxc_reallocate_fn reallocate;
  hxc_release_fn release;
} hxc_allocator;

/**
 * Move-only-by-convention allocation owner.
 *
 * The allocator handle is stored by value so its identity follows the block
 * across internal ABI boundaries. Its context must remain alive until the
 * owner is disposed. Use `hxc_allocation_move` instead of copying a live owner.
 */
typedef struct hxc_allocation {
  void *memory;
  size_t size;
  size_t alignment;
  hxc_allocator allocator;
} hxc_allocation;

#define HXC_ALLOCATION_INITIALIZER \
  { NULL, 0u, 0u, { NULL, NULL, NULL, NULL } }

/** Hosted default allocator. The handle is invalid in `HXC_FREESTANDING` builds. */
HXC_API hxc_allocator hxc_default_allocator(void);

/** Allocate/release are required; reallocate is an optional fast path. */
HXC_API bool hxc_allocator_is_valid(const hxc_allocator *allocator);

/** Compare the context and callback tuple that owns allocator-backed storage. */
HXC_API bool hxc_allocator_same_identity(
  const hxc_allocator *left,
  const hxc_allocator *right
);

/** Checked size arithmetic. The output is written only on success. */
HXC_API hxc_status hxc_size_add(size_t left, size_t right, size_t *out_size);
HXC_API hxc_status hxc_size_mul(size_t left, size_t right, size_t *out_size);

/**
 * Allocate `size` bytes with power-of-two `alignment`.
 *
 * A zero-size request succeeds as canonical null storage without calling the
 * allocator. Every failure leaves `out_memory` unchanged.
 */
HXC_API hxc_status hxc_alloc(
  const hxc_allocator *allocator,
  size_t size,
  size_t alignment,
  void **out_memory
);

/**
 * Resize a canonical allocation.
 *
 * A zero new size releases the old block and publishes null. Any other failure
 * leaves the old block owned and leaves `out_memory` unchanged.
 */
HXC_API hxc_status hxc_realloc(
  const hxc_allocator *allocator,
  void *memory,
  size_t old_size,
  size_t new_size,
  size_t alignment,
  void **out_memory
);

/** Null/zero is a no-op; all other calls require a matching non-null/size pair. */
HXC_API hxc_status hxc_free(
  const hxc_allocator *allocator,
  void *memory,
  size_t size,
  size_t alignment
);

/** Validate the canonical owner shape and the stored allocator identity. */
HXC_API bool hxc_allocation_is_valid(const hxc_allocation *allocation);

/**
 * Allocate `count * element_size` bytes with checked multiplication.
 * Initialize the destination with `HXC_ALLOCATION_INITIALIZER`; it is written
 * only on success and must still be empty.
 */
HXC_API hxc_status hxc_allocation_allocate(
  const hxc_allocator *allocator,
  size_t count,
  size_t element_size,
  size_t alignment,
  hxc_allocation *out_allocation
);

/** Resize in place while leaving the complete owner unchanged on failure. */
HXC_API hxc_status hxc_allocation_resize(
  hxc_allocation *allocation,
  size_t count,
  size_t element_size
);

/** Transfer one live owner and clear the source; initialize an empty destination. */
HXC_API hxc_status hxc_allocation_move(
  hxc_allocation *source,
  hxc_allocation *out_allocation
);

/** Dispose through the stored allocator. Disposing an empty owner is a no-op. */
HXC_API hxc_status hxc_allocation_dispose(hxc_allocation *allocation);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_ALLOCATOR_H_INCLUDED */
