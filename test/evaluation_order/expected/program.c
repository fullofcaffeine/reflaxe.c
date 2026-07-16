#include "hxc/program.h"

bool hxc_field_EvaluationFixture_callFlag = false;

uint32_t hxc_field_EvaluationFixture_counter = 0;

bool hxc_field_EvaluationFixture_shortCircuitIntact = true;

uint32_t hxc_field_EvaluationFixture_switchCalls = 0;

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
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n2 = true;
  if (true)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n4;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n5;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n4:
    ;
  bool hxc_temp_EvaluationFixture_run_callzx2Dresult_n6 = hxc_method_EvaluationFixture_setCallFlag(true);
  hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n2 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n6;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n5;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n5:
    ;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n7 = hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n2;
  (void)hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n7;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n8 = hxc_field_EvaluationFixture_callFlag;
  if (!hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n8)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n6;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n7;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n6:
    ;
  hxc_field_EvaluationFixture_shortCircuitIntact = false;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n7;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n7:
    ;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n3 = false;
  if (false)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n9;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n8;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n8:
    ;
  bool hxc_temp_EvaluationFixture_run_callzx2Dresult_n9 = hxc_method_EvaluationFixture_setCallFlag(false);
  hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n3 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n9;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n9;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n9:
    ;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n10 = hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n3;
  (void)hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n10;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n11 = hxc_field_EvaluationFixture_callFlag;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n11)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n10;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n11;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n10:
    ;
  hxc_field_EvaluationFixture_shortCircuitIntact = false;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n11;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n11:
    ;
  hxc_field_EvaluationFixture_ternaryIntact = true;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n12 = hxc_field_EvaluationFixture_ternaryIntact;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n5 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n12)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n12;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n13;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n12:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_callzx2Dresult_n13 = hxc_method_EvaluationFixture_keepTernary();
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n5 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n13;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n14;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n13:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_callzx2Dresult_n14 = hxc_method_EvaluationFixture_spoilTernary();
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n5 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n14;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n14;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n14:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n15 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n5;
  uint32_t hxc_local_EvaluationFixture_run_selected_n4 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n15;
  hxc_field_EvaluationFixture_counter = 0;
  uint32_t hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n16 = hxc_field_EvaluationFixture_counter;
  hxc_field_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n16 + (uint64_t)(uint32_t)1);
  uint32_t hxc_local_EvaluationFixture_run_old_n6 = hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n16;
  uint32_t hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n17 = hxc_field_EvaluationFixture_counter;
  hxc_field_EvaluationFixture_counter = (uint32_t)((uint64_t)(uint32_t)hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n17 + (uint64_t)(uint32_t)1);
  uint32_t hxc_local_EvaluationFixture_run_fresh_n7 = (uint32_t)((uint64_t)(uint32_t)hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n17 + (uint64_t)(uint32_t)1);
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n18 = hxc_local_EvaluationFixture_run_old_n6;
  (void)hxc_temp_EvaluationFixture_run_loadzx2Dresult_n18;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n19 = hxc_local_EvaluationFixture_run_fresh_n7;
  (void)hxc_temp_EvaluationFixture_run_loadzx2Dresult_n19;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n20 = hxc_local_EvaluationFixture_run_selected_n4;
  (void)hxc_temp_EvaluationFixture_run_loadzx2Dresult_n20;
  uint32_t hxc_local_EvaluationFixture_run_assigned_n8 = 99;
  uint32_t hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n21 = hxc_field_EvaluationFixture_counter;
  hxc_local_EvaluationFixture_run_assigned_n8 = hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n21;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n22 = hxc_field_EvaluationFixture_callFlag;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n22)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n15;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n16;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n15:
    ;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10 = 91;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n17;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n16:
    ;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n23 = hxc_field_EvaluationFixture_shortCircuitIntact;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n11 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n23)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n18;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n19;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n17:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n28 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10;
  uint32_t hxc_local_EvaluationFixture_run_result_n9 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n28;
  int32_t hxc_local_EvaluationFixture_run_total_n13 = 0;
  int32_t hxc_local_EvaluationFixture_run_outer_n14 = 0;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dcondition_n24;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n18:
    ;
  bool hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n24 = hxc_field_EvaluationFixture_ternaryIntact;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n12 = 0;
  if (hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n24)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n21;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n22;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n19:
    ;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n11 = 94;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n20;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n20:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n27 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n11;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n10 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n27;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n17;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dtrue_n21:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n25 = hxc_local_EvaluationFixture_run_assigned_n8;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n12 = hxc_temp_EvaluationFixture_run_loadzx2Dresult_n25;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n23;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Dfalse_n22:
    ;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n12 = 93;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n23;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n23:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n26 = hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n12;
  hxc_temp_EvaluationFixture_run_conditionalzx2Dresult_n11 = hxc_temp_EvaluationFixture_run_conditionalzx2Dloadzx2Dresult_n26;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_conditionalzx2Djoin_n20;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dcondition_n24:
    ;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n29 = hxc_local_EvaluationFixture_run_outer_n14;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n29 < 4)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dbody_n25;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dexit_n26;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dbody_n25:
    ;
  int32_t hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n30 = hxc_local_EvaluationFixture_run_outer_n14;
  hxc_local_EvaluationFixture_run_outer_n14 = hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ei32zx2Eaddzx2Ewrapping(hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n30, 1);
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n31 = hxc_local_EvaluationFixture_run_outer_n14;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n31 == 2)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n27;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n28;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dexit_n26:
    ;
  int32_t hxc_local_EvaluationFixture_run_rangeTotal_n17 = 0;
  int32_t hxc_local_EvaluationFixture_run_rangeEnd_n18 = 3;
  int32_t hxc_local_EvaluationFixture_run_zx60_n19 = 0;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n43 = hxc_local_EvaluationFixture_run_rangeEnd_n18;
  int32_t hxc_local_EvaluationFixture_run_zx60_n20 = hxc_temp_EvaluationFixture_run_loadzx2Dresult_n43;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dcondition_n40;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n27:
    ;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dcondition_n24;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n28:
    ;
  int32_t hxc_local_EvaluationFixture_run_inner_n15 = 0;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dbody_n29;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dbody_n29:
    ;
  int32_t hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n32 = hxc_local_EvaluationFixture_run_inner_n15;
  hxc_local_EvaluationFixture_run_inner_n15 = hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ei32zx2Eaddzx2Ewrapping(hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n32, 1);
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n33 = hxc_local_EvaluationFixture_run_inner_n15;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n16 = hxc_temp_EvaluationFixture_run_loadzx2Dresult_n33 == 2;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n33 == 2)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n32;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n33;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Drhs_n32:
    ;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n34 = hxc_local_EvaluationFixture_run_outer_n14;
  hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n16 = hxc_temp_EvaluationFixture_run_loadzx2Dresult_n34 == 3;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n33;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_shortzx2Dcircuitzx2Djoin_n33:
    ;
  bool hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n35 = hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dresult_n16;
  if (hxc_temp_EvaluationFixture_run_shortzx2Dcircuitzx2Dloadzx2Dresult_n35)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n34;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n35;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n34:
    ;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dexit_n31;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n35:
    ;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n36 = hxc_local_EvaluationFixture_run_inner_n15;
  switch (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n36) {
    case 1:
      {
        goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dcasezx2D0_n36;
      }
    case 2:
      {
        goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dcasezx2D1_n37;
      }
    case 3:
      {
        goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dcasezx2D1_n37;
      }
    default:
      {
        goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Ddefault_n38;
      }
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dcasezx2D0_n36:
    ;
  int32_t hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n37 = hxc_local_EvaluationFixture_run_total_n13;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n38 = hxc_local_EvaluationFixture_run_outer_n14;
  hxc_local_EvaluationFixture_run_total_n13 = hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ei32zx2Eaddzx2Ewrapping(hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n37, hxc_temp_EvaluationFixture_run_loadzx2Dresult_n38);
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dexit_n39;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dcasezx2D1_n37:
    ;
  int32_t hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n39 = hxc_local_EvaluationFixture_run_total_n13;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n40 = hxc_local_EvaluationFixture_run_inner_n15;
  hxc_local_EvaluationFixture_run_total_n13 = hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ei32zx2Eaddzx2Ewrapping(hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n39, hxc_temp_EvaluationFixture_run_loadzx2Dresult_n40);
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dexit_n39;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Ddefault_n38:
    ;
  int32_t hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n41 = hxc_local_EvaluationFixture_run_total_n13;
  hxc_local_EvaluationFixture_run_total_n13 = hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ei32zx2Eaddzx2Ewrapping(hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n41, 9);
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dexit_n39;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dexit_n39:
    ;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dcondition_n30;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dcondition_n30:
    ;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n42 = hxc_local_EvaluationFixture_run_inner_n15;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n42 < 3)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dbody_n29;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dexit_n31;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_dozx2Dexit_n31:
    ;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dcondition_n24;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dcondition_n40:
    ;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n44 = hxc_local_EvaluationFixture_run_zx60_n19;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n45 = hxc_local_EvaluationFixture_run_zx60_n20;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n44 < hxc_temp_EvaluationFixture_run_loadzx2Dresult_n45)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dbody_n41;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dexit_n42;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dbody_n41:
    ;
  int32_t hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n46 = hxc_local_EvaluationFixture_run_zx60_n19;
  hxc_local_EvaluationFixture_run_zx60_n19 = hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ei32zx2Eaddzx2Ewrapping(hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n46, 1);
  int32_t hxc_local_EvaluationFixture_run_index_n21 = hxc_temp_EvaluationFixture_run_incrementzx2Dloadzx2Dresult_n46;
  int32_t hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n47 = hxc_local_EvaluationFixture_run_rangeTotal_n17;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n48 = hxc_local_EvaluationFixture_run_index_n21;
  hxc_local_EvaluationFixture_run_rangeTotal_n17 = hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Ei32zx2Eaddzx2Ewrapping(hxc_temp_EvaluationFixture_run_compoundzx2Dloadzx2Dresult_n47, hxc_temp_EvaluationFixture_run_loadzx2Dresult_n48);
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dcondition_n40;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_whilezx2Dexit_n42:
    ;
  hxc_field_EvaluationFixture_switchCalls = 0;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n49 = hxc_local_EvaluationFixture_run_total_n13;
  int32_t hxc_temp_EvaluationFixture_run_callzx2Dresult_n50 = hxc_method_EvaluationFixture_switchSubject(hxc_temp_EvaluationFixture_run_loadzx2Dresult_n49);
  int32_t hxc_local_EvaluationFixture_run_zx60_n23 = hxc_temp_EvaluationFixture_run_callzx2Dresult_n50;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n51 = hxc_local_EvaluationFixture_run_zx60_n23;
  uint32_t hxc_temp_EvaluationFixture_run_switchzx2Dresult_n24 = 0;
  switch (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n51) {
    case 18:
      {
        goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Dcasezx2D0_n43;
      }
    case 19:
      {
        goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Dcasezx2D1_n44;
      }
    default:
      {
        goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Ddefault_n45;
      }
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Dcasezx2D0_n43:
    ;
  hxc_temp_EvaluationFixture_run_switchzx2Dresult_n24 = 2;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Djoin_n46;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Dcasezx2D1_n44:
    ;
  hxc_temp_EvaluationFixture_run_switchzx2Dresult_n24 = 3;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Djoin_n46;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Ddefault_n45:
    ;
  hxc_temp_EvaluationFixture_run_switchzx2Dresult_n24 = 95;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Djoin_n46;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_switchzx2Dvaluezx2Djoin_n46:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_switchzx2Dresultzx2Dloadzx2Dresult_n52 = hxc_temp_EvaluationFixture_run_switchzx2Dresult_n24;
  uint32_t hxc_local_EvaluationFixture_run_selectedBySwitch_n22 = hxc_temp_EvaluationFixture_run_switchzx2Dresultzx2Dloadzx2Dresult_n52;
  bool hxc_local_EvaluationFixture_run_controlIntact_n25 = true;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n53 = hxc_local_EvaluationFixture_run_total_n13;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n53 != 18)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n47;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n48;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n47:
    ;
  hxc_local_EvaluationFixture_run_controlIntact_n25 = false;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n48;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n48:
    ;
  int32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n54 = hxc_local_EvaluationFixture_run_rangeTotal_n17;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n54 != 3)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n49;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n50;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n49:
    ;
  hxc_local_EvaluationFixture_run_controlIntact_n25 = false;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n50;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n50:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n55 = hxc_field_EvaluationFixture_switchCalls;
  if (hxc_spec_compiler_primitivezx2Dhelper_of_hxczx2Eu32zx2Etozx2Ei32zx2Ebits(hxc_temp_EvaluationFixture_run_globalzx2Dloadzx2Dresult_n55) != 1)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n51;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n52;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n51:
    ;
  hxc_local_EvaluationFixture_run_controlIntact_n25 = false;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n52;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n52:
    ;
  bool hxc_temp_EvaluationFixture_run_loadzx2Dresult_n56 = hxc_local_EvaluationFixture_run_controlIntact_n25;
  if (hxc_temp_EvaluationFixture_run_loadzx2Dresult_n56)
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n53;
  }
  else
  {
    goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dfalse_n54;
  }
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dtrue_n53:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n57 = hxc_local_EvaluationFixture_run_selectedBySwitch_n22;
  hxc_local_EvaluationFixture_run_result_n9 = hxc_temp_EvaluationFixture_run_loadzx2Dresult_n57;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n55;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Dfalse_n54:
    ;
  hxc_local_EvaluationFixture_run_result_n9 = 96;
  goto hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n55;
hxc_temp_EvaluationFixture_run_blockzx2Dlabel_ifzx2Djoin_n55:
    ;
  uint32_t hxc_temp_EvaluationFixture_run_loadzx2Dresult_n58 = hxc_local_EvaluationFixture_run_result_n9;
  uint32_t hxc_temp_EvaluationFixture_run_callzx2Dresult_n59 = hxc_method_EvaluationFixture_finish(hxc_temp_EvaluationFixture_run_loadzx2Dresult_n58);
  return hxc_temp_EvaluationFixture_run_callzx2Dresult_n59;
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

int32_t hxc_method_EvaluationFixture_switchSubject(int32_t hxc_local_EvaluationFixture_switchSubject_value_n0)
{
  uint32_t hxc_temp_EvaluationFixture_switchSubject_incrementzx2Dloadzx2Dresult_n0 = hxc_field_EvaluationFixture_switchCalls;
  hxc_field_EvaluationFixture_switchCalls = (uint32_t)((uint64_t)(uint32_t)hxc_temp_EvaluationFixture_switchSubject_incrementzx2Dloadzx2Dresult_n0 + (uint64_t)(uint32_t)1);
  return hxc_local_EvaluationFixture_switchSubject_value_n0;
}

int main(void)
{
  hxc_method_EvaluationFixture_main();
  return 0;
}
