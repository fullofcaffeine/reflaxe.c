#include "pointlib.h"

#include <stddef.h>
#include <stdio.h>

_Static_assert(sizeof(pointlib_coord) == sizeof(int32_t), "pointlib_coord must retain int32_t width");
_Static_assert(_Alignof(pointlib_coord) == _Alignof(int32_t), "pointlib_coord must retain int32_t alignment");
_Static_assert(offsetof(pointlib_point, x) == 0U, "pointlib_point.x offset drifted");
_Static_assert(offsetof(pointlib_point, y) == sizeof(pointlib_coord), "pointlib_point.y offset drifted");
_Static_assert(sizeof(pointlib_point) == (2U * sizeof(pointlib_coord)), "pointlib_point size drifted");
_Static_assert(_Alignof(pointlib_point) == _Alignof(pointlib_coord), "pointlib_point alignment drifted");
_Static_assert(POINTLIB_AXIS_X == 0, "POINTLIB_AXIS_X value drifted");
_Static_assert(POINTLIB_AXIS_Y == 1, "POINTLIB_AXIS_Y value drifted");
_Static_assert(POINTLIB_COORD_NEGATIVE_THREE == -3, "negative constant drifted");
_Static_assert(POINTLIB_COORD_SEVEN == 7, "positive constant drifted");

int main(void) {
  pointlib_point left = pointlib_point_make(POINTLIB_COORD_ONE, POINTLIB_COORD_NEGATIVE_THREE);
  const pointlib_point right = pointlib_point_make(POINTLIB_COORD_FIVE, POINTLIB_COORD_SEVEN);
  left.x = POINTLIB_COORD_ONE;
  left = pointlib_point_translate(left, POINTLIB_COORD_ONE, POINTLIB_COORD_FIVE);
  const int64_t dot = pointlib_point_dot(left, right);
  const pointlib_coord component = pointlib_point_component(left, POINTLIB_AXIS_Y);
  if (!pointlib_point_verify(left, right, dot, component, POINTLIB_AXIS_Y, "c-import-é")) {
    return 1;
  }
  (void)puts("pointlib-abi: OK");
  return 0;
}
