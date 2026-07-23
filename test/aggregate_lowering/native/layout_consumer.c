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
  struct HXC_ACTOR_PHASE_VALUE_TAG phase = {
    .HXC_ACTOR_PHASE_TAG_MEMBER = HXC_ACTOR_PHASE_MOVING,
    .HXC_ACTOR_PHASE_PAYLOAD_MEMBER.HXC_ACTOR_PHASE_MOVING_UNION_MEMBER.HXC_ACTOR_PHASE_MOVING_SPEED = INT32_C(5),
  };
  struct HXC_ACTOR_TAG actor = {
    .HXC_ACTOR_PHASE = phase,
  };
  struct HXC_ACTOR_TAG actor_copy = actor;

  return envelope.HXC_ENVELOPE_ENABLED &&
      envelope.HXC_ENVELOPE_POINT.HXC_PAIR_A == INT32_C(3) &&
      envelope.HXC_ENVELOPE_POINT.HXC_PAIR_Z == INT32_C(4) &&
      actor_copy.HXC_ACTOR_PHASE.HXC_ACTOR_PHASE_TAG_MEMBER == HXC_ACTOR_PHASE_MOVING &&
      actor_copy.HXC_ACTOR_PHASE.HXC_ACTOR_PHASE_PAYLOAD_MEMBER.HXC_ACTOR_PHASE_MOVING_UNION_MEMBER.HXC_ACTOR_PHASE_MOVING_SPEED == INT32_C(5) &&
      HXC_ACTOR_PHASE_WAITING != HXC_ACTOR_PHASE_MOVING
    ? 0
    : 1;
}
