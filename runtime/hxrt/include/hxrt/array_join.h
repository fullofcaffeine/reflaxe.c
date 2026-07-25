/*
 * hxrt feature: array-join.
 *
 * This narrow composition layer turns one managed Array<String> into one
 * managed String. Keeping it separate means ordinary Array and String programs
 * do not package cross-feature code when they never call `Array.join`.
 */
#ifndef HXRT_ARRAY_JOIN_H_INCLUDED
#define HXRT_ARRAY_JOIN_H_INCLUDED

#include "hxrt/array.h"
#include "hxrt/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Join an exact managed Array<String> with one immutable separator.
 *
 * Elements and the separator are read-only borrows. The builder publishes one
 * fresh reference-counted String only after every append succeeds; allocation
 * failure destroys private partial storage and leaves `out_string` unchanged.
 */
HXC_API hxc_status hxc_array_string_join(
  const hxc_array_ref *array,
  hxc_string separator,
  hxc_allocator allocator,
  hxc_string *out_string
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_ARRAY_JOIN_H_INCLUDED */
