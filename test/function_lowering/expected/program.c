#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8, hxc_invoke) == 0, "closed record hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8) >= _Alignof(int32_t (*)(void *, int32_t)), "closed record hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 alignment admits field 0");

_Static_assert(offsetof(struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8, hxc_context) >= offsetof(struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8, hxc_invoke) + sizeof(int32_t (*)(void *, int32_t)), "closed record hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8) >= _Alignof(void *), "closed record hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 alignment admits field 1");

_Static_assert(sizeof(struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8) >= offsetof(struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8, hxc_context) + sizeof(void *), "closed record hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 size contains its final field");

_Static_assert(offsetof(struct hxc_FunctionFixture_CallbackPoint, hxc_x) == 0, "closed record hxc_FunctionFixture_CallbackPoint first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FunctionFixture_CallbackPoint) >= _Alignof(int32_t), "closed record hxc_FunctionFixture_CallbackPoint alignment admits field 0");

_Static_assert(offsetof(struct hxc_FunctionFixture_CallbackPoint, hxc_y) >= offsetof(struct hxc_FunctionFixture_CallbackPoint, hxc_x) + sizeof(int32_t), "closed record hxc_FunctionFixture_CallbackPoint field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FunctionFixture_CallbackPoint) >= _Alignof(int32_t), "closed record hxc_FunctionFixture_CallbackPoint alignment admits field 1");

_Static_assert(sizeof(struct hxc_FunctionFixture_CallbackPoint) >= offsetof(struct hxc_FunctionFixture_CallbackPoint, hxc_y) + sizeof(int32_t), "closed record hxc_FunctionFixture_CallbackPoint size contains its final field");

_Static_assert(offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_calls) == 0, "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment) >= _Alignof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment alignment admits field 0");

_Static_assert(offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_seed) >= offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_calls) + sizeof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment) >= _Alignof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment alignment admits field 1");

_Static_assert(sizeof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment) >= offsetof(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment, hxc_seed) + sizeof(int32_t *), "closed record hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment size contains its final field");

_Static_assert(offsetof(struct hxc_FunctionFixture_StackClosure_h89578ffbedab, hxc_invoke) == 0, "closed record hxc_FunctionFixture_StackClosure_h89578ffbedab first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FunctionFixture_StackClosure_h89578ffbedab) >= _Alignof(int32_t (*)(void *, struct hxc_FunctionFixture_CallbackPoint)), "closed record hxc_FunctionFixture_StackClosure_h89578ffbedab alignment admits field 0");

_Static_assert(offsetof(struct hxc_FunctionFixture_StackClosure_h89578ffbedab, hxc_context) >= offsetof(struct hxc_FunctionFixture_StackClosure_h89578ffbedab, hxc_invoke) + sizeof(int32_t (*)(void *, struct hxc_FunctionFixture_CallbackPoint)), "closed record hxc_FunctionFixture_StackClosure_h89578ffbedab field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FunctionFixture_StackClosure_h89578ffbedab) >= _Alignof(void *), "closed record hxc_FunctionFixture_StackClosure_h89578ffbedab alignment admits field 1");

_Static_assert(sizeof(struct hxc_FunctionFixture_StackClosure_h89578ffbedab) >= offsetof(struct hxc_FunctionFixture_StackClosure_h89578ffbedab, hxc_context) + sizeof(void *), "closed record hxc_FunctionFixture_StackClosure_h89578ffbedab size contains its final field");

int32_t hxc_FunctionFixture_apply(int32_t hxc_l_value, struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 hxc_l_operation)
{
  int32_t hxc_l_tmp_indirect_call_result_n0 = hxc_l_operation.hxc_invoke(hxc_l_operation.hxc_context, hxc_l_value);
  return hxc_l_tmp_indirect_call_result_n0;
}

int32_t hxc_FunctionFixture_applyPoint(struct hxc_FunctionFixture_CallbackPoint hxc_l_point, struct hxc_FunctionFixture_StackClosure_h89578ffbedab hxc_l_operation)
{
  int32_t hxc_l_tmp_indirect_call_result_n0 = hxc_l_operation.hxc_invoke(hxc_l_operation.hxc_context, hxc_l_point);
  return hxc_l_tmp_indirect_call_result_n0;
}

int32_t hxc_FunctionFixture_applyTwice(int32_t hxc_l_value, struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8 hxc_l_operation)
{
  int32_t hxc_l_tmp_indirect_call_result_n0 = hxc_l_operation.hxc_invoke(hxc_l_operation.hxc_context, hxc_l_value);
  int32_t hxc_l_first = hxc_l_tmp_indirect_call_result_n0;
  int32_t hxc_l_tmp_indirect_call_result_n2 = hxc_l_operation.hxc_invoke(hxc_l_operation.hxc_context, hxc_l_first);
  return hxc_l_tmp_indirect_call_result_n2;
}

double hxc_FunctionFixture_asFloat(double hxc_l_value)
{
  return hxc_l_value;
}

int32_t hxc_FunctionFixture_captureRoundTrip(int32_t hxc_l_seed)
{
  int32_t hxc_l_calls = 0;
  int32_t *hxc_l_tmp_stack_closure_capture_calls_n0 = &hxc_l_calls;
  int32_t hxc_l_tmp_stack_closure_capture_seed_n3 = hxc_l_seed;
  int32_t *hxc_l_tmp_stack_closure_capture_seed_n1 = &hxc_l_tmp_stack_closure_capture_seed_n3;
  struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment hxc_l_tmp_stack_closure_environment_n4 = (struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment){ .hxc_calls = hxc_l_tmp_stack_closure_capture_calls_n0, .hxc_seed = hxc_l_tmp_stack_closure_capture_seed_n1 };
  struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment *hxc_l_tmp_stack_closure_environment_address_n2 = &hxc_l_tmp_stack_closure_environment_n4;
  int32_t hxc_l_tmp_call_result_n5 = hxc_FunctionFixture_applyTwice(1, (struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8){ .hxc_invoke = hxc_captureRoundTrip_lambda_stack_2535_n2535, .hxc_context = (void *)hxc_l_tmp_stack_closure_environment_address_n2 });
  int32_t hxc_l_result = hxc_l_tmp_call_result_n5;
  int32_t hxc_l_tmp_load_result_n6 = hxc_l_calls;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_load_result_n6 != 2;
  if (!(hxc_l_tmp_load_result_n6 != 2))
  {
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_stack_closure_capture_seed_n3 != 6;
  }
  if (!hxc_l_tmp_short_circuit_result_n5)
  {
    return hxc_l_result;
  }
  return 0;
}

int32_t hxc_FunctionFixture_chain(int32_t hxc_l_value)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_FunctionFixture_passthrough(hxc_l_value);
  return hxc_l_tmp_call_result_n0;
}

int32_t (*hxc_FunctionFixture_choose(void))(int32_t)
{
  return hxc_FunctionFixture_chain;
}

int32_t (*hxc_FunctionFixture_chooseConditional(bool hxc_l_enabled))(int32_t)
{
  int32_t (*hxc_l_tmp_conditional_result_n1)(int32_t) = { 0 };
  if (hxc_l_enabled)
  {
    hxc_l_tmp_conditional_result_n1 = hxc_FunctionFixture_selectedFive;
  }
  else
  {
    hxc_l_tmp_conditional_result_n1 = hxc_FunctionFixture_selectedTen;
  }
  return hxc_l_tmp_conditional_result_n1;
}

int32_t (*hxc_FunctionFixture_chooseSwitch(int32_t hxc_l_mode))(int32_t)
{
  int32_t (*hxc_l_tmp_switch_function_result_n1)(int32_t) = { 0 };
  switch (hxc_l_mode) {
    case 0:
      {
        hxc_l_tmp_switch_function_result_n1 = hxc_FunctionFixture_selectedFive;
        break;
      }
    case 1:
      {
        hxc_l_tmp_switch_function_result_n1 = hxc_FunctionFixture_selectedTen;
        break;
      }
    default:
      {
        hxc_l_tmp_switch_function_result_n1 = hxc_FunctionFixture_passthrough;
        break;
      }
  }
  return hxc_l_tmp_switch_function_result_n1;
}

double hxc_FunctionFixture_convert(int32_t hxc_l_value)
{
  double hxc_l_tmp_call_result_n0 = hxc_FunctionFixture_asFloat((double)hxc_l_value);
  return hxc_l_tmp_call_result_n0;
}

void hxc_FunctionFixture_discarded(int32_t hxc_l_value)
{
  hxc_FunctionFixture_passthrough(hxc_l_value);
  return;
}

int32_t hxc_FunctionFixture_first(int32_t hxc_l_left, int32_t hxc_l_right)
{
  (void)hxc_l_right;
  return hxc_l_left;
}

int32_t hxc_FunctionFixture_indirect(int32_t hxc_l_value)
{
  int32_t (*hxc_l_tmp_call_result_n0)(int32_t) = hxc_FunctionFixture_choose();
  int32_t (*hxc_l_operation)(int32_t) = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_indirect_call_result_n2 = hxc_l_operation(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n4 = hxc_FunctionFixture_apply(hxc_l_tmp_indirect_call_result_n2, (struct hxc_FunctionFixture_StackClosure_h5d2e2fd891a8){ .hxc_invoke = hxc_passthrough_synchronous_callback_adapter_n779, .hxc_context = NULL });
  return hxc_l_tmp_call_result_n4;
}

void hxc_FunctionFixture_main(void)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_FunctionFixture_chain(7);
  int32_t hxc_l_integer = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_integer;
  (void)hxc_l_tmp_load_result_n1;
  double hxc_l_tmp_call_result_n2 = hxc_FunctionFixture_convert(9);
  double hxc_l_floating = hxc_l_tmp_call_result_n2;
  double hxc_l_tmp_load_result_n3 = hxc_l_floating;
  (void)hxc_l_tmp_load_result_n3;
  hxc_FunctionFixture_discarded(11);
  int32_t hxc_l_tmp_call_result_n4 = hxc_FunctionFixture_ordered(13);
  int32_t hxc_l_orderedValue = hxc_l_tmp_call_result_n4;
  int32_t hxc_l_tmp_load_result_n5 = hxc_l_orderedValue;
  (void)hxc_l_tmp_load_result_n5;
  int32_t hxc_l_tmp_call_result_n6 = hxc_FunctionFixture_indirect(17);
  int32_t hxc_l_indirectValue = hxc_l_tmp_call_result_n6;
  while (1)
  {
    if (!(hxc_l_indirectValue != 17))
    {
      break;
    }
  }
  while (1)
  {
    int32_t (*hxc_l_tmp_call_result_n8)(int32_t) = hxc_FunctionFixture_chooseConditional(true);
    int32_t hxc_l_tmp_indirect_call_result_n9 = hxc_l_tmp_call_result_n8(4);
    if (!(hxc_l_tmp_indirect_call_result_n9 != 5))
    {
      break;
    }
  }
  while (1)
  {
    int32_t (*hxc_l_tmp_call_result_n10)(int32_t) = hxc_FunctionFixture_chooseConditional(false);
    int32_t hxc_l_tmp_indirect_call_result_n11 = hxc_l_tmp_call_result_n10(4);
    if (!(hxc_l_tmp_indirect_call_result_n11 != 10))
    {
      break;
    }
  }
  while (1)
  {
    int32_t (*hxc_l_tmp_call_result_n12)(int32_t) = hxc_FunctionFixture_chooseSwitch(0);
    int32_t hxc_l_tmp_indirect_call_result_n13 = hxc_l_tmp_call_result_n12(5);
    if (!(hxc_l_tmp_indirect_call_result_n13 != 5))
    {
      break;
    }
  }
  while (1)
  {
    int32_t (*hxc_l_tmp_call_result_n14)(int32_t) = hxc_FunctionFixture_chooseSwitch(1);
    int32_t hxc_l_tmp_indirect_call_result_n15 = hxc_l_tmp_call_result_n14(5);
    if (!(hxc_l_tmp_indirect_call_result_n15 != 10))
    {
      break;
    }
  }
  while (1)
  {
    int32_t (*hxc_l_tmp_call_result_n16)(int32_t) = hxc_FunctionFixture_chooseSwitch(2);
    int32_t hxc_l_tmp_indirect_call_result_n17 = hxc_l_tmp_call_result_n16(5);
    if (!(hxc_l_tmp_indirect_call_result_n17 != 5))
    {
      break;
    }
  }
  while (1)
  {
    int32_t hxc_l_tmp_call_result_n18 = hxc_FunctionFixture_recursiveThroughValue(true);
    if (!(hxc_l_tmp_call_result_n18 != 4))
    {
      break;
    }
  }
  int32_t hxc_l_tmp_call_result_n21 = hxc_FunctionFixture_applyPoint((struct hxc_FunctionFixture_CallbackPoint){ .hxc_x = 2, .hxc_y = 3 }, (struct hxc_FunctionFixture_StackClosure_h89578ffbedab){ .hxc_invoke = hxc_pointValue_synchronous_callback_adapter_n1607, .hxc_context = NULL });
  int32_t hxc_l_pointResult = hxc_l_tmp_call_result_n21;
  while (1)
  {
    if (!(hxc_l_pointResult != 5))
    {
      break;
    }
  }
  int32_t hxc_l_tmp_call_result_n23 = hxc_FunctionFixture_captureRoundTrip(5);
  int32_t hxc_l_captured = hxc_l_tmp_call_result_n23;
  while (1)
  {
    if (!(hxc_l_captured != 15))
    {
      break;
    }
  }
  int32_t hxc_l_tmp_call_result_n25 = hxc_FunctionFixture_mutateParameters(3, 2, false);
  int32_t hxc_l_mutated = hxc_l_tmp_call_result_n25;
  while (1)
  {
    if (!(hxc_l_mutated != 3))
    {
      break;
    }
  }
  double hxc_l_tmp_call_result_n27 = hxc_FunctionFixture_mutateFloat(3.0);
  double hxc_l_mutatedFloat = hxc_l_tmp_call_result_n27;
  while (1)
  {
    if (!(hxc_l_mutatedFloat != 4.5))
    {
      break;
    }
  }
  int32_t hxc_l_tmp_call_result_n29 = hxc_FunctionFixture_readOnlyParameters(9, 4, true);
  int32_t hxc_l_readOnly = hxc_l_tmp_call_result_n29;
  while (1)
  {
    if (!(hxc_l_readOnly != 9))
    {
      break;
    }
  }
  return;
}

double hxc_FunctionFixture_mutateFloat(double hxc_l_value)
{
  double hxc_l_value_mutable = hxc_l_value;
  hxc_l_value_mutable = hxc_l_value_mutable + 1.5;
  return hxc_l_value_mutable;
}

int32_t hxc_FunctionFixture_mutateParameters(int32_t hxc_l_seed, int32_t hxc_l_remaining, bool hxc_l_flag)
{
  int32_t hxc_l_seed_mutable = hxc_l_seed;
  int32_t hxc_l_remaining_mutable = hxc_l_remaining;
  bool hxc_l_flag_mutable = hxc_l_flag;
  int32_t hxc_l_original = hxc_l_seed_mutable;
  hxc_l_seed_mutable = hxc_l_remaining_mutable;
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_original;
  hxc_l_seed_mutable = hxc_l_tmp_load_result_n2;
  int32_t hxc_l_assigned = hxc_l_tmp_load_result_n2;
  while (1)
  {
    if (!(hxc_l_remaining_mutable > 0))
    {
      break;
    }
    hxc_l_seed_mutable = hxc_l_remaining_mutable;
    hxc_l_remaining_mutable = 0;
  }
  hxc_l_flag_mutable = !hxc_l_flag_mutable;
  if (hxc_l_flag_mutable)
  {
    hxc_l_seed_mutable = hxc_l_original;
  }
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_assigned;
  if (!(hxc_l_tmp_load_result_n8 != hxc_l_original))
  {
    return hxc_l_seed_mutable;
  }
  return 0;
}

int32_t hxc_FunctionFixture_ordered(int32_t hxc_l_value)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_FunctionFixture_passthrough(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n1 = hxc_FunctionFixture_chain(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n2 = hxc_FunctionFixture_first(hxc_l_tmp_call_result_n0, hxc_l_tmp_call_result_n1);
  return hxc_l_tmp_call_result_n2;
}

int32_t hxc_FunctionFixture_passthrough(int32_t hxc_l_value)
{
  return hxc_l_value;
}

int32_t hxc_FunctionFixture_pointValue(struct hxc_FunctionFixture_CallbackPoint hxc_l_point)
{
  return hxc_i32_add_wrapping(hxc_l_point.hxc_x, hxc_l_point.hxc_y);
}

int32_t hxc_FunctionFixture_readOnlyParameters(int32_t hxc_l_left, int32_t hxc_l_right, bool hxc_l_enabled)
{
  int32_t hxc_l_tmp_conditional_result_n3 = 0;
  if (hxc_l_enabled)
  {
    hxc_l_tmp_conditional_result_n3 = hxc_l_left;
  }
  else
  {
    hxc_l_tmp_conditional_result_n3 = hxc_l_right;
  }
  return hxc_l_tmp_conditional_result_n3;
}

int32_t hxc_FunctionFixture_recursiveThroughValue(bool hxc_l_reenter)
{
  int32_t (*hxc_l_next)(bool) = { 0 };
  if (!!hxc_l_reenter)
  {
    hxc_l_next = hxc_FunctionFixture_recursiveThroughValue;
    int32_t hxc_l_tmp_indirect_call_result_n1 = hxc_l_next(false);
    return hxc_l_tmp_indirect_call_result_n1;
  }
  return 4;
}

int32_t hxc_FunctionFixture_selectedFive(int32_t hxc_l_value)
{
  (void)hxc_l_value;
  return 5;
}

int32_t hxc_FunctionFixture_selectedTen(int32_t hxc_l_value)
{
  (void)hxc_l_value;
  return 10;
}

int32_t hxc_captureRoundTrip_lambda_stack_2535_n2535(void *hxc_l_context, int32_t hxc_l_value)
{
  if (hxc_l_context == NULL)
  {
    abort();
  }
  int32_t *hxc_l_tmp_closure_capture_address_calls_n1 = (*(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment *)hxc_l_context).hxc_calls;
  int32_t *hxc_l_tmp_closure_capture_address_calls_n2 = hxc_l_tmp_closure_capture_address_calls_n1;
  int32_t *hxc_l_tmp_closure_capture_address_seed_n2 = (*(struct hxc_FunctionFixture_captureRoundTrip_LambdaEnvironment *)hxc_l_context).hxc_seed;
  int32_t *hxc_l_tmp_closure_capture_address_seed_n3 = hxc_l_tmp_closure_capture_address_seed_n2;
  int32_t hxc_l_output = 0;
  int32_t hxc_l_tmp_closure_capture_load_calls_result_n4 = *hxc_l_tmp_closure_capture_address_calls_n2;
  if (hxc_l_tmp_closure_capture_load_calls_result_n4 == 0)
  {
    *hxc_l_tmp_closure_capture_address_calls_n2 = 1;
    int32_t hxc_l_tmp_closure_capture_load_seed_result_n7 = *hxc_l_tmp_closure_capture_address_seed_n3;
    hxc_l_output = hxc_l_tmp_closure_capture_load_seed_result_n7;
    *hxc_l_tmp_closure_capture_address_seed_n3 = 6;
  }
  else
  {
    *hxc_l_tmp_closure_capture_address_calls_n2 = 2;
    bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_value == 5;
    if (hxc_l_value == 5)
    {
      int32_t hxc_l_tmp_closure_capture_load_seed_result_n11 = *hxc_l_tmp_closure_capture_address_seed_n3;
      hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_closure_capture_load_seed_result_n11 == 6;
    }
    if (hxc_l_tmp_short_circuit_result_n5)
    {
      hxc_l_output = 15;
    }
  }
  return hxc_l_output;
}

int32_t hxc_passthrough_synchronous_callback_adapter_n779(void *hxc_l_context, int32_t hxc_l_argument_0)
{
  (void)hxc_l_context;
  int32_t hxc_l_result_n0 = hxc_FunctionFixture_passthrough(hxc_l_argument_0);
  return hxc_l_result_n0;
}

int32_t hxc_pointValue_synchronous_callback_adapter_n1607(void *hxc_l_context, struct hxc_FunctionFixture_CallbackPoint hxc_l_argument_0)
{
  (void)hxc_l_context;
  int32_t hxc_l_result_n0 = hxc_FunctionFixture_pointValue(hxc_l_argument_0);
  return hxc_l_result_n0;
}

int main(void)
{
  hxc_FunctionFixture_main();
  return 0;
}
