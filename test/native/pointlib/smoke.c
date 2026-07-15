#include "pointlib.h"

#include <stdio.h>

int main(void) {
  const hxc_fixture_point left = {2, -3};
  const hxc_fixture_point right = {5, 7};
  if (hxc_fixture_point_dot(left, right) != -11) {
    return 1;
  }
  (void)puts("pointlib-smoke: OK");
  return 0;
}
