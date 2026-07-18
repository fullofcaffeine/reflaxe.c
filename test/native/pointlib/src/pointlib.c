#include "pointlib.h"

pointlib_point pointlib_point_make(pointlib_coord x, pointlib_coord y) {
  const pointlib_point result = {x, y};
  return result;
}

pointlib_point pointlib_point_translate(pointlib_point point, pointlib_coord dx, pointlib_coord dy) {
  point.x += dx;
  point.y += dy;
  return point;
}

int64_t pointlib_point_dot(pointlib_point left, pointlib_point right) {
  return ((int64_t)left.x * (int64_t)right.x)
    + ((int64_t)left.y * (int64_t)right.y);
}

pointlib_coord pointlib_point_component(pointlib_point point, pointlib_axis axis) {
  return axis == POINTLIB_AXIS_X ? point.x : point.y;
}

bool pointlib_point_verify(pointlib_point left, pointlib_point right, int64_t dot, pointlib_coord component, pointlib_axis axis, const char *label) {
  return left.x == INT32_C(2)
    && left.y == INT32_C(2)
    && right.x == INT32_C(5)
    && right.y == INT32_C(7)
    && dot == INT64_C(24)
    && component == POINTLIB_COORD_TWO
    && axis == POINTLIB_AXIS_Y
    && label != NULL
    && label[0] == 'c'
    && label[1] == '-'
    && label[2] == 'i'
    && label[3] == 'm'
    && label[4] == 'p'
    && label[5] == 'o'
    && label[6] == 'r'
    && label[7] == 't'
    && label[8] == '-'
    && (unsigned char)label[9] == UINT8_C(0xc3)
    && (unsigned char)label[10] == UINT8_C(0xa9)
    && label[11] == '\0';
}

void pointlib_build_fact_probe(void) {}
