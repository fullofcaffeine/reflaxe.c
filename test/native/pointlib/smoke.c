#include "pointlib.h"

#include <stdio.h>

int main(void) {
  pointlib_point left = pointlib_point_make(POINTLIB_COORD_ONE, POINTLIB_COORD_NEGATIVE_THREE);
  const pointlib_point right = pointlib_point_make(POINTLIB_COORD_FIVE, POINTLIB_COORD_SEVEN);
  left.x = POINTLIB_COORD_ONE;
  left = pointlib_point_translate(left, POINTLIB_COORD_ONE, POINTLIB_COORD_FIVE);
  const int64_t dot = pointlib_point_dot(left, right);
  if (!pointlib_point_verify(left, right, dot, pointlib_point_component(left, POINTLIB_AXIS_Y), POINTLIB_AXIS_Y, "c-import-é")) {
    return 1;
  }
  (void)puts("pointlib-smoke: OK");
  return 0;
}
