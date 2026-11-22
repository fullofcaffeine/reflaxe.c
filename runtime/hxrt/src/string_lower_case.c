/*
 * Implementation of compiler-selectable feature `string-lower-case`.
 *
 * The mapping table is generated from pinned Haxe Eval instead of libc locale
 * state. A checked String builder owns all temporary storage and transfers one
 * fresh reference-counted result only after the complete conversion succeeds.
 */
#include "hxrt/string_lower_case.h"
#include "hxrt/string_lower_case_data.h"
#include "hxrt/string_decode.h"

static hxc_status hxc_string_lower_case_fail(
  hxc_string_buffer *buffer,
  hxc_status primary
) {
  hxc_status cleanup = hxc_string_buffer_dispose(buffer);
  return cleanup == HXC_STATUS_OK ? primary : cleanup;
}

static uint32_t hxc_simple_lower_case(uint32_t source) {
  size_t lower = 0u;
  size_t upper = HXC_LOWER_CASE_MAPPING_COUNT;
  while (lower < upper) {
    const size_t middle = lower + ((upper - lower) / 2u);
    const uint32_t mapping_source = hxc_lower_case_mappings[middle][0];
    if (source < mapping_source) {
      upper = middle;
    } else if (source > mapping_source) {
      lower = middle + 1u;
    } else {
      return hxc_lower_case_mappings[middle][1];
    }
  }
  return source;
}

hxc_status hxc_string_to_lower_case(
  hxc_string source,
  hxc_allocator allocator,
  hxc_string *out_string
) {
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  size_t byte_offset = 0u;
  hxc_status status;

  if (!hxc_allocator_is_valid(&allocator)
    || out_string == NULL
    || out_string->data != NULL
    || out_string->byte_length != 0u
    || out_string->has_trailing_nul
    || out_string->owner != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_string_is_valid(source)) {
    return HXC_STATUS_INVALID_UTF8;
  }
  status = hxc_string_buffer_init(&allocator, &buffer);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  while (byte_offset < source.byte_length) {
    const hxc_utf8_step step = hxc_utf8_read(
      source.data + byte_offset,
      source.byte_length - byte_offset
    );
    if (!step.valid) {
      return hxc_string_lower_case_fail(&buffer, HXC_STATUS_INVALID_UTF8);
    }
    status = hxc_string_buffer_append_scalar(
      &buffer,
      hxc_simple_lower_case(step.scalar)
    );
    if (status != HXC_STATUS_OK) {
      return hxc_string_lower_case_fail(&buffer, status);
    }
    byte_offset += step.consumed;
  }
  status = hxc_string_buffer_finish_ref(&buffer, out_string);
  if (status != HXC_STATUS_OK) {
    return hxc_string_lower_case_fail(&buffer, status);
  }
  return HXC_STATUS_OK;
}
