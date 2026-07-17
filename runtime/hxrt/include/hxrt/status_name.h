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
