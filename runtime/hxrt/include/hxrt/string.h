#ifndef HXRT_STRING_H_INCLUDED
#define HXRT_STRING_H_INCLUDED

#include "hxrt/allocator.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Untrusted or binary bytes. No encoding, ownership, or terminator is implied. */
typedef struct hxc_byte_view {
  const uint8_t *data;
  size_t length;
} hxc_byte_view;

/**
 * Private runtime String value; this is not a stable exported C ABI.
 *
 * Runtime and compiler constructors admit only shortest-form UTF-8 scalar
 * sequences. `has_trailing_nul` is a storage fact outside the logical byte
 * length, not part of String equality or content.
 */
typedef struct hxc_string {
  const uint8_t *data;
  size_t byte_length;
  bool has_trailing_nul;
} hxc_string;

/** Allocator-owned immutable String storage. Move only by convention. */
typedef struct hxc_owned_string {
  hxc_string value;
  hxc_allocation storage;
} hxc_owned_string;

/** Mutable valid-UTF-8 construction buffer; finish transfers its allocation. */
typedef struct hxc_string_buffer {
  hxc_allocation storage;
  size_t byte_length;
} hxc_string_buffer;

/**
 * NUL-terminated borrowed bytes tied to `owner`.
 *
 * The pointer is valid only while the owner and its stable storage remain
 * alive. Copying this record does not extend that lifetime.
 */
typedef struct hxc_borrowed_cstring {
  const char *data;
  size_t byte_length;
  const hxc_string *owner;
} hxc_borrowed_cstring;

/** Allocator-owned NUL-terminated bytes. Move only by convention. */
typedef struct hxc_owned_cstring {
  char *data;
  size_t byte_length;
  hxc_allocation storage;
} hxc_owned_cstring;

#define HXC_BYTE_VIEW_INITIALIZER \
  { NULL, 0u }
#define HXC_STRING_INITIALIZER \
  { NULL, 0u, false }
#define HXC_OWNED_STRING_INITIALIZER \
  { HXC_STRING_INITIALIZER, HXC_ALLOCATION_INITIALIZER }
#define HXC_STRING_BUFFER_INITIALIZER \
  { HXC_ALLOCATION_INITIALIZER, 0u }
#define HXC_BORROWED_CSTRING_INITIALIZER \
  { NULL, 0u, NULL }
#define HXC_OWNED_CSTRING_INITIALIZER \
  { NULL, 0u, HXC_ALLOCATION_INITIALIZER }

/**
 * Compiler-owned direct literal initializer.
 *
 * `value` must be an actual valid UTF-8 C string-literal array, not a pointer.
 * Embedded NUL bytes remain content because `sizeof` supplies the byte length.
 */
#define HXC_STRING_LITERAL(value) \
  { (const uint8_t *)(value), sizeof(value) - 1u, true }

/** Length-delimited view of a C string; null is rejected. Encoding is unchecked. */
HXC_API hxc_status hxc_byte_view_from_cstring(
  const char *value,
  hxc_byte_view *out_view
);

/** Validate shortest-form scalar UTF-8. Scalar output is written only on success. */
HXC_API hxc_status hxc_utf8_validate(
  hxc_byte_view source,
  size_t *out_scalar_length
);

/** Validate the complete private String invariant. */
HXC_API bool hxc_string_is_valid(hxc_string value);

/** Checked decode/copy. Malformed input is rejected before allocation. */
HXC_API hxc_status hxc_string_from_utf8_checked(
  hxc_byte_view source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
);

/**
 * Total decode/copy. Each maximal ill-formed subpart becomes one U+FFFD.
 * The result is valid UTF-8 and is published only after all allocation succeeds.
 */
HXC_API hxc_status hxc_string_from_utf8_lossy(
  hxc_byte_view source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
);

/** Copy a known-valid String through the selected allocator. */
HXC_API hxc_status hxc_string_copy(
  hxc_string source,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
);

/** Concatenate with checked sizing and at most one allocation. */
HXC_API hxc_status hxc_string_concat(
  hxc_string left,
  hxc_string right,
  const hxc_allocator *allocator,
  hxc_owned_string *out_string
);

/** Release owned String storage through its retained allocator identity. */
HXC_API hxc_status hxc_owned_string_dispose(hxc_owned_string *value);

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

/** Initialize an empty builder without invoking the allocation callback. */
HXC_API hxc_status hxc_string_buffer_init(
  const hxc_allocator *allocator,
  hxc_string_buffer *out_buffer
);

/** Borrow the current builder value until its next mutation or disposal. */
HXC_API hxc_status hxc_string_buffer_view(
  const hxc_string_buffer *buffer,
  hxc_string *out_view
);

/** Append validated UTF-8; malformed input and allocation failure are atomic. */
HXC_API hxc_status hxc_string_buffer_append_utf8_checked(
  hxc_string_buffer *buffer,
  hxc_byte_view source
);

/** Append a scalar, replacing surrogates and out-of-range values with U+FFFD. */
HXC_API hxc_status hxc_string_buffer_append_scalar(
  hxc_string_buffer *buffer,
  uint32_t scalar
);

/** Transfer the builder allocation into an immutable owner. */
HXC_API hxc_status hxc_string_buffer_finish(
  hxc_string_buffer *buffer,
  hxc_owned_string *out_string
);

/** Dispose an unfinished builder through its retained allocator. */
HXC_API hxc_status hxc_string_buffer_dispose(hxc_string_buffer *buffer);

/**
 * Borrow a CString only from stable trailing-NUL storage.
 * Embedded NUL is rejected instead of truncating.
 */
HXC_API hxc_status hxc_string_borrow_cstring(
  const hxc_string *source,
  hxc_borrowed_cstring *out_cstring
);

/** Copy to explicitly owned NUL-terminated storage; embedded NUL is rejected. */
HXC_API hxc_status hxc_string_to_cstring_owned(
  const hxc_string *source,
  const hxc_allocator *allocator,
  hxc_owned_cstring *out_cstring
);

/** Release an owned CString through its retained allocator identity. */
HXC_API hxc_status hxc_owned_cstring_dispose(hxc_owned_cstring *value);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STRING_H_INCLUDED */
