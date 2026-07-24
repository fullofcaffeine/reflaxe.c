/*
 * hxrt feature: string-split.
 *
 * This narrow composition layer turns one immutable Haxe String into a managed
 * Array<String>. It stays separate from the general String owner so programs
 * that never call `String.split` do not package Array code.
 */
#ifndef HXRT_STRING_SPLIT_H_INCLUDED
#define HXRT_STRING_SPLIT_H_INCLUDED

#include "hxrt/array.h"
#include "hxrt/string.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Split `source` with the pinned Haxe Unicode-scalar rules.
 *
 * `elements` is the compiler-generated lifecycle plan for exactly
 * `Array<String>`; its copy callback retains each borrowed slice and its
 * destroy callback releases that owner. The output remains unchanged on
 * failure, including when allocation or an element retain fails.
 */
HXC_API hxc_status hxc_string_split(
  hxc_string source,
  hxc_string delimiter,
  hxc_allocator allocator,
  hxc_array_element_ops elements,
  hxc_array_ref **out_parts
);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STRING_SPLIT_H_INCLUDED */
