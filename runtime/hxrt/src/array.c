/*
 * Implementation of native-seed-only feature `array`.
 *
 * Array differential and selective-package native fixtures call this runtime-
 * sized unboxed owner; generated Haxe cannot select it yet. The slice depends
 * on alloc, stores allocator and element-callback lifetimes in the owner, and
 * invalidates element borrows on mutation. Fallible operations preserve the
 * previous live value, cleanup runs in reverse element order, and no hidden
 * global/thread state or application-public layout is introduced.
 */
#include "hxrt/array.h"

static bool hxc_array_is_power_of_two(size_t value) {
  return value != 0u && (value & (value - 1u)) == 0u;
}

static bool hxc_array_has_lifecycle(
  const hxc_array_element_ops *elements
) {
  return elements->copy != NULL;
}

static bool hxc_array_is_initializer(const hxc_array *array) {
  return array != NULL
    && array->storage.memory == NULL
    && array->storage.size == 0u
    && array->storage.alignment == 0u
    && array->storage.allocator.context == NULL
    && array->storage.allocator.allocate == NULL
    && array->storage.allocator.reallocate == NULL
    && array->storage.allocator.release == NULL
    && array->length == 0u
    && array->capacity == 0u
    && array->elements.size == 0u
    && array->elements.alignment == 0u
    && array->elements.context == NULL
    && array->elements.copy == NULL
    && array->elements.assign == NULL
    && array->elements.destroy == NULL;
}

static void hxc_array_clear(hxc_array *array) {
  array->storage = (hxc_allocation)HXC_ALLOCATION_INITIALIZER;
  array->length = 0u;
  array->capacity = 0u;
  array->elements.size = 0u;
  array->elements.alignment = 0u;
  array->elements.context = NULL;
  array->elements.copy = NULL;
  array->elements.assign = NULL;
  array->elements.destroy = NULL;
}

static void *hxc_array_slot(hxc_array *array, size_t index) {
  return (unsigned char *)array->storage.memory
    + (index * array->elements.size);
}

static const void *hxc_array_slot_const(
  const hxc_array *array,
  size_t index
) {
  return (const unsigned char *)array->storage.memory
    + (index * array->elements.size);
}

static void hxc_array_copy_bytes(
  void *destination,
  const void *source,
  size_t size
) {
  unsigned char *output = (unsigned char *)destination;
  const unsigned char *input = (const unsigned char *)source;
  size_t index;
  for (index = 0u; index < size; index++) {
    output[index] = input[index];
  }
}

static void hxc_array_move_bytes_right(
  void *destination,
  const void *source,
  size_t size
) {
  unsigned char *output = (unsigned char *)destination;
  const unsigned char *input = (const unsigned char *)source;
  /* Copy backwards so overlapping insertion shifts behave like memmove. */
  size_t index = size;
  while (index != 0u) {
    index--;
    output[index] = input[index];
  }
}

static hxc_status hxc_array_construct(
  const hxc_array *array,
  void *destination,
  const void *source
) {
  if (hxc_array_has_lifecycle(&array->elements)) {
    return array->elements.copy(
      array->elements.context,
      destination,
      source
    );
  }
  hxc_array_copy_bytes(destination, source, array->elements.size);
  return HXC_STATUS_OK;
}

static void hxc_array_destroy(const hxc_array *array, void *element) {
  if (hxc_array_has_lifecycle(&array->elements)) {
    array->elements.destroy(array->elements.context, element);
  }
}

static bool hxc_array_find_source(
  const hxc_array *array,
  const void *element,
  size_t *out_index
) {
  size_t index;
  /* Record a slot identity before reserve can relocate the backing storage. */
  for (index = 0u; index < array->length; index++) {
    if (hxc_array_slot_const(array, index) == element) {
      *out_index = index;
      return true;
    }
  }
  return false;
}

bool hxc_array_element_ops_is_valid(
  const hxc_array_element_ops *elements
) {
  bool has_copy;
  bool has_assign;
  bool has_destroy;
  if (elements == NULL
    || elements->size == 0u
    || !hxc_array_is_power_of_two(elements->alignment)) {
    return false;
  }
#if SIZE_MAX > UINTPTR_MAX
  if (elements->alignment > (size_t)UINTPTR_MAX) {
    return false;
  }
#endif
  has_copy = elements->copy != NULL;
  has_assign = elements->assign != NULL;
  has_destroy = elements->destroy != NULL;
  return (has_copy && has_assign && has_destroy)
    || (!has_copy && !has_assign && !has_destroy);
}

hxc_status hxc_array_init(
  const hxc_allocator *allocator,
  hxc_array_element_ops elements,
  hxc_array *out_array
) {
  hxc_array array = HXC_ARRAY_INITIALIZER;
  hxc_status status;
  if (out_array == NULL
    || !hxc_array_is_initializer(out_array)
    || !hxc_array_element_ops_is_valid(&elements)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_allocation_allocate(
    allocator,
    0u,
    elements.size,
    elements.alignment,
    &array.storage
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  array.elements = elements;
  *out_array = array;
  return HXC_STATUS_OK;
}

bool hxc_array_is_valid(const hxc_array *array) {
  size_t expected_size;
  if (array == NULL
    || !hxc_array_element_ops_is_valid(&array->elements)
    || !hxc_allocation_is_valid(&array->storage)
    || array->storage.alignment != array->elements.alignment
    || array->length > array->capacity
    || hxc_size_mul(
      array->capacity,
      array->elements.size,
      &expected_size
    ) != HXC_STATUS_OK) {
    return false;
  }
  return array->storage.size == expected_size;
}

hxc_status hxc_array_reserve(
  hxc_array *array,
  size_t minimum_capacity
) {
  size_t maximum_capacity;
  size_t capacity;
  hxc_status status;
  if (!hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (minimum_capacity <= array->capacity) {
    return HXC_STATUS_OK;
  }
  maximum_capacity = SIZE_MAX / array->elements.size;
  if (minimum_capacity > maximum_capacity) {
    return HXC_STATUS_SIZE_OVERFLOW;
  }
  capacity = array->capacity == 0u ? 4u : array->capacity;
  if (capacity > maximum_capacity) {
    capacity = maximum_capacity;
  }
  /* Double deterministically, then jump exactly to the request near overflow. */
  while (capacity < minimum_capacity) {
    if (capacity > maximum_capacity / 2u) {
      capacity = minimum_capacity;
    } else {
      capacity *= 2u;
    }
  }
  status = hxc_allocation_resize(
    &array->storage,
    capacity,
    array->elements.size
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  array->capacity = capacity;
  return HXC_STATUS_OK;
}

hxc_status hxc_array_resize(
  hxc_array *array,
  size_t new_length,
  const void *default_element
) {
  size_t source_index = 0u;
  size_t constructed;
  bool source_is_slot;
  hxc_status status;
  if (!hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (new_length == array->length) {
    return HXC_STATUS_OK;
  }
  if (new_length < array->length) {
    size_t index = array->length;
    while (index > new_length) {
      index--;
      hxc_array_destroy(array, hxc_array_slot(array, index));
    }
    array->length = new_length;
    return HXC_STATUS_OK;
  }
  if (default_element == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  source_is_slot = hxc_array_find_source(
    array,
    default_element,
    &source_index
  );
  status = hxc_array_reserve(array, new_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (source_is_slot) {
    /* Reserve may move storage, so reconstruct the borrowed source by index. */
    default_element = hxc_array_slot_const(array, source_index);
  }
  constructed = array->length;
  while (constructed < new_length) {
    status = hxc_array_construct(
      array,
      hxc_array_slot(array, constructed),
      default_element
    );
    if (status != HXC_STATUS_OK) {
      /* Destroy only newly constructed elements; the old logical array survives. */
      while (constructed > array->length) {
        constructed--;
        hxc_array_destroy(array, hxc_array_slot(array, constructed));
      }
      return status;
    }
    constructed++;
  }
  array->length = new_length;
  return HXC_STATUS_OK;
}

hxc_status hxc_array_at(
  hxc_array *array,
  size_t index,
  void **out_element
) {
  if (out_element == NULL || !hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (index >= array->length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  *out_element = hxc_array_slot(array, index);
  return HXC_STATUS_OK;
}

hxc_status hxc_array_at_const(
  const hxc_array *array,
  size_t index,
  const void **out_element
) {
  if (out_element == NULL || !hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (index >= array->length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  *out_element = hxc_array_slot_const(array, index);
  return HXC_STATUS_OK;
}

hxc_status hxc_array_push_copy(
  hxc_array *array,
  const void *element
) {
  size_t source_index = 0u;
  size_t new_length;
  bool source_is_slot;
  hxc_status status;
  if (element == NULL || !hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  source_is_slot = hxc_array_find_source(array, element, &source_index);
  status = hxc_size_add(array->length, 1u, &new_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_array_reserve(array, new_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  if (source_is_slot) {
    /* Account for both reserve relocation and the insertion shift. */
    element = hxc_array_slot_const(array, source_index);
  }
  status = hxc_array_construct(
    array,
    hxc_array_slot(array, array->length),
    element
  );
  if (status != HXC_STATUS_OK) {
    return status;
  }
  array->length = new_length;
  return HXC_STATUS_OK;
}

hxc_status hxc_array_insert_copy(
  hxc_array *array,
  size_t index,
  const void *element
) {
  size_t source_index = 0u;
  size_t new_length;
  size_t shifted_size;
  bool source_is_slot;
  hxc_status status;
  if (element == NULL || !hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (index > array->length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  source_is_slot = hxc_array_find_source(array, element, &source_index);
  status = hxc_size_add(array->length, 1u, &new_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_array_reserve(array, new_length);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  shifted_size = (array->length - index) * array->elements.size;
  hxc_array_move_bytes_right(
    hxc_array_slot(array, index + 1u),
    hxc_array_slot_const(array, index),
    shifted_size
  );
  if (source_is_slot) {
    if (source_index >= index) {
      source_index++;
    }
    element = hxc_array_slot_const(array, source_index);
  }
  status = hxc_array_construct(
    array,
    hxc_array_slot(array, index),
    element
  );
  if (status != HXC_STATUS_OK) {
    /* Restore the byte-relocated live range before reporting copy failure. */
    hxc_array_copy_bytes(
      hxc_array_slot(array, index),
      hxc_array_slot_const(array, index + 1u),
      shifted_size
    );
    return status;
  }
  array->length = new_length;
  return HXC_STATUS_OK;
}

hxc_status hxc_array_set_copy(
  hxc_array *array,
  size_t index,
  const void *element
) {
  void *destination;
  if (element == NULL || !hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (index >= array->length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  destination = hxc_array_slot(array, index);
  if (destination == element) {
    return HXC_STATUS_OK;
  }
  if (hxc_array_has_lifecycle(&array->elements)) {
    return array->elements.assign(
      array->elements.context,
      destination,
      element
    );
  }
  hxc_array_copy_bytes(destination, element, array->elements.size);
  return HXC_STATUS_OK;
}

hxc_status hxc_array_remove_at(hxc_array *array, size_t index) {
  size_t shifted_size;
  if (!hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (index >= array->length) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  hxc_array_destroy(array, hxc_array_slot(array, index));
  shifted_size = (array->length - index - 1u) * array->elements.size;
  /* Live elements are relocatable bytes; the duplicate tail becomes non-live. */
  hxc_array_copy_bytes(
    hxc_array_slot(array, index),
    hxc_array_slot_const(array, index + 1u),
    shifted_size
  );
  array->length--;
  return HXC_STATUS_OK;
}

hxc_status hxc_array_move(
  hxc_array *source,
  hxc_array *out_array
) {
  if (!hxc_array_is_valid(source)
    || out_array == NULL
    || source == out_array
    || !hxc_array_is_initializer(out_array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  *out_array = *source;
  hxc_array_clear(source);
  return HXC_STATUS_OK;
}

hxc_status hxc_array_dispose(hxc_array *array) {
  hxc_status status;
  size_t index;
  if (array == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (hxc_array_is_initializer(array)) {
    return HXC_STATUS_OK;
  }
  if (!hxc_array_is_valid(array)) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  /* Reverse destruction mirrors stack-like construction and cleanup ordering. */
  index = array->length;
  while (index != 0u) {
    index--;
    hxc_array_destroy(array, hxc_array_slot(array, index));
  }
  status = hxc_allocation_dispose(&array->storage);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  hxc_array_clear(array);
  return HXC_STATUS_OK;
}
