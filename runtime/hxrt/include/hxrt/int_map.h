/*
 * hxrt feature: int-map (compiler-selectable).
 *
 * This first internal ABI preserves ordinary Haxe Map<Int, Bool> identity.
 * Integer keys and Bool values stay exact and unboxed. The compiler currently
 * selects only construction, set, and membership; other IntMap methods remain
 * source-positioned unsupported operations rather than guessed runtime calls.
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

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_INT_MAP_H_INCLUDED */
