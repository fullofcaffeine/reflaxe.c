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

typedef pointlib_point pointlib_point_alias;

typedef struct pointlib_float_point {
  float x;
  float y;
} pointlib_float_point;

#define POINTLIB_COORD_ZERO INT32_C(0)
#define POINTLIB_COORD_ONE INT32_C(1)
#define POINTLIB_COORD_TWO INT32_C(2)
#define POINTLIB_COORD_NEGATIVE_THREE (-INT32_C(3))
#define POINTLIB_COORD_FIVE INT32_C(5)
#define POINTLIB_COORD_SEVEN INT32_C(7)
#define POINTLIB_FLOAT_ONE_POINT_FIVE 1.5f
#define POINTLIB_FLOAT_NEGATIVE_TWO (-2.0f)

pointlib_point pointlib_point_make(pointlib_coord x, pointlib_coord y);
pointlib_point pointlib_point_translate(pointlib_point point, pointlib_coord dx, pointlib_coord dy);
pointlib_point_alias pointlib_point_alias_identity(pointlib_point_alias point);
int64_t pointlib_point_dot(pointlib_point left, pointlib_point right);
pointlib_coord pointlib_point_component(pointlib_point point, pointlib_axis axis);
bool pointlib_point_verify(pointlib_point left, pointlib_point right, int64_t dot, pointlib_coord component, pointlib_axis axis, const char *label);
pointlib_float_point pointlib_float_point_make(float x, float y);
pointlib_float_point pointlib_float_point_scale(pointlib_float_point point, float scale);
float pointlib_float_point_dot(pointlib_float_point left, pointlib_float_point right);
bool pointlib_float32_verify(pointlib_float_point point, float dot, double widened, float tie, float subnormal,
  float positive_infinity, float nan_value, float negative_zero, float finite_overflow);
bool pointlib_inline_float32_verify(float from_integer_local, float from_integer_arithmetic, float from_float_local,
  float from_side_effect, float from_non_inline);
void pointlib_build_fact_probe(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* HXC_FIXTURE_POINTLIB_H_INCLUDED */
