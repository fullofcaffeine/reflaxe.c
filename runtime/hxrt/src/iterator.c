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
    return iterator->cursor <= iterator->length
      && iterator->array == NULL
      && hxc_iterator_element_ops_is_valid(&iterator->elements)
      && hxc_allocation_is_valid(&iterator->storage)
      && ((iterator->anchor == NULL && iterator->release_anchor == NULL)
        || (iterator->anchor != NULL && iterator->release_anchor != NULL));
  }
  if ((iterator->kind != HXC_ITERATOR_ARRAY_VALUES
      && iterator->kind != HXC_ITERATOR_ARRAY_PAIRS)
    || !hxc_array_ref_is_valid(iterator->array)
    || iterator->anchor != NULL
    || iterator->release_anchor != NULL
    || iterator->storage.memory != NULL
    || iterator->storage.size != 0u) {
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
  hxc_iterator_ref *iterator = NULL;
  hxc_status status;
  size_t constructed = 0u;
  if (out_iterator == NULL
    || *out_iterator != NULL
    || fill == NULL
    || !hxc_allocator_is_valid(&allocator)
    || !hxc_iterator_element_ops_is_valid(&elements)
    || ((anchor == NULL) != (release_anchor == NULL))) {
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
  if (status != HXC_STATUS_OK) {
    while (constructed != 0u) {
      constructed--;
      if (elements.destroy != NULL) {
        elements.destroy(elements.context, hxc_iterator_slot(iterator, constructed));
      }
    }
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
  iterator->cursor++;
  return HXC_STATUS_OK;
}
