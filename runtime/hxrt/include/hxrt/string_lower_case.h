/*
 * hxrt feature: string-lower-case.
 *
 * This allocator-backed slice owns locale-independent Haxe String lower case
 * conversion. It depends on the ordinary managed String builder but remains a
 * separate feature so programs that only construct, concatenate, or inspect
 * Strings do not package the Unicode mapping table.
 */
#ifndef HXRT_STRING_LOWER_CASE_H_INCLUDED
#define HXRT_STRING_LOWER_CASE_H_INCLUDED

#include "hxrt/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Return a fresh managed String using the pinned Haxe Eval lowercase mapping.
 *
 * Mapping is locale-independent, preserves embedded NUL, maps each scalar to
 * at most one scalar, and leaves values outside Eval's table unchanged. The
 * output slot must contain HXC_STRING_INITIALIZER. Invalid UTF-8, allocation
 * failure, or an occupied output slot leaves the output unchanged.
 */
HXC_API hxc_status hxc_string_to_lower_case(
  hxc_string source,
  hxc_allocator allocator,
  hxc_string *out_string
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STRING_LOWER_CASE_H_INCLUDED */
