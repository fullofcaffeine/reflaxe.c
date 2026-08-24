/*
 * Implementation of feature `iterator`.
 *
 * Generated Haxe and independent native fixtures use this owner for exact
 * typed snapshots. Aliases share one cursor. Construction publishes only a
 * complete snapshot, and final release destroys only unconsumed elements.
 */
#include "hxrt/iterator.h"

#include <string.h>

struct hxc_iterator_ref {
  size_t references;
  size_t cursor;
  size_t length;
  hxc_iterator_element_ops elements;
  hxc_allocator allocator;
  hxc_allocation storage;
  void *anchor;
  hxc_iterator_anchor_release_fn release_anchor;
};

static bool hxc_iterator_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

static void *hxc_iterator_slot(hxc_iterator_ref *iterator, size_t index) {
  return (unsigned char *)iterator->storage.memory
    + (index * iterator->elements.size);
}

static bool hxc_iterator_is_valid(const hxc_iterator_ref *iterator) {
  return iterator != NULL
    && iterator->references > 0u
    && iterator->cursor <= iterator->length
    && hxc_iterator_element_ops_is_valid(&iterator->elements)
    && hxc_allocator_is_valid(&iterator->allocator)
    && hxc_allocation_is_valid(&iterator->storage)
    && ((iterator->anchor == NULL && iterator->release_anchor == NULL)
      || (iterator->anchor != NULL && iterator->release_anchor != NULL));
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
  *out_has_next = iterator->cursor < iterator->length;
  return HXC_STATUS_OK;
}

hxc_status hxc_iterator_ref_next_move(
  hxc_iterator_ref *iterator,
  void *out_element
) {
  if (!hxc_iterator_is_valid(iterator)
    || out_element == NULL
    || iterator->cursor >= iterator->length) {
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
