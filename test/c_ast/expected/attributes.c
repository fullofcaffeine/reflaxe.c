#include <stdio.h>

static int hxc_attribute_marker __attribute__((used)) = 1;

int main(void)
{
  if (hxc_attribute_marker != 1)
  {
    return 1;
  }
  puts("c-ast-attributes: OK");
  return 0;
}
