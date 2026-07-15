#ifndef HXC_FIXTURE_POINTLIB_H_INCLUDED
#define HXC_FIXTURE_POINTLIB_H_INCLUDED

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct hxc_fixture_point {
  int32_t x;
  int32_t y;
} hxc_fixture_point;

int64_t hxc_fixture_point_dot(hxc_fixture_point left, hxc_fixture_point right);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXC_FIXTURE_POINTLIB_H_INCLUDED */
