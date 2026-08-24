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

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_INT_MAP_H_INCLUDED */
