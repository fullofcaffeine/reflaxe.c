#include "api.h"
#include "detail/state.h"
#include "nodes.h"
#include "types/value.h"

#include <stdio.h>

int main(void) {
  value payload = { .scalar = 7 };
  node_a left = { .next = NULL, .value = 1 };
  node_b right = { .previous = &left, .value = 2 };
  api_request request = {
    .payload = payload,
    .node = &left,
    .hidden = NULL,
    .clock = NULL,
    .state = NULL,
    .count = 1,
    .config_tag = 0
  };
  api_envelope envelope = { .request = request };
  private_state state = { .current = payload, .hidden = NULL, .config_tag = 0 };

  left.next = &right;
  if (envelope.request.node->next != &right || state.current.scalar != 7) {
    return 1;
  }
  puts("declaration-plan-headers: OK");
  return 0;
}
