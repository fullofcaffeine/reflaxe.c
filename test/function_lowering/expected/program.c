#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_FunctionFixture_StackClosure, hxc_invoke) == 0, "closed record hxc_FunctionFixture_StackClosure first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FunctionFixture_StackClosure) >= _Alignof(int32_t (*)(void *, int32_t)), "closed record hxc_FunctionFixture_StackClosure alignment admits field 0");

_Static_assert(offsetof(struct hxc_FunctionFixture_StackClosure, hxc_context) >= offsetof(struct hxc_FunctionFixture_StackClosure, hxc_invoke) + sizeof(int32_t (*)(void *, int32_t)), "closed record hxc_FunctionFixture_StackClosure field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FunctionFixture_StackClosure) >= _Alignof(void *), "closed record hxc_FunctionFixture_StackClosure alignment admits field 1");

_Static_assert(sizeof(struct hxc_FunctionFixture_StackClosure) >= offsetof(struct hxc_FunctionFixture_StackClosure, hxc_context) + sizeof(void *), "closed record hxc_FunctionFixture_StackClosure size contains its final field");

_Static_assert(offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_calls) == 0, "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment) >= _Alignof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment alignment admits field 0");

_Static_assert(offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_seed) >= offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_calls) + sizeof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment) >= _Alignof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment alignment admits field 1");

_Static_assert(sizeof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment) >= offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_seed) + sizeof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment size contains its final field");

int32_t hxc_FunctionFixture_apply(int32_t hxc_value, struct hxc_FunctionFixture_StackClosure hxc_operation)
{
  int32_t hxc_tmp_indirect_call_result_n0 = hxc_operation.hxc_invoke(hxc_operation.hxc_context, hxc_value);
  return hxc_tmp_indirect_call_result_n0;
}

int32_t hxc_FunctionFixture_applyTwice(int32_t hxc_value, struct hxc_FunctionFixture_StackClosure hxc_operation)
{
  int32_t hxc_tmp_indirect_call_result_n0 = hxc_operation.hxc_invoke(hxc_operation.hxc_context, hxc_value);
  int32_t hxc_first = hxc_tmp_indirect_call_result_n0;
  int32_t hxc_tmp_indirect_call_result_n2 = hxc_operation.hxc_invoke(hxc_operation.hxc_context, hxc_first);
  return hxc_tmp_indirect_call_result_n2;
}

double hxc_FunctionFixture_asFloat(double hxc_value)
{
  return hxc_value;
}

int32_t hxc_FunctionFixture_captureRoundTrip(int32_t hxc_seed)
{
  int32_t hxc_calls = 0;
  int32_t *hxc_tmp_stack_closure_capture_calls_n0 = &hxc_calls;
  int32_t hxc_tmp_stack_closure_capture_seed_n3 = hxc_seed;
  int32_t *hxc_tmp_stack_closure_capture_seed_n1 = &hxc_tmp_stack_closure_capture_seed_n3;
  struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment hxc_tmp_stack_closure_environment_n4 = (struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment){ .hxc_calls = hxc_tmp_stack_closure_capture_calls_n0, .hxc_seed = hxc_tmp_stack_closure_capture_seed_n1 };
  struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment *hxc_tmp_stack_closure_environment_address_n2 = &hxc_tmp_stack_closure_environment_n4;
  int32_t hxc_tmp_call_result_n5 = hxc_FunctionFixture_applyTwice(1, (struct hxc_FunctionFixture_StackClosure){ .hxc_invoke = hxc_captureRoundTrip_lambda_stack_1542_n1542, .hxc_context = (void *)hxc_tmp_stack_closure_environment_address_n2 });
  int32_t hxc_result = hxc_tmp_call_result_n5;
  int32_t hxc_tmp_load_result_n6 = hxc_calls;
  bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_load_result_n6 != 2;
  if (!(hxc_tmp_load_result_n6 != 2))
  {
    hxc_tmp_short_circuit_result_n5 = hxc_tmp_stack_closure_capture_seed_n3 != 6;
  }
  if (!hxc_tmp_short_circuit_result_n5)
  {
    return hxc_result;
  }
  return 0;
}

int32_t hxc_FunctionFixture_chain(int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_FunctionFixture_passthrough(hxc_value);
  return hxc_tmp_call_result_n0;
}

int32_t (*hxc_FunctionFixture_choose(void))(int32_t)
{
  return hxc_FunctionFixture_chain;
}

double hxc_FunctionFixture_convert(int32_t hxc_value)
{
  double hxc_tmp_call_result_n0 = hxc_FunctionFixture_asFloat((double)hxc_value);
  return hxc_tmp_call_result_n0;
}

void hxc_FunctionFixture_discarded(int32_t hxc_value)
{
  hxc_FunctionFixture_passthrough(hxc_value);
  return;
}

int32_t hxc_FunctionFixture_first(int32_t hxc_left, int32_t hxc_right)
{
  (void)hxc_right;
  return hxc_left;
}

int32_t hxc_FunctionFixture_indirect(int32_t hxc_value)
{
  int32_t (*hxc_tmp_call_result_n0)(int32_t) = hxc_FunctionFixture_choose();
  int32_t (*hxc_operation)(int32_t) = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_indirect_call_result_n2 = hxc_operation(hxc_value);
  int32_t hxc_tmp_call_result_n4 = hxc_FunctionFixture_apply(hxc_tmp_indirect_call_result_n2, (struct hxc_FunctionFixture_StackClosure){ .hxc_invoke = hxc_passthrough_synchronous_callback_adapter_n68, .hxc_context = NULL });
  return hxc_tmp_call_result_n4;
}

void hxc_FunctionFixture_main(void)
{
  int32_t hxc_tmp_call_result_n0 = hxc_FunctionFixture_chain(7);
  int32_t hxc_integer = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_load_result_n1 = hxc_integer;
  (void)hxc_tmp_load_result_n1;
  double hxc_tmp_call_result_n2 = hxc_FunctionFixture_convert(9);
  double hxc_floating = hxc_tmp_call_result_n2;
  double hxc_tmp_load_result_n3 = hxc_floating;
  (void)hxc_tmp_load_result_n3;
  hxc_FunctionFixture_discarded(11);
  int32_t hxc_tmp_call_result_n4 = hxc_FunctionFixture_ordered(13);
  int32_t hxc_orderedValue = hxc_tmp_call_result_n4;
  int32_t hxc_tmp_load_result_n5 = hxc_orderedValue;
  (void)hxc_tmp_load_result_n5;
  int32_t hxc_tmp_call_result_n6 = hxc_FunctionFixture_indirect(17);
  int32_t hxc_indirectValue = hxc_tmp_call_result_n6;
  while (1)
  {
    if (!(hxc_indirectValue != 17))
    {
      break;
    }
  }
  int32_t hxc_tmp_call_result_n8 = hxc_FunctionFixture_captureRoundTrip(5);
  int32_t hxc_captured = hxc_tmp_call_result_n8;
  while (1)
  {
    if (!(hxc_captured != 15))
    {
      break;
    }
  }
  return;
}

int32_t hxc_FunctionFixture_ordered(int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_FunctionFixture_passthrough(hxc_value);
  int32_t hxc_tmp_call_result_n1 = hxc_FunctionFixture_chain(hxc_value);
  int32_t hxc_tmp_call_result_n2 = hxc_FunctionFixture_first(hxc_tmp_call_result_n0, hxc_tmp_call_result_n1);
  return hxc_tmp_call_result_n2;
}

int32_t hxc_FunctionFixture_passthrough(int32_t hxc_value)
{
  return hxc_value;
}

int32_t hxc_captureRoundTrip_lambda_stack_1542_n1542(void *hxc_context, int32_t hxc_value)
{
  if (hxc_context == NULL)
  {
    abort();
  }
  int32_t *hxc_tmp_closure_capture_address_calls_n1 = (*(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment *)hxc_context).hxc_calls;
  int32_t *hxc_tmp_closure_capture_address_calls_n2 = hxc_tmp_closure_capture_address_calls_n1;
  int32_t *hxc_tmp_closure_capture_address_seed_n2 = (*(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment *)hxc_context).hxc_seed;
  int32_t *hxc_tmp_closure_capture_address_seed_n3 = hxc_tmp_closure_capture_address_seed_n2;
  int32_t hxc_output = 0;
  int32_t hxc_tmp_closure_capture_load_calls_result_n4 = *hxc_tmp_closure_capture_address_calls_n2;
  if (hxc_tmp_closure_capture_load_calls_result_n4 == 0)
  {
    *hxc_tmp_closure_capture_address_calls_n2 = 1;
    int32_t hxc_tmp_closure_capture_load_seed_result_n7 = *hxc_tmp_closure_capture_address_seed_n3;
    hxc_output = hxc_tmp_closure_capture_load_seed_result_n7;
    *hxc_tmp_closure_capture_address_seed_n3 = 6;
  }
  else
  {
    *hxc_tmp_closure_capture_address_calls_n2 = 2;
    bool hxc_tmp_short_circuit_result_n5 = hxc_value == 5;
    if (hxc_value == 5)
    {
      int32_t hxc_tmp_closure_capture_load_seed_result_n11 = *hxc_tmp_closure_capture_address_seed_n3;
      hxc_tmp_short_circuit_result_n5 = hxc_tmp_closure_capture_load_seed_result_n11 == 6;
    }
    if (hxc_tmp_short_circuit_result_n5)
    {
      hxc_output = 15;
    }
  }
  return hxc_output;
}

int32_t hxc_passthrough_synchronous_callback_adapter_n68(void *hxc_context, int32_t hxc_argument_0)
{
  (void)hxc_context;
  int32_t hxc_result_n0 = hxc_FunctionFixture_passthrough(hxc_argument_0);
  return hxc_result_n0;
}

int main(void)
{
  hxc_FunctionFixture_main();
  return 0;
}
