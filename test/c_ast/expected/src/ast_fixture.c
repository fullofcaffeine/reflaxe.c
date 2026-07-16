#include "hxc/ast_fixture.h"

uint32_t hxc_ast_sum(const struct hxc_ast_pair *pair)
{
  return pair->left + pair->right;
}
