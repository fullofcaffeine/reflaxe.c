/*
 * hxrt feature `int-map`: shared open-addressed Map<Int, Bool> storage.
 *
 * The table uses power-of-two capacity and keeps at least one empty slot, so a
 * lookup always terminates. Resizing allocates and fully rehashes replacement
 * storage before disposing the old block. Allocation failure therefore leaves
 * every observable entry and alias unchanged.
 */
#include "hxrt/int_map.h"

#include <string.h>

static hxc_status hxc_int_bool_map_append_literal(
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
  HXC_INT_BOOL_MAP_EMPTY = 0,
  HXC_INT_BOOL_MAP_OCCUPIED = 1,
  HXC_INT_BOOL_MAP_TOMBSTONE = 2
};

typedef struct hxc_int_bool_map_slot {
  int32_t key;
  bool value;
  uint8_t state;
} hxc_int_bool_map_slot;

struct hxc_int_bool_map_ref {
  size_t references;
  size_t length;
  size_t capacity;
  hxc_allocator allocator;
  hxc_allocation slots;
};

typedef enum hxc_int_bool_map_iterator_mode {
  HXC_INT_BOOL_MAP_ITERATOR_VALUE,
  HXC_INT_BOOL_MAP_ITERATOR_KEY,
  HXC_INT_BOOL_MAP_ITERATOR_PAIR
} hxc_int_bool_map_iterator_mode;

typedef struct hxc_int_bool_map_iterator_fill {
  const hxc_int_bool_map_ref *map;
  size_t slot;
  hxc_int_bool_map_iterator_mode mode;
  size_t key_offset;
  size_t value_offset;
} hxc_int_bool_map_iterator_fill;

static bool hxc_int_bool_map_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

static uint32_t hxc_int_bool_map_hash(int32_t key) {
  uint32_t value = (uint32_t)key;
  value ^= value >> 16u;
  value *= UINT32_C(0x7feb352d);
  value ^= value >> 15u;
  value *= UINT32_C(0x846ca68b);
  value ^= value >> 16u;
  return value;
}

static hxc_int_bool_map_slot *hxc_int_bool_map_slots(
  const hxc_int_bool_map_ref *map
) {
  return (hxc_int_bool_map_slot *)map->slots.memory;
}

static bool hxc_int_bool_map_is_valid(const hxc_int_bool_map_ref *map) {
  return map != NULL
    && map->references > 0u
    && hxc_allocator_is_valid(&map->allocator)
    && map->length <= map->capacity
    && ((map->capacity == 0u
        && map->slots.memory == NULL
        && map->slots.size == 0u
        && map->slots.alignment == 0u)
      || (hxc_int_bool_map_power_of_two(map->capacity)
        && map->slots.memory != NULL
        && hxc_allocation_is_valid(&map->slots)));
}

static hxc_status hxc_int_bool_map_iterator_fill_next(
  void *context,
  void *destination
) {
  hxc_int_bool_map_iterator_fill *fill = context;
  while (fill->slot < fill->map->capacity) {
    const hxc_int_bool_map_slot *slot =
      &hxc_int_bool_map_slots(fill->map)[fill->slot++];
    if (slot->state != HXC_INT_BOOL_MAP_OCCUPIED) {
      continue;
    }
    switch (fill->mode) {
      case HXC_INT_BOOL_MAP_ITERATOR_VALUE:
        *(bool *)destination = slot->value;
        break;
      case HXC_INT_BOOL_MAP_ITERATOR_KEY:
        *(int32_t *)destination = slot->key;
        break;
      case HXC_INT_BOOL_MAP_ITERATOR_PAIR:
        memcpy((unsigned char *)destination + fill->key_offset, &slot->key, sizeof(slot->key));
        memcpy((unsigned char *)destination + fill->value_offset, &slot->value, sizeof(slot->value));
        break;
    }
    return HXC_STATUS_OK;
  }
  return HXC_STATUS_INVALID_ARGUMENT;
}

static hxc_status hxc_int_bool_map_iterator_anchor_release(void *anchor) {
  return hxc_int_bool_map_ref_release(anchor);
}

static hxc_status hxc_int_bool_map_iterator(
  hxc_int_bool_map_ref *map,
  hxc_iterator_element_ops elements,
  hxc_int_bool_map_iterator_mode mode,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
) {
  hxc_int_bool_map_iterator_fill fill;
  hxc_status status;
  if (!hxc_int_bool_map_is_valid(map) || out_iterator == NULL
    || *out_iterator != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_int_bool_map_ref_retain(map);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  fill = (hxc_int_bool_map_iterator_fill){map, 0u, mode, key_offset, value_offset};
  status = hxc_iterator_ref_create_snapshot(
    map->allocator,
    elements,
    map->length,
    hxc_int_bool_map_iterator_fill_next,
    &fill,
    map,
    hxc_int_bool_map_iterator_anchor_release,
    out_iterator
  );
  if (status != HXC_STATUS_OK) {
    (void)hxc_int_bool_map_ref_release(map);
  }
  return status;
}

static size_t hxc_int_bool_map_find_slot(
  const hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_found
) {
  size_t index = (size_t)hxc_int_bool_map_hash(key) & (map->capacity - 1u);
  size_t first_tombstone = SIZE_MAX;
  hxc_int_bool_map_slot *slots = hxc_int_bool_map_slots(map);
  for (;;) {
    const hxc_int_bool_map_slot *slot = &slots[index];
    if (slot->state == HXC_INT_BOOL_MAP_EMPTY) {
      *out_found = false;
      return first_tombstone == SIZE_MAX ? index : first_tombstone;
    }
    if (slot->state == HXC_INT_BOOL_MAP_TOMBSTONE) {
      if (first_tombstone == SIZE_MAX) {
        first_tombstone = index;
      }
    } else if (slot->key == key) {
      *out_found = true;
      return index;
    }
    index = (index + 1u) & (map->capacity - 1u);
  }
}

static hxc_status hxc_int_bool_map_reserve(
  hxc_int_bool_map_ref *map,
  size_t requested_capacity
) {
  hxc_allocation replacement = HXC_ALLOCATION_INITIALIZER;
  hxc_allocation prior;
  size_t capacity = 8u;
  size_t index;
  hxc_status status;
  if (!hxc_int_bool_map_is_valid(map)) {
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
    sizeof(hxc_int_bool_map_slot),
    HXC_ALIGNOF(hxc_int_bool_map_slot),
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
    const size_t prior_capacity = prior.size / sizeof(hxc_int_bool_map_slot);
    const hxc_int_bool_map_slot *prior_slots = prior.memory;
    for (index = 0u; index < prior_capacity; index++) {
      const hxc_int_bool_map_slot *source = &prior_slots[index];
      if (source->state == HXC_INT_BOOL_MAP_OCCUPIED) {
        bool found = false;
        const size_t destination = hxc_int_bool_map_find_slot(
          map,
          source->key,
          &found
        );
        hxc_int_bool_map_slots(map)[destination] = *source;
      }
    }
    status = hxc_allocation_dispose(&prior);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_create(
  hxc_allocator allocator,
  hxc_int_bool_map_ref **out_map
) {
  hxc_int_bool_map_ref *map = NULL;
  hxc_status status;
  if (out_map == NULL || *out_map != NULL
    || !hxc_allocator_is_valid(&allocator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alloc(
    &allocator,
    sizeof(hxc_int_bool_map_ref),
    HXC_ALIGNOF(hxc_int_bool_map_ref),
    (void **)&map
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *map = (hxc_int_bool_map_ref){0};
  map->references = 1u;
  map->allocator = allocator;
  map->slots = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  *out_map = map;
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_retain(hxc_int_bool_map_ref *map) {
  if (map == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_int_bool_map_is_valid(map) || map->references == SIZE_MAX) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  map->references++;
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_release(hxc_int_bool_map_ref *map) {
  hxc_allocator allocator;
  hxc_status status;
  if (map == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_int_bool_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->references > 1u) {
    map->references--;
    return HXC_STATUS_OK;
  }
  allocator = map->allocator;
  status = hxc_allocation_dispose(&map->slots);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  return hxc_free(
    &allocator,
    map,
    sizeof(hxc_int_bool_map_ref),
    HXC_ALIGNOF(hxc_int_bool_map_ref)
  );
}

hxc_status hxc_int_bool_map_ref_copy(
  const hxc_int_bool_map_ref *source,
  hxc_int_bool_map_ref **out_map
) {
  hxc_int_bool_map_ref *copy = NULL;
  size_t index;
  hxc_status cleanup_status;
  hxc_status status;
  if (out_map == NULL || *out_map != NULL
    || !hxc_int_bool_map_is_valid(source)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_int_bool_map_ref_create(source->allocator, &copy);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (source->capacity != 0u) {
    status = hxc_int_bool_map_reserve(copy, source->capacity);
  }
  for (index = 0u; status == HXC_STATUS_OK && index < source->capacity; index++) {
    const hxc_int_bool_map_slot *slot = &hxc_int_bool_map_slots(source)[index];
    if (slot->state == HXC_INT_BOOL_MAP_OCCUPIED) {
      status = hxc_int_bool_map_ref_set(copy, slot->key, slot->value);
    }
  }
  if (status != HXC_STATUS_OK) {
    cleanup_status = hxc_int_bool_map_ref_release(copy);
    return cleanup_status == HXC_STATUS_OK ? status : cleanup_status;
  }
  *out_map = copy;
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_set(
  hxc_int_bool_map_ref *map,
  int32_t key,
  bool value
) {
  size_t index;
  bool found;
  hxc_status status;
  hxc_int_bool_map_slot *slot;
  if (!hxc_int_bool_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->capacity == 0u
    || map->length >= map->capacity - (map->capacity / 4u)) {
    if (map->capacity > SIZE_MAX / 2u) {
      return HXC_STATUS_SIZE_OVERFLOW;
    }
    status = hxc_int_bool_map_reserve(
      map,
      map->capacity == 0u ? 8u : map->capacity * 2u
    );
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  index = hxc_int_bool_map_find_slot(map, key, &found);
  slot = &hxc_int_bool_map_slots(map)[index];
  slot->key = key;
  slot->value = value;
  if (!found) {
    slot->state = HXC_INT_BOOL_MAP_OCCUPIED;
    map->length++;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_exists(
  const hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_exists
) {
  bool found = false;
  if (out_exists == NULL || !hxc_int_bool_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->capacity != 0u) {
    (void)hxc_int_bool_map_find_slot(map, key, &found);
  }
  *out_exists = found;
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_get(
  const hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_value,
  bool *out_found
) {
  size_t index = 0u;
  bool found = false;
  if (out_value == NULL || out_found == NULL || !hxc_int_bool_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->capacity != 0u) {
    index = hxc_int_bool_map_find_slot(map, key, &found);
  }
  if (found) {
    *out_value = hxc_int_bool_map_slots(map)[index].value;
  }
  *out_found = found;
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_remove(
  hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_removed
) {
  size_t index = 0u;
  bool found = false;
  if (out_removed == NULL || !hxc_int_bool_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->capacity != 0u) {
    index = hxc_int_bool_map_find_slot(map, key, &found);
  }
  if (found) {
    hxc_int_bool_map_slot *slot = &hxc_int_bool_map_slots(map)[index];
    slot->state = HXC_INT_BOOL_MAP_TOMBSTONE;
    map->length--;
  }
  *out_removed = found;
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_clear(hxc_int_bool_map_ref *map) {
  if (!hxc_int_bool_map_is_valid(map)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (map->slots.memory != NULL) {
    memset(map->slots.memory, 0, map->slots.size);
  }
  map->length = 0u;
  return HXC_STATUS_OK;
}

hxc_status hxc_int_bool_map_ref_value_iterator(
  hxc_int_bool_map_ref *map,
  hxc_iterator_ref **out_iterator
) {
  const hxc_iterator_element_ops elements = {
    sizeof(bool), HXC_ALIGNOF(bool), NULL, NULL, NULL, NULL
  };
  return hxc_int_bool_map_iterator(
    map, elements, HXC_INT_BOOL_MAP_ITERATOR_VALUE, 0u, 0u, out_iterator
  );
}

hxc_status hxc_int_bool_map_ref_key_iterator(
  hxc_int_bool_map_ref *map,
  hxc_iterator_ref **out_iterator
) {
  const hxc_iterator_element_ops elements = {
    sizeof(int32_t), HXC_ALIGNOF(int32_t), NULL, NULL, NULL, NULL
  };
  return hxc_int_bool_map_iterator(
    map, elements, HXC_INT_BOOL_MAP_ITERATOR_KEY, 0u, 0u, out_iterator
  );
}

hxc_status hxc_int_bool_map_ref_pair_iterator(
  hxc_int_bool_map_ref *map,
  size_t pair_size,
  size_t pair_alignment,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
) {
  const hxc_iterator_element_ops elements = {
    pair_size, pair_alignment, NULL, NULL, NULL, NULL
  };
  if (key_offset > pair_size || pair_size - key_offset < sizeof(int32_t)
    || value_offset > pair_size || pair_size - value_offset < sizeof(bool)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  return hxc_int_bool_map_iterator(
    map,
    elements,
    HXC_INT_BOOL_MAP_ITERATOR_PAIR,
    key_offset,
    value_offset,
    out_iterator
  );
}

hxc_status hxc_int_bool_map_ref_to_string(
  const hxc_int_bool_map_ref *map,
  hxc_string *out_string
) {
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_status status;
  size_t index;
  size_t emitted = 0u;
  if (!hxc_int_bool_map_is_valid(map) || out_string == NULL
    || out_string->data != NULL || out_string->byte_length != 0u
    || out_string->owner != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_buffer_init(&map->allocator, &buffer);
  if (status == HXC_STATUS_OK) status = hxc_int_bool_map_append_literal(&buffer, "[");
  for (index = 0u; status == HXC_STATUS_OK && index < map->capacity; index++) {
    const hxc_int_bool_map_slot *slot = &hxc_int_bool_map_slots(map)[index];
    hxc_string key = HXC_STRING_INITIALIZER;
    if (slot->state != HXC_INT_BOOL_MAP_OCCUPIED) continue;
    if (emitted++ != 0u) status = hxc_int_bool_map_append_literal(&buffer, ",");
    if (status == HXC_STATUS_OK) status = hxc_string_from_int32(slot->key, map->allocator, &key);
    if (status == HXC_STATUS_OK) {
      const hxc_byte_view view = {key.data, key.byte_length};
      status = hxc_string_buffer_append_utf8_checked(&buffer, view);
    }
    (void)hxc_string_release(&key);
    if (status == HXC_STATUS_OK) status = hxc_int_bool_map_append_literal(&buffer, " => ");
    if (status == HXC_STATUS_OK) status = hxc_int_bool_map_append_literal(&buffer, slot->value ? "true" : "false");
  }
  if (status == HXC_STATUS_OK) status = hxc_int_bool_map_append_literal(&buffer, "]");
  if (status == HXC_STATUS_OK) status = hxc_string_buffer_finish_ref(&buffer, out_string);
  if (status != HXC_STATUS_OK) (void)hxc_string_buffer_dispose(&buffer);
  return status;
}

hxc_status hxc_int_bool_map_ref_release_slot(void *context) {
  return context == NULL
    ? HXC_STATUS_INVALID_ARGUMENT
    : hxc_int_bool_map_ref_release((hxc_int_bool_map_ref *)context);
}
