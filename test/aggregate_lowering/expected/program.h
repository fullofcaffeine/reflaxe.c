#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

struct hxc_SwitchRecord {
  int32_t hxc_state;
};

struct hxc_OrderA {
  int32_t hxc_a;
  int32_t hxc_z;
};

struct hxc_Envelope {
  bool hxc_enabled;
  struct hxc_OrderA hxc_point;
};

struct hxc_OrderA hxc_AggregateFixture_copy(struct hxc_OrderA hxc_value);

struct hxc_Envelope hxc_AggregateFixture_envelope(struct hxc_OrderA hxc_value);

int32_t hxc_AggregateFixture_envelopeSum(struct hxc_Envelope hxc_value);

int32_t hxc_AggregateFixture_identity(int32_t hxc_value);

int32_t hxc_AggregateFixture_localSum(int32_t hxc_left, int32_t hxc_right);

void hxc_AggregateFixture_main(void);

struct hxc_OrderA hxc_AggregateFixture_make(int32_t hxc_left, int32_t hxc_right);

struct hxc_SwitchRecord hxc_AggregateFixture_makeSwitch(int32_t hxc_state);

int32_t hxc_AggregateFixture_sum(struct hxc_OrderA hxc_value);

bool hxc_AggregateFixture_switchIsOn(struct hxc_SwitchRecord hxc_value);

int32_t hxc_AggregateFixture_switchStateValue(int32_t hxc_value);

#endif /* HXC_PROGRAM_H_INCLUDED */
