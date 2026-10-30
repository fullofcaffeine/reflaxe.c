#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_l_value)
{
  if (hxc_l_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_l_value;
  }
  return INT32_MIN + (int32_t)(hxc_l_value - UINT32_C(2147483648));
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left + (uint64_t)(uint32_t)hxc_l_right));
}

struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 {
  int32_t (*hxc_invoke)(void *, int32_t);
  void *hxc_context;
};

struct hxc_FunctionFixture_CallbackPoint {
  int32_t hxc_x;
  int32_t hxc_y;
};

struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment {
  int32_t *hxc_calls;
  int32_t *hxc_seed;
};

struct hxc_FunctionFixture_StackClosure_h89578ffbedab {
  int32_t (*hxc_invoke)(void *, struct hxc_FunctionFixture_CallbackPoint);
  void *hxc_context;
};

int32_t hxc_FunctionFixture_apply(int32_t hxc_l_value, struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 hxc_l_operation);

int32_t hxc_FunctionFixture_applyPoint(struct hxc_FunctionFixture_CallbackPoint hxc_l_point, struct hxc_FunctionFixture_StackClosure_h89578ffbedab hxc_l_operation);

int32_t hxc_FunctionFixture_applyTwice(int32_t hxc_l_value, struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 hxc_l_operation);

double hxc_FunctionFixture_asFloat(double hxc_l_value);

int32_t hxc_FunctionFixture_captureRoundTrip(int32_t hxc_l_seed);

int32_t hxc_FunctionFixture_chain(int32_t hxc_l_value);

int32_t (*hxc_FunctionFixture_choose(void))(int32_t);

int32_t (*hxc_FunctionFixture_chooseConditional(bool hxc_l_enabled))(int32_t);

int32_t (*hxc_FunctionFixture_chooseSwitch(int32_t hxc_l_mode))(int32_t);

double hxc_FunctionFixture_convert(int32_t hxc_l_value);

void hxc_FunctionFixture_discarded(int32_t hxc_l_value);

int32_t hxc_FunctionFixture_first(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_FunctionFixture_indirect(int32_t hxc_l_value);

void hxc_FunctionFixture_main(void);

double hxc_FunctionFixture_mutateFloat(double hxc_l_value);

int32_t hxc_FunctionFixture_mutateParameters(int32_t hxc_l_seed, int32_t hxc_l_remaining, bool hxc_l_flag);

_Noreturn void hxc_FunctionFixture_mutualLeft(int32_t hxc_l_value);

_Noreturn void hxc_FunctionFixture_mutualRight(int32_t hxc_l_value);

int32_t hxc_FunctionFixture_ordered(int32_t hxc_l_value);

int32_t hxc_FunctionFixture_passthrough(int32_t hxc_l_value);

int32_t hxc_FunctionFixture_pointValue(struct hxc_FunctionFixture_CallbackPoint hxc_l_point);

int32_t hxc_FunctionFixture_readOnlyParameters(int32_t hxc_l_left, int32_t hxc_l_right, bool hxc_l_enabled);

_Noreturn void hxc_FunctionFixture_recursive(int32_t hxc_l_left, int32_t hxc_l_right);

int32_t hxc_FunctionFixture_recursiveThroughValue(bool hxc_l_reenter);

int32_t hxc_FunctionFixture_selectedFive(int32_t hxc_l_value);

int32_t hxc_FunctionFixture_selectedTen(int32_t hxc_l_value);

int32_t hxc_captureRoundTrip_lambda_stack_2535_n2535(void *hxc_l_context, int32_t hxc_l_value);

int32_t hxc_passthrough_synchronous_callback_adapter_n779(void *hxc_l_context, int32_t hxc_l_argument_0);

int32_t hxc_pointValue_synchronous_callback_adapter_n1607(void *hxc_l_context, struct hxc_FunctionFixture_CallbackPoint hxc_l_argument_0);

#endif /* HXC_PROGRAM_H_INCLUDED */
