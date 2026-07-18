/*
 * hxrt feature: string-literal (compiler-selectable, dependency-only).
 *
 * Generated C constructs valid UTF-8 literals directly with this allocation-free
 * private view; a selected consumer such as io adds the header transitively.
 * The carrier borrows compiler-owned static bytes, never allocates or fails, and
 * has no mutable/thread state. It is an internal call layout, not a public String
 * ABI and not evidence for the native-seed full string feature.
 */
#ifndef HXRT_STRING_LITERAL_H_INCLUDED
#define HXRT_STRING_LITERAL_H_INCLUDED

#include "hxrt/base.h"

/**
 * Private immutable String value used by compiler-owned literal storage.
 *
 * `data[0..byte_length)` is valid shortest-form UTF-8 for Unicode scalar
 * values. Embedded NUL is ordinary content. `has_trailing_nul` describes one
 * byte outside the logical value and never changes its length or equality.
 */
typedef struct hxc_string {
  const uint8_t *data;
  size_t byte_length;
  bool has_trailing_nul;
} hxc_string;

#define HXC_STRING_INITIALIZER \
  { NULL, 0u, false }

/**
 * Compiler-owned direct literal initializer.
 *
 * `value` must be an actual valid UTF-8 C string-literal array, not a pointer.
 * Embedded NUL bytes remain content because `sizeof` supplies the byte length.
 */
#define HXC_STRING_LITERAL(value) \
  { (const uint8_t *)(value), sizeof(value) - 1u, true }

#endif /* HXRT_STRING_LITERAL_H_INCLUDED */
