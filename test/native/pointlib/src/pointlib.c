#include "pointlib.h"

#include <math.h>
#include <string.h>

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

static uint32_t pointlib_float_bits(float value) {
  uint32_t bits = UINT32_C(0);
  (void)memcpy(&bits, &value, sizeof(bits));
  return bits;
}

pointlib_float_point pointlib_float_point_make(float x, float y) {
  const pointlib_float_point result = {x, y};
  return result;
}

pointlib_float_point pointlib_float_point_scale(pointlib_float_point point, float scale) {
  point.x *= scale;
  point.y *= scale;
  return point;
}

float pointlib_float_point_dot(pointlib_float_point left, pointlib_float_point right) {
  return (left.x * right.x) + (left.y * right.y);
}

bool pointlib_float32_verify(pointlib_float_point point, float dot, double widened, float tie, float subnormal,
    float positive_infinity, float nan_value, float negative_zero, float finite_overflow) {
  return point.x == 1.0f
    && point.y == -4.0f
    && dot == 17.0f
    && widened == 17.0
    && pointlib_float_bits(tie) == UINT32_C(0x3F800000)
    && pointlib_float_bits(subnormal) == UINT32_C(0x00000001)
    && isinf(positive_infinity)
    && !signbit(positive_infinity)
    && isnan(nan_value)
    && pointlib_float_bits(negative_zero) == UINT32_C(0x80000000)
    && isinf(finite_overflow)
    && !signbit(finite_overflow);
}

void pointlib_build_fact_probe(void) {}
