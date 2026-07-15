#include "pointlib.h"

int64_t hxc_fixture_point_dot(hxc_fixture_point left, hxc_fixture_point right) {
  return ((int64_t)left.x * (int64_t)right.x)
    + ((int64_t)left.y * (int64_t)right.y);
}
