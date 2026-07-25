/*
 * Implementation of the compiler-selectable feature `array-join`.
 *
 * The operation appends each already-valid UTF-8 element and the separator to
 * one growable buffer, then transfers that allocation into an ordinary Haxe
 * String owner. This avoids both repeated-concatenation quadratic copying and
 * a second full-result copy at completion.
 */
#include "hxrt/array_join.h"

static hxc_status hxc_array_join_fail(
  hxc_string_buffer *buffer,
  hxc_status primary
) {
  hxc_status cleanup = hxc_string_buffer_dispose(buffer);
  return cleanup == HXC_STATUS_OK ? primary : cleanup;
}

hxc_status hxc_array_string_join(
  const hxc_array_ref *array,
  hxc_string separator,
  hxc_allocator allocator,
  hxc_string *out_string
) {
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_status status;
  size_t index;

  if (!hxc_array_ref_is_valid(array)
    || array->value.elements.size != sizeof(hxc_string)
    || array->value.elements.alignment != HXC_ALIGNOF(hxc_string)
    || !hxc_string_is_valid(separator)
    || !hxc_allocator_is_valid(&allocator)
    || out_string == NULL
    || out_string->data != NULL
    || out_string->byte_length != 0u
    || out_string->has_trailing_nul
    || out_string->owner != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  status = hxc_string_buffer_init(&allocator, &buffer);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  for (index = 0u; index < array->value.length; index++) {
    const void *slot = NULL;
    const hxc_string *element;
    if (index != 0u) {
      hxc_byte_view separator_bytes;
      separator_bytes.data = separator.data;
      separator_bytes.length = separator.byte_length;
      status = hxc_string_buffer_append_utf8_checked(
        &buffer,
        separator_bytes
      );
      if (status != HXC_STATUS_OK) {
        return hxc_array_join_fail(&buffer, status);
      }
    }
    status = hxc_array_at_const(&array->value, index, &slot);
    if (status != HXC_STATUS_OK) {
      return hxc_array_join_fail(&buffer, status);
    }
    element = (const hxc_string *)slot;
    if (!hxc_string_is_valid(*element)) {
      return hxc_array_join_fail(&buffer, HXC_STATUS_INVALID_ARGUMENT);
    }
    {
      hxc_byte_view element_bytes;
      element_bytes.data = element->data;
      element_bytes.length = element->byte_length;
      status = hxc_string_buffer_append_utf8_checked(
        &buffer,
        element_bytes
      );
    }
    if (status != HXC_STATUS_OK) {
      return hxc_array_join_fail(&buffer, status);
    }
  }
  status = hxc_string_buffer_finish_ref(&buffer, out_string);
  if (status != HXC_STATUS_OK) {
    return hxc_array_join_fail(&buffer, status);
  }
  return HXC_STATUS_OK;
}
