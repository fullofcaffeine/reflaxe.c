/*
 * hxrt feature: bytes.
 *
 * A Haxe Bytes value is fixed-length mutable binary storage with shared
 * identity. It is not text and it is not a resizable Array<Int>. Generated
 * code retains and releases this private reference-counted owner; every index
 * and size crosses a checked status boundary before C pointer arithmetic.
 */
#ifndef HXRT_BYTES_H_INCLUDED
#define HXRT_BYTES_H_INCLUDED

#include "hxrt/allocator.h"
#include "hxrt/string_literal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Private shared owner for one exact-length sequence of arbitrary bytes. */
typedef struct hxc_bytes_ref {
  size_t references;
  hxc_allocation storage;
  size_t length;
  hxc_allocator allocator;
} hxc_bytes_ref;

/** Allocate a fixed-length buffer and initialize every byte to zero. */
HXC_API hxc_status hxc_bytes_ref_create_zeroed(
  hxc_allocator allocator,
  int32_t length,
  hxc_bytes_ref **out_bytes
);

/** Copy an exact byte view into a newly owned buffer. */
HXC_API hxc_status hxc_bytes_ref_create_copy(
  hxc_allocator allocator,
  const uint8_t *source,
  size_t length,
  hxc_bytes_ref **out_bytes
);

/** Copy the exact UTF-8 bytes of a validated Haxe String view. */
HXC_API hxc_status hxc_bytes_ref_create_utf8_copy(
  hxc_allocator allocator,
  hxc_string source,
  hxc_bytes_ref **out_bytes
);

/** Validate reference count, allocator identity, and exact storage length. */
HXC_API bool hxc_bytes_ref_is_valid(const hxc_bytes_ref *bytes);

/** Acquire one alias, rejecting reference-count overflow before mutation. */
HXC_API hxc_status hxc_bytes_ref_retain(hxc_bytes_ref *bytes);

/** Release one alias and free the buffer after the final release. */
HXC_API hxc_status hxc_bytes_ref_release(hxc_bytes_ref *bytes);

/** Read a length that is guaranteed to fit Haxe's signed 32-bit Int. */
HXC_API hxc_status hxc_bytes_ref_length(
  const hxc_bytes_ref *bytes,
  int32_t *out_length
);

/** Read one byte as a Haxe Int in the inclusive range 0 through 255. */
HXC_API hxc_status hxc_bytes_ref_get(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t *out_value
);

/** Store the low eight bits of a Haxe Int at one checked position. */
HXC_API hxc_status hxc_bytes_ref_set(
  hxc_bytes_ref *bytes,
  int32_t position,
  int32_t value
);

/** Copy a checked range into an independent newly owned Bytes value. */
HXC_API hxc_status hxc_bytes_ref_sub(
  const hxc_bytes_ref *bytes,
  int32_t position,
  int32_t length,
  hxc_bytes_ref **out_bytes
);

/**
 * Copy a checked range, with memmove-like behavior when source and destination
 * are the same Bytes value and their ranges overlap.
 */
HXC_API hxc_status hxc_bytes_ref_blit(
  hxc_bytes_ref *destination,
  int32_t destination_position,
  const hxc_bytes_ref *source,
  int32_t source_position,
  int32_t length
);

/** Fill a checked range with the low eight bits of one Haxe Int. */
HXC_API hxc_status hxc_bytes_ref_fill(
  hxc_bytes_ref *bytes,
  int32_t position,
  int32_t length,
  int32_t value
);

/** Apply Haxe Bytes.compare ordering without subtracting overflowing sizes. */
HXC_API hxc_status hxc_bytes_ref_compare(
  const hxc_bytes_ref *left,
  const hxc_bytes_ref *right,
  int32_t *out_order
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_BYTES_H_INCLUDED */
