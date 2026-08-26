/*
 * Implementation of feature `iterator`.
 *
 * Generated Haxe and independent native fixtures use this owner for exact
 * typed snapshots and live Array cursors. Aliases share one cursor. Snapshot
 * construction publishes only complete storage; live cursors retain the Array
 * and copy its current element only when next() succeeds.
 */
#include "hxrt/iterator.h"

#include <stdint.h>
#include <string.h>

typedef enum hxc_iterator_kind {
  HXC_ITERATOR_SNAPSHOT = 0,
  HXC_ITERATOR_ARRAY_VALUES = 1,
  HXC_ITERATOR_ARRAY_PAIRS = 2
} hxc_iterator_kind;

struct hxc_iterator_ref {
  size_t references;
  size_t cursor;
  size_t length;

  hxc_iterator_kind kind;
  hxc_iterator_element_ops elements;
  hxc_allocator allocator;
  hxc_allocation storage;
  hxc_allocation root_slots;
  hxc_allocation root_offsets;
  void *root_registration;
  hxc_iterator_root_ops roots;
  void *anchor;
  hxc_iterator_anchor_release_fn release_anchor;

  hxc_array_ref *array;
  size_t pair_size;
  size_t pair_alignment;
  size_t key_offset;
  size_t value_offset;
};

static bool hxc_iterator_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

static bool hxc_iterator_root_ops_is_valid(
  const hxc_iterator_root_ops *roots
) {
  return roots != NULL
    && ((roots->register_roots == NULL && roots->unregister_roots == NULL)
      || (roots->register_roots != NULL && roots->unregister_roots != NULL))
    && (roots->trace_element == NULL || roots->register_roots != NULL);
}

/** True when two already-bounds-checked pair members occupy separate bytes. */
static bool hxc_iterator_pair_members_are_disjoint(
  size_t key_offset,
  size_t value_offset,
  size_t value_size
) {
  return key_offset + sizeof(int32_t) <= value_offset
    || value_offset + value_size <= key_offset;
}

static void *hxc_iterator_slot(hxc_iterator_ref *iterator, size_t index) {
  return (unsigned char *)iterator->storage.memory
    + (index * iterator->elements.size);
}

static bool hxc_iterator_is_valid(const hxc_iterator_ref *iterator) {
  if (iterator == NULL
    || iterator->references == 0u
    || !hxc_allocator_is_valid(&iterator->allocator)) {
    return false;
  }
  if (iterator->kind == HXC_ITERATOR_SNAPSHOT) {
    const bool roots_registered = iterator->root_registration != NULL;
    const bool roots_absent = !roots_registered
      && iterator->root_slots.memory == NULL
      && iterator->root_offsets.memory == NULL;
    const bool roots_present = roots_registered
      && hxc_allocation_is_valid(&iterator->root_slots)
      && hxc_allocation_is_valid(&iterator->root_offsets)
      && iterator->roots.unregister_roots != NULL;
    return iterator->cursor <= iterator->length
      && iterator->array == NULL
      && hxc_iterator_element_ops_is_valid(&iterator->elements)
      && hxc_allocation_is_valid(&iterator->storage)
      && (roots_absent || roots_present)
      && ((iterator->anchor == NULL && iterator->release_anchor == NULL)
        || (iterator->anchor != NULL && iterator->release_anchor != NULL));
  }
  if ((iterator->kind != HXC_ITERATOR_ARRAY_VALUES
      && iterator->kind != HXC_ITERATOR_ARRAY_PAIRS)
    || !hxc_array_ref_is_valid(iterator->array)
    || iterator->anchor != NULL
    || iterator->release_anchor != NULL
    || iterator->storage.memory != NULL
    || iterator->storage.size != 0u
    || iterator->root_slots.memory != NULL
    || iterator->root_offsets.memory != NULL
    || iterator->root_registration != NULL
    || iterator->roots.register_roots != NULL
    || iterator->roots.unregister_roots != NULL) {
    return false;
  }
  if (iterator->kind == HXC_ITERATOR_ARRAY_VALUES) {
    return true;
  }
  return iterator->pair_size != 0u
    && hxc_iterator_power_of_two(iterator->pair_alignment)
    && iterator->pair_size % iterator->pair_alignment == 0u
    && iterator->pair_size >= sizeof(int32_t)
    && iterator->pair_size >= iterator->array->value.elements.size
    && iterator->pair_alignment >= HXC_ALIGNOF(int32_t)
    && iterator->pair_alignment >= iterator->array->value.elements.alignment
    && iterator->key_offset <= iterator->pair_size - sizeof(int32_t)
    && iterator->value_offset
      <= iterator->pair_size - iterator->array->value.elements.size
    && iterator->key_offset % HXC_ALIGNOF(int32_t) == 0u
    && iterator->value_offset % iterator->array->value.elements.alignment == 0u
    && hxc_iterator_pair_members_are_disjoint(
      iterator->key_offset,
      iterator->value_offset,
      iterator->array->value.elements.size
    );
}

/** Allocate and publish one live Array iterator after retaining its anchor. */
static hxc_status hxc_iterator_ref_create_array(
  hxc_array_ref *array,
  hxc_iterator_kind kind,
  size_t pair_size,
  size_t pair_alignment,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
) {
  hxc_iterator_ref *iterator = NULL;
  hxc_status status;
  if (out_iterator == NULL
    || *out_iterator != NULL
    || !hxc_array_ref_is_valid(array)
    || (kind != HXC_ITERATOR_ARRAY_VALUES
      && kind != HXC_ITERATOR_ARRAY_PAIRS)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (kind == HXC_ITERATOR_ARRAY_PAIRS
    && (pair_size < sizeof(int32_t)
      || pair_size < array->value.elements.size
      || !hxc_iterator_power_of_two(pair_alignment)
      || pair_size % pair_alignment != 0u
      || pair_alignment < HXC_ALIGNOF(int32_t)
      || pair_alignment < array->value.elements.alignment
      || key_offset > pair_size - sizeof(int32_t)
      || value_offset > pair_size - array->value.elements.size
      || key_offset % HXC_ALIGNOF(int32_t) != 0u
      || value_offset % array->value.elements.alignment != 0u
      || !hxc_iterator_pair_members_are_disjoint(
        key_offset,
        value_offset,
        array->value.elements.size
      ))) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alloc(
    &array->allocator,
    sizeof(hxc_iterator_ref),
    HXC_ALIGNOF(hxc_iterator_ref),
    (void **)&iterator
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *iterator = (hxc_iterator_ref){0};
  iterator->references = 1u;
  iterator->kind = kind;
  iterator->allocator = array->allocator;
  iterator->storage = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  iterator->root_slots = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  iterator->root_offsets = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  iterator->array = array;
  iterator->pair_size = pair_size;
  iterator->pair_alignment = pair_alignment;
  iterator->key_offset = key_offset;
  iterator->value_offset = value_offset;
  status = hxc_array_ref_retain(array);
  if (status != HXC_STATUS_OK) {
    (void)hxc_free(
      &iterator->allocator,
      iterator,
      sizeof(hxc_iterator_ref),
      HXC_ALIGNOF(hxc_iterator_ref)
    );
    return status;
  }
  *out_iterator = iterator;
  return HXC_STATUS_OK;
}

bool hxc_iterator_element_ops_is_valid(
  const hxc_iterator_element_ops *elements
) {
  if (elements == NULL
    || elements->size == 0u
    || !hxc_iterator_power_of_two(elements->alignment)) {
    return false;
  }
  return (elements->copy == NULL && elements->destroy == NULL)
    || (elements->copy != NULL && elements->destroy != NULL);
}

typedef struct hxc_iterator_trace_count {
  size_t count;
  bool overflow;
} hxc_iterator_trace_count;

typedef struct hxc_iterator_trace_fill {
  const void **slots;
  size_t count;
  size_t capacity;
} hxc_iterator_trace_fill;

static void hxc_iterator_count_root(void *context, const void *object) {
  hxc_iterator_trace_count *count = context;
  if (object == NULL || count->overflow) {
    return;
  }
  if (count->count == SIZE_MAX) {
    count->overflow = true;
  } else {
    count->count++;
  }
}

static void hxc_iterator_fill_root(void *context, const void *object) {
  hxc_iterator_trace_fill *fill = context;
  if (object != NULL && fill->count < fill->capacity) {
    fill->slots[fill->count++] = object;
  }
}

static void hxc_iterator_destroy_constructed(
  hxc_iterator_ref *iterator,
  size_t constructed
) {
  while (constructed != 0u) {
    constructed--;
    if (iterator->elements.destroy != NULL) {
      iterator->elements.destroy(
        iterator->elements.context,
        hxc_iterator_slot(iterator, constructed)
      );
    }
  }
}

static hxc_status hxc_iterator_register_snapshot_roots(
  hxc_iterator_ref *iterator,
  hxc_iterator_root_ops roots
) {
  hxc_iterator_trace_count count = {0u, false};
  hxc_iterator_trace_fill fill;
  hxc_iterator_element_trace_fn trace;
  void *trace_context;
  size_t *offsets;
  size_t index;
  hxc_status status;
  if (!hxc_iterator_root_ops_is_valid(&roots)
    || roots.register_roots == NULL
    || iterator->length == SIZE_MAX) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  trace = roots.trace_element == NULL
    ? iterator->elements.trace
    : roots.trace_element;
  trace_context = roots.trace_element == NULL
    ? iterator->elements.context
    : roots.trace_context;
  if (trace == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_allocation_allocate(
    &iterator->allocator,
    iterator->length + 1u,
    sizeof(size_t),
    HXC_ALIGNOF(size_t),
    &iterator->root_offsets
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  offsets = iterator->root_offsets.memory;
  for (index = 0u; index < iterator->length; index++) {
    offsets[index] = count.count;
    trace(
      trace_context,
      hxc_iterator_slot(iterator, index),
      hxc_iterator_count_root,
      &count
    );
    if (count.overflow) {
      return HXC_STATUS_SIZE_OVERFLOW;
    }
  }
  offsets[iterator->length] = count.count;
  if (count.count == 0u) {
    return hxc_allocation_dispose(&iterator->root_offsets);
  }
  status = hxc_allocation_allocate(
    &iterator->allocator,
    count.count,
    sizeof(void *),
    HXC_ALIGNOF(void *),
    &iterator->root_slots
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  fill = (hxc_iterator_trace_fill){
    iterator->root_slots.memory,
    0u,
    count.count
  };
  for (index = 0u; index < iterator->length; index++) {
    trace(
      trace_context,
      hxc_iterator_slot(iterator, index),
      hxc_iterator_fill_root,
      &fill
    );
  }
  if (fill.count != count.count) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  status = roots.register_roots(
    roots.context,
    iterator->root_slots.memory,
    count.count,
    &iterator->root_registration
  );
  if (status != HXC_STATUS_OK && iterator->root_registration != NULL) {
    (void)roots.unregister_roots(iterator->root_registration);
    iterator->root_registration = NULL;
  } else if (status == HXC_STATUS_OK && iterator->root_registration == NULL) {
    status = HXC_STATUS_INTERNAL_ERROR;
  } else if (status == HXC_STATUS_OK) {
    iterator->roots = roots;
    iterator->roots.context = NULL;
    iterator->roots.trace_context = NULL;
    iterator->roots.trace_element = NULL;
    iterator->roots.register_roots = NULL;
  }
  return status;
}

static hxc_status hxc_iterator_ref_create_snapshot_internal(
  hxc_iterator_root_ops roots,
  hxc_allocator allocator,
  hxc_iterator_element_ops elements,
  size_t length,
  hxc_iterator_snapshot_fill_fn fill,
  void *fill_context,
  void *anchor,
  hxc_iterator_anchor_release_fn release_anchor,
  hxc_iterator_ref **out_iterator
) {
  hxc_iterator_ref *iterator = NULL;
  hxc_status status;
  size_t constructed = 0u;
  if (out_iterator == NULL
    || *out_iterator != NULL
    || fill == NULL
    || !hxc_allocator_is_valid(&allocator)
    || !hxc_iterator_element_ops_is_valid(&elements)
    || !hxc_iterator_root_ops_is_valid(&roots)
    || ((anchor == NULL) != (release_anchor == NULL))
    || (roots.register_roots == NULL
      ? elements.trace != NULL
      : roots.trace_element == NULL && elements.trace == NULL)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_alloc(
    &allocator,
    sizeof(hxc_iterator_ref),
    HXC_ALIGNOF(hxc_iterator_ref),
    (void **)&iterator
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  *iterator = (hxc_iterator_ref){0};
  iterator->references = 1u;
  iterator->length = length;
  iterator->kind = HXC_ITERATOR_SNAPSHOT;
  iterator->elements = elements;
  iterator->allocator = allocator;
  iterator->storage = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  iterator->root_slots = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  iterator->root_offsets = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  iterator->anchor = anchor;
  iterator->release_anchor = release_anchor;
  status = hxc_allocation_allocate(
    &iterator->allocator,
    length,
    elements.size,
    elements.alignment,
    &iterator->storage
  );
  while (status == HXC_STATUS_OK && constructed < length) {
    status = fill(fill_context, hxc_iterator_slot(iterator, constructed));
    if (status == HXC_STATUS_OK) {
      constructed++;
    }
  }
  if (status == HXC_STATUS_OK && roots.register_roots != NULL) {
    status = hxc_iterator_register_snapshot_roots(iterator, roots);
  }
  if (status != HXC_STATUS_OK) {
    if (iterator->root_registration != NULL) {
      (void)iterator->roots.unregister_roots(iterator->root_registration);
    }
    (void)hxc_allocation_dispose(&iterator->root_slots);
    (void)hxc_allocation_dispose(&iterator->root_offsets);
    hxc_iterator_destroy_constructed(iterator, constructed);
    (void)hxc_allocation_dispose(&iterator->storage);
    (void)hxc_free(
      &allocator,
      iterator,
      sizeof(hxc_iterator_ref),
      HXC_ALIGNOF(hxc_iterator_ref)
    );
    return status;
  }
  *out_iterator = iterator;
  return HXC_STATUS_OK;
}

hxc_status hxc_iterator_ref_create_snapshot(
  hxc_allocator allocator,
  hxc_iterator_element_ops elements,
  size_t length,
  hxc_iterator_snapshot_fill_fn fill,
  void *fill_context,
  void *anchor,
  hxc_iterator_anchor_release_fn release_anchor,
  hxc_iterator_ref **out_iterator
) {
  return hxc_iterator_ref_create_snapshot_internal(
    (hxc_iterator_root_ops){0},
    allocator,
    elements,
    length,
    fill,
    fill_context,
    anchor,
    release_anchor,
    out_iterator
  );
}

hxc_status hxc_iterator_ref_create_traced_snapshot(
  hxc_iterator_root_ops roots,
  hxc_allocator allocator,
  hxc_iterator_element_ops elements,
  size_t length,
  hxc_iterator_snapshot_fill_fn fill,
  void *fill_context,
  void *anchor,
  hxc_iterator_anchor_release_fn release_anchor,
  hxc_iterator_ref **out_iterator
) {
  return hxc_iterator_ref_create_snapshot_internal(
    roots,
    allocator,
    elements,
    length,
    fill,
    fill_context,
    anchor,
    release_anchor,
    out_iterator
  );
}

hxc_status hxc_iterator_ref_create_array_values(
  hxc_array_ref *array,
  hxc_iterator_ref **out_iterator
) {
  return hxc_iterator_ref_create_array(
    array,
    HXC_ITERATOR_ARRAY_VALUES,
    0u,
    0u,
    0u,
    0u,
    out_iterator
  );
}

hxc_status hxc_iterator_ref_create_array_pairs(
  hxc_array_ref *array,
  size_t pair_size,
  size_t pair_alignment,
  size_t key_offset,
  size_t value_offset,
  hxc_iterator_ref **out_iterator
) {
  return hxc_iterator_ref_create_array(
    array,
    HXC_ITERATOR_ARRAY_PAIRS,
    pair_size,
    pair_alignment,
    key_offset,
    value_offset,
    out_iterator
  );
}

hxc_status hxc_iterator_ref_retain(hxc_iterator_ref *iterator) {
  if (iterator == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_iterator_is_valid(iterator) || iterator->references == SIZE_MAX) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  iterator->references++;
  return HXC_STATUS_OK;
}

hxc_status hxc_iterator_ref_release(hxc_iterator_ref *iterator) {
  hxc_allocator allocator;
  hxc_status status;
  if (iterator == NULL) {
    return HXC_STATUS_OK;
  }
  if (!hxc_iterator_is_valid(iterator)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (iterator->references > 1u) {
    iterator->references--;
    return HXC_STATUS_OK;
  }
  allocator = iterator->allocator;
  if (iterator->kind != HXC_ITERATOR_SNAPSHOT) {
    status = hxc_array_ref_release(iterator->array);
    if (status != HXC_STATUS_OK) {
      return status;
    }
    return hxc_free(
      &allocator,
      iterator,
      sizeof(hxc_iterator_ref),
      HXC_ALIGNOF(hxc_iterator_ref)
    );
  }
  if (iterator->elements.destroy != NULL) {
    size_t index = iterator->length;
    while (index > iterator->cursor) {
      index--;
      iterator->elements.destroy(
        iterator->elements.context,
        hxc_iterator_slot(iterator, index)
      );
    }
  }
  if (iterator->root_registration != NULL) {
    status = iterator->roots.unregister_roots(iterator->root_registration);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  status = hxc_allocation_dispose(&iterator->root_slots);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_dispose(&iterator->root_offsets);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_allocation_dispose(&iterator->storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (iterator->release_anchor != NULL) {
    status = iterator->release_anchor(iterator->anchor);
    if (status != HXC_STATUS_OK) {
      return status;
    }
  }
  return hxc_free(
    &allocator,
    iterator,
    sizeof(hxc_iterator_ref),
    HXC_ALIGNOF(hxc_iterator_ref)
  );
}

hxc_status hxc_iterator_ref_has_next(
  const hxc_iterator_ref *iterator,
  bool *out_has_next
) {
  if (!hxc_iterator_is_valid(iterator) || out_has_next == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_has_next = iterator->kind == HXC_ITERATOR_SNAPSHOT
    ? iterator->cursor < iterator->length
    : iterator->cursor < iterator->array->value.length;
  return HXC_STATUS_OK;
}

hxc_status hxc_iterator_ref_next_move(
  hxc_iterator_ref *iterator,
  void *out_element
) {
  if (!hxc_iterator_is_valid(iterator) || out_element == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (iterator->kind != HXC_ITERATOR_SNAPSHOT) {
    hxc_status status;
    if (iterator->cursor >= iterator->array->value.length) {
      return HXC_STATUS_INVALID_ARGUMENT;
    }
    if (iterator->kind == HXC_ITERATOR_ARRAY_PAIRS) {
      int32_t key;
      if (iterator->cursor > (size_t)INT32_MAX) {
        return HXC_STATUS_SIZE_OVERFLOW;
      }
      status = hxc_array_ref_get_copy(
        iterator->array,
        iterator->cursor,
        (unsigned char *)out_element + iterator->value_offset
      );
      if (status != HXC_STATUS_OK) {
        return status;
      }
      key = (int32_t)iterator->cursor;
      memcpy(
        (unsigned char *)out_element + iterator->key_offset,
        &key,
        sizeof(key)
      );
    } else {
      status = hxc_array_ref_get_copy(
        iterator->array,
        iterator->cursor,
        out_element
      );
      if (status != HXC_STATUS_OK) {
        return status;
      }
    }
    iterator->cursor++;
    return HXC_STATUS_OK;
  }
  if (iterator->cursor >= iterator->length) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  memcpy(
    out_element,
    hxc_iterator_slot(iterator, iterator->cursor),
    iterator->elements.size
  );
  if (iterator->root_offsets.memory != NULL) {
    const size_t *offsets = iterator->root_offsets.memory;
    const void **roots = iterator->root_slots.memory;
    size_t root = offsets[iterator->cursor];
    while (root < offsets[iterator->cursor + 1u]) {
      roots[root++] = NULL;
    }
  }
  iterator->cursor++;
  return HXC_STATUS_OK;
}
