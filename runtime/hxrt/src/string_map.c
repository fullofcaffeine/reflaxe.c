/*
 * hxrt feature `string-map`: open-addressed UTF-8 storage for Map<String, V>.
 *
 * Slot storage and every key retain allocator identity. Rehashing moves owned
 * key records and relocates unboxed value bytes only after the replacement
 * slot block exists. Relocation does not create a new logical owner; insertion,
 * replacement, lookup, removal, and clear use the exact value callbacks.
 */
#include "hxrt/string_map.h"

#include <string.h>

enum {
  HXC_STRING_MAP_EMPTY = 0,
  HXC_STRING_MAP_OCCUPIED = 1,
  HXC_STRING_MAP_TOMBSTONE = 2
};

typedef struct hxc_string_map_slot {
  uint32_t hash;
  uint8_t state;
  hxc_allocation key_storage;
  hxc_string key;
} hxc_string_map_slot;

struct hxc_string_map_ref {
  size_t references;
  size_t length;
  size_t tombstones;
  size_t capacity;
  size_t stride;
  size_t value_offset;
  hxc_string_map_value_ops values;
  hxc_allocator allocator;
  hxc_allocation slots;
};

static bool hxc_string_map_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

static hxc_status hxc_string_map_align_up(
  size_t value,
  size_t alignment,
  size_t *out_value
) {
  size_t remainder;
  size_t padding;
  if (out_value == NULL || !hxc_string_map_power_of_two(alignment)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  remainder = value & (alignment - 1u);
  padding = remainder == 0u ? 0u : alignment - remainder;
  return hxc_size_add(value, padding, out_value);
}

static hxc_status hxc_string_map_hash(hxc_string key, uint32_t *out_hash) {
  size_t index;
  uint32_t hash = UINT32_C(2166136261);
  if (out_hash == NULL || (key.data == NULL && key.byte_length != 0u)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  for (index = 0u; index < key.byte_length; index++) {
    hash ^= (uint32_t)key.data[index];
    hash *= UINT32_C(16777619);
  }
  *out_hash = hash;
  return HXC_STATUS_OK;
}

static hxc_string_map_slot *hxc_string_map_slot_at(
  const hxc_string_map_ref *map,
  size_t index
) {
  void *slot_memory = (void *)(
    (uint8_t *)map->slots.memory + (index * map->stride)
  );
  return slot_memory;
}

static void *hxc_string_map_value_at(
  const hxc_string_map_ref *map,
  size_t index
) {
  return (void *)(
    (uint8_t *)map->slots.memory + (index * map->stride) + map->value_offset
  );
}

static bool hxc_string_map_key_equal(
  const hxc_string_map_slot *slot,
  hxc_string key,
  uint32_t hash
) {
  return slot->state == HXC_STRING_MAP_OCCUPIED
    && slot->hash == hash
    && slot->key.byte_length == key.byte_length
    && (key.byte_length == 0u
      || memcmp(slot->key.data, key.data, key.byte_length) == 0);
}

static bool hxc_string_map_is_valid(const hxc_string_map_ref *map) {
  return map != NULL
    && map->references > 0u
    && hxc_string_map_value_ops_is_valid(&map->values)
    && hxc_allocator_is_valid(&map->allocator)
    && ((map->capacity == 0u
        && map->slots.memory == NULL
        && map->slots.size == 0u
        && map->slots.alignment == 0u)
      || (hxc_string_map_power_of_two(map->capacity)
        && map->slots.memory != NULL
        && hxc_allocation_is_valid(&map->slots)))
    && map->length <= map->capacity
    && map->tombstones <= map->capacity - map->length;
}

static bool hxc_string_map_has_lifecycle(
  const hxc_string_map_value_ops *values
) {
  return values->copy != NULL;
}

static hxc_status hxc_string_map_value_construct(
  const hxc_string_map_ref *map,
  void *destination,
  const void *source
) {
  if (hxc_string_map_has_lifecycle(&map->values)) {
    return map->values.copy(map->values.context, destination, source);
  }
  memcpy(destination, source, map->values.size);
  return HXC_STATUS_OK;
}

static hxc_status hxc_string_map_value_assign(
  const hxc_string_map_ref *map,
  void *destination,
  const void *source
) {
  if (hxc_string_map_has_lifecycle(&map->values)) {
    return map->values.assign(map->values.context, destination, source);
  }
  memcpy(destination, source, map->values.size);
  return HXC_STATUS_OK;
}

static void hxc_string_map_value_destroy(
  const hxc_string_map_ref *map,
  void *value
) {
  if (hxc_string_map_has_lifecycle(&map->values)) {
    map->values.destroy(map->values.context, value);
  }
}

static size_t hxc_string_map_find_slot(
  const hxc_string_map_ref *map,
  hxc_string key,
  uint32_t hash,
  bool *out_found
) {
  size_t index = (size_t)hash & (map->capacity - 1u);
  size_t first_tombstone = SIZE_MAX;
  for (;;) {
    const hxc_string_map_slot *slot = hxc_string_map_slot_at(map, index);
    if (slot->state == HXC_STRING_MAP_EMPTY) {
      *out_found = false;
      return first_tombstone == SIZE_MAX ? index : first_tombstone;
    }
    if (slot->state == HXC_STRING_MAP_TOMBSTONE) {
      if (first_tombstone == SIZE_MAX) {
        first_tombstone = index;
      }
    } else if (hxc_string_map_key_equal(slot, key, hash)) {
      *out_found = true;
      return index;
    }
    index = (index + 1u) & (map->capacity - 1u);
  }
}

static hxc_status hxc_string_map_reserve(
  hxc_string_map_ref *map,
  size_t requested_capacity
) {
  hxc_allocation replacement = HXC_ALLOCATION_INITIALIZER;
  hxc_allocation prior;
  size_t capacity = 8u;
  size_t index;
  hxc_status status;
  if (!hxc_string_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  while (capacity < requested_capacity) {
    if (capacity > SIZE_MAX / 2u) {
      return HXC_STATUS_SIZE_OVERFLOW;
    }
    capacity *= 2u;
  }
  if (capacity == map->capacity && map->tombstones == 0u) {
    return HXC_STATUS_OK;
  }
  status = hxc_allocation_allocate(
    &map->allocator,
    capacity,
    map->stride,
    map->values.alignment > HXC_ALIGNOF(hxc_string_map_slot)
      ? map->values.alignment
      : HXC_ALIGNOF(hxc_string_map_slot),
    &replacement
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  memset(replacement.memory, 0, replacement.size);
  prior = map->slots;
  map->slots = replacement;
  map->capacity = capacity;
  map->tombstones = 0u;
  if (prior.memory != NULL) {
    for (index = 0u; index < prior.size / map->stride; index++) {
      void *old_memory = (void *)(
        (uint8_t *)prior.memory + (index * map->stride)
      );
      hxc_string_map_slot *old_slot = old_memory;
      if (old_slot->state == HXC_STRING_MAP_OCCUPIED) {
        bool found = false;
        size_t destination = hxc_string_map_find_slot(
          map,
          old_slot->key,
          old_slot->hash,
          &found
        );
        hxc_string_map_slot *new_slot = hxc_string_map_slot_at(map, destination);
        *new_slot = *old_slot;
        memcpy(
          hxc_string_map_value_at(map, destination),
          (uint8_t *)prior.memory + (index * map->stride) + map->value_offset,
          map->values.size
        );
      }
    }
    status = hxc_allocation_dispose(&prior);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  return HXC_STATUS_OK;
}

bool hxc_string_map_value_ops_is_valid(
  const hxc_string_map_value_ops *values
) {
  bool has_copy;
  bool has_assign;
  bool has_destroy;
  if (values == NULL
    || values->size == 0u
    || !hxc_string_map_power_of_two(values->alignment)) {
    return false;
  }
#if SIZE_MAX > UINTPTR_MAX
  if (values->alignment > (size_t)UINTPTR_MAX) {
    return false;
  }
#endif
  has_copy = values->copy != NULL;
  has_assign = values->assign != NULL;
  has_destroy = values->destroy != NULL;
  return (has_copy && has_assign && has_destroy)
    || (!has_copy && !has_assign && !has_destroy);
}

hxc_status hxc_string_map_ref_create(
  hxc_allocator allocator,
  size_t value_size,
  size_t value_alignment,
  hxc_string_map_ref **out_map
) {
  hxc_string_map_value_ops values = {
    value_size,
    value_alignment,
    NULL,
    NULL,
    NULL,
    NULL
  };
  return hxc_string_map_ref_create_with_ops(allocator, values, out_map);
}

hxc_status hxc_string_map_ref_create_with_ops(
  hxc_allocator allocator,
  hxc_string_map_value_ops values,
  hxc_string_map_ref **out_map
) {
  hxc_string_map_ref *map = NULL;
  hxc_status status;
  size_t value_offset;
  size_t stride;
  size_t storage_alignment;
  if (out_map == NULL || *out_map != NULL
    || !hxc_string_map_value_ops_is_valid(&values)
    || !hxc_allocator_is_valid(&allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_align_up(
    sizeof(hxc_string_map_slot),
    values.alignment,
    &value_offset
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_size_add(value_offset, values.size, &stride);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  storage_alignment = values.alignment > HXC_ALIGNOF(hxc_string_map_slot)
    ? values.alignment
    : HXC_ALIGNOF(hxc_string_map_slot);
  status = hxc_string_map_align_up(stride, storage_alignment, &stride);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_alloc(
    &allocator,
    sizeof(hxc_string_map_ref),
    HXC_ALIGNOF(hxc_string_map_ref),
    (void **)&map
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *map = (hxc_string_map_ref){0};
  map->references = 1u;
  map->stride = stride;
  map->value_offset = value_offset;
  map->values = values;
  map->allocator = allocator;
  map->slots = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  *out_map = map;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_retain(hxc_string_map_ref *map) {
  if (map == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_string_map_is_valid(map) || map->references == SIZE_MAX) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  map->references++;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_release(hxc_string_map_ref *map) {
  hxc_allocator allocator;
  hxc_status status;
  if (map == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_string_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->references > 1u) {
    map->references--;
    return HXC_STATUS_OK;
  }
  allocator = map->allocator;
  status = hxc_string_map_ref_clear(map);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_dispose(&map->slots);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  return hxc_free(
    &allocator,
    map,
    sizeof(hxc_string_map_ref),
    HXC_ALIGNOF(hxc_string_map_ref)
  );
}

hxc_status hxc_string_map_ref_set_copy(
  hxc_string_map_ref *map,
  hxc_string key,
  const void *value
) {
  hxc_allocation key_storage = HXC_ALLOCATION_INITIALIZER;
  hxc_status cleanup_status;
  hxc_string_map_slot *slot;
  uint32_t hash;
  size_t index;
  bool found;
  hxc_status status;
  if (!hxc_string_map_is_valid(map) || value == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_hash(key, &hash);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (map->capacity == 0u
    || map->length + map->tombstones >= map->capacity - (map->capacity / 4u)) {
    if (map->capacity > SIZE_MAX / 2u) {
      return HXC_STATUS_SIZE_OVERFLOW;
    }
    status = hxc_string_map_reserve(
      map,
      map->capacity == 0u ? 8u : map->capacity * 2u
    );
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  index = hxc_string_map_find_slot(map, key, hash, &found);
  if (found) {
    return hxc_string_map_value_assign(
      map,
      hxc_string_map_value_at(map, index),
      value
    );
  }
  status = hxc_allocation_allocate(
    &map->allocator,
    key.byte_length,
    1u,
    HXC_ALIGNOF(uint8_t),
    &key_storage
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (key.byte_length > 0u) {
    memcpy(key_storage.memory, key.data, key.byte_length);
  }
  status = hxc_string_map_value_construct(
    map,
    hxc_string_map_value_at(map, index),
    value
  );
  if (status != HXC_STATUS_OK) {
    cleanup_status = hxc_allocation_dispose(&key_storage);
    return cleanup_status == HXC_STATUS_OK ? status : cleanup_status;
  }
  slot = hxc_string_map_slot_at(map, index);
  if (slot->state == HXC_STRING_MAP_TOMBSTONE) {
    map->tombstones--;
  }
  slot->hash = hash;
  slot->key_storage = key_storage;
  slot->key.data = (const uint8_t *)key_storage.memory;
  slot->key.byte_length = key.byte_length;
  slot->key.has_trailing_nul = false;
  slot->state = HXC_STRING_MAP_OCCUPIED;
  map->length++;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_exists(
  const hxc_string_map_ref *map,
  hxc_string key,
  bool *out_exists
) {
  uint32_t hash;
  bool found = false;
  hxc_status status;
  if (out_exists == NULL || !hxc_string_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_hash(key, &hash);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (map->capacity != 0u) {
    (void)hxc_string_map_find_slot(map, key, hash, &found);
  }
  *out_exists = found;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_get_copy(
  const hxc_string_map_ref *map,
  hxc_string key,
  void *out_value,
  bool *out_found
) {
  uint32_t hash;
  size_t index = 0u;
  bool found = false;
  hxc_status status;
  if (out_value == NULL || out_found == NULL || !hxc_string_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_hash(key, &hash);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (map->capacity != 0u) {
    index = hxc_string_map_find_slot(map, key, hash, &found);
  }
  if (found) {
    status = hxc_string_map_value_construct(
      map,
      out_value,
      hxc_string_map_value_at(map, index)
    );
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  *out_found = found;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_remove(
  hxc_string_map_ref *map,
  hxc_string key,
  bool *out_removed
) {
  hxc_string_map_slot *slot;
  uint32_t hash;
  size_t index = 0u;
  bool found = false;
  hxc_status status;
  if (out_removed == NULL || !hxc_string_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_hash(key, &hash);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (map->capacity != 0u) {
    index = hxc_string_map_find_slot(map, key, hash, &found);
  }
  if (!found) {
    *out_removed = false;
    return HXC_STATUS_OK;
  }
  slot = hxc_string_map_slot_at(map, index);
  status = hxc_allocation_dispose(&slot->key_storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  hxc_string_map_value_destroy(map, hxc_string_map_value_at(map, index));
  slot->key = (hxc_string)HXC_STRING_INITIALIZER;
  slot->state = HXC_STRING_MAP_TOMBSTONE;
  map->length--;
  map->tombstones++;
  *out_removed = true;
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_clear(hxc_string_map_ref *map) {
  size_t index;
  hxc_status status;
  if (!hxc_string_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  for (index = 0u; index < map->capacity; index++) {
    hxc_string_map_slot *slot = hxc_string_map_slot_at(map, index);
    if (slot->state == HXC_STRING_MAP_OCCUPIED) {
      status = hxc_allocation_dispose(&slot->key_storage);
      if (status != HXC_STATUS_OK) {
        return status;
      }
      hxc_string_map_value_destroy(map, hxc_string_map_value_at(map, index));
    }
  }
  if (map->slots.memory != NULL) {
    memset(map->slots.memory, 0, map->slots.size);
  }
  map->length = 0u;
  map->tombstones = 0u;
  return HXC_STATUS_OK;
}
