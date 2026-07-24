/*
 * hxrt feature: string-literal (compiler-selectable, dependency-only).
 *
 * Generated C constructs valid UTF-8 literals directly with this allocation-free
 * private view; a selected consumer such as io adds the header transitively.
 * Literal values borrow immutable bytes and never allocate. Runtime-created
 * values may use the optional private owner pointer, but the literal-only
 * feature neither defines nor selects that owner. A null data pointer is Haxe
 * `null`, while a real empty String has a non-null zero-length address. The
 * carrier has no mutable/thread state. It is an internal call layout, not a
 * public String ABI and not evidence for the broader string feature.
 */
#ifndef HXRT_STRING_LITERAL_H_INCLUDED
#define HXRT_STRING_LITERAL_H_INCLUDED

#include "hxrt/base.h"

/**
 * Private immutable String value used by compiler-owned literal storage.
 *
 * When `data` is non-null, `data[0..byte_length)` is valid shortest-form UTF-8
 * for Unicode scalar values. Embedded NUL is ordinary content.
 * `has_trailing_nul` describes one byte outside the logical value and never
 * changes its length or equality. `{ NULL, 0, false }` is Haxe `null`, not the
 * empty String.
 */
typedef struct hxc_string {
  const uint8_t *data;
  size_t byte_length;
  bool has_trailing_nul;
  void *owner;
} hxc_string;

#define HXC_STRING_INITIALIZER \
  { NULL, 0u, false, NULL }

/** A real non-null empty Haxe String backed by compiler-owned static storage. */
#define HXC_STRING_EMPTY_INITIALIZER \
  { (const uint8_t *)"", 0u, true, NULL }

/**
 * Compiler-owned direct literal initializer.
 *
 * `value` must be an actual valid UTF-8 C string-literal array, not a pointer.
 * Embedded NUL bytes remain content because `sizeof` supplies the byte length.
 */
#define HXC_STRING_LITERAL(value) \
  { (const uint8_t *)(value), sizeof(value) - 1u, true, NULL }

#endif /* HXRT_STRING_LITERAL_H_INCLUDED */
