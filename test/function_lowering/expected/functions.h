#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct hxc_FunctionFixture_StackClosure {
  int32_t (*hxc_invoke)(void *, int32_t);
  void *hxc_context;
};

struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment {
  int32_t *hxc_calls;
  int32_t *hxc_seed;
};

int32_t hxc_FunctionFixture_apply(int32_t hxc_value, struct hxc_FunctionFixture_StackClosure hxc_operation);

int32_t hxc_FunctionFixture_applyTwice(int32_t hxc_value, struct hxc_FunctionFixture_StackClosure hxc_operation);

double hxc_FunctionFixture_asFloat(double hxc_value);

int32_t hxc_FunctionFixture_captureRoundTrip(int32_t hxc_seed);

int32_t hxc_FunctionFixture_chain(int32_t hxc_value);

int32_t (*hxc_FunctionFixture_choose(void))(int32_t);

double hxc_FunctionFixture_convert(int32_t hxc_value);

void hxc_FunctionFixture_discarded(int32_t hxc_value);

int32_t hxc_FunctionFixture_first(int32_t hxc_left, int32_t hxc_right);

int32_t hxc_FunctionFixture_indirect(int32_t hxc_value);

void hxc_FunctionFixture_main(void);

_Noreturn void hxc_FunctionFixture_mutualLeft(int32_t hxc_value);

_Noreturn void hxc_FunctionFixture_mutualRight(int32_t hxc_value);

int32_t hxc_FunctionFixture_ordered(int32_t hxc_value);

int32_t hxc_FunctionFixture_passthrough(int32_t hxc_value);

_Noreturn void hxc_FunctionFixture_recursive(int32_t hxc_left, int32_t hxc_right);

int32_t hxc_captureRoundTrip_lambda_stack_1542_n1542(void *hxc_context, int32_t hxc_value);

int32_t hxc_passthrough_synchronous_callback_adapter_n68(void *hxc_context, int32_t hxc_argument_0);

#endif /* HXC_PROGRAM_H_INCLUDED */
