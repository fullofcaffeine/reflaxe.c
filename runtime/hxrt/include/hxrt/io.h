#ifndef HXRT_IO_H_INCLUDED
#define HXRT_IO_H_INCLUDED

#include "hxrt/status.h"
#include "hxrt/string_literal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Write every logical String byte followed by the hosted text newline.
 *
 * The input must satisfy the compiler-owned `hxc_string` invariant. Embedded
 * NUL is ordinary content. Success also proves the stream was flushed; write
 * and flush failures return HXC_STATUS_IO_ERROR. This narrow feature is
 * compiler-selectable only for the hosted environment.
 */
HXC_API hxc_status hxc_io_println(hxc_string value);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_IO_H_INCLUDED */
