#include "hxc/program.h"

int32_t hxc_EvaluationFixture_barrierValue = 0;

bool hxc_EvaluationFixture_callFlag = false;

uint32_t hxc_EvaluationFixture_counter = 0;

bool hxc_EvaluationFixture_shortCircuitIntact = true;

uint32_t hxc_EvaluationFixture_switchCalls = 0;

bool hxc_EvaluationFixture_ternaryIntact = true;

void hxc_EvaluationFixture_consumePair(bool hxc_first, bool hxc_second)
{
  (void)hxc_first;
  (void)hxc_second;
  return;
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
  bool hxc_tmp_call_result_n1 = hxc_EvaluationFixture_setCallFlag(false);
  hxc_EvaluationFixture_consumePair(hxc_tmp_call_result_n0, hxc_tmp_call_result_n1);
  hxc_EvaluationFixture_shortCircuitIntact = true;
  bool hxc_tmp_short_circuit_result_n0 = false;
  if (false)
  {
    bool hxc_tmp_call_result_n2 = hxc_EvaluationFixture_spoilShortCircuit();
    hxc_tmp_short_circuit_result_n0 = hxc_tmp_call_result_n2;
  }
  bool hxc_tmp_short_circuit_load_result_n3 = hxc_tmp_short_circuit_result_n0;
  (void)hxc_tmp_short_circuit_load_result_n3;
  bool hxc_tmp_short_circuit_result_n1 = true;
  if (!true)
  {
    bool hxc_tmp_call_result_n4 = hxc_EvaluationFixture_spoilShortCircuit();
    hxc_tmp_short_circuit_result_n1 = hxc_tmp_call_result_n4;
  }
  bool hxc_tmp_short_circuit_load_result_n5 = hxc_tmp_short_circuit_result_n1;
  (void)hxc_tmp_short_circuit_load_result_n5;
  bool hxc_tmp_short_circuit_result_n2 = true;
  if (true)
  {
    bool hxc_tmp_call_result_n6 = hxc_EvaluationFixture_setCallFlag(true);
    hxc_tmp_short_circuit_result_n2 = hxc_tmp_call_result_n6;
  }
  bool hxc_tmp_short_circuit_load_result_n7 = hxc_tmp_short_circuit_result_n2;
  (void)hxc_tmp_short_circuit_load_result_n7;
  bool hxc_tmp_global_load_result_n8 = hxc_EvaluationFixture_callFlag;
  if (!hxc_tmp_global_load_result_n8)
  {
    hxc_EvaluationFixture_shortCircuitIntact = false;
  }
  bool hxc_tmp_short_circuit_result_n3 = false;
  if (!false)
  {
    bool hxc_tmp_call_result_n9 = hxc_EvaluationFixture_setCallFlag(false);
    hxc_tmp_short_circuit_result_n3 = hxc_tmp_call_result_n9;
  }
  bool hxc_tmp_short_circuit_load_result_n10 = hxc_tmp_short_circuit_result_n3;
  (void)hxc_tmp_short_circuit_load_result_n10;
  bool hxc_tmp_global_load_result_n11 = hxc_EvaluationFixture_callFlag;
  if (hxc_tmp_global_load_result_n11)
  {
    hxc_EvaluationFixture_shortCircuitIntact = false;
  }
  hxc_EvaluationFixture_ternaryIntact = true;
  bool hxc_tmp_global_load_result_n12 = hxc_EvaluationFixture_ternaryIntact;
  uint32_t hxc_tmp_conditional_result_n5 = 0;
  if (hxc_tmp_global_load_result_n12)
  {
    uint32_t hxc_tmp_call_result_n13 = hxc_EvaluationFixture_keepTernary();
    hxc_tmp_conditional_result_n5 = hxc_tmp_call_result_n13;
  }
  else
  {
    uint32_t hxc_tmp_call_result_n14 = hxc_EvaluationFixture_spoilTernary();
    hxc_tmp_conditional_result_n5 = hxc_tmp_call_result_n14;
  }
  uint32_t hxc_selected = hxc_tmp_conditional_result_n5;
  hxc_EvaluationFixture_counter = 0;
  uint32_t hxc_tmp_increment_load_result_n16 = hxc_EvaluationFixture_counter;
  hxc_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_tmp_increment_load_result_n16 + (uint64_t)(uint32_t)1);
  uint32_t hxc_old = hxc_tmp_increment_load_result_n16;
  uint32_t hxc_tmp_increment_load_result_n17 = hxc_EvaluationFixture_counter;
  hxc_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_tmp_increment_load_result_n17 + (uint64_t)(uint32_t)1);
  uint32_t hxc_fresh = (uint32_t)((uint64_t)(uint32_t)hxc_tmp_increment_load_result_n17 + (uint64_t)(uint32_t)1);
  uint32_t hxc_tmp_load_result_n18 = hxc_old;
  (void)hxc_tmp_load_result_n18;
  uint32_t hxc_tmp_load_result_n19 = hxc_fresh;
  (void)hxc_tmp_load_result_n19;
  uint32_t hxc_tmp_load_result_n20 = hxc_selected;
  (void)hxc_tmp_load_result_n20;
  uint32_t hxc_assigned = 99;
  uint32_t hxc_tmp_global_load_result_n21 = hxc_EvaluationFixture_counter;
  hxc_assigned = hxc_tmp_global_load_result_n21;
  bool hxc_tmp_global_load_result_n22 = hxc_EvaluationFixture_callFlag;
  uint32_t hxc_tmp_conditional_result_n10 = 0;
  if (hxc_tmp_global_load_result_n22)
  {
    hxc_tmp_conditional_result_n10 = 91;
  }
  else
  {
    bool hxc_tmp_global_load_result_n23 = hxc_EvaluationFixture_shortCircuitIntact;
    uint32_t hxc_tmp_conditional_result_n11 = 0;
    if (hxc_tmp_global_load_result_n23)
    {
      bool hxc_tmp_global_load_result_n24 = hxc_EvaluationFixture_ternaryIntact;
      uint32_t hxc_tmp_conditional_result_n12 = 0;
      if (hxc_tmp_global_load_result_n24)
      {
        hxc_tmp_conditional_result_n12 = hxc_assigned;
      }
      else
      {
        hxc_tmp_conditional_result_n12 = 93;
      }
      hxc_tmp_conditional_result_n11 = hxc_tmp_conditional_result_n12;
    }
    else
    {
      hxc_tmp_conditional_result_n11 = 94;
    }
    hxc_tmp_conditional_result_n10 = hxc_tmp_conditional_result_n11;
  }
  uint32_t hxc_result = hxc_tmp_conditional_result_n10;
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
      int32_t hxc_tmp_load_result_n33 = hxc_inner;
      bool hxc_tmp_short_circuit_result_n16 = hxc_tmp_load_result_n33 == 2;
      if (hxc_tmp_load_result_n33 == 2)
      {
        hxc_tmp_short_circuit_result_n16 = hxc_outer == 3;
      }
      if (hxc_tmp_short_circuit_result_n16)
      {
        break;
      }
      else
      {
        switch (hxc_inner) {
          case 1:
            {
              int32_t hxc_tmp_compound_load_result_n37 = hxc_total;
              hxc_total = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n37, hxc_outer);
              break;
            }
          case 2:
          case 3:
            {
              int32_t hxc_tmp_compound_load_result_n39 = hxc_total;
              hxc_total = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n39, hxc_inner);
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
  int32_t hxc_symbol_h9dc820c6c711 = 0;
  int32_t hxc_symbol_hff91343ee89b = hxc_rangeEnd;
  while (1)
  {
    int32_t hxc_tmp_load_result_n44 = hxc_symbol_h9dc820c6c711;
    if (!(hxc_tmp_load_result_n44 < hxc_symbol_hff91343ee89b))
    {
      break;
    }
    int32_t hxc_tmp_increment_load_result_n46 = hxc_symbol_h9dc820c6c711;
    hxc_symbol_h9dc820c6c711 = hxc_i32_add_wrapping(hxc_tmp_increment_load_result_n46, 1);
    int32_t hxc_index = hxc_tmp_increment_load_result_n46;
    int32_t hxc_tmp_compound_load_result_n47 = hxc_rangeTotal;
    hxc_rangeTotal = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n47, hxc_index);
  }
  hxc_EvaluationFixture_switchCalls = 0;
  int32_t hxc_tmp_call_result_n50 = hxc_EvaluationFixture_switchSubject(hxc_total);
  int32_t hxc_symbol_h0c443bac736a = hxc_tmp_call_result_n50;
  int32_t hxc_tmp_load_result_n51 = hxc_symbol_h0c443bac736a;
  uint32_t hxc_tmp_switch_result_n24 = 0;
  switch (hxc_tmp_load_result_n51) {
    case 18:
      {
        hxc_tmp_switch_result_n24 = 2;
        break;
      }
    case 19:
      {
        hxc_tmp_switch_result_n24 = 3;
        break;
      }
    default:
      {
        hxc_tmp_switch_result_n24 = 95;
        break;
      }
  }
  uint32_t hxc_selectedBySwitch = hxc_tmp_switch_result_n24;
  bool hxc_controlIntact = true;
  if (hxc_total != 18)
  {
    hxc_controlIntact = false;
  }
  if (hxc_rangeTotal != 3)
  {
    hxc_controlIntact = false;
  }
  uint32_t hxc_tmp_global_load_result_n55 = hxc_EvaluationFixture_switchCalls;
  if (hxc_u32_to_i32_bits(hxc_tmp_global_load_result_n55) != 1)
  {
    hxc_controlIntact = false;
  }
  int32_t hxc_tmp_call_result_n56 = hxc_EvaluationFixture_readGlobalBeforeCall();
  if (hxc_tmp_call_result_n56 != 5)
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
  uint32_t hxc_tmp_call_result_n60 = hxc_EvaluationFixture_finish(hxc_result);
  return hxc_tmp_call_result_n60;
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
