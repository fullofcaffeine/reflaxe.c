/*
 * Implementation of the compiler-selectable feature `bytes-string`.
 *
 * The public Bytes owner remains opaque to Haxe code but exposes its exact
 * length and allocation to this internal runtime composition. One UTF-8
 * builder validates and copies the selected range, then transfers its storage
 * into the ordinary reference-counted Haxe String carrier.
 */
#include "hxrt/bytes_string.h"

static hxc_status hxc_bytes_string_fail(
  hxc_string_buffer *buffer,
  hxc_status primary
) {
  hxc_status cleanup = hxc_string_buffer_dispose(buffer);
  return cleanup == HXC_STATUS_OK ? primary : cleanup;
}

hxc_status hxc_bytes_ref_get_string_utf8(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t length,
  hxc_allocator allocator,
  hxc_string *out_string
) {
  hxc_string_buffer buffer = HXC_STRING_BUFFER_INITIALIZER;
  hxc_byte_view view = HXC_BYTE_VIEW_INITIALIZER;
  size_t start;
  size_t count;
  hxc_status status;

  if (!hxc_bytes_ref_is_valid(bytes)
    || !hxc_allocator_is_valid(&allocator)
    || out_string == NULL
    || out_string->data != NULL
    || out_string->byte_length != 0u
    || out_string->has_trailing_nul
    || out_string->owner != NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (position < 0 || length < 0) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  start = (size_t)position;
  count = (size_t)length;
  if (start > bytes->length || count > bytes->length - start) {
    return HXC_STATUS_OUT_OF_RANGE;
  }

  view.data = count == 0u
    ? NULL
    : (const uint8_t *)bytes->storage.memory + start;
  view.length = count;
  status = hxc_string_buffer_init(&allocator, &buffer);
  if (status != HXC_STATUS_OK) {
    return status;
  }
  status = hxc_string_buffer_append_utf8_checked(&buffer, view);
  if (status != HXC_STATUS_OK) {
    return hxc_bytes_string_fail(&buffer, status);
  }
  status = hxc_string_buffer_finish_ref(&buffer, out_string);
  if (status != HXC_STATUS_OK) {
    return hxc_bytes_string_fail(&buffer, status);
  }
  return HXC_STATUS_OK;
}
