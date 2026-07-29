/*
 * hxrt feature: bytes-string.
 *
 * This narrow composition layer turns a checked range of mutable Haxe Bytes
 * into one independent immutable Haxe String. Keeping it separate means binary
 * programs do not package owned-string allocation or UTF-8 decoding unless
 * they actually ask to interpret bytes as text.
 */
#ifndef HXRT_BYTES_STRING_H_INCLUDED
#define HXRT_BYTES_STRING_H_INCLUDED

#include "hxrt/bytes.h"
#include "hxrt/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Decode one checked byte range as UTF-8 and copy it into a fresh Haxe String.
 *
 * `position` and `length` use Haxe's byte-based `Bytes.getString` contract.
 * Invalid bounds or malformed UTF-8 fail before a String is published. The
 * returned String owns a separate copy, so later mutation or release of
 * `bytes` cannot change it.
 */
HXC_API hxc_status hxc_bytes_ref_get_string_utf8(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t length,
  hxc_allocator allocator,
  hxc_string *out_string
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_BYTES_STRING_H_INCLUDED */
