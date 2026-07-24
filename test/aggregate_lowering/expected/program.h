#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_value)
{
  if (hxc_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_value;
  }
  return INT32_MIN + (int32_t)(hxc_value - UINT32_C(2147483648));
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left + (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_multiply_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left * (uint64_t)(uint32_t)hxc_right));
}

static inline int32_t hxc_i32_negate_wrapping(int32_t hxc_value)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)0 - (uint64_t)(uint32_t)hxc_value));
}

struct hxc_ActorPhase;

struct hxc_optional_OrderA;

struct hxc_OrderA {
  int32_t hxc_a;
  int32_t hxc_z;
};

struct hxc_optional_OrderA {
  bool hxc_has_value;
  struct hxc_OrderA hxc_value;
};

struct hxc_OptionalEnvelope {
  struct hxc_optional_OrderA hxc_point;
};

struct hxc_SwitchRecord {
  int32_t hxc_state;
};

struct hxc_ConditionalRecord {
  int32_t hxc_order;
  struct hxc_OrderA hxc_value;
};

struct hxc_FlowRecord {
  int32_t hxc_first;
  int32_t hxc_order;
  int32_t hxc_second;
  int32_t hxc_third;
};

struct hxc_Envelope {
  bool hxc_enabled;
  struct hxc_OrderA hxc_point;
};

enum hxc_ActorPhase_tag {
  hxc_ActorPhase_Waiting = 0,
  hxc_ActorPhase_Moving = 1
};

struct hxc_ActorPhase_Moving_payload {
  int32_t hxc_speed;
};

union hxc_ActorPhase_payload {
  struct hxc_ActorPhase_Moving_payload hxc_Moving;
};

struct hxc_ActorPhase {
  enum hxc_ActorPhase_tag hxc_tag;
  union hxc_ActorPhase_payload hxc_payload;
};

struct hxc_ActorRecord {
  struct hxc_ActorPhase hxc_phase;
};

int32_t hxc_AggregateFixture_actorSpeed(struct hxc_ActorRecord hxc_value);

int32_t hxc_AggregateFixture_checkedField(int32_t hxc_value);

int32_t hxc_AggregateFixture_conditionalRecordSum(bool hxc_chooseFirst, struct hxc_OrderA hxc_first, struct hxc_OrderA hxc_second);

struct hxc_OrderA hxc_AggregateFixture_copy(struct hxc_OrderA hxc_value);

struct hxc_Envelope hxc_AggregateFixture_envelope(struct hxc_OrderA hxc_value);

int32_t hxc_AggregateFixture_envelopeSum(struct hxc_Envelope hxc_value);

int32_t hxc_AggregateFixture_identity(int32_t hxc_value);

int32_t hxc_AggregateFixture_localSum(int32_t hxc_left, int32_t hxc_right);

void hxc_AggregateFixture_main(void);

struct hxc_OrderA hxc_AggregateFixture_make(int32_t hxc_left, int32_t hxc_right);

struct hxc_ActorRecord hxc_AggregateFixture_makeActor(struct hxc_ActorPhase hxc_phase);

struct hxc_FlowRecord hxc_AggregateFixture_makeFlowRecord(int32_t hxc_first, int32_t hxc_second, int32_t hxc_third);

struct hxc_SwitchRecord hxc_AggregateFixture_makeSwitch(int32_t hxc_state);

struct hxc_OrderA hxc_AggregateFixture_nestedRecord(bool hxc_chooseOuter, bool hxc_chooseInner, struct hxc_OrderA hxc_first, struct hxc_OrderA hxc_second);

struct hxc_optional_OrderA hxc_AggregateFixture_noPoint(void);

struct hxc_OptionalEnvelope hxc_AggregateFixture_optionalEnvelope(struct hxc_optional_OrderA hxc_value);

int32_t hxc_AggregateFixture_optionalSum(struct hxc_optional_OrderA hxc_value);

struct hxc_ActorPhase hxc_AggregateFixture_selectPhase(bool hxc_moving, int32_t hxc_speed);

struct hxc_ConditionalRecord hxc_AggregateFixture_selectRecord(bool hxc_chooseFirst, struct hxc_OrderA hxc_first, struct hxc_OrderA hxc_second);

struct hxc_optional_OrderA hxc_AggregateFixture_somePoint(struct hxc_OrderA hxc_value);

int32_t hxc_AggregateFixture_sum(struct hxc_OrderA hxc_value);

bool hxc_AggregateFixture_switchIsOn(struct hxc_SwitchRecord hxc_value);

int32_t hxc_AggregateFixture_switchStateValue(int32_t hxc_value);

#endif /* HXC_PROGRAM_H_INCLUDED */
