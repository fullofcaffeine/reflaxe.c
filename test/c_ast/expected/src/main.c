#include <stdio.h>

#include "hxc/ast_fixture.h"
#include "hxc/ast_fixture.h"

int main(void)
{
  struct hxc_ast_pair pair = { .left = 19U, .right = 23U };
  if (hxc_ast_sum(&pair) != 42U)
  {
    return 1;
  }
  puts("c-ast-header-source: OK");
  return 0;
}
