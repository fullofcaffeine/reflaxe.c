/*
 * hxrt feature: string-map (compiler-selectable).
 *
 * This internal ABI preserves ordinary Haxe Map<String, V> identity while the
 * compiler keeps V exact and unboxed. Keys are copied as canonical UTF-8 bytes,
 * so a table never borrows temporary String storage. Mutations are fail-atomic:
 * allocation failure may reserve spare capacity but never changes visible
 * entries. Plain values copy as bytes; managed direct values provide one
 * complete copy/assign/destroy strategy so every table slot owns its contents.
 */
#ifndef HXRT_STRING_MAP_H_INCLUDED
#define HXRT_STRING_MAP_H_INCLUDED

#include "hxrt/allocator.h"
#include "hxrt/string_literal.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_string_map_ref hxc_string_map_ref;

/**
 * Construct one value in uninitialized slot or lookup-result storage.
 *
 * On failure no live value remains at the destination and the source is
 * unchanged.
 */
typedef hxc_status (*hxc_string_map_value_copy_fn)(
  void *context,
  void *destination,
  const void *source
);

/**
 * Replace one live slot without changing either value on failure.
 *
 * Reference-like implementations retain the source before destroying the old
 * destination, including when both values contain the same nested owner.
 */
typedef hxc_status (*hxc_string_map_value_assign_fn)(
  void *context,
  void *destination,
  const void *source
);

/** Destroy one live stored or copied value. Destruction cannot fail. */
typedef void (*hxc_string_map_value_destroy_fn)(
  void *context,
  void *value
);

/**
 * Exact unboxed storage and lifetime policy for one Map<String, V>.
 *
 * Trivial byte-relocatable values use three null callbacks. Managed direct
 * values use all three callbacks. Context and callback code must outlive the
 * map, and callbacks must not re-enter or mutate that map.
 */
typedef struct hxc_string_map_value_ops {
  size_t size;
  size_t alignment;
  void *context;
  hxc_string_map_value_copy_fn copy;
  hxc_string_map_value_assign_fn assign;
  hxc_string_map_value_destroy_fn destroy;
} hxc_string_map_value_ops;

/** Accept a non-zero layout and either zero or three lifecycle callbacks. */
HXC_API bool hxc_string_map_value_ops_is_valid(
  const hxc_string_map_value_ops *values
);

/** Create one empty shared table for a trivial exact unboxed value layout. */
HXC_API hxc_status hxc_string_map_ref_create(
  hxc_allocator allocator,
  size_t value_size,
  size_t value_alignment,
  hxc_string_map_ref **out_map
);

/** Create one empty shared table with a complete managed value policy. */
HXC_API hxc_status hxc_string_map_ref_create_with_ops(
  hxc_allocator allocator,
  hxc_string_map_value_ops values,
  hxc_string_map_ref **out_map
);

/**
 * Retain/release one alias to the same mutable Haxe Map object.
 *
 * NULL is the exact absent `Null<Map<String, V>>` carrier, so both lifecycle
 * operations accept it as a successful no-op. Invalid non-NULL objects remain
 * errors.
 */
HXC_API hxc_status hxc_string_map_ref_retain(hxc_string_map_ref *map);
HXC_API hxc_status hxc_string_map_ref_release(hxc_string_map_ref *map);

/**
 * Insert or replace one key after copying its UTF-8 bytes and value.
 *
 * A failed key allocation or value callback leaves every visible entry
 * unchanged. Capacity may grow before the later operation fails.
 */
HXC_API hxc_status hxc_string_map_ref_set_copy(
  hxc_string_map_ref *map,
  hxc_string key,
  const void *value
);

/** Query membership without conflating a stored false value with absence. */
HXC_API hxc_status hxc_string_map_ref_exists(
  const hxc_string_map_ref *map,
  hxc_string key,
  bool *out_exists
);

/**
 * Copy-construct a value only when present and report presence separately.
 *
 * Present output is a new owner that the caller must eventually destroy with
 * the same value policy. On callback failure neither output is changed.
 */
HXC_API hxc_status hxc_string_map_ref_get_copy(
  const hxc_string_map_ref *map,
  hxc_string key,
  void *out_value,
  bool *out_found
);

/** Remove one key, or clear all entries, while preserving the table object. */
HXC_API hxc_status hxc_string_map_ref_remove(
  hxc_string_map_ref *map,
  hxc_string key,
  bool *out_removed
);
HXC_API hxc_status hxc_string_map_ref_clear(hxc_string_map_ref *map);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STRING_MAP_H_INCLUDED */
