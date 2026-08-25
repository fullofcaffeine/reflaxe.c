/*
 * hxrt feature: iterator (compiler-selectable).
 *
 * Standard Haxe Iterator<T> values use one shared cursor. Map producers publish
 * immutable typed snapshots; Array iterators retain the live Array identity so
 * later mutations follow the pinned standard-library cursor behavior.
 */
#ifndef HXRT_ITERATOR_H_INCLUDED
#define HXRT_ITERATOR_H_INCLUDED

#include "hxrt/allocator.h"
#include "hxrt/array.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_iterator_ref hxc_iterator_ref;

/** Copy-construct one exact element in uninitialized snapshot storage. */
typedef hxc_status (*hxc_iterator_element_copy_fn)(
  void *context,
  void *destination,
  const void *source
);

/** Destroy one live snapshot or yielded element. */
typedef void (*hxc_iterator_element_destroy_fn)(void *context, void *element);

/** Complete unboxed element layout and lifetime policy for Iterator<T>. */
typedef struct hxc_iterator_element_ops {
  size_t size;
  size_t alignment;
  void *context;
  hxc_iterator_element_copy_fn copy;
  hxc_iterator_element_destroy_fn destroy;
} hxc_iterator_element_ops;

/** Fill the next uninitialized snapshot slot from one producer-owned cursor. */
typedef hxc_status (*hxc_iterator_snapshot_fill_fn)(
  void *context,
  void *destination
);

/** Release an optional producer anchor after all element callbacks have ended. */
typedef hxc_status (*hxc_iterator_anchor_release_fn)(void *anchor);

/** Accept a non-zero layout and either zero or two lifecycle callbacks. */
HXC_API bool hxc_iterator_element_ops_is_valid(
  const hxc_iterator_element_ops *elements
);

/**
 * Build one complete snapshot before publishing its iterator reference.
 *
 * `fill` is called exactly `length` times in order. Failure destroys the
 * constructed prefix and leaves `out_iterator` unchanged. The optional anchor
 * transfers to the iterator only on success; it keeps callback context alive
 * until the final iterator alias is released.
 */
HXC_API hxc_status hxc_iterator_ref_create_snapshot(
  hxc_allocator allocator,
  hxc_iterator_element_ops elements,
  size_t length,
  hxc_iterator_snapshot_fill_fn fill,
  void *fill_context,
  void *anchor,
  hxc_iterator_anchor_release_fn release_anchor,
  hxc_iterator_ref **out_iterator
);

/**
 * Create a shared cursor that reads values from the current live Array.
 *
 * The iterator retains `array`. `hasNext()` reads its current length, and
 * `next()` copy-constructs the element at the current cursor before advancing.
 * This makes appends visible and shrinking stop iteration without snapshotting.
 */
HXC_API hxc_status hxc_iterator_ref_create_array_values(
  hxc_array_ref *array,
  hxc_iterator_ref **out_iterator
);

/**
 * Create the live key/value variant with one compiler-proved pair layout.
 *
 * `key_offset` names an aligned `int32_t` member and `value_offset` names one
 * exact Array element member inside the uninitialized `pair_size` result. The
 * members must not overlap, and the pair size must be a multiple of its
 * alignment, as it is for a complete C object type.
 */
HXC_API hxc_status hxc_iterator_ref_create_array_pairs(
  hxc_array_ref *array,
  size_t pair_size,
  size_t pair_alignment,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
);

/** Retain or release one alias to the same cursor; NULL is a successful no-op. */
HXC_API hxc_status hxc_iterator_ref_retain(hxc_iterator_ref *iterator);
HXC_API hxc_status hxc_iterator_ref_release(hxc_iterator_ref *iterator);

/** Report whether the snapshot or current live Array has another element. */
HXC_API hxc_status hxc_iterator_ref_has_next(
  const hxc_iterator_ref *iterator,
  bool *out_has_next
);

/**
 * Move or copy the next exact element owner into uninitialized caller storage.
 *
 * Calling this operation after exhaustion is an invalid argument. On success
 * the shared cursor advances exactly once. Snapshot release no longer destroys
 * that yielded element; live Array iteration leaves the Array slot unchanged.
 */
HXC_API hxc_status hxc_iterator_ref_next_move(
  hxc_iterator_ref *iterator,
  void *out_element
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_ITERATOR_H_INCLUDED */
