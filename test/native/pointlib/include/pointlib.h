#ifndef HXC_FIXTURE_POINTLIB_H_INCLUDED
#define HXC_FIXTURE_POINTLIB_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef int32_t pointlib_coord;

typedef enum {
  POINTLIB_AXIS_X = 0,
  POINTLIB_AXIS_Y = 1
} pointlib_axis;

typedef struct pointlib_point {
  pointlib_coord x;
  pointlib_coord y;
} pointlib_point;

#define POINTLIB_COORD_ZERO INT32_C(0)
#define POINTLIB_COORD_ONE INT32_C(1)
#define POINTLIB_COORD_TWO INT32_C(2)
#define POINTLIB_COORD_NEGATIVE_THREE (-INT32_C(3))
#define POINTLIB_COORD_FIVE INT32_C(5)
#define POINTLIB_COORD_SEVEN INT32_C(7)

pointlib_point pointlib_point_make(pointlib_coord x, pointlib_coord y);
pointlib_point pointlib_point_translate(pointlib_point point, pointlib_coord dx, pointlib_coord dy);
int64_t pointlib_point_dot(pointlib_point left, pointlib_point right);
pointlib_coord pointlib_point_component(pointlib_point point, pointlib_axis axis);
bool pointlib_point_verify(pointlib_point left, pointlib_point right, int64_t dot, pointlib_coord component, pointlib_axis axis, const char *label);
void pointlib_build_fact_probe(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXC_FIXTURE_POINTLIB_H_INCLUDED */
