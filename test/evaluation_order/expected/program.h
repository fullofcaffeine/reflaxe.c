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

struct hxc_EvaluationFixture_EvaluationChoiceRecord {
  int32_t hxc_amount;
  bool hxc_enabled;
};

extern int32_t hxc_EvaluationFixture_barrierValue;

extern bool hxc_EvaluationFixture_callFlag;

extern uint32_t hxc_EvaluationFixture_counter;

extern bool hxc_EvaluationFixture_shortCircuitIntact;

extern uint32_t hxc_EvaluationFixture_switchCalls;

extern bool hxc_EvaluationFixture_ternaryIntact;

bool hxc_EvaluationFixture_consumePair(bool hxc_first, bool hxc_second);

struct hxc_EvaluationFixture_EvaluationChoiceRecord hxc_EvaluationFixture_exhaustiveAbstractRecord(int32_t hxc_value);

int32_t hxc_EvaluationFixture_exhaustiveAbstractReturn(int32_t hxc_value);

uint32_t hxc_EvaluationFixture_finish(uint32_t hxc_value);

uint32_t hxc_EvaluationFixture_keepTernary(void);

void hxc_EvaluationFixture_main(void);

int32_t hxc_EvaluationFixture_overwriteBarrierValue(void);

int32_t hxc_EvaluationFixture_readGlobalBeforeCall(void);

uint32_t hxc_EvaluationFixture_run(void);

bool hxc_EvaluationFixture_setCallFlag(bool hxc_value);

bool hxc_EvaluationFixture_spoilShortCircuit(void);

uint32_t hxc_EvaluationFixture_spoilTernary(void);

int32_t hxc_EvaluationFixture_switchSubject(int32_t hxc_value);

#endif /* HXC_PROGRAM_H_INCLUDED */
