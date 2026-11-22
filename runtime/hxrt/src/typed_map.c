/*
 * hxrt feature `typed-map`: checked open-addressed storage shared by
 * compiler-specialized ObjectMap and EnumValueMap families.
 *
 * Key meaning stays in generated typed callbacks. This module owns allocation,
 * collisions, rollback, mutation, and snapshots only.
 */
#include "hxrt/typed_map.h"

#include <string.h>

enum {
  HXC_TYPED_MAP_EMPTY = 0,
  HXC_TYPED_MAP_OCCUPIED = 1,
  HXC_TYPED_MAP_TOMBSTONE = 2
};

struct hxc_typed_map_ref {
  size_t references;
  size_t length;
  size_t capacity;
  size_t slot_size;
  size_t slot_alignment;
  size_t hash_offset;
  size_t key_offset;
  size_t value_offset;
  hxc_allocator allocator;
  hxc_allocation slots;
  hxc_typed_map_key_ops keys;
  hxc_typed_map_value_ops values;
  hxc_gc *collector;
  bool collector_owned;
  bool initialized;
};

typedef enum hxc_typed_map_iterator_mode {
  HXC_TYPED_MAP_ITERATOR_VALUE,
  HXC_TYPED_MAP_ITERATOR_KEY,
  HXC_TYPED_MAP_ITERATOR_PAIR
} hxc_typed_map_iterator_mode;

typedef struct hxc_typed_map_iterator_fill {
  const hxc_typed_map_ref *map;
  size_t slot;
  size_t key_offset;
  size_t value_offset;
  hxc_typed_map_iterator_mode mode;
} hxc_typed_map_iterator_fill;

typedef struct hxc_typed_map_iterator_trace {
  const hxc_typed_map_ref *map;
  size_t key_offset;
  size_t value_offset;
  hxc_typed_map_iterator_mode mode;
} hxc_typed_map_iterator_trace;

/** GC registration storage owned only by the collector-backed producer. */
typedef struct hxc_typed_map_iterator_roots {
  hxc_allocator allocator;
  hxc_gc_root_table table;
} hxc_typed_map_iterator_roots;

static bool hxc_typed_map_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

static bool hxc_typed_map_checked_add(
  size_t left,
  size_t right,
  size_t *out_value
) {
  if (out_value == NULL || left > SIZE_MAX - right) {
    return false;
  }
  *out_value = left + right;
  return true;
}

static bool hxc_typed_map_align_up(
  size_t value,
  size_t alignment,
  size_t *out_value
) {
  const size_t mask = alignment - 1u;
  size_t with_mask;
  if (!hxc_typed_map_power_of_two(alignment)
    || !hxc_typed_map_checked_add(value, mask, &with_mask)) {
    return false;
  }
  *out_value = with_mask & ~mask;
  return true;
}

bool hxc_typed_map_key_ops_is_valid(const hxc_typed_map_key_ops *keys) {
  return keys != NULL
    && keys->size != 0u
    && hxc_typed_map_power_of_two(keys->alignment)
    && ((keys->copy == NULL && keys->destroy == NULL)
      || (keys->copy != NULL && keys->destroy != NULL))
    && keys->hash != NULL
    && keys->equal != NULL;
}

bool hxc_typed_map_value_ops_is_valid(
  const hxc_typed_map_value_ops *values
) {
  return values != NULL
    && values->size != 0u
    && hxc_typed_map_power_of_two(values->alignment)
    && ((values->copy == NULL && values->destroy == NULL)
      || (values->copy != NULL && values->destroy != NULL));
}

uint64_t hxc_typed_map_hash_mix(uint64_t state, uint64_t value) {
  value += UINT64_C(0x9e3779b97f4a7c15);
  value = (value ^ (value >> 30u)) * UINT64_C(0xbf58476d1ce4e5b9);
  value = (value ^ (value >> 27u)) * UINT64_C(0x94d049bb133111eb);
  value ^= value >> 31u;
  return state ^ (value + UINT64_C(0x9e3779b97f4a7c15)
    + (state << 6u) + (state >> 2u));
}

uint64_t hxc_typed_map_identity_hash(const void *identity) {
  return hxc_typed_map_hash_mix(
    UINT64_C(0x6a09e667f3bcc909),
    (uint64_t)(uintptr_t)identity
  );
}

static hxc_status hxc_typed_map_plan_layout(hxc_typed_map_ref *map) {
  size_t offset = 1u;
  size_t alignment = HXC_ALIGNOF(uint64_t);
  if (map->keys.alignment > alignment) {
    alignment = map->keys.alignment;
  }
  if (map->values.alignment > alignment) {
    alignment = map->values.alignment;
  }
  if (!hxc_typed_map_align_up(offset, HXC_ALIGNOF(uint64_t), &offset)) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  map->hash_offset = offset;
  if (!hxc_typed_map_checked_add(offset, sizeof(uint64_t), &offset)
    || !hxc_typed_map_align_up(offset, map->keys.alignment, &offset)) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  map->key_offset = offset;
  if (!hxc_typed_map_checked_add(offset, map->keys.size, &offset)
    || !hxc_typed_map_align_up(offset, map->values.alignment, &offset)) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  map->value_offset = offset;
  if (!hxc_typed_map_checked_add(offset, map->values.size, &offset)
    || !hxc_typed_map_align_up(offset, alignment, &offset)) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  map->slot_size = offset;
  map->slot_alignment = alignment;
  return HXC_STATUS_OK;
}

static unsigned char *hxc_typed_map_slot(
  const hxc_typed_map_ref *map,
  size_t index
) {
  return (unsigned char *)map->slots.memory + (index * map->slot_size);
}

static uint8_t *hxc_typed_map_slot_state(
  const hxc_typed_map_ref *map,
  size_t index
) {
  return hxc_typed_map_slot(map, index);
}

static uint64_t *hxc_typed_map_slot_hash(
  const hxc_typed_map_ref *map,
  size_t index
) {
  return (uint64_t *)(void *)(hxc_typed_map_slot(map, index) + map->hash_offset);
}

static void *hxc_typed_map_slot_key(
  const hxc_typed_map_ref *map,
  size_t index
) {
  return hxc_typed_map_slot(map, index) + map->key_offset;
}

static void *hxc_typed_map_slot_value(
  const hxc_typed_map_ref *map,
  size_t index
) {
  return hxc_typed_map_slot(map, index) + map->value_offset;
}

static bool hxc_typed_map_is_valid(const hxc_typed_map_ref *map) {
  return map != NULL
    && map->initialized
    && ((map->collector_owned
        && map->references == 0u
        && map->collector != NULL)
      || (!map->collector_owned
        && map->references > 0u
        && map->collector == NULL))
    && hxc_allocator_is_valid(&map->allocator)
    && hxc_typed_map_key_ops_is_valid(&map->keys)
    && hxc_typed_map_value_ops_is_valid(&map->values)
    && map->slot_size != 0u
    && hxc_typed_map_power_of_two(map->slot_alignment)
    && map->length <= map->capacity
    && ((map->capacity == 0u
        && map->slots.memory == NULL
        && map->slots.size == 0u)
      || (hxc_typed_map_power_of_two(map->capacity)
        && hxc_allocation_is_valid(&map->slots)));
}

static hxc_status hxc_typed_map_copy_key(
  const hxc_typed_map_ref *map,
  void *destination,
  const void *source
) {
  if (map->keys.copy != NULL) {
    return map->keys.copy(map->keys.context, destination, source);
  }
  memcpy(destination, source, map->keys.size);
  return HXC_STATUS_OK;
}

static hxc_status hxc_typed_map_copy_value(
  const hxc_typed_map_ref *map,
  void *destination,
  const void *source
) {
  if (map->values.copy != NULL) {
    return map->values.copy(map->values.context, destination, source);
  }
  memcpy(destination, source, map->values.size);
  return HXC_STATUS_OK;
}

static void hxc_typed_map_destroy_slot(
  hxc_typed_map_ref *map,
  size_t index
) {
  if (map->values.destroy != NULL) {
    map->values.destroy(map->values.context, hxc_typed_map_slot_value(map, index));
  }
  if (map->keys.destroy != NULL) {
    map->keys.destroy(map->keys.context, hxc_typed_map_slot_key(map, index));
  }
}

static size_t hxc_typed_map_find_slot(
  const hxc_typed_map_ref *map,
  const void *key,
  uint64_t hash,
  bool *out_found
) {
  size_t index = (size_t)hash & (map->capacity - 1u);
  size_t first_tombstone = SIZE_MAX;
  for (;;) {
    const uint8_t state = *hxc_typed_map_slot_state(map, index);
    if (state == HXC_TYPED_MAP_EMPTY) {
      *out_found = false;
      return first_tombstone == SIZE_MAX ? index : first_tombstone;
    }
    if (state == HXC_TYPED_MAP_TOMBSTONE) {
      if (first_tombstone == SIZE_MAX) {
        first_tombstone = index;
      }
    } else if (*hxc_typed_map_slot_hash(map, index) == hash
      && map->keys.equal(
        map->keys.context,
        hxc_typed_map_slot_key(map, index),
        key
      )) {
      *out_found = true;
      return index;
    }
    index = (index + 1u) & (map->capacity - 1u);
  }
}

static hxc_status hxc_typed_map_reserve(
  hxc_typed_map_ref *map,
  size_t requested_capacity
) {
  hxc_allocation replacement = HXC_ALLOCATION_INITIALIZER;
  hxc_allocation prior;
  size_t capacity = 8u;
  size_t index;
  hxc_status status;
  if (!hxc_typed_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  while (capacity < requested_capacity) {
    if (capacity > SIZE_MAX / 2u) {
      return HXC_STATUS_SIZE_OVERFLOW;
    }
    capacity *= 2u;
  }
  if (capacity == map->capacity) {
    return HXC_STATUS_OK;
  }
  status = hxc_allocation_allocate(
    &map->allocator,
    capacity,
    map->slot_size,
    map->slot_alignment,
    &replacement
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  memset(replacement.memory, 0, replacement.size);
  prior = map->slots;
  map->slots = replacement;
  map->capacity = capacity;
  if (prior.memory != NULL) {
    const size_t prior_capacity = prior.size / map->slot_size;
    for (index = 0u; index < prior_capacity; index++) {
      unsigned char *source = (unsigned char *)prior.memory
        + (index * map->slot_size);
      if (*source == HXC_TYPED_MAP_OCCUPIED) {
        const uint64_t hash = *(uint64_t *)(void *)(source + map->hash_offset);
        bool found = false;
        const size_t destination = hxc_typed_map_find_slot(
          map,
          source + map->key_offset,
          hash,
          &found
        );
        memcpy(hxc_typed_map_slot(map, destination), source, map->slot_size);
      }
    }
    status = hxc_allocation_dispose(&prior);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  return HXC_STATUS_OK;
}

static hxc_status hxc_typed_map_initialize(
  hxc_gc *collector,
  bool collector_owned,
  hxc_allocator allocator,
  hxc_typed_map_key_ops keys,
  hxc_typed_map_value_ops values,
  hxc_typed_map_ref *map
) {
  hxc_status status;
  if (map == NULL
    || map->initialized
    || !hxc_allocator_is_valid(&allocator)
    || !hxc_typed_map_key_ops_is_valid(&keys)
    || !hxc_typed_map_value_ops_is_valid(&values)
    || collector_owned != (collector != NULL)
    || (!collector_owned && (keys.trace != NULL || values.trace != NULL))) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *map = (hxc_typed_map_ref){0};
  map->references = collector_owned ? 0u : 1u;
  map->allocator = allocator;
  map->slots = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  map->keys = keys;
  map->values = values;
  map->collector = collector;
  map->collector_owned = collector_owned;
  map->initialized = true;
  status = hxc_typed_map_plan_layout(map);
  if (status != HXC_STATUS_OK) {
    *map = (hxc_typed_map_ref){0};
  }
  return status;
}

static void hxc_typed_map_trace(
  const void *object,
  hxc_trace_visit_fn visit,
  void *visit_context
) {
  const hxc_typed_map_ref *map = object;
  size_t index;
  if (!hxc_typed_map_is_valid(map) || visit == NULL) {
    return;
  }
  for (index = 0u; index < map->capacity; index++) {
    if (*hxc_typed_map_slot_state(map, index) != HXC_TYPED_MAP_OCCUPIED) {
      continue;
    }
    if (map->keys.trace != NULL) {
      map->keys.trace(
        map->keys.context,
        hxc_typed_map_slot_key(map, index),
        visit,
        visit_context
      );
    }
    if (map->values.trace != NULL) {
      map->values.trace(
        map->values.context,
        hxc_typed_map_slot_value(map, index),
        visit,
        visit_context
      );
    }
  }
}

static void hxc_typed_map_finalize(void *object) {
  (void)hxc_typed_map_dispose_in_place(object);
}

static const hxc_type_descriptor HXC_TYPED_MAP_DESCRIPTOR = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER,
  sizeof(hxc_typed_map_ref),
  HXC_ALIGNOF(hxc_typed_map_ref),
  hxc_typed_map_trace,
  hxc_typed_map_finalize
};

const hxc_type_descriptor *hxc_typed_map_type_descriptor(void) {
  return &HXC_TYPED_MAP_DESCRIPTOR;
}

hxc_status hxc_typed_map_ref_create(
  hxc_allocator allocator,
  hxc_typed_map_key_ops keys,
  hxc_typed_map_value_ops values,
  hxc_typed_map_ref **out_map
) {
  hxc_typed_map_ref *map = NULL;
  hxc_status status;
  if (out_map == NULL || *out_map != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alloc(
    &allocator,
    sizeof(hxc_typed_map_ref),
    HXC_ALIGNOF(hxc_typed_map_ref),
    (void **)&map
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *map = (hxc_typed_map_ref){0};
  status = hxc_typed_map_initialize(NULL, false, allocator, keys, values, map);
  if (status != HXC_STATUS_OK) {
    (void)hxc_free(
      &allocator,
      map,
      sizeof(hxc_typed_map_ref),
      HXC_ALIGNOF(hxc_typed_map_ref)
    );
    return status;
  }
  *out_map = map;
  return HXC_STATUS_OK;
}

hxc_status hxc_typed_map_init_collector_owned(
  hxc_gc *collector,
  hxc_allocator allocator,
  hxc_typed_map_key_ops keys,
  hxc_typed_map_value_ops values,
  hxc_typed_map_ref *map
) {
  return hxc_typed_map_initialize(
    collector,
    true,
    allocator,
    keys,
    values,
    map
  );
}

hxc_status hxc_typed_map_dispose_in_place(hxc_typed_map_ref *map) {
  hxc_status status;
  if (!hxc_typed_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_typed_map_ref_clear(map);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_dispose(&map->slots);
  if (status == HXC_STATUS_OK) {
    *map = (hxc_typed_map_ref){0};
  }
  return status;
}

hxc_status hxc_typed_map_ref_retain(hxc_typed_map_ref *map) {
  if (map == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_typed_map_is_valid(map)
    || map->collector_owned
    || map->references == SIZE_MAX) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  map->references++;
  return HXC_STATUS_OK;
}

hxc_status hxc_typed_map_ref_release(hxc_typed_map_ref *map) {
  hxc_allocator allocator;
  hxc_status status;
  if (map == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_typed_map_is_valid(map) || map->collector_owned) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->references > 1u) {
    map->references--;
    return HXC_STATUS_OK;
  }
  allocator = map->allocator;
  status = hxc_typed_map_dispose_in_place(map);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  return hxc_free(
    &allocator,
    map,
    sizeof(hxc_typed_map_ref),
    HXC_ALIGNOF(hxc_typed_map_ref)
  );
}

hxc_status hxc_typed_map_ref_set_copy(
  hxc_typed_map_ref *map,
  const void *key,
  const void *value
) {
  uint64_t hash;
  size_t index;
  bool found;
  hxc_status status;
  uint8_t prior_state;
  if (!hxc_typed_map_is_valid(map) || key == NULL || value == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  hash = map->keys.hash(map->keys.context, key);
  if (map->capacity != 0u) {
    index = hxc_typed_map_find_slot(map, key, hash, &found);
    if (found) {
      hxc_allocation staged = HXC_ALLOCATION_INITIALIZER;
      if (map->keys.copy == NULL && map->values.copy == NULL) {
        memcpy(hxc_typed_map_slot_key(map, index), key, map->keys.size);
        memcpy(hxc_typed_map_slot_value(map, index), value, map->values.size);
        *hxc_typed_map_slot_hash(map, index) = hash;
        return HXC_STATUS_OK;
      }
      status = hxc_allocation_allocate(
        &map->allocator,
        1u,
        map->slot_size,
        map->slot_alignment,
        &staged
      );
      if (status != HXC_STATUS_OK) {
        return status;
      }
      memset(staged.memory, 0, staged.size);
      status = hxc_typed_map_copy_key(
        map,
        (unsigned char *)staged.memory + map->key_offset,
        key
      );
      if (status == HXC_STATUS_OK) {
        status = hxc_typed_map_copy_value(
          map,
          (unsigned char *)staged.memory + map->value_offset,
          value
        );
        if (status != HXC_STATUS_OK && map->keys.destroy != NULL) {
          map->keys.destroy(
            map->keys.context,
            (unsigned char *)staged.memory + map->key_offset
          );
        }
      }
      if (status != HXC_STATUS_OK) {
        (void)hxc_allocation_dispose(&staged);
        return status;
      }
      hxc_typed_map_destroy_slot(map, index);
      memcpy(
        hxc_typed_map_slot_key(map, index),
        (unsigned char *)staged.memory + map->key_offset,
        map->keys.size
      );
      memcpy(
        hxc_typed_map_slot_value(map, index),
        (unsigned char *)staged.memory + map->value_offset,
        map->values.size
      );
      *hxc_typed_map_slot_hash(map, index) = hash;
      (void)hxc_allocation_dispose(&staged);
      return HXC_STATUS_OK;
    }
  }
  if (map->capacity == 0u
    || map->length >= map->capacity - (map->capacity / 4u)) {
    if (map->capacity > SIZE_MAX / 2u) {
      return HXC_STATUS_SIZE_OVERFLOW;
    }
    status = hxc_typed_map_reserve(
      map,
      map->capacity == 0u ? 8u : map->capacity * 2u
    );
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  index = hxc_typed_map_find_slot(map, key, hash, &found);
  if (found) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  prior_state = *hxc_typed_map_slot_state(map, index);
  status = hxc_typed_map_copy_key(
    map,
    hxc_typed_map_slot_key(map, index),
    key
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_typed_map_copy_value(
    map,
    hxc_typed_map_slot_value(map, index),
    value
  );
  if (status != HXC_STATUS_OK) {
    if (map->keys.destroy != NULL) {
      map->keys.destroy(
        map->keys.context,
        hxc_typed_map_slot_key(map, index)
      );
    }
    *hxc_typed_map_slot_state(map, index) = prior_state;
    return status;
  }
  *hxc_typed_map_slot_hash(map, index) = hash;
  *hxc_typed_map_slot_state(map, index) = HXC_TYPED_MAP_OCCUPIED;
  map->length++;
  return HXC_STATUS_OK;
}

hxc_status hxc_typed_map_ref_exists(
  const hxc_typed_map_ref *map,
  const void *key,
  bool *out_exists
) {
  bool found = false;
  if (!hxc_typed_map_is_valid(map) || key == NULL || out_exists == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->capacity != 0u) {
    const uint64_t hash = map->keys.hash(map->keys.context, key);
    (void)hxc_typed_map_find_slot(map, key, hash, &found);
  }
  *out_exists = found;
  return HXC_STATUS_OK;
}

hxc_status hxc_typed_map_ref_get_copy(
  const hxc_typed_map_ref *map,
  const void *key,
  void *out_value,
  bool *out_found
) {
  bool found = false;
  size_t index = 0u;
  hxc_status status = HXC_STATUS_OK;
  if (!hxc_typed_map_is_valid(map)
    || key == NULL
    || out_value == NULL
    || out_found == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->capacity != 0u) {
    const uint64_t hash = map->keys.hash(map->keys.context, key);
    index = hxc_typed_map_find_slot(map, key, hash, &found);
  }
  if (found) {
    status = hxc_typed_map_copy_value(
      map,
      out_value,
      hxc_typed_map_slot_value(map, index)
    );
  }
  if (status == HXC_STATUS_OK) {
    *out_found = found;
  }
  return status;
}

hxc_status hxc_typed_map_ref_remove(
  hxc_typed_map_ref *map,
  const void *key,
  bool *out_removed
) {
  bool found = false;
  size_t index = 0u;
  if (!hxc_typed_map_is_valid(map) || key == NULL || out_removed == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->capacity != 0u) {
    const uint64_t hash = map->keys.hash(map->keys.context, key);
    index = hxc_typed_map_find_slot(map, key, hash, &found);
  }
  if (found) {
    hxc_typed_map_destroy_slot(map, index);
    *hxc_typed_map_slot_state(map, index) = HXC_TYPED_MAP_TOMBSTONE;
    map->length--;
  }
  *out_removed = found;
  return HXC_STATUS_OK;
}

hxc_status hxc_typed_map_ref_clear(hxc_typed_map_ref *map) {
  size_t index;
  if (!hxc_typed_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  for (index = 0u; index < map->capacity; index++) {
    if (*hxc_typed_map_slot_state(map, index) == HXC_TYPED_MAP_OCCUPIED) {
      hxc_typed_map_destroy_slot(map, index);
    }
  }
  if (map->slots.memory != NULL) {
    memset(map->slots.memory, 0, map->slots.size);
  }
  map->length = 0u;
  return HXC_STATUS_OK;
}

hxc_status hxc_typed_map_copy_in_place(
  const hxc_typed_map_ref *source,
  hxc_typed_map_ref *destination
) {
  hxc_status status = HXC_STATUS_OK;
  size_t index;
  if (!hxc_typed_map_is_valid(source)
    || !hxc_typed_map_is_valid(destination)
    || destination->length != 0u
    || destination->capacity != 0u
    || destination->keys.size != source->keys.size
    || destination->values.size != source->values.size
    || destination->keys.hash != source->keys.hash
    || destination->keys.equal != source->keys.equal) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (source->capacity != 0u) {
    status = hxc_typed_map_reserve(destination, source->capacity);
  }
  for (index = 0u;
    status == HXC_STATUS_OK && index < source->capacity;
    index++) {
    if (*hxc_typed_map_slot_state(source, index) == HXC_TYPED_MAP_OCCUPIED) {
      status = hxc_typed_map_ref_set_copy(
        destination,
        hxc_typed_map_slot_key(source, index),
        hxc_typed_map_slot_value(source, index)
      );
    }
  }
  if (status != HXC_STATUS_OK) {
    (void)hxc_typed_map_ref_clear(destination);
  }
  return status;
}

hxc_status hxc_typed_map_ref_copy(
  const hxc_typed_map_ref *source,
  hxc_typed_map_ref **out_map
) {
  hxc_typed_map_ref *copy = NULL;
  hxc_status status;
  hxc_status cleanup_status;
  if (!hxc_typed_map_is_valid(source)
    || source->collector_owned
    || out_map == NULL
    || *out_map != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_typed_map_ref_create(
    source->allocator,
    source->keys,
    source->values,
    &copy
  );
  if (status == HXC_STATUS_OK) {
    status = hxc_typed_map_copy_in_place(source, copy);
  }
  if (status != HXC_STATUS_OK) {
    cleanup_status = hxc_typed_map_ref_release(copy);
    return cleanup_status == HXC_STATUS_OK ? status : cleanup_status;
  }
  *out_map = copy;
  return HXC_STATUS_OK;
}

static bool hxc_typed_map_pair_layout_is_valid(
  const hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  size_t key_offset,
  size_t value_offset
) {
  return key_offset <= elements.size - map->keys.size
    && value_offset <= elements.size - map->values.size
    && key_offset % map->keys.alignment == 0u
    && value_offset % map->values.alignment == 0u
    && (key_offset + map->keys.size <= value_offset
      || value_offset + map->values.size <= key_offset);
}

static hxc_status hxc_typed_map_iterator_fill_next(
  void *context,
  void *destination
) {
  hxc_typed_map_iterator_fill *fill = context;
  while (fill->slot < fill->map->capacity) {
    const size_t slot = fill->slot++;
    hxc_status status;
    if (*hxc_typed_map_slot_state(fill->map, slot)
      != HXC_TYPED_MAP_OCCUPIED) {
      continue;
    }
    if (fill->mode == HXC_TYPED_MAP_ITERATOR_VALUE) {
      return hxc_typed_map_copy_value(
        fill->map,
        destination,
        hxc_typed_map_slot_value(fill->map, slot)
      );
    }
    status = hxc_typed_map_copy_key(
      fill->map,
      fill->mode == HXC_TYPED_MAP_ITERATOR_KEY
        ? destination
        : (unsigned char *)destination + fill->key_offset,
      hxc_typed_map_slot_key(fill->map, slot)
    );
    if (status != HXC_STATUS_OK
      || fill->mode == HXC_TYPED_MAP_ITERATOR_KEY) {
      return status;
    }
    status = hxc_typed_map_copy_value(
      fill->map,
      (unsigned char *)destination + fill->value_offset,
      hxc_typed_map_slot_value(fill->map, slot)
    );
    if (status != HXC_STATUS_OK && fill->map->keys.destroy != NULL) {
      fill->map->keys.destroy(
        fill->map->keys.context,
        (unsigned char *)destination + fill->key_offset
      );
    }
    return status;
  }
  return HXC_STATUS_INVALID_ARGUMENT;
}

/** Register roots on the map's collector without coupling plain iterators to it. */
static hxc_status hxc_typed_map_register_iterator_roots(
  void *context,
  const void **slots,
  size_t slot_count,
  void **out_registration
) {
  hxc_typed_map_ref *map = context;
  hxc_typed_map_iterator_roots *registration = NULL;
  hxc_status status;
  if (!hxc_typed_map_is_valid(map)
    || !map->collector_owned
    || map->collector == NULL
    || out_registration == NULL
    || *out_registration != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alloc(
    &map->allocator,
    sizeof(hxc_typed_map_iterator_roots),
    HXC_ALIGNOF(hxc_typed_map_iterator_roots),
    (void **)&registration
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *registration = (hxc_typed_map_iterator_roots){
    map->allocator,
    HXC_GC_ROOT_TABLE_INITIALIZER
  };
  status = hxc_gc_root_table_register(
    map->collector,
    slots,
    slot_count,
    &registration->table
  );
  if (status != HXC_STATUS_OK) {
    (void)hxc_free(
      &registration->allocator,
      registration,
      sizeof(hxc_typed_map_iterator_roots),
      HXC_ALIGNOF(hxc_typed_map_iterator_roots)
    );
    return status;
  }
  *out_registration = registration;
  return HXC_STATUS_OK;
}

/** Match the registration callback while keeping its owner in typed-map. */
static hxc_status hxc_typed_map_unregister_iterator_roots(
  void *registration_value
) {
  hxc_typed_map_iterator_roots *registration = registration_value;
  hxc_allocator allocator;
  hxc_status status;
  if (registration == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  allocator = registration->allocator;
  status = hxc_gc_root_table_unregister(&registration->table);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  return hxc_free(
    &allocator,
    registration,
    sizeof(hxc_typed_map_iterator_roots),
    HXC_ALIGNOF(hxc_typed_map_iterator_roots)
  );
}

/** Trace one completed snapshot element through the map's exact policies. */
static void hxc_typed_map_trace_iterator_element(
  void *context,
  const void *element,
  hxc_iterator_root_visit_fn visit,
  void *visit_context
) {
  const hxc_typed_map_iterator_trace *trace = context;
  const unsigned char *bytes = element;
  if (trace == NULL || trace->map == NULL || element == NULL || visit == NULL) {
    return;
  }
  if (trace->mode != HXC_TYPED_MAP_ITERATOR_VALUE
    && trace->map->keys.trace != NULL) {
    trace->map->keys.trace(
      trace->map->keys.context,
      bytes + trace->key_offset,
      visit,
      visit_context
    );
  }
  if (trace->mode != HXC_TYPED_MAP_ITERATOR_KEY
    && trace->map->values.trace != NULL) {
    trace->map->values.trace(
      trace->map->values.context,
      bytes + trace->value_offset,
      visit,
      visit_context
    );
  }
}

static hxc_status hxc_typed_map_iterator(
  hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  hxc_typed_map_iterator_mode mode,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
) {
  hxc_typed_map_iterator_fill fill;
  hxc_typed_map_iterator_trace trace;
  bool needs_trace;
  if (!hxc_typed_map_is_valid(map)
    || !hxc_iterator_element_ops_is_valid(&elements)
    || out_iterator == NULL
    || *out_iterator != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if ((mode == HXC_TYPED_MAP_ITERATOR_KEY
      && (elements.size != map->keys.size
        || elements.alignment != map->keys.alignment))
    || (mode == HXC_TYPED_MAP_ITERATOR_VALUE
      && (elements.size != map->values.size
        || elements.alignment != map->values.alignment))
    || (mode == HXC_TYPED_MAP_ITERATOR_PAIR
      && !hxc_typed_map_pair_layout_is_valid(
        map,
        elements,
        key_offset,
        value_offset
      ))) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  fill = (hxc_typed_map_iterator_fill){
    map,
    0u,
    key_offset,
    value_offset,
    mode
  };
  trace = (hxc_typed_map_iterator_trace){
    map,
    mode == HXC_TYPED_MAP_ITERATOR_VALUE ? 0u : key_offset,
    mode == HXC_TYPED_MAP_ITERATOR_KEY ? 0u : value_offset,
    mode
  };
  needs_trace = (mode != HXC_TYPED_MAP_ITERATOR_VALUE
      && map->keys.trace != NULL)
    || (mode != HXC_TYPED_MAP_ITERATOR_KEY
      && map->values.trace != NULL);
  if (needs_trace) {
    if (!map->collector_owned || map->collector == NULL) {
      return HXC_STATUS_INVALID_ARGUMENT;
    }
    return hxc_iterator_ref_create_traced_snapshot(
      (hxc_iterator_root_ops){
        map,
        &trace,
        hxc_typed_map_trace_iterator_element,
        hxc_typed_map_register_iterator_roots,
        hxc_typed_map_unregister_iterator_roots
      },
      map->allocator,
      elements,
      map->length,
      hxc_typed_map_iterator_fill_next,
      &fill,
      NULL,
      NULL,
      out_iterator
    );
  }
  return hxc_iterator_ref_create_snapshot(
    map->allocator,
    elements,
    map->length,
    hxc_typed_map_iterator_fill_next,
    &fill,
    NULL,
    NULL,
    out_iterator
  );
}

hxc_status hxc_typed_map_ref_value_iterator(
  hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  hxc_iterator_ref **out_iterator
) {
  return hxc_typed_map_iterator(
    map,
    elements,
    HXC_TYPED_MAP_ITERATOR_VALUE,
    0u,
    0u,
    out_iterator
  );
}

hxc_status hxc_typed_map_ref_key_iterator(
  hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  hxc_iterator_ref **out_iterator
) {
  return hxc_typed_map_iterator(
    map,
    elements,
    HXC_TYPED_MAP_ITERATOR_KEY,
    0u,
    0u,
    out_iterator
  );
}

hxc_status hxc_typed_map_ref_pair_iterator(
  hxc_typed_map_ref *map,
  hxc_iterator_element_ops elements,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
) {
  return hxc_typed_map_iterator(
    map,
    elements,
    HXC_TYPED_MAP_ITERATOR_PAIR,
    key_offset,
    value_offset,
    out_iterator
  );
}
