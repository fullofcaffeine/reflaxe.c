#include "library.h"

int main(void) {
  return hxc_platform_matrix_value() == 42 ? 0 : 1;
}
