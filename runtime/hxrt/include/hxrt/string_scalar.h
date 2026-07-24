/*
 * hxrt feature: string-scalar.
 *
 * This allocation-free slice owns valid-UTF-8 inspection and Unicode-scalar
 * indexing for the private hxc_string view. Generated Haxe selects it for
 * ordinary String operations such as charAt without pulling in the allocator,
 * owned strings, mutable builders, or CString conversion. Checked native
 * helpers retain hxc_status results; hxc_string_char_at is total because Haxe
 * defines negative and out-of-range indices as the empty String.
 */
#ifndef HXRT_STRING_SCALAR_H_INCLUDED
#define HXRT_STRING_SCALAR_H_INCLUDED

#include "hxrt/status.h"
#include "hxrt/string_literal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Untrusted or binary bytes. No encoding, ownership, or terminator is implied. */
typedef struct hxc_byte_view {
  const uint8_t *data;
  size_t length;
} hxc_byte_view;

#define HXC_BYTE_VIEW_INITIALIZER \
  { NULL, 0u }

/** Validate shortest-form scalar UTF-8. Scalar output is written only on success. */
HXC_API hxc_status hxc_utf8_validate(
  hxc_byte_view source,
  size_t *out_scalar_length
);

/** Validate the complete private String invariant. */
HXC_API bool hxc_string_is_valid(hxc_string value);

/** Count Unicode scalar values; the output is unchanged for invalid input. */
HXC_API hxc_status hxc_string_scalar_length(
  hxc_string value,
  size_t *out_length
);

/** Read one zero-based Unicode scalar index. */
HXC_API hxc_status hxc_string_scalar_at(
  hxc_string value,
  size_t scalar_index,
  uint32_t *out_scalar
);

/**
 * Produce an allocation-free scalar-indexed slice tied to the source lifetime.
 * A suffix retains the trailing-NUL fact; an interior slice does not invent one.
 */
HXC_API hxc_status hxc_string_slice(
  hxc_string source,
  size_t scalar_start,
  size_t scalar_length,
  hxc_string *out_slice
);

/**
 * Implement Haxe String.charAt without allocation.
 *
 * The returned view borrows the receiver's immutable storage. A negative or
 * out-of-range index returns an empty view. The private hxc_string contract
 * requires valid UTF-8; an invalid internal value also returns empty rather
 * than exposing an unsafe byte range.
 */
HXC_API hxc_string hxc_string_char_at(
  hxc_string source,
  int32_t scalar_index
);

/** Byte-wise fast comparison of canonical UTF-8 scalar sequences. */
HXC_API hxc_status hxc_string_compare(
  hxc_string left,
  hxc_string right,
  int32_t *out_order
);

/** Stable 32-bit FNV-1a over canonical UTF-8 bytes; embedded NUL is included. */
HXC_API hxc_status hxc_string_hash(
  hxc_string value,
  uint32_t *out_hash
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STRING_SCALAR_H_INCLUDED */
