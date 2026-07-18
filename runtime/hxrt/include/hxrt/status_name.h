/*
 * hxrt feature: status-name (native-seed-only).
 *
 * Native smoke diagnostics use this helper to render hxc_status values. It is
 * not selected by generated Haxe, owns no returned memory (all names have
 * static lifetime), cannot fail, and has no mutable or thread state. status.h
 * owns the numeric vocabulary; status.c owns the lookup implementation.
 */
#ifndef HXRT_STATUS_NAME_H_INCLUDED
#define HXRT_STATUS_NAME_H_INCLUDED

#include "hxrt/status.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Stable symbolic status name; never returns null. */
HXC_API const char *hxc_status_name(hxc_status status);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_STATUS_NAME_H_INCLUDED */
