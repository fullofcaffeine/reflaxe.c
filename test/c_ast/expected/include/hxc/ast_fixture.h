#ifndef HXC_AST_FIXTURE_H
#define HXC_AST_FIXTURE_H

#include <stdint.h>

struct hxc_ast_pair {
  uint32_t left;
  uint32_t right;
};

extern uint32_t hxc_ast_sum(const struct hxc_ast_pair *pair);

#endif /* HXC_AST_FIXTURE_H */
