/*
 * hxrt feature: string-float (compiler-selectable, hosted only).
 *
 * Float text conversion needs the host C library's correctly rounded decimal
 * formatter and parser. Keeping that dependency separate preserves the
 * freestanding `string` feature while giving hosted programs Haxe-compatible
 * `Std.string(Float)` behavior.
 */
#ifndef HXRT_STRING_FLOAT_H_INCLUDED
#define HXRT_STRING_FLOAT_H_INCLUDED

#include "hxrt/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Format one Haxe Float as an independently owned String.
 *
 * Finite values use the first 12-, 15-, or 18-significant-digit spelling that
 * round-trips to the same binary64 value. The decimal separator is always a
 * dot, independent of the process locale. The output slot must contain
 * HXC_STRING_INITIALIZER.
 */
HXC_API hxc_status hxc_string_from_float64(
  double value,
  hxc_allocator allocator,
  hxc_string *out_string
);

#if defined(__cplusplus)
}
#endif

#endif /* HXRT_STRING_FLOAT_H_INCLUDED */
