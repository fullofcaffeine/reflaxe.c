/*
 * hxrt feature `string-map`: open-addressed UTF-8 storage for Map<String, V>.
 *
 * Slot storage and every key retain allocator identity. Rehashing moves owned
 * key records and relocates unboxed value bytes only after the replacement
 * slot block exists. Relocation does not create a new logical owner; insertion,
 * replacement, lookup, removal, and clear use the exact value callbacks.
 */
#include "hxrt/string_map.h"
#include "hxrt/string.h"

#include <string.h>

static hxc_status hxc_string_map_append_literal(
  hxc_string_buffer *buffer,
  const char *text
) {
  hxc_byte_view view = HXC_BYTE_VIEW_INITIALIZER;
  hxc_status status = hxc_byte_view_from_cstring(text, &view);
  return status == HXC_STATUS_OK
    ? hxc_string_buffer_append_utf8_checked(buffer, view)
    : status;
}

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

/** Stack-owned cursor used only while the generic iterator copies a snapshot. */
typedef struct hxc_string_map_value_iterator_fill {
  hxc_string_map_ref *map;
  size_t slot_index;
} hxc_string_map_value_iterator_fill;

typedef struct hxc_string_map_pair_iterator_fill {
  hxc_string_map_ref *map;
  size_t slot_index;
  size_t key_offset;
  size_t value_offset;
} hxc_string_map_pair_iterator_fill;

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

static hxc_status hxc_string_map_value_iterator_fill_next(
  void *context,
  void *destination
) {
  hxc_string_map_value_iterator_fill *fill = context;
  while (fill->slot_index < fill->map->capacity) {
    size_t index = fill->slot_index;
    const hxc_string_map_slot *slot;
    fill->slot_index++;
    slot = hxc_string_map_slot_at(fill->map, index);
    if (slot->state == HXC_STRING_MAP_OCCUPIED) {
      return hxc_string_map_value_construct(
        fill->map,
        destination,
        hxc_string_map_value_at(fill->map, index)
      );
    }
  }
  return HXC_STATUS_INTERNAL_ERROR;
}

static hxc_status hxc_string_map_key_copy(
  void *context,
  void *destination,
  const void *source
) {
  const hxc_string_map_ref *map = context;
  *(hxc_string *)destination = (hxc_string)HXC_STRING_INITIALIZER;
  return hxc_string_copy_ref(
    *(const hxc_string *)source,
    map->allocator,
    destination
  );
}

static void hxc_string_map_key_destroy(void *context, void *element) {
  (void)context;
  (void)hxc_string_release(element);
}

static hxc_status hxc_string_map_key_iterator_fill_next(
  void *context,
  void *destination
) {
  hxc_string_map_value_iterator_fill *fill = context;
  while (fill->slot_index < fill->map->capacity) {
    const hxc_string_map_slot *slot =
      hxc_string_map_slot_at(fill->map, fill->slot_index++);
    if (slot->state == HXC_STRING_MAP_OCCUPIED) {
      return hxc_string_map_key_copy(
        fill->map, destination, &slot->key
      );
    }
  }
  return HXC_STATUS_INTERNAL_ERROR;
}

static hxc_status hxc_string_map_pair_copy(
  void *context,
  void *destination,
  const void *source
) {
  const hxc_string_map_pair_iterator_fill *fill = context;
  hxc_string destination_key = HXC_STRING_INITIALIZER;
  hxc_string source_key = HXC_STRING_INITIALIZER;
  hxc_status status;
  memcpy(&source_key, (const unsigned char *)source + fill->key_offset, sizeof(source_key));
  status = hxc_string_copy_ref(source_key, fill->map->allocator, &destination_key);
  if (status == HXC_STATUS_OK) {
    memcpy((unsigned char *)destination + fill->key_offset, &destination_key, sizeof(destination_key));
  }
  if (status == HXC_STATUS_OK) {
    status = hxc_string_map_value_construct(
      fill->map,
      (unsigned char *)destination + fill->value_offset,
      (const unsigned char *)source + fill->value_offset
    );
  }
  if (status != HXC_STATUS_OK) {
    (void)hxc_string_release(&destination_key);
  }
  return status;
}

static void hxc_string_map_pair_destroy(void *context, void *element) {
  const hxc_string_map_pair_iterator_fill *fill = context;
  hxc_string key = HXC_STRING_INITIALIZER;
  memcpy(&key, (unsigned char *)element + fill->key_offset, sizeof(key));
  (void)hxc_string_release(&key);
  hxc_string_map_value_destroy(
    fill->map,
    (unsigned char *)element + fill->value_offset
  );
}

static hxc_status hxc_string_map_pair_iterator_fill_next(
  void *context,
  void *destination
) {
  hxc_string_map_pair_iterator_fill *fill = context;
  while (fill->slot_index < fill->map->capacity) {
    const size_t index = fill->slot_index++;
    const hxc_string_map_slot *slot = hxc_string_map_slot_at(fill->map, index);
    hxc_string key = HXC_STRING_INITIALIZER;
    hxc_status status;
    if (slot->state != HXC_STRING_MAP_OCCUPIED) {
      continue;
    }
    status = hxc_string_copy_ref(slot->key, fill->map->allocator, &key);
    if (status == HXC_STATUS_OK) {
      memcpy((unsigned char *)destination + fill->key_offset, &key, sizeof(key));
    }
    if (status == HXC_STATUS_OK) {
      status = hxc_string_map_value_construct(
        fill->map,
        (unsigned char *)destination + fill->value_offset,
        hxc_string_map_value_at(fill->map, index)
      );
    }
    if (status != HXC_STATUS_OK) {
      (void)hxc_string_release(&key);
    }
    return status;
  }
  return HXC_STATUS_INTERNAL_ERROR;
}

static hxc_status hxc_string_map_iterator_release_anchor(void *anchor) {
  return hxc_string_map_ref_release(anchor);
}

static hxc_status hxc_string_map_pair_iterator_release_anchor(void *anchor) {
  hxc_string_map_pair_iterator_fill *fill = anchor;
  hxc_allocator allocator = fill->map->allocator;
  hxc_status status = hxc_string_map_ref_release(fill->map);
  hxc_status free_status = hxc_free(
    &allocator,
    fill,
    sizeof(hxc_string_map_pair_iterator_fill),
    HXC_ALIGNOF(hxc_string_map_pair_iterator_fill)
  );
  return status == HXC_STATUS_OK ? free_status : status;
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

hxc_status hxc_string_map_ref_copy(
  const hxc_string_map_ref *source,
  hxc_string_map_ref **out_map
) {
  hxc_string_map_ref *copy = NULL;
  size_t index;
  hxc_status cleanup_status;
  hxc_status status;
  if (out_map == NULL || *out_map != NULL || !hxc_string_map_is_valid(source)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_ref_create_with_ops(
    source->allocator,
    source->values,
    &copy
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (source->capacity != 0u) {
    status = hxc_string_map_reserve(copy, source->capacity);
  }
  for (index = 0u; status == HXC_STATUS_OK && index < source->capacity; index++) {
    const hxc_string_map_slot *slot = hxc_string_map_slot_at(source, index);
    if (slot->state == HXC_STRING_MAP_OCCUPIED) {
      status = hxc_string_map_ref_set_copy(
        copy,
        slot->key,
        hxc_string_map_value_at(source, index)
      );
    }
  }
  if (status != HXC_STATUS_OK) {
    cleanup_status = hxc_string_map_ref_release(copy);
    return cleanup_status == HXC_STATUS_OK ? status : cleanup_status;
  }
  *out_map = copy;
  return HXC_STATUS_OK;
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
  slot->key.owner = NULL;
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

hxc_status hxc_string_map_ref_value_iterator(
  hxc_string_map_ref *map,
  hxc_iterator_ref **out_iterator
) {
  hxc_iterator_element_ops elements;
  hxc_string_map_value_iterator_fill fill;
  hxc_status status;
  if (!hxc_string_map_is_valid(map)
    || out_iterator == NULL
    || *out_iterator != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_ref_retain(map);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  elements = (hxc_iterator_element_ops){
    map->values.size,
    map->values.alignment,
    map->values.context,
    map->values.copy,
    map->values.destroy,
    NULL
  };
  fill = (hxc_string_map_value_iterator_fill){map, 0u};
  status = hxc_iterator_ref_create_snapshot(
    map->allocator,
    elements,
    map->length,
    hxc_string_map_value_iterator_fill_next,
    &fill,
    map,
    hxc_string_map_iterator_release_anchor,
    out_iterator
  );
  if (status != HXC_STATUS_OK) {
    hxc_status cleanup_status = hxc_string_map_ref_release(map);
    return cleanup_status == HXC_STATUS_OK ? status : cleanup_status;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_key_iterator(
  hxc_string_map_ref *map,
  hxc_iterator_ref **out_iterator
) {
  hxc_string_map_value_iterator_fill fill;
  hxc_iterator_element_ops elements;
  hxc_status status;
  if (!hxc_string_map_is_valid(map) || out_iterator == NULL
    || *out_iterator != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_map_ref_retain(map);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  elements = (hxc_iterator_element_ops){
    sizeof(hxc_string),
    HXC_ALIGNOF(hxc_string),
    map,
    hxc_string_map_key_copy,
    hxc_string_map_key_destroy,
    NULL
  };
  fill = (hxc_string_map_value_iterator_fill){map, 0u};
  status = hxc_iterator_ref_create_snapshot(
    map->allocator,
    elements,
    map->length,
    hxc_string_map_key_iterator_fill_next,
    &fill,
    map,
    hxc_string_map_iterator_release_anchor,
    out_iterator
  );
  if (status != HXC_STATUS_OK) {
    hxc_status cleanup_status = hxc_string_map_ref_release(map);
    return cleanup_status == HXC_STATUS_OK ? status : cleanup_status;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_string_map_ref_pair_iterator(
  hxc_string_map_ref *map,
  size_t pair_size,
  size_t pair_alignment,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
) {
  hxc_string_map_pair_iterator_fill *fill = NULL;
  hxc_iterator_element_ops elements;
  hxc_status status;
  if (!hxc_string_map_is_valid(map) || out_iterator == NULL
    || *out_iterator != NULL
    || key_offset > pair_size || pair_size - key_offset < sizeof(hxc_string)
    || value_offset > pair_size || pair_size - value_offset < map->values.size) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alloc(
    &map->allocator,
    sizeof(hxc_string_map_pair_iterator_fill),
    HXC_ALIGNOF(hxc_string_map_pair_iterator_fill),
    (void **)&fill
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *fill = (hxc_string_map_pair_iterator_fill){
    map, 0u, key_offset, value_offset
  };
  status = hxc_string_map_ref_retain(map);
  if (status != HXC_STATUS_OK) {
    (void)hxc_free(
      &map->allocator,
      fill,
      sizeof(hxc_string_map_pair_iterator_fill),
      HXC_ALIGNOF(hxc_string_map_pair_iterator_fill)
    );
    return status;
  }
  elements = (hxc_iterator_element_ops){
    pair_size,
    pair_alignment,
    fill,
    hxc_string_map_pair_copy,
    hxc_string_map_pair_destroy,
    NULL
  };
  status = hxc_iterator_ref_create_snapshot(
    map->allocator,
    elements,
    map->length,
    hxc_string_map_pair_iterator_fill_next,
    fill,
    fill,
    hxc_string_map_pair_iterator_release_anchor,
    out_iterator
  );
  if (status != HXC_STATUS_OK) {
    hxc_allocator allocator = map->allocator;
    (void)hxc_string_map_ref_release(map);
    (void)hxc_free(
      &allocator,
      fill,
      sizeof(hxc_string_map_pair_iterator_fill),
      HXC_ALIGNOF(hxc_string_map_pair_iterator_fill)
    );
  }
  return status;
}

hxc_status hxc_string_map_ref_to_string(
  const hxc_string_map_ref *map,
  hxc_string_map_format_kind kind,
  hxc_string *out_string
) {
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_status status;
  size_t index;
  size_t emitted = 0u;
  const size_t expected_size = kind == HXC_STRING_MAP_FORMAT_BOOL
    ? sizeof(bool) : sizeof(int32_t);
  if (!hxc_string_map_is_valid(map) || out_string == NULL
    || (kind != HXC_STRING_MAP_FORMAT_BOOL && kind != HXC_STRING_MAP_FORMAT_INT32)
    || map->values.size != expected_size
    || out_string->data != NULL || out_string->byte_length != 0u
    || out_string->owner != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_buffer_init(&map->allocator, &buffer);
  if (status == HXC_STATUS_OK) status = hxc_string_map_append_literal(&buffer, "[");
  for (index = 0u; status == HXC_STATUS_OK && index < map->capacity; index++) {
    const hxc_string_map_slot *slot = hxc_string_map_slot_at(map, index);
    if (slot->state != HXC_STRING_MAP_OCCUPIED) continue;
    if (emitted++ != 0u) status = hxc_string_map_append_literal(&buffer, ",");
    if (status == HXC_STATUS_OK) {
      const hxc_byte_view key = {slot->key.data, slot->key.byte_length};
      status = hxc_string_buffer_append_utf8_checked(&buffer, key);
    }
    if (status == HXC_STATUS_OK) status = hxc_string_map_append_literal(&buffer, " => ");
    if (status == HXC_STATUS_OK && kind == HXC_STRING_MAP_FORMAT_BOOL) {
      status = hxc_string_map_append_literal(
        &buffer,
        *(const bool *)hxc_string_map_value_at(map, index) ? "true" : "false"
      );
    } else if (status == HXC_STATUS_OK) {
      hxc_string value = HXC_STRING_INITIALIZER;
      status = hxc_string_from_int32(
        *(const int32_t *)hxc_string_map_value_at(map, index),
        map->allocator,
        &value
      );
      if (status == HXC_STATUS_OK) {
        const hxc_byte_view view = {value.data, value.byte_length};
        status = hxc_string_buffer_append_utf8_checked(&buffer, view);
      }
      (void)hxc_string_release(&value);
    }
  }
  if (status == HXC_STATUS_OK) status = hxc_string_map_append_literal(&buffer, "]");
  if (status == HXC_STATUS_OK) status = hxc_string_buffer_finish_ref(&buffer, out_string);
  if (status != HXC_STATUS_OK) (void)hxc_string_buffer_dispose(&buffer);
  return status;
}
