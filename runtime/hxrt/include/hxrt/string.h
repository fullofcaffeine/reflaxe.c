#ifndef HXRT_STRING_H_INCLUDED
#define HXRT_STRING_H_INCLUDED

#include "hxrt/allocator.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_string_view {
  const uint8_t *data;
  size_t length;
} hxc_string_view;

typedef struct hxc_owned_string {
  uint8_t *data;
  size_t length;
  size_t capacity;
  hxc_allocator allocator;
} hxc_owned_string;

HXC_API hxc_string_view hxc_string_view_from_cstr(const char *value);
HXC_API hxc_status hxc_string_copy(
  hxc_string_view source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
);
HXC_API void hxc_owned_string_dispose(hxc_owned_string *value);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STRING_H_INCLUDED */
