#include "hxc/program.h"

bool hxc_field_EvaluationFixture_callFlag = false;

uint32_t hxc_field_EvaluationFixture_counter = 0;

bool hxc_field_EvaluationFixture_shortCircuitIntact = true;

bool hxc_field_EvaluationFixture_ternaryIntact = true;

void hxc_method_EvaluationFixture_consumePair(bool hxc_local_EvaluationFixture_consumePair_first_n0, bool hxc_local_EvaluationFixture_consumePair_second_n1)
{
  (void)hxc_local_EvaluationFixture_consumePair_first_n0;
  (void)hxc_local_EvaluationFixture_consumePair_second_n1;
  return;
}

uint32_t hxc_method_EvaluationFixture_finish(uint32_t hxc_local_EvaluationFixture_finish_value_n0)
{
  return hxc_local_EvaluationFixture_finish_value_n0;
}

uint32_t hxc_method_EvaluationFixture_keepTernary(void)
{
  return 7;
}

void hxc_method_EvaluationFixture_main(void)
{
  hxc_method_EvaluationFixture_run();
  return;
}

uint32_t hxc_method_EvaluationFixture_run(void)
{
  hxc_field_EvaluationFixture_callFlag = false;
  bool hxc_temp_EvaluationFixture_run_callzx2Dresult_n0 = hxc_method_EvaluationFixture_setCallFlag(true);
  bool hxc_temp_EvaluationFixture_run_callzx2Dresult_n1 = hxc_method_EvaluationFixture_setCallFlag(false);
  hxc_method_EvaluationFixture_consumePair(hxc_temp_EvaluationFixture_run_callzx2Dresult_n0, hxc_temp_EvaluationFixture_run_callzx2Dresult_n1);
  hxc_field_EvaluationFixture_shortCircuitIntact = true;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n0 = false;
  if (false)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n0;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n1;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n0:
    ;
  bool hxc_temp_EvaluationFixture_run_callzx2Dresult_n2 = hxc_method_EvaluationFixture_spoilShortCircuit();
  hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n0 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n2;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n1;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n1:
    ;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n3 = hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n0;
  (void)hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n3;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n1 = true;
  if (true)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n3;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n2;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n2:
    ;
  bool hxc_temp_EvaluationFixture_run_callzx2Dresult_n4 = hxc_method_EvaluationFixture_spoilShortCircuit();
  hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n1 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n4;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n3;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n3:
    ;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n5 = hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n1;
  (void)hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n5;
  hxc_field_EvaluationFixture_ternaryIntact = true;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n6 = hxc_field_EvaluationFixture_ternaryIntact;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n3 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n6)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n4;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n5;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n4:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_callzx2Dresult_n7 = hxc_method_EvaluationFixture_keepTernary();
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n3 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n7;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n6;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n5:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_callzx2Dresult_n8 = hxc_method_EvaluationFixture_spoilTernary();
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n3 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n8;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n6;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n6:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n9 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n3;
  uint32_t hxc_local_EvaluationFixture_run_selected_n2 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n9;
  hxc_field_EvaluationFixture_counter = 0;
  uint32_t hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n10 = hxc_field_EvaluationFixture_counter;
  hxc_field_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n10 + (uint64_t)(uint32_t)1);
  uint32_t hxc_local_EvaluationFixture_run_old_n4 = hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n10;
  uint32_t hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n11 = hxc_field_EvaluationFixture_counter;
  hxc_field_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n11 + (uint64_t)(uint32_t)1);
  uint32_t hxc_local_EvaluationFixture_run_fresh_n5 = (uint32_t)((uint64_t)(uint32_t)hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n11 + (uint64_t)(uint32_t)1);
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n12 = hxc_local_EvaluationFixture_run_old_n4;
  (void)hxc_temp_EvaluationFixture_run_loadzx2Dresult_n12;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n13 = hxc_local_EvaluationFixture_run_fresh_n5;
  (void)hxc_temp_EvaluationFixture_run_loadzx2Dresult_n13;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n14 = hxc_local_EvaluationFixture_run_selected_n2;
  (void)hxc_temp_EvaluationFixture_run_loadzx2Dresult_n14;
  uint32_t hxc_local_EvaluationFixture_run_assigned_n6 = 99;
  uint32_t hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n15 = hxc_field_EvaluationFixture_counter;
  hxc_local_EvaluationFixture_run_assigned_n6 = hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n15;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n16 = hxc_field_EvaluationFixture_callFlag;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n8 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n16)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n7;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n8;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n7:
    ;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n8 = 91;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n9;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n8:
    ;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n17 = hxc_field_EvaluationFixture_shortCircuitIntact;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n9 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n17)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n10;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n11;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n9:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n22 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n8;
  uint32_t hxc_local_EvaluationFixture_run_result_n7 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n22;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n23 = hxc_local_EvaluationFixture_run_result_n7;
  uint32_t hxc_temp_EvaluationFixture_run_callzx2Dresult_n24 = hxc_method_EvaluationFixture_finish(hxc_temp_EvaluationFixture_run_loadzx2Dresult_n23);
  return hxc_temp_EvaluationFixture_run_callzx2Dresult_n24;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n10:
    ;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n18 = hxc_field_EvaluationFixture_ternaryIntact;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n18)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n13;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n14;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n11:
    ;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n9 = 94;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n12;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n12:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n21 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n9;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n8 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n21;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n9;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n13:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n19 = hxc_local_EvaluationFixture_run_assigned_n6;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10 = hxc_temp_EvaluationFixture_run_loadzx2Dresult_n19;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n15;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n14:
    ;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10 = 93;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n15;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n15:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n20 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n9 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n20;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n12;
}

bool hxc_method_EvaluationFixture_setCallFlag(bool hxc_local_EvaluationFixture_setCallFlag_value_n0)
{
  hxc_field_EvaluationFixture_callFlag = hxc_local_EvaluationFixture_setCallFlag_value_n0;
  return hxc_local_EvaluationFixture_setCallFlag_value_n0;
}

bool hxc_method_EvaluationFixture_spoilShortCircuit(void)
{
  hxc_field_EvaluationFixture_shortCircuitIntact = false;
  return true;
}

uint32_t hxc_method_EvaluationFixture_spoilTernary(void)
{
  hxc_field_EvaluationFixture_ternaryIntact = false;
  return 8;
}

int main(void)
{
  hxc_method_EvaluationFixture_main();
  return 0;
}
