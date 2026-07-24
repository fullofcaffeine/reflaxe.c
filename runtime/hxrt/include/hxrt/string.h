/*
 * hxrt feature: string (compiler-selectable and independently native-tested).
 *
 * The string differential/native package harness calls this API for runtime
 * UTF-8 validation, Unicode-scalar indexing, owned values, builders, slicing,
 * hashing, and explicit CString conversion. Generated Haxe selects the narrow
 * owner API for runtime `String.fromCharCode`, concatenation, and the ordinary
 * `StringBuf.addChar` path; literal-only programs still use string_literal.h
 * without allocation. A runtime-created ordinary Haxe String carries an
 * optional reference-counted owner inside its immutable view, so aliases,
 * fields, containers, and returns can share bytes safely.
 * The explicit hxc_owned_string boundary remains move-only for native callers.
 * Failures use hxc_status and preserve output slots. The slice has no hidden
 * global/thread state and all layouts are internal ABI.
 */
#ifndef HXRT_STRING_H_INCLUDED
#define HXRT_STRING_H_INCLUDED

#include "hxrt/allocator.h"
#include "hxrt/string_scalar.h"

#if defined(__cplusplus)
extern "C" {
#endif

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

#define HXC_OWNED_STRING_INITIALIZER \
  { HXC_STRING_INITIALIZER, HXC_ALLOCATION_INITIALIZER }
#define HXC_STRING_BUFFER_INITIALIZER \
  { HXC_ALLOCATION_INITIALIZER, 0u }
#define HXC_BORROWED_CSTRING_INITIALIZER \
  { NULL, 0u, NULL }
#define HXC_OWNED_CSTRING_INITIALIZER \
  { NULL, 0u, HXC_ALLOCATION_INITIALIZER }

/**
 * Acquire one logical alias of an ordinary Haxe String.
 *
 * Null and compiler-owned literal views have no owner, so retaining either is
 * an allocation-free no-op. Runtime-created views reject owner-count overflow.
 */
HXC_API hxc_status hxc_string_retain(hxc_string value);

/**
 * Release one ordinary Haxe String alias and reset the caller's slot to null.
 *
 * The final runtime-created alias frees its private UTF-8 backing allocation
 * through the allocator identity captured at construction. Literal views only
 * reset the slot because their bytes live for the whole program.
 */
HXC_API hxc_status hxc_string_release(hxc_string *value);

/**
 * Construct one valid UTF-8 scalar as an independently owned Haxe String.
 *
 * Surrogates, negative values, and values above U+10FFFF become U+FFFD, matching
 * ADR 0004. The output slot must contain HXC_STRING_INITIALIZER.
 */
HXC_API hxc_status hxc_string_from_scalar(
  int32_t scalar,
  hxc_allocator allocator,
  hxc_string *out_string
);

/**
 * Concatenate two nullable-checked String values into one owned Haxe String.
 *
 * The output slot must contain HXC_STRING_INITIALIZER. Size overflow and
 * allocation failure leave it unchanged.
 */
HXC_API hxc_status hxc_string_concat_ref(
  hxc_string left,
  hxc_string right,
  hxc_allocator allocator,
  hxc_string *out_string
);

/** Length-delimited view of a C string; null is rejected. Encoding is unchecked. */
HXC_API hxc_status hxc_byte_view_from_cstring(
  const char *value,
  hxc_byte_view *out_view
);

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
