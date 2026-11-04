/*
 * hxrt feature: typed-map (compiler-selectable dependency).
 *
 * ObjectMap and EnumValueMap use this private storage engine without erasing
 * their Haxe key semantics. Generated adapters supply one exact key hash,
 * equality, copy, destruction, and trace policy; values retain their exact C
 * layout and lifecycle. The runtime owns only checked table mechanics.
 */
#ifndef HXRT_TYPED_MAP_H_INCLUDED
#define HXRT_TYPED_MAP_H_INCLUDED

#include "hxrt/gc.h"
#include "hxrt/iterator.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_typed_map_ref hxc_typed_map_ref;

/** Copy-construct one exact key or value in uninitialized storage. */
typedef hxc_status (*hxc_typed_map_copy_fn)(
  void *context,
  void *destination,
  const void *source
);

/** Destroy one live exact key or value. */
typedef void (*hxc_typed_map_destroy_fn)(void *context, void *value);

/** Visit every collector object reachable from one exact stored value. */
typedef void (*hxc_typed_map_trace_fn)(
  void *context,
  const void *value,
  hxc_trace_visit_fn visit,
  void *visit_context
);

/** Hash one exact key. Equality, not the hash, remains authoritative. */
typedef uint64_t (*hxc_typed_map_hash_fn)(
  void *context,
  const void *key
);

/** Compare two exact keys under the compiler-selected Haxe key policy. */
typedef bool (*hxc_typed_map_equal_fn)(
  void *context,
  const void *left,
  const void *right
);

/** Exact key layout, identity/equality policy, lifecycle, and root tracing. */
typedef struct hxc_typed_map_key_ops {
  size_t size;
  size_t alignment;
  void *context;
  hxc_typed_map_copy_fn copy;
  hxc_typed_map_destroy_fn destroy;
  hxc_typed_map_trace_fn trace;
  hxc_typed_map_hash_fn hash;
  hxc_typed_map_equal_fn equal;
} hxc_typed_map_key_ops;

/** Exact unboxed value layout, replacement policy, and root tracing. */
typedef struct hxc_typed_map_value_ops {
  size_t size;
  size_t alignment;
  void *context;
  hxc_typed_map_copy_fn copy;
  hxc_typed_map_destroy_fn destroy;
  hxc_typed_map_trace_fn trace;
} hxc_typed_map_value_ops;

/** Validate complete key policy and paired lifecycle callbacks. */
HXC_API bool hxc_typed_map_key_ops_is_valid(
  const hxc_typed_map_key_ops *keys
);

/** Validate exact value layout and either zero or two lifecycle callbacks. */
HXC_API bool hxc_typed_map_value_ops_is_valid(
  const hxc_typed_map_value_ops *values
);

/** Hash one canonical managed identity without exposing pointer arithmetic. */
HXC_API uint64_t hxc_typed_map_identity_hash(const void *identity);

/** Mix one exact typed hash contribution into a stable fixed-width state. */
HXC_API uint64_t hxc_typed_map_hash_mix(uint64_t state, uint64_t value);

/**
 * Descriptor for one collector-owned generic table payload.
 *
 * The descriptor's trace and finalizer read the exact immutable operation
 * policies stored during initialization. It contains no Dynamic metadata.
 */
HXC_API const hxc_type_descriptor *hxc_typed_map_type_descriptor(void);

/** Create one reference-counted map whose key/value policies need no GC roots. */
HXC_API hxc_status hxc_typed_map_ref_create(
  hxc_allocator allocator,
  hxc_typed_map_key_ops keys,
  hxc_typed_map_value_ops values,
  hxc_typed_map_ref **out_map
);

/** Initialize storage already allocated by the precise collector. */
HXC_API hxc_status hxc_typed_map_init_collector_owned(
  hxc_gc *collector,
  hxc_allocator allocator,
  hxc_typed_map_key_ops keys,
  hxc_typed_map_value_ops values,
  hxc_typed_map_ref *map
);

/** Dispose slots and owned non-GC values without freeing collector storage. */
HXC_API hxc_status hxc_typed_map_dispose_in_place(hxc_typed_map_ref *map);

/** Retain or release one alias to a reference-counted map. */
HXC_API hxc_status hxc_typed_map_ref_retain(hxc_typed_map_ref *map);
HXC_API hxc_status hxc_typed_map_ref_release(hxc_typed_map_ref *map);

/** Copy into a newly allocated independent reference-counted map. */
HXC_API hxc_status hxc_typed_map_ref_copy(
  const hxc_typed_map_ref *source,
  hxc_typed_map_ref **out_map
);

/** Copy entries into an initialized empty collector-owned map. */
HXC_API hxc_status hxc_typed_map_copy_in_place(
  const hxc_typed_map_ref *source,
  hxc_typed_map_ref *destination
);

/** Insert or atomically replace one exact key/value pair. */
HXC_API hxc_status hxc_typed_map_ref_set_copy(
  hxc_typed_map_ref *map,
  const void *key,
  const void *value
);

/** Query membership through the exact generated equality policy. */
HXC_API hxc_status hxc_typed_map_ref_exists(
  const hxc_typed_map_ref *map,
  const void *key,
  bool *out_exists
);

/** Copy one value when present and report presence separately. */
HXC_API hxc_status hxc_typed_map_ref_get_copy(
  const hxc_typed_map_ref *map,
  const void *key,
  void *out_value,
  bool *out_found
);

/** Remove one equal key and destroy that slot exactly once. */
HXC_API hxc_status hxc_typed_map_ref_remove(
  hxc_typed_map_ref *map,
  const void *key,
  bool *out_removed
);

/** Destroy every occupied slot while preserving aliases to the map object. */
HXC_API hxc_status hxc_typed_map_ref_clear(hxc_typed_map_ref *map);

/** Snapshot exact values using the compiler-provided Iterator element policy. */
HXC_API hxc_status hxc_typed_map_ref_value_iterator(
  hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  hxc_iterator_ref **out_iterator
);

/** Snapshot exact keys using the compiler-provided Iterator element policy. */
HXC_API hxc_status hxc_typed_map_ref_key_iterator(
  hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  hxc_iterator_ref **out_iterator
);

/** Snapshot exact `{key, value}` records in the generated aggregate layout. */
HXC_API hxc_status hxc_typed_map_ref_pair_iterator(
  hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_TYPED_MAP_H_INCLUDED */
