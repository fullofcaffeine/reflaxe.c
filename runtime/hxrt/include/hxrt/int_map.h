/*
 * hxrt feature: int-map (compiler-selectable).
 *
 * This internal ABI preserves ordinary Haxe Map<Int, Bool> identity. Integer
 * keys and Bool values stay exact and unboxed, while lookup reports presence
 * separately so a stored false value is not confused with a missing key.
 */
#ifndef HXRT_INT_MAP_H_INCLUDED
#define HXRT_INT_MAP_H_INCLUDED

#include "hxrt/allocator.h"
#include "hxrt/iterator.h"
#include "hxrt/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_int_bool_map_ref hxc_int_bool_map_ref;

/** Create one empty shared Map<Int, Bool> using the supplied allocator. */
HXC_API hxc_status hxc_int_bool_map_ref_create(
  hxc_allocator allocator,
  hxc_int_bool_map_ref **out_map
);

/**
 * Retain or release one alias to the same mutable Haxe Map object.
 *
 * NULL is the exact absent `Null<Map<Int, Bool>>` carrier, so both operations
 * accept it as a successful no-op.
 */
HXC_API hxc_status hxc_int_bool_map_ref_retain(hxc_int_bool_map_ref *map);
HXC_API hxc_status hxc_int_bool_map_ref_release(hxc_int_bool_map_ref *map);

/** Copy every entry into one independent shared Map object. */
HXC_API hxc_status hxc_int_bool_map_ref_copy(
  const hxc_int_bool_map_ref *source,
  hxc_int_bool_map_ref **out_map
);

/**
 * Insert or replace one exact key/value pair.
 *
 * A stored false value is still present: `exists(key)` reports key membership,
 * not the truthiness of the associated value.
 */
HXC_API hxc_status hxc_int_bool_map_ref_set(
  hxc_int_bool_map_ref *map,
  int32_t key,
  bool value
);

/** Query membership without allocating or changing the table. */
HXC_API hxc_status hxc_int_bool_map_ref_exists(
  const hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_exists
);

/** Copy one unboxed value when present and report presence separately. */
HXC_API hxc_status hxc_int_bool_map_ref_get(
  const hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_value,
  bool *out_found
);

/** Remove one key and report whether it was present. */
HXC_API hxc_status hxc_int_bool_map_ref_remove(
  hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_removed
);

/** Remove every entry while preserving the shared Map object and its aliases. */
HXC_API hxc_status hxc_int_bool_map_ref_clear(hxc_int_bool_map_ref *map);

/** Snapshot the current values in table iteration order. */
HXC_API hxc_status hxc_int_bool_map_ref_value_iterator(
  hxc_int_bool_map_ref *map,
  hxc_iterator_ref **out_iterator
);

/** Snapshot the current keys in table iteration order. */
HXC_API hxc_status hxc_int_bool_map_ref_key_iterator(
  hxc_int_bool_map_ref *map,
  hxc_iterator_ref **out_iterator
);

/**
 * Snapshot `{key, value}` records using the exact generated aggregate layout.
 * Both fields are unboxed, so the resulting elements need no callbacks.
 */
HXC_API hxc_status hxc_int_bool_map_ref_pair_iterator(
  hxc_int_bool_map_ref *map,
  size_t pair_size,
  size_t pair_alignment,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
);

/** Format the map with the pinned Eval `[key => value]` spelling. */
HXC_API hxc_status hxc_int_bool_map_ref_to_string(
  const hxc_int_bool_map_ref *map,
  hxc_string *out_string
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_INT_MAP_H_INCLUDED */
