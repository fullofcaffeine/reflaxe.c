/* Implementation of runtime feature `date-time` for hosted targets. */
#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

#include "hxrt/date_time.h"

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#endif

static bool hxc_date_time_time_t_from_milliseconds(double milliseconds, time_t *out_value) {
  double seconds;
  if (out_value == NULL || milliseconds != milliseconds) {
    return false;
  }
  seconds = milliseconds / 1000.0;
  if (sizeof(time_t) == sizeof(int32_t)) {
    if ((time_t)-1 < (time_t)0) {
      if (seconds < (double)INT32_MIN || seconds > (double)INT32_MAX) {
        return false;
      }
    } else if (seconds < 0.0 || seconds > (double)UINT32_MAX) {
      return false;
    }
  } else if (sizeof(time_t) == sizeof(int64_t)) {
    if ((time_t)-1 < (time_t)0) {
      if (seconds < -9223372036854775808.0 || seconds >= 9223372036854775808.0) {
        return false;
      }
    } else if (seconds < 0.0 || seconds >= 18446744073709551616.0) {
      return false;
    }
  } else {
    return false;
  }
  *out_value = (time_t)seconds;
  return true;
}

static bool hxc_date_time_local_tm(time_t value, struct tm *out_value) {
  if (out_value == NULL) {
    return false;
  }
#if defined(_WIN32)
  return localtime_s(out_value, &value) == 0;
#else
  return localtime_r(&value, out_value) != NULL;
#endif
}

static bool hxc_date_time_utc_tm(time_t value, struct tm *out_value) {
  if (out_value == NULL) {
    return false;
  }
#if defined(_WIN32)
  return gmtime_s(out_value, &value) == 0;
#else
  return gmtime_r(&value, out_value) != NULL;
#endif
}

/* Gregorian civil date to days relative to 1970-01-01. */
static int64_t hxc_date_time_days_from_civil(int64_t year, int64_t month, int64_t day) {
  const int64_t adjusted_year = year - (month <= 2 ? 1 : 0);
  const int64_t era = (adjusted_year >= 0 ? adjusted_year : adjusted_year - 399) / 400;
  const uint32_t year_of_era = (uint32_t)(adjusted_year - era * 400);
  const uint32_t shifted_month = (uint32_t)(month + (month > 2 ? -3 : 9));
  const uint32_t day_of_year = (153u * shifted_month + 2u) / 5u + (uint32_t)day - 1u;
  const uint32_t day_of_era = year_of_era * 365u + year_of_era / 4u - year_of_era / 100u + day_of_year;
  return era * 146097 + (int64_t)day_of_era - 719468;
}

static int64_t hxc_date_time_tm_seconds(const struct tm *value) {
  const int64_t days = hxc_date_time_days_from_civil((int64_t)value->tm_year + 1900, (int64_t)value->tm_mon + 1,
                                                     (int64_t)value->tm_mday);
  return days * 86400 + (int64_t)value->tm_hour * 3600 + (int64_t)value->tm_min * 60 + (int64_t)value->tm_sec;
}

hxc_status hxc_date_time_wall_milliseconds(double *out_milliseconds) {
  struct timespec value;
  if (out_milliseconds == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (timespec_get(&value, TIME_UTC) != TIME_UTC) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  *out_milliseconds = (double)value.tv_sec * 1000.0 + (double)value.tv_nsec / 1000000.0;
  return HXC_STATUS_OK;
}

hxc_status hxc_date_time_monotonic_seconds(double *out_seconds) {
  if (out_seconds == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
#if defined(_WIN32)
  LARGE_INTEGER frequency;
  LARGE_INTEGER counter;
  if (QueryPerformanceFrequency(&frequency) == 0 || QueryPerformanceCounter(&counter) == 0 || frequency.QuadPart <= 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  *out_seconds = (double)counter.QuadPart / (double)frequency.QuadPart;
#else
  struct timespec value;
  if (clock_gettime(CLOCK_MONOTONIC, &value) != 0) {
    return HXC_STATUS_INTERNAL_ERROR;
  }
  *out_seconds = (double)value.tv_sec + (double)value.tv_nsec / 1000000000.0;
#endif
  return HXC_STATUS_OK;
}

hxc_status hxc_date_time_local_to_milliseconds(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute,
                                               int32_t second, double *out_milliseconds) {
  struct tm local_value = {0};
  time_t timestamp;
  const int64_t tm_year = (int64_t)year - 1900;
  if (out_milliseconds == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (tm_year < INT_MIN || tm_year > INT_MAX) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  local_value.tm_year = (int)tm_year;
  local_value.tm_mon = (int)month;
  local_value.tm_mday = (int)day;
  local_value.tm_hour = (int)hour;
  local_value.tm_min = (int)minute;
  local_value.tm_sec = (int)second;
  local_value.tm_isdst = -1;
  errno = 0;
  timestamp = mktime(&local_value);
  if (timestamp == (time_t)-1 && errno != 0) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  *out_milliseconds = (double)timestamp * 1000.0;
  return HXC_STATUS_OK;
}

hxc_status hxc_date_time_timezone_offset(double milliseconds, int32_t *out_minutes) {
  time_t timestamp;
  struct tm local_value;
  struct tm utc_value;
  int64_t local_minus_utc;
  int64_t offset_minutes;
  if (out_minutes == NULL) {
    return HXC_STATUS_INVALID_ARGUMENT;
  }
  if (!hxc_date_time_time_t_from_milliseconds(milliseconds, &timestamp)) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  if (!hxc_date_time_local_tm(timestamp, &local_value) || !hxc_date_time_utc_tm(timestamp, &utc_value)) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  local_minus_utc = hxc_date_time_tm_seconds(&local_value) - hxc_date_time_tm_seconds(&utc_value);
  if (local_minus_utc % 60 != 0) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  offset_minutes = -(local_minus_utc / 60);
  if (offset_minutes < INT32_MIN || offset_minutes > INT32_MAX) {
    return HXC_STATUS_OUT_OF_RANGE;
  }
  *out_minutes = (int32_t)offset_minutes;
  return HXC_STATUS_OK;
}
