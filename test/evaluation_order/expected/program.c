#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_EvaluationFixture_EvaluationChoiceRecord, hxc_amount) == 0, "closed record hxc_EvaluationFixture_EvaluationChoiceRecord first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_EvaluationFixture_EvaluationChoiceRecord) >= _Alignof(int32_t), "closed record hxc_EvaluationFixture_EvaluationChoiceRecord alignment admits field 0");

_Static_assert(offsetof(struct hxc_EvaluationFixture_EvaluationChoiceRecord, hxc_enabled) >= offsetof(struct hxc_EvaluationFixture_EvaluationChoiceRecord, hxc_amount) + sizeof(int32_t), "closed record hxc_EvaluationFixture_EvaluationChoiceRecord field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_EvaluationFixture_EvaluationChoiceRecord) >= _Alignof(bool), "closed record hxc_EvaluationFixture_EvaluationChoiceRecord alignment admits field 1");

_Static_assert(sizeof(struct hxc_EvaluationFixture_EvaluationChoiceRecord) >= offsetof(struct hxc_EvaluationFixture_EvaluationChoiceRecord, hxc_enabled) + sizeof(bool), "closed record hxc_EvaluationFixture_EvaluationChoiceRecord size contains its final field");

int32_t hxc_EvaluationFixture_barrierValue = 0;

bool hxc_EvaluationFixture_callFlag = false;

uint32_t hxc_EvaluationFixture_counter = 0;

bool hxc_EvaluationFixture_shortCircuitIntact = true;

uint32_t hxc_EvaluationFixture_switchCalls = 0;

bool hxc_EvaluationFixture_ternaryIntact = true;

bool hxc_EvaluationFixture_consumePair(bool hxc_first, bool hxc_second)
{
  bool hxc_tmp_short_circuit_result_n2 = hxc_first;
  if (hxc_first)
  {
    hxc_tmp_short_circuit_result_n2 = !hxc_second;
  }
  return hxc_tmp_short_circuit_result_n2;
}

struct hxc_EvaluationFixture_EvaluationChoiceRecord hxc_EvaluationFixture_exhaustiveAbstractRecord(int32_t hxc_value)
{
  struct hxc_EvaluationFixture_EvaluationChoiceRecord hxc_tmp_switch_result_n2 = { 0 };
  switch (hxc_value) {
    case 0:
      {
        hxc_tmp_switch_result_n2 = (struct hxc_EvaluationFixture_EvaluationChoiceRecord){ .hxc_amount = 4, .hxc_enabled = false };
        break;
      }
    case 1:
      {
        hxc_tmp_switch_result_n2 = (struct hxc_EvaluationFixture_EvaluationChoiceRecord){ .hxc_amount = 7, .hxc_enabled = true };
        break;
      }
    case 2:
      {
        hxc_tmp_switch_result_n2 = (struct hxc_EvaluationFixture_EvaluationChoiceRecord){ .hxc_amount = 9, .hxc_enabled = false };
        break;
      }
    default:
      {
        abort();
      }
  }
  struct hxc_EvaluationFixture_EvaluationChoiceRecord hxc_selected = hxc_tmp_switch_result_n2;
  return hxc_selected;
}

int32_t hxc_EvaluationFixture_exhaustiveAbstractReturn(int32_t hxc_value)
{
  int32_t hxc_tmp_switch_result_n1 = 0;
  switch (hxc_value) {
    case 0:
      {
        hxc_tmp_switch_result_n1 = 11;
        break;
      }
    case 1:
      {
        hxc_tmp_switch_result_n1 = 22;
        break;
      }
    case 2:
      {
        hxc_tmp_switch_result_n1 = 33;
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_tmp_switch_result_n1;
}

uint32_t hxc_EvaluationFixture_finish(uint32_t hxc_value)
{
  return hxc_value;
}

uint32_t hxc_EvaluationFixture_keepTernary(void)
{
  return 7;
}

void hxc_EvaluationFixture_main(void)
{
  hxc_EvaluationFixture_run();
  return;
}

int32_t hxc_EvaluationFixture_overwriteBarrierValue(void)
{
  hxc_EvaluationFixture_barrierValue = 41;
  return 0;
}

int32_t hxc_EvaluationFixture_readGlobalBeforeCall(void)
{
  hxc_EvaluationFixture_barrierValue = 5;
  int32_t hxc_tmp_global_load_result_n0 = hxc_EvaluationFixture_barrierValue;
  int32_t hxc_before = hxc_tmp_global_load_result_n0;
  hxc_EvaluationFixture_overwriteBarrierValue();
  return hxc_before;
}

uint32_t hxc_EvaluationFixture_run(void)
{
  hxc_EvaluationFixture_callFlag = false;
  bool hxc_tmp_call_result_n0 = hxc_EvaluationFixture_setCallFlag(true);
  bool hxc_tmp_static_call_argument_0_n1 = hxc_tmp_call_result_n0;
  bool hxc_tmp_global_load_result_n1 = hxc_EvaluationFixture_callFlag;
  bool hxc_tmp_conditional_result_n2 = false;
  if (hxc_tmp_global_load_result_n1)
  {
    bool hxc_tmp_call_result_n2 = hxc_EvaluationFixture_setCallFlag(false);
    hxc_tmp_conditional_result_n2 = hxc_tmp_call_result_n2;
  }
  else
  {
    bool hxc_tmp_call_result_n3 = hxc_EvaluationFixture_setCallFlag(true);
    hxc_tmp_conditional_result_n2 = hxc_tmp_call_result_n3;
  }
  bool hxc_tmp_conditional_load_result_n4 = hxc_tmp_conditional_result_n2;
  bool hxc_tmp_call_result_n6 = hxc_EvaluationFixture_consumePair(hxc_tmp_static_call_argument_0_n1, hxc_tmp_conditional_load_result_n4);
  bool hxc_callArgumentsIntact = hxc_tmp_call_result_n6;
  hxc_EvaluationFixture_shortCircuitIntact = hxc_callArgumentsIntact;
  bool hxc_tmp_short_circuit_result_n3 = false;
  if (false)
  {
    bool hxc_tmp_call_result_n8 = hxc_EvaluationFixture_spoilShortCircuit();
    hxc_tmp_short_circuit_result_n3 = hxc_tmp_call_result_n8;
  }
  bool hxc_tmp_short_circuit_load_result_n9 = hxc_tmp_short_circuit_result_n3;
  (void)hxc_tmp_short_circuit_load_result_n9;
  bool hxc_tmp_short_circuit_result_n4 = true;
  if (!true)
  {
    bool hxc_tmp_call_result_n10 = hxc_EvaluationFixture_spoilShortCircuit();
    hxc_tmp_short_circuit_result_n4 = hxc_tmp_call_result_n10;
  }
  bool hxc_tmp_short_circuit_load_result_n11 = hxc_tmp_short_circuit_result_n4;
  (void)hxc_tmp_short_circuit_load_result_n11;
  bool hxc_tmp_short_circuit_result_n5 = true;
  if (true)
  {
    bool hxc_tmp_call_result_n12 = hxc_EvaluationFixture_setCallFlag(true);
    hxc_tmp_short_circuit_result_n5 = hxc_tmp_call_result_n12;
  }
  bool hxc_tmp_short_circuit_load_result_n13 = hxc_tmp_short_circuit_result_n5;
  (void)hxc_tmp_short_circuit_load_result_n13;
  bool hxc_tmp_global_load_result_n14 = hxc_EvaluationFixture_callFlag;
  if (!hxc_tmp_global_load_result_n14)
  {
    hxc_EvaluationFixture_shortCircuitIntact = false;
  }
  bool hxc_tmp_short_circuit_result_n6 = false;
  if (!false)
  {
    bool hxc_tmp_call_result_n15 = hxc_EvaluationFixture_setCallFlag(false);
    hxc_tmp_short_circuit_result_n6 = hxc_tmp_call_result_n15;
  }
  bool hxc_tmp_short_circuit_load_result_n16 = hxc_tmp_short_circuit_result_n6;
  (void)hxc_tmp_short_circuit_load_result_n16;
  bool hxc_tmp_global_load_result_n17 = hxc_EvaluationFixture_callFlag;
  if (hxc_tmp_global_load_result_n17)
  {
    hxc_EvaluationFixture_shortCircuitIntact = false;
  }
  hxc_EvaluationFixture_ternaryIntact = true;
  bool hxc_tmp_global_load_result_n18 = hxc_EvaluationFixture_ternaryIntact;
  uint32_t hxc_tmp_conditional_result_n8 = 0;
  if (hxc_tmp_global_load_result_n18)
  {
    uint32_t hxc_tmp_call_result_n19 = hxc_EvaluationFixture_keepTernary();
    hxc_tmp_conditional_result_n8 = hxc_tmp_call_result_n19;
  }
  else
  {
    uint32_t hxc_tmp_call_result_n20 = hxc_EvaluationFixture_spoilTernary();
    hxc_tmp_conditional_result_n8 = hxc_tmp_call_result_n20;
  }
  uint32_t hxc_selected = hxc_tmp_conditional_result_n8;
  hxc_EvaluationFixture_counter = 0;
  uint32_t hxc_tmp_increment_load_result_n22 = hxc_EvaluationFixture_counter;
  hxc_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_tmp_increment_load_result_n22 + (uint64_t)(uint32_t)1);
  uint32_t hxc_old = hxc_tmp_increment_load_result_n22;
  uint32_t hxc_tmp_increment_load_result_n23 = hxc_EvaluationFixture_counter;
  hxc_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_tmp_increment_load_result_n23 + (uint64_t)(uint32_t)1);
  uint32_t hxc_fresh = (uint32_t)((uint64_t)(uint32_t)hxc_tmp_increment_load_result_n23 + (uint64_t)(uint32_t)1);
  uint32_t hxc_tmp_load_result_n24 = hxc_old;
  (void)hxc_tmp_load_result_n24;
  uint32_t hxc_tmp_load_result_n25 = hxc_fresh;
  (void)hxc_tmp_load_result_n25;
  uint32_t hxc_tmp_load_result_n26 = hxc_selected;
  (void)hxc_tmp_load_result_n26;
  uint32_t hxc_assigned = 99;
  uint32_t hxc_tmp_global_load_result_n27 = hxc_EvaluationFixture_counter;
  hxc_assigned = hxc_tmp_global_load_result_n27;
  bool hxc_tmp_global_load_result_n28 = hxc_EvaluationFixture_callFlag;
  uint32_t hxc_tmp_conditional_result_n13 = 0;
  if (hxc_tmp_global_load_result_n28)
  {
    hxc_tmp_conditional_result_n13 = 91;
  }
  else
  {
    bool hxc_tmp_global_load_result_n29 = hxc_EvaluationFixture_shortCircuitIntact;
    uint32_t hxc_tmp_conditional_result_n14 = 0;
    if (hxc_tmp_global_load_result_n29)
    {
      bool hxc_tmp_global_load_result_n30 = hxc_EvaluationFixture_ternaryIntact;
      uint32_t hxc_tmp_conditional_result_n15 = 0;
      if (hxc_tmp_global_load_result_n30)
      {
        hxc_tmp_conditional_result_n15 = hxc_assigned;
      }
      else
      {
        hxc_tmp_conditional_result_n15 = 93;
      }
      hxc_tmp_conditional_result_n14 = hxc_tmp_conditional_result_n15;
    }
    else
    {
      hxc_tmp_conditional_result_n14 = 94;
    }
    hxc_tmp_conditional_result_n13 = hxc_tmp_conditional_result_n14;
  }
  uint32_t hxc_result = hxc_tmp_conditional_result_n13;
  int32_t hxc_total = 0;
  int32_t hxc_outer = 0;
  while (1)
  {
    if (!(hxc_outer < 4))
    {
      break;
    }
    hxc_outer = hxc_i32_add_wrapping(hxc_outer, 1);
    if (hxc_outer == 2)
    {
      continue;
    }
    int32_t hxc_inner = 0;
    do
    {
      hxc_inner = hxc_i32_add_wrapping(hxc_inner, 1);
      int32_t hxc_tmp_load_result_n39 = hxc_inner;
      bool hxc_tmp_short_circuit_result_n19 = hxc_tmp_load_result_n39 == 2;
      if (hxc_tmp_load_result_n39 == 2)
      {
        hxc_tmp_short_circuit_result_n19 = hxc_outer == 3;
      }
      if (hxc_tmp_short_circuit_result_n19)
      {
        break;
      }
      else
      {
        switch (hxc_inner) {
          case 1:
            {
              int32_t hxc_tmp_compound_load_result_n43 = hxc_total;
              hxc_total = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n43, hxc_outer);
              break;
            }
          case 2:
          case 3:
            {
              int32_t hxc_tmp_compound_load_result_n45 = hxc_total;
              hxc_total = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n45, hxc_inner);
              break;
            }
          default:
            {
              hxc_total = hxc_i32_add_wrapping(hxc_total, 9);
              break;
            }
        }
      }
      if (!(hxc_inner < 3))
      {
        break;
      }
    }
    while (1);
  }
  int32_t hxc_rangeTotal = 0;
  int32_t hxc_rangeEnd = 3;
  int32_t hxc_symbol_habfcdbf4fca5 = 0;
  int32_t hxc_symbol_h0c443bac736a = hxc_rangeEnd;
  while (1)
  {
    int32_t hxc_tmp_load_result_n50 = hxc_symbol_habfcdbf4fca5;
    if (!(hxc_tmp_load_result_n50 < hxc_symbol_h0c443bac736a))
    {
      break;
    }
    int32_t hxc_tmp_increment_load_result_n52 = hxc_symbol_habfcdbf4fca5;
    hxc_symbol_habfcdbf4fca5 = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n52, 1);
    int32_t hxc_index = hxc_tmp_increment_load_result_n52;
    int32_t hxc_tmp_compound_load_result_n53 = hxc_rangeTotal;
    hxc_rangeTotal = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n53, hxc_index);
  }
  hxc_EvaluationFixture_switchCalls = 0;
  int32_t hxc_tmp_call_result_n56 = hxc_EvaluationFixture_switchSubject(hxc_total);
  int32_t hxc_symbol_h73c8af38b72c = hxc_tmp_call_result_n56;
  int32_t hxc_tmp_load_result_n57 = hxc_symbol_h73c8af38b72c;
  uint32_t hxc_tmp_switch_result_n27 = 0;
  switch (hxc_tmp_load_result_n57) {
    case 18:
      {
        hxc_tmp_switch_result_n27 = 2;
        break;
      }
    case 19:
      {
        hxc_tmp_switch_result_n27 = 3;
        break;
      }
    default:
      {
        hxc_tmp_switch_result_n27 = 95;
        break;
      }
  }
  uint32_t hxc_selectedBySwitch = hxc_tmp_switch_result_n27;
  bool hxc_controlIntact = true;
  if (hxc_total != 18)
  {
    hxc_controlIntact = false;
  }
  if (hxc_rangeTotal != 3)
  {
    hxc_controlIntact = false;
  }
  uint32_t hxc_tmp_global_load_result_n61 = hxc_EvaluationFixture_switchCalls;
  if (hxc_u32_to_i32_bits(hxc_tmp_global_load_result_n61) != 1)
  {
    hxc_controlIntact = false;
  }
  int32_t hxc_tmp_call_result_n62 = hxc_EvaluationFixture_exhaustiveAbstractReturn(2);
  if (hxc_tmp_call_result_n62 != 33)
  {
    hxc_controlIntact = false;
  }
  struct hxc_EvaluationFixture_EvaluationChoiceRecord hxc_tmp_call_result_n63 = hxc_EvaluationFixture_exhaustiveAbstractRecord(1);
  struct hxc_EvaluationFixture_EvaluationChoiceRecord hxc_abstractRecord = hxc_tmp_call_result_n63;
  int32_t hxc_tmp_record_field_load_result_n64 = hxc_abstractRecord.hxc_amount;
  bool hxc_tmp_short_circuit_result_n30 = hxc_tmp_record_field_load_result_n64 != 7;
  if (!(hxc_tmp_record_field_load_result_n64 != 7))
  {
    hxc_tmp_short_circuit_result_n30 = !hxc_abstractRecord.hxc_enabled;
  }
  if (hxc_tmp_short_circuit_result_n30)
  {
    hxc_controlIntact = false;
  }
  int32_t hxc_tmp_call_result_n67 = hxc_EvaluationFixture_readGlobalBeforeCall();
  if (hxc_tmp_call_result_n67 != 5)
  {
    hxc_controlIntact = false;
  }
  if (hxc_controlIntact)
  {
    hxc_result = hxc_selectedBySwitch;
  }
  else
  {
    hxc_result = 96;
  }
  uint32_t hxc_tmp_call_result_n71 = hxc_EvaluationFixture_finish(hxc_result);
  return hxc_tmp_call_result_n71;
}

bool hxc_EvaluationFixture_setCallFlag(bool hxc_value)
{
  hxc_EvaluationFixture_callFlag = hxc_value;
  return hxc_value;
}

bool hxc_EvaluationFixture_spoilShortCircuit(void)
{
  hxc_EvaluationFixture_shortCircuitIntact = false;
  return true;
}

uint32_t hxc_EvaluationFixture_spoilTernary(void)
{
  hxc_EvaluationFixture_ternaryIntact = false;
  return 8;
}

int32_t hxc_EvaluationFixture_switchSubject(int32_t hxc_value)
{
  uint32_t hxc_tmp_increment_load_result_n0 = hxc_EvaluationFixture_switchCalls;
  hxc_EvaluationFixture_switchCalls = (uint32_t)((uint64_t)(uint32_t)hxc_tmp_increment_load_result_n0 + (uint64_t)(uint32_t)1);
  return hxc_value;
}

int main(void)
{
  hxc_EvaluationFixture_main();
  return 0;
}
