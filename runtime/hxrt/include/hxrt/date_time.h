/*
 * hxrt feature: date-time (compiler-selectable, hosted-only).
 *
 * This narrow adapter exposes wall-clock, monotonic-clock, local-calendar, and
 * timezone services that ISO C cannot express as direct deterministic
 * arithmetic. Every operation writes through a checked out parameter and
 * returns hxc_status; it allocates nothing and keeps no process-global state.
 * Timezone behavior comes from the host process, including its explicit TZ
 * setting when the platform supports that convention.
 */
#ifndef HXRT_DATE_TIME_H_INCLUDED
#define HXRT_DATE_TIME_H_INCLUDED

#include "hxrt/status.h"

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** Read Unix-epoch wall-clock milliseconds. */
HXC_API hxc_status hxc_date_time_wall_milliseconds(double *out_milliseconds);

/** Read monotonic elapsed seconds from an unspecified process-wide epoch. */
HXC_API hxc_status hxc_date_time_monotonic_seconds(double *out_seconds);

/** Convert normalized local civil fields to Unix-epoch milliseconds. */
HXC_API hxc_status hxc_date_time_local_to_milliseconds(int32_t year, int32_t month, int32_t day, int32_t hour,
                                                       int32_t minute, int32_t second, double *out_milliseconds);

/** Return Haxe's UTC-minus-local offset in whole minutes at one timestamp. */
HXC_API hxc_status hxc_date_time_timezone_offset(double milliseconds, int32_t *out_minutes);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXRT_DATE_TIME_H_INCLUDED */
