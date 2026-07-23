/*
 * hxrt feature: string-map (compiler-selectable).
 *
 * This internal ABI preserves ordinary Haxe Map<String, V> identity while the
 * compiler keeps V exact and unboxed. Keys are copied as canonical UTF-8 bytes,
 * so a table never borrows temporary String storage. Mutations are fail-atomic:
 * allocation failure may reserve spare capacity but never changes visible
 * entries. The first compiler slice admits trivially copied Bool values.
 */
#ifndef HXRT_STRING_MAP_H_INCLUDED
#define HXRT_STRING_MAP_H_INCLUDED

#include "hxrt/allocator.h"
#include "hxrt/string_literal.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_string_map_ref hxc_string_map_ref;

/** Create one empty shared table for an exact unboxed value layout. */
HXC_API hxc_status hxc_string_map_ref_create(
  hxc_allocator allocator,
  size_t value_size,
  size_t value_alignment,
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

/** Insert or replace one key after copying its UTF-8 bytes. */
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

/** Copy a value only when present and report presence through a separate flag. */
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
