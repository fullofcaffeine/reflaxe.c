/*
 * Implementation of the compiler-selectable feature `string-split`.
 *
 * This composition scans immutable UTF-8 Strings only at Unicode-scalar
 * boundaries and appends borrowed pieces to a managed Array<String>. The
 * compiler supplies exact String element callbacks, so each stored piece
 * retains its source owner. Any failure destroys the partial result before the
 * function returns.
 */
#include "hxrt/string_split.h"

#include "hxrt/string_decode.h"

#include <string.h>

static hxc_string hxc_string_split_view(
  hxc_string source,
  size_t first,
  size_t last
) {
  hxc_string result = HXC_STRING_EMPTY_INITIALIZER;
  result.data = source.data + first;
  result.byte_length = last - first;
  result.has_trailing_nul = source.has_trailing_nul
    && last == source.byte_length;
  result.owner = source.owner;
  return result;
}

static hxc_status hxc_string_split_push(
  hxc_array_ref *parts,
  hxc_string value
) {
  int32_t ignored_length;
  return hxc_array_ref_push_copy(parts, &value, &ignored_length);
}

static hxc_status hxc_string_split_fail(
  hxc_array_ref *parts,
  hxc_status primary
) {
  hxc_status cleanup = hxc_array_ref_release(parts);
  return cleanup == HXC_STATUS_OK ? primary : cleanup;
}

hxc_status hxc_string_split(
  hxc_string source,
  hxc_string delimiter,
  hxc_allocator allocator,
  hxc_array_element_ops elements,
  hxc_array_ref **out_parts
) {
  hxc_array_ref *parts = NULL;
  hxc_status status;
  size_t segment_start = 0u;
  size_t byte_index = 0u;

  if (out_parts == NULL
    || !hxc_string_is_valid(source)
    || !hxc_string_is_valid(delimiter)
    || !hxc_allocator_is_valid(&allocator)
    || !hxc_array_element_ops_is_valid(&elements)
    || elements.size != sizeof(hxc_string)
    || elements.alignment != HXC_ALIGNOF(hxc_string)
    || elements.copy == NULL
    || elements.assign == NULL
    || elements.destroy == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }

  status = hxc_array_ref_create(allocator, elements, &parts);
  if (status != HXC_STATUS_OK) {
    return status;
  }

  if (delimiter.byte_length == 0u) {
    while (byte_index < source.byte_length) {
      hxc_utf8_step step = hxc_utf8_read(
        source.data + byte_index,
        source.byte_length - byte_index
      );
      hxc_string scalar = hxc_string_split_view(
        source,
        byte_index,
        byte_index + step.consumed
      );
      status = hxc_string_split_push(parts, scalar);
      if (status != HXC_STATUS_OK) {
        return hxc_string_split_fail(parts, status);
      }
      byte_index += step.consumed;
    }
    *out_parts = parts;
    return HXC_STATUS_OK;
  }

  while (byte_index < source.byte_length) {
    if (delimiter.byte_length <= source.byte_length - byte_index
      && memcmp(
        source.data + byte_index,
        delimiter.data,
        delimiter.byte_length
      ) == 0) {
      hxc_string part = hxc_string_split_view(
        source,
        segment_start,
        byte_index
      );
      status = hxc_string_split_push(parts, part);
      if (status != HXC_STATUS_OK) {
        return hxc_string_split_fail(parts, status);
      }
      byte_index += delimiter.byte_length;
      segment_start = byte_index;
    } else {
      hxc_utf8_step step = hxc_utf8_read(
        source.data + byte_index,
        source.byte_length - byte_index
      );
      byte_index += step.consumed;
    }
  }

  status = hxc_string_split_push(
    parts,
    hxc_string_split_view(source, segment_start, source.byte_length)
  );
  if (status != HXC_STATUS_OK) {
    return hxc_string_split_fail(parts, status);
  }
  *out_parts = parts;
  return HXC_STATUS_OK;
}
