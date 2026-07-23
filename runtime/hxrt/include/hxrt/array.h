/*
 * hxrt feature: array.
 *
 * The compiler and independent native harnesses call this API for runtime-sized
 * contiguous unboxed storage. It depends on alloc; fixed arrays and nonescaping
 * spans remain direct C and runtime-free.
 * hxc_array owns its allocation and is move-only by convention. Mutations can
 * invalidate borrowed element pointers, failures preserve the prior live array,
 * and callback context must outlive the owner. The slice has no global/thread
 * state and its layouts are internal ABI, never application-export types.
 */
#ifndef HXRT_ARRAY_H_INCLUDED
#define HXRT_ARRAY_H_INCLUDED

#include "hxrt/allocator.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Construct one element in uninitialized storage.
 *
 * On failure the destination contains no live element. The source remains
 * live and unchanged. Array operations may pass two elements from the same
 * array, so implementations must preserve source identity under aliasing.
 */
typedef hxc_status (*hxc_array_copy_fn)(
  void *context,
  void *destination,
  const void *source
);

/**
 * Replace one live element while preserving failure atomicity.
 *
 * On failure both elements remain live and unchanged. Implementations for
 * reference-like values must retain the source before releasing the old
 * destination so distinct slots containing the same reference are safe.
 */
typedef hxc_status (*hxc_array_assign_fn)(
  void *context,
  void *destination,
  const void *source
);

/** Destroy one live element. Destruction cannot fail. */
typedef void (*hxc_array_destroy_fn)(void *context, void *element);

/**
 * Closed element strategy for one unboxed array representation.
 *
 * Trivial byte-relocatable values use three null callbacks. Values with
 * ownership use a complete copy/assign/destroy trio. Every admitted element
 * representation must remain valid when its bytes move to another aligned
 * address; callbacks manage logical copies, assignments, and destruction, not
 * storage relocation. Callback context must outlive the array and callbacks
 * must not re-enter or mutate that array.
 */
typedef struct hxc_array_element_ops {
  size_t size;
  size_t alignment;
  void *context;
  hxc_array_copy_fn copy;
  hxc_array_assign_fn assign;
  hxc_array_destroy_fn destroy;
} hxc_array_element_ops;

/**
 * Move-only-by-convention owner for contiguous unboxed elements.
 *
 * Generated typed wrappers may expose a concrete `T *`; this private storage
 * carrier remains forbidden in application exports. Fixed C arrays and spans
 * are separate non-owning/direct representations and never require this type.
 */
typedef struct hxc_array {
  hxc_allocation storage;
  size_t length;
  size_t capacity;
  hxc_array_element_ops elements;
} hxc_array;

/**
 * Shared Haxe Array identity around one move-only storage owner.
 *
 * Copying this pointer preserves Haxe aliasing; copying the embedded hxc_array
 * remains forbidden. The first compiler-selected slice admits only acyclic,
 * byte-relocatable element representations, so local reference counting is
 * sufficient without pretending to provide the future tracing collector.
 */
typedef struct hxc_array_ref {
  size_t references;
  hxc_array value;
  hxc_allocator allocator;
} hxc_array_ref;

#define HXC_ARRAY_INITIALIZER \
  { HXC_ALLOCATION_INITIALIZER, 0u, 0u, { 0u, 0u, NULL, NULL, NULL, NULL } }

/** Accept a non-zero size/alignment and either zero or three callbacks. */
HXC_API bool hxc_array_element_ops_is_valid(
  const hxc_array_element_ops *elements
);

/**
 * Initialize an empty array without allocating.
 *
 * The allocator identity is retained immediately. Initialize the destination
 * with `HXC_ARRAY_INITIALIZER`; it is written only on success.
 */
HXC_API hxc_status hxc_array_init(
  const hxc_allocator *allocator,
  hxc_array_element_ops elements,
  hxc_array *out_array
);

/** Validate storage size, alignment, length/capacity, and element strategy. */
HXC_API bool hxc_array_is_valid(const hxc_array *array);

/**
 * Ensure space for at least `minimum_capacity` elements.
 *
 * Growth is deterministic and overflow checked. On failure storage, length,
 * and capacity remain unchanged. Live element bytes may move on success, so
 * pointers returned by `hxc_array_at` are borrows only until the next mutation.
 */
HXC_API hxc_status hxc_array_reserve(
  hxc_array *array,
  size_t minimum_capacity
);

/**
 * Change the logical length.
 *
 * Shrinking destroys removed elements in reverse order. Growing copy-constructs
 * each new element from `default_element`; callers provide the statically
 * correct Haxe default (zero, false, or null) for the concrete element type.
 * A failed growth destroys its constructed prefix and preserves the old
 * logical contents and length, although private capacity may have increased.
 */
HXC_API hxc_status hxc_array_resize(
  hxc_array *array,
  size_t new_length,
  const void *default_element
);

/** Borrow one mutable element; the output is unchanged on failure. */
HXC_API hxc_status hxc_array_at(
  hxc_array *array,
  size_t index,
  void **out_element
);

/** Borrow one immutable element; the output is unchanged on failure. */
HXC_API hxc_status hxc_array_at_const(
  const hxc_array *array,
  size_t index,
  const void **out_element
);

/** Copy-construct one element at the end, including from an existing slot. */
HXC_API hxc_status hxc_array_push_copy(
  hxc_array *array,
  const void *element
);

/**
 * Copy-construct at `index` and shift the suffix right.
 *
 * `index == length` appends. Exact existing-slot sources remain valid even
 * when growth relocates storage or the insertion shifts that source. A failed
 * element copy restores the original logical sequence and length.
 */
HXC_API hxc_status hxc_array_insert_copy(
  hxc_array *array,
  size_t index,
  const void *element
);

/** Assign one live slot, including from another slot in the same array. */
HXC_API hxc_status hxc_array_set_copy(
  hxc_array *array,
  size_t index,
  const void *element
);

/** Destroy one element and shift its suffix left without allocation. */
HXC_API hxc_status hxc_array_remove_at(hxc_array *array, size_t index);

/** Transfer a valid owner and reset the source to `HXC_ARRAY_INITIALIZER`. */
HXC_API hxc_status hxc_array_move(
  hxc_array *source,
  hxc_array *out_array
);

/** Destroy live elements in reverse order and release owned storage. */
HXC_API hxc_status hxc_array_dispose(hxc_array *array);

/**
 * Allocate one empty shared container with a validated typed element strategy.
 *
 * The callback context and every function it refers to must outlive the shared
 * Array value. Generated programs normally use process-lifetime, program-local
 * typed callbacks and a null context.
 */
HXC_API hxc_status hxc_array_ref_create(
  hxc_allocator allocator,
  hxc_array_element_ops elements,
  hxc_array_ref **out_array
);

/**
 * Initialize caller-owned `hxc_array_ref` storage without allocating the outer
 * container. Collector-managed Arrays use this after `hxc_gc_allocate`; the
 * ordinary reference-counted constructor uses the same path after `hxc_alloc`.
 * The output must be all-zero storage and is changed only on success.
 */
HXC_API hxc_status hxc_array_ref_init_in_place(
  hxc_allocator allocator,
  hxc_array_element_ops elements,
  hxc_array_ref *out_array
);

/**
 * Dispose only the backing buffer and clear caller-owned outer storage.
 *
 * This is the matching collector finalizer operation: it never frees `array`
 * itself. A zeroed value is a safe no-op, which also makes partially initialized
 * managed objects safe to sweep after an abrupt constructor failure.
 */
HXC_API hxc_status hxc_array_ref_dispose_in_place(hxc_array_ref *array);

/** Allocate one empty shared container for a trivial unboxed element type. */
HXC_API hxc_status hxc_array_ref_create_trivial(
  hxc_allocator allocator,
  size_t element_size,
  size_t element_alignment,
  hxc_array_ref **out_array
);

/** Validate the reference count, allocator identity, and embedded owner. */
HXC_API bool hxc_array_ref_is_valid(const hxc_array_ref *array);

/**
 * Acquire one alias, rejecting reference-count overflow before mutation.
 *
 * A null pointer is the exact carrier for an absent Haxe Array and is a
 * successful no-op. Any non-null pointer must name a valid live reference.
 */
HXC_API hxc_status hxc_array_ref_retain(hxc_array_ref *array);

/**
 * Release one alias and destroy/free the container after the last release.
 *
 * A null pointer is the exact carrier for an absent Haxe Array and is a
 * successful no-op. Any non-null pointer must name a valid live reference.
 */
HXC_API hxc_status hxc_array_ref_release(hxc_array_ref *array);

/** Read a length that is guaranteed to fit Haxe's signed 32-bit Int. */
HXC_API hxc_status hxc_array_ref_length(
  const hxc_array_ref *array,
  int32_t *out_length
);

/**
 * Copy-construct one element into uninitialized typed caller storage.
 *
 * For a managed element, success gives the caller a new live value that must be
 * destroyed according to the same element strategy. Failure leaves the output
 * storage non-live and unchanged by contract.
 */
HXC_API hxc_status hxc_array_ref_get_copy(
  const hxc_array_ref *array,
  size_t index,
  void *out_element
);

/** Append one trivial element and return the new Haxe Int length. */
HXC_API hxc_status hxc_array_ref_push_copy(
  hxc_array_ref *array,
  const void *element,
  int32_t *out_length
);

/** Replace one trivial element after a bounds check. */
HXC_API hxc_status hxc_array_ref_set_copy(
  hxc_array_ref *array,
  size_t index,
  const void *element
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_ARRAY_H_INCLUDED */
