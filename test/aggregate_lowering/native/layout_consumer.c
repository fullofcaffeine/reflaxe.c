#include "hxc/program.h"

#include <stdint.h>

int main(void);

int main(void)
{
  struct HXC_PAIR_TAG first = {
    .HXC_PAIR_A = INT32_C(3),
    .HXC_PAIR_Z = INT32_C(4),
  };
  struct HXC_PAIR_TAG copied = first;
  struct HXC_ENVELOPE_TAG envelope = {
    .HXC_ENVELOPE_ENABLED = true,
    .HXC_ENVELOPE_POINT = copied,
  };

  return envelope.HXC_ENVELOPE_ENABLED &&
      envelope.HXC_ENVELOPE_POINT.HXC_PAIR_A == INT32_C(3) &&
      envelope.HXC_ENVELOPE_POINT.HXC_PAIR_Z == INT32_C(4)
    ? 0
    : 1;
}
