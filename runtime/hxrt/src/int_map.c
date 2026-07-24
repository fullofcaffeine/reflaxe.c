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

enum {
  HXC_INT_BOOL_MAP_EMPTY = 0,
  HXC_INT_BOOL_MAP_OCCUPIED = 1
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

static size_t hxc_int_bool_map_find_slot(
  const hxc_int_bool_map_ref *map,
  int32_t key,
  bool *out_found
) {
  size_t index = (size_t)hxc_int_bool_map_hash(key) & (map->capacity - 1u);
  hxc_int_bool_map_slot *slots = hxc_int_bool_map_slots(map);
  for (;;) {
    const hxc_int_bool_map_slot *slot = &slots[index];
    if (slot->state == HXC_INT_BOOL_MAP_EMPTY) {
      *out_found = false;
      return index;
    }
    if (slot->key == key) {
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
