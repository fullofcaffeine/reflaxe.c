#include "hxc/program.h"

void hxc_caxecraft_qa_DomainProbe_clear(uint8_t *hxc_l_cells, size_t hxc_l_length)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n1 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n1;
  int32_t hxc_l_index = 0;
  while (1)
  {
    if (!(hxc_l_index < 32768))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_index;
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_index;
    struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_World_coord(hxc_i32_bit_and(hxc_l_tmp_load_result_n1, 63), hxc_i32_bit_and(hxc_i32_unsigned_shift_right_masked(hxc_l_tmp_load_result_n2, 6), 15), hxc_i32_unsigned_shift_right_masked(hxc_l_index, 10));
    hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n4, hxc_caxecraft_domain_BlockKind_Air);
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  return;
}

void hxc_caxecraft_qa_DomainProbe_main(void)
{
  hxc_caxecraft_qa_DomainProbe_selfCheck();
  hxc_caxecraft_domain_CaxecraftTrace_runTrace();
  return;
}

bool hxc_caxecraft_qa_DomainProbe_near(double hxc_l_left, double hxc_l_right)
{
  double hxc_l_tmp_conditional_result_n3 = 0.0;
  if (hxc_l_left < hxc_l_right)
  {
    hxc_l_tmp_conditional_result_n3 = hxc_l_right - hxc_l_left;
  }
  else
  {
    hxc_l_tmp_conditional_result_n3 = hxc_l_left - hxc_l_right;
  }
  double hxc_l_difference = hxc_l_tmp_conditional_result_n3;
  return hxc_l_difference < 0.00001;
}

int32_t hxc_caxecraft_qa_DomainProbe_selfCheck(void)
{
  uint8_t hxc_l_storage[32768] = { 0 };
  uint8_t *hxc_l_cells = hxc_l_storage;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  const uint8_t *hxc_l_view = hxc_l_storage;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(0, 0, 0);
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_World_indexOf(hxc_l_tmp_call_result_n0);
  if (hxc_l_tmp_call_result_n1 != 0)
  {
    return 1;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_World_coord(63, 15, 31);
  int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_World_indexOf(hxc_l_tmp_call_result_n2);
  if (hxc_l_tmp_call_result_n3 != 32767)
  {
    return 2;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_World_coord(-2147483647, 15, 31);
  int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_World_indexOf(hxc_l_tmp_call_result_n4);
  if (hxc_l_tmp_call_result_n5 != -1)
  {
    return 3;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_World_coord(64, 0, 0);
  bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n6, hxc_caxecraft_domain_BlockKind_Stone);
  if (hxc_l_tmp_call_result_n8)
  {
    return 4;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_l_tmp_call_result_n11 = hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n9, hxc_caxecraft_domain_BlockKind_Stone);
  if (!hxc_l_tmp_call_result_n11)
  {
    return 5;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n12 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_axis = hxc_l_tmp_call_result_n12;
  bool hxc_l_tmp_record_field_load_result_n13 = hxc_l_axis.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n4 = !hxc_l_tmp_record_field_load_result_n13;
  if (!!hxc_l_tmp_record_field_load_result_n13)
  {
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_axis.hxc_cellX != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n15 = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_short_circuit_load_result_n15;
  if (!hxc_l_tmp_short_circuit_load_result_n15)
  {
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_axis.hxc_cellY != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n17 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n17;
  if (!hxc_l_tmp_short_circuit_load_result_n17)
  {
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_axis.hxc_cellZ != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n19 = hxc_l_tmp_short_circuit_result_n6;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_short_circuit_load_result_n19;
  if (!hxc_l_tmp_short_circuit_load_result_n19)
  {
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_axis.hxc_previousX != 7;
  }
  bool hxc_l_tmp_short_circuit_load_result_n21 = hxc_l_tmp_short_circuit_result_n7;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n21;
  if (!hxc_l_tmp_short_circuit_load_result_n21)
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_axis.hxc_normalX != -1;
  }
  bool hxc_l_tmp_short_circuit_load_result_n23 = hxc_l_tmp_short_circuit_result_n8;
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_short_circuit_load_result_n23;
  if (!hxc_l_tmp_short_circuit_load_result_n23)
  {
    bool hxc_l_tmp_call_result_n25 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_axis.hxc_distance, 5.5);
    hxc_l_tmp_short_circuit_result_n9 = !hxc_l_tmp_call_result_n25;
  }
  bool hxc_l_tmp_short_circuit_load_result_n26 = hxc_l_tmp_short_circuit_result_n9;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_short_circuit_load_result_n26;
  if (!hxc_l_tmp_short_circuit_load_result_n26)
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_axis.hxc_visited != 7;
  }
  if (hxc_l_tmp_short_circuit_result_n10)
  {
    return 10;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n29 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 12.5, 8.5, 8.5, -1.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_negative = hxc_l_tmp_call_result_n29;
  bool hxc_l_tmp_record_field_load_result_n30 = hxc_l_negative.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n12 = !hxc_l_tmp_record_field_load_result_n30;
  if (!!hxc_l_tmp_record_field_load_result_n30)
  {
    hxc_l_tmp_short_circuit_result_n12 = hxc_l_negative.hxc_previousX != 9;
  }
  bool hxc_l_tmp_short_circuit_load_result_n32 = hxc_l_tmp_short_circuit_result_n12;
  bool hxc_l_tmp_short_circuit_result_n13 = hxc_l_tmp_short_circuit_load_result_n32;
  if (!hxc_l_tmp_short_circuit_load_result_n32)
  {
    hxc_l_tmp_short_circuit_result_n13 = hxc_l_negative.hxc_normalX != 1;
  }
  bool hxc_l_tmp_short_circuit_load_result_n34 = hxc_l_tmp_short_circuit_result_n13;
  bool hxc_l_tmp_short_circuit_result_n14 = hxc_l_tmp_short_circuit_load_result_n34;
  if (!hxc_l_tmp_short_circuit_load_result_n34)
  {
    bool hxc_l_tmp_call_result_n36 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_negative.hxc_distance, 3.5);
    hxc_l_tmp_short_circuit_result_n14 = !hxc_l_tmp_call_result_n36;
  }
  if (hxc_l_tmp_short_circuit_result_n14)
  {
    return 11;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n38 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n38, hxc_caxecraft_domain_BlockKind_Air);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n40 = hxc_caxecraft_domain_World_coord(10, 10, 10);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n40, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n42 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_diagonal = hxc_l_tmp_call_result_n42;
  bool hxc_l_tmp_record_field_load_result_n43 = hxc_l_diagonal.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n16 = !hxc_l_tmp_record_field_load_result_n43;
  if (!!hxc_l_tmp_record_field_load_result_n43)
  {
    hxc_l_tmp_short_circuit_result_n16 = hxc_l_diagonal.hxc_cellX != 10;
  }
  bool hxc_l_tmp_short_circuit_load_result_n45 = hxc_l_tmp_short_circuit_result_n16;
  bool hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_short_circuit_load_result_n45;
  if (!hxc_l_tmp_short_circuit_load_result_n45)
  {
    hxc_l_tmp_short_circuit_result_n17 = hxc_l_diagonal.hxc_cellY != 10;
  }
  bool hxc_l_tmp_short_circuit_load_result_n47 = hxc_l_tmp_short_circuit_result_n17;
  bool hxc_l_tmp_short_circuit_result_n18 = hxc_l_tmp_short_circuit_load_result_n47;
  if (!hxc_l_tmp_short_circuit_load_result_n47)
  {
    hxc_l_tmp_short_circuit_result_n18 = hxc_l_diagonal.hxc_cellZ != 10;
  }
  bool hxc_l_tmp_short_circuit_load_result_n49 = hxc_l_tmp_short_circuit_result_n18;
  bool hxc_l_tmp_short_circuit_result_n19 = hxc_l_tmp_short_circuit_load_result_n49;
  if (!hxc_l_tmp_short_circuit_load_result_n49)
  {
    hxc_l_tmp_short_circuit_result_n19 = hxc_l_diagonal.hxc_visited != 10;
  }
  if (hxc_l_tmp_short_circuit_result_n19)
  {
    return 12;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n52 = hxc_caxecraft_domain_World_coord(8, 7, 7);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n52, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n54 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tieX = hxc_l_tmp_call_result_n54;
  bool hxc_l_tmp_record_field_load_result_n55 = hxc_l_tieX.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n21 = !hxc_l_tmp_record_field_load_result_n55;
  if (!!hxc_l_tmp_record_field_load_result_n55)
  {
    hxc_l_tmp_short_circuit_result_n21 = hxc_l_tieX.hxc_cellX != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n57 = hxc_l_tmp_short_circuit_result_n21;
  bool hxc_l_tmp_short_circuit_result_n22 = hxc_l_tmp_short_circuit_load_result_n57;
  if (!hxc_l_tmp_short_circuit_load_result_n57)
  {
    hxc_l_tmp_short_circuit_result_n22 = hxc_l_tieX.hxc_cellY != 7;
  }
  bool hxc_l_tmp_short_circuit_load_result_n59 = hxc_l_tmp_short_circuit_result_n22;
  bool hxc_l_tmp_short_circuit_result_n23 = hxc_l_tmp_short_circuit_load_result_n59;
  if (!hxc_l_tmp_short_circuit_load_result_n59)
  {
    hxc_l_tmp_short_circuit_result_n23 = hxc_l_tieX.hxc_cellZ != 7;
  }
  bool hxc_l_tmp_short_circuit_load_result_n61 = hxc_l_tmp_short_circuit_result_n23;
  bool hxc_l_tmp_short_circuit_result_n24 = hxc_l_tmp_short_circuit_load_result_n61;
  if (!hxc_l_tmp_short_circuit_load_result_n61)
  {
    hxc_l_tmp_short_circuit_result_n24 = hxc_l_tieX.hxc_normalX != -1;
  }
  bool hxc_l_tmp_short_circuit_load_result_n63 = hxc_l_tmp_short_circuit_result_n24;
  bool hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_short_circuit_load_result_n63;
  if (!hxc_l_tmp_short_circuit_load_result_n63)
  {
    hxc_l_tmp_short_circuit_result_n25 = hxc_l_tieX.hxc_visited != 2;
  }
  if (hxc_l_tmp_short_circuit_result_n25)
  {
    return 18;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n66 = hxc_caxecraft_domain_World_coord(8, 7, 7);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n66, hxc_caxecraft_domain_BlockKind_Air);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n68 = hxc_caxecraft_domain_World_coord(8, 8, 7);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n68, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n70 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tieY = hxc_l_tmp_call_result_n70;
  bool hxc_l_tmp_record_field_load_result_n71 = hxc_l_tieY.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n27 = !hxc_l_tmp_record_field_load_result_n71;
  if (!!hxc_l_tmp_record_field_load_result_n71)
  {
    hxc_l_tmp_short_circuit_result_n27 = hxc_l_tieY.hxc_cellX != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n73 = hxc_l_tmp_short_circuit_result_n27;
  bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_short_circuit_load_result_n73;
  if (!hxc_l_tmp_short_circuit_load_result_n73)
  {
    hxc_l_tmp_short_circuit_result_n28 = hxc_l_tieY.hxc_cellY != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n75 = hxc_l_tmp_short_circuit_result_n28;
  bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_short_circuit_load_result_n75;
  if (!hxc_l_tmp_short_circuit_load_result_n75)
  {
    hxc_l_tmp_short_circuit_result_n29 = hxc_l_tieY.hxc_cellZ != 7;
  }
  bool hxc_l_tmp_short_circuit_load_result_n77 = hxc_l_tmp_short_circuit_result_n29;
  bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_short_circuit_load_result_n77;
  if (!hxc_l_tmp_short_circuit_load_result_n77)
  {
    hxc_l_tmp_short_circuit_result_n30 = hxc_l_tieY.hxc_normalY != -1;
  }
  bool hxc_l_tmp_short_circuit_load_result_n79 = hxc_l_tmp_short_circuit_result_n30;
  bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_short_circuit_load_result_n79;
  if (!hxc_l_tmp_short_circuit_load_result_n79)
  {
    hxc_l_tmp_short_circuit_result_n31 = hxc_l_tieY.hxc_visited != 3;
  }
  if (hxc_l_tmp_short_circuit_result_n31)
  {
    return 19;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n82 = hxc_caxecraft_domain_World_coord(8, 8, 7);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n82, hxc_caxecraft_domain_BlockKind_Air);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n84 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n84, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n86 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tieZ = hxc_l_tmp_call_result_n86;
  bool hxc_l_tmp_record_field_load_result_n87 = hxc_l_tieZ.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n33 = !hxc_l_tmp_record_field_load_result_n87;
  if (!!hxc_l_tmp_record_field_load_result_n87)
  {
    hxc_l_tmp_short_circuit_result_n33 = hxc_l_tieZ.hxc_cellX != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n89 = hxc_l_tmp_short_circuit_result_n33;
  bool hxc_l_tmp_short_circuit_result_n34 = hxc_l_tmp_short_circuit_load_result_n89;
  if (!hxc_l_tmp_short_circuit_load_result_n89)
  {
    hxc_l_tmp_short_circuit_result_n34 = hxc_l_tieZ.hxc_cellY != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n91 = hxc_l_tmp_short_circuit_result_n34;
  bool hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_short_circuit_load_result_n91;
  if (!hxc_l_tmp_short_circuit_load_result_n91)
  {
    hxc_l_tmp_short_circuit_result_n35 = hxc_l_tieZ.hxc_cellZ != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n93 = hxc_l_tmp_short_circuit_result_n35;
  bool hxc_l_tmp_short_circuit_result_n36 = hxc_l_tmp_short_circuit_load_result_n93;
  if (!hxc_l_tmp_short_circuit_load_result_n93)
  {
    hxc_l_tmp_short_circuit_result_n36 = hxc_l_tieZ.hxc_normalZ != -1;
  }
  bool hxc_l_tmp_short_circuit_load_result_n95 = hxc_l_tmp_short_circuit_result_n36;
  bool hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_short_circuit_load_result_n95;
  if (!hxc_l_tmp_short_circuit_load_result_n95)
  {
    hxc_l_tmp_short_circuit_result_n37 = hxc_l_tieZ.hxc_visited != 4;
  }
  if (hxc_l_tmp_short_circuit_result_n37)
  {
    return 23;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n98 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 1.5, 1.5, 1.5, 0.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_zeroDirection = hxc_l_tmp_call_result_n98;
  bool hxc_l_tmp_record_field_load_result_n99 = hxc_l_zeroDirection.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n39 = hxc_l_tmp_record_field_load_result_n99;
  if (!hxc_l_tmp_record_field_load_result_n99)
  {
    hxc_l_tmp_short_circuit_result_n39 = hxc_l_zeroDirection.hxc_visited != 1;
  }
  if (hxc_l_tmp_short_circuit_result_n39)
  {
    return 13;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n102 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 9.0, 8.5, 8.5, -1.0, 0.0, 0.0, 2.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_exactBoundary = hxc_l_tmp_call_result_n102;
  bool hxc_l_tmp_record_field_load_result_n103 = hxc_l_exactBoundary.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n41 = !hxc_l_tmp_record_field_load_result_n103;
  if (!!hxc_l_tmp_record_field_load_result_n103)
  {
    hxc_l_tmp_short_circuit_result_n41 = hxc_l_exactBoundary.hxc_cellX != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n105 = hxc_l_tmp_short_circuit_result_n41;
  bool hxc_l_tmp_short_circuit_result_n42 = hxc_l_tmp_short_circuit_load_result_n105;
  if (!hxc_l_tmp_short_circuit_load_result_n105)
  {
    bool hxc_l_tmp_call_result_n107 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_exactBoundary.hxc_distance, 0.0);
    hxc_l_tmp_short_circuit_result_n42 = !hxc_l_tmp_call_result_n107;
  }
  if (hxc_l_tmp_short_circuit_result_n42)
  {
    return 14;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n109 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 8.5, 8.5, 8.5, 0.0, 1.0, 0.0, 2.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_inside = hxc_l_tmp_call_result_n109;
  bool hxc_l_tmp_record_field_load_result_n110 = hxc_l_inside.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n44 = !hxc_l_tmp_record_field_load_result_n110;
  if (!!hxc_l_tmp_record_field_load_result_n110)
  {
    hxc_l_tmp_short_circuit_result_n44 = hxc_l_inside.hxc_previousX != 8;
  }
  bool hxc_l_tmp_short_circuit_load_result_n112 = hxc_l_tmp_short_circuit_result_n44;
  bool hxc_l_tmp_short_circuit_result_n45 = hxc_l_tmp_short_circuit_load_result_n112;
  if (!hxc_l_tmp_short_circuit_load_result_n112)
  {
    bool hxc_l_tmp_call_result_n114 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_inside.hxc_distance, 0.0);
    hxc_l_tmp_short_circuit_result_n45 = !hxc_l_tmp_call_result_n114;
  }
  bool hxc_l_tmp_short_circuit_load_result_n115 = hxc_l_tmp_short_circuit_result_n45;
  bool hxc_l_tmp_short_circuit_result_n46 = hxc_l_tmp_short_circuit_load_result_n115;
  if (!hxc_l_tmp_short_circuit_load_result_n115)
  {
    hxc_l_tmp_short_circuit_result_n46 = hxc_l_inside.hxc_visited != 1;
  }
  if (hxc_l_tmp_short_circuit_result_n46)
  {
    return 15;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n118 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 5.49);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tooShort = hxc_l_tmp_call_result_n118;
  bool hxc_l_tmp_record_field_load_result_n119 = hxc_l_tooShort.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n48 = hxc_l_tmp_record_field_load_result_n119;
  if (!hxc_l_tmp_record_field_load_result_n119)
  {
    hxc_l_tmp_short_circuit_result_n48 = hxc_l_tooShort.hxc_previousX != 7;
  }
  if (hxc_l_tmp_short_circuit_result_n48)
  {
    return 16;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n122 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 62.5, 14.5, 30.5, 1.0, 0.0, 0.0, 8.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_worldEdge = hxc_l_tmp_call_result_n122;
  bool hxc_l_tmp_record_field_load_result_n123 = hxc_l_worldEdge.hxc_hit;
  bool hxc_l_tmp_short_circuit_result_n50 = hxc_l_tmp_record_field_load_result_n123;
  if (!hxc_l_tmp_record_field_load_result_n123)
  {
    hxc_l_tmp_short_circuit_result_n50 = hxc_l_worldEdge.hxc_previousX != 63;
  }
  bool hxc_l_tmp_short_circuit_load_result_n125 = hxc_l_tmp_short_circuit_result_n50;
  bool hxc_l_tmp_short_circuit_result_n51 = hxc_l_tmp_short_circuit_load_result_n125;
  if (!hxc_l_tmp_short_circuit_load_result_n125)
  {
    bool hxc_l_tmp_call_result_n127 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_worldEdge.hxc_distance, 1.5);
    hxc_l_tmp_short_circuit_result_n51 = !hxc_l_tmp_call_result_n127;
  }
  if (hxc_l_tmp_short_circuit_result_n51)
  {
    return 17;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n129 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_l_tmp_call_result_n130 = hxc_caxecraft_domain_World_remove(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n129);
  bool hxc_l_tmp_short_circuit_result_n52 = !hxc_l_tmp_call_result_n130;
  if (!!hxc_l_tmp_call_result_n130)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n131 = hxc_caxecraft_domain_World_coord(8, 8, 8);
    bool hxc_l_tmp_call_result_n132 = hxc_caxecraft_domain_World_remove(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n131);
    hxc_l_tmp_short_circuit_result_n52 = hxc_l_tmp_call_result_n132;
  }
  if (hxc_l_tmp_short_circuit_result_n52)
  {
    return 20;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n134 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_l_tmp_call_result_n136 = hxc_caxecraft_domain_World_place(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n134, hxc_caxecraft_domain_BlockKind_Dirt);
  bool hxc_l_tmp_short_circuit_result_n53 = !hxc_l_tmp_call_result_n136;
  if (!!hxc_l_tmp_call_result_n136)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n137 = hxc_caxecraft_domain_World_coord(8, 8, 8);
    bool hxc_l_tmp_call_result_n139 = hxc_caxecraft_domain_World_place(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n137, hxc_caxecraft_domain_BlockKind_Stone);
    hxc_l_tmp_short_circuit_result_n53 = hxc_l_tmp_call_result_n139;
  }
  if (hxc_l_tmp_short_circuit_result_n53)
  {
    return 21;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n141 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n141, hxc_caxecraft_domain_BlockKind_Bedrock);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n143 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_l_tmp_call_result_n144 = hxc_caxecraft_domain_World_remove(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n143);
  if (hxc_l_tmp_call_result_n144)
  {
    return 22;
  }
  hxc_caxecraft_qa_DomainProbe_clear(hxc_l_cells, hxc_l_tmp_length_n1);
  hxc_caxecraft_domain_CaxecraftTrace_makeFloor(hxc_l_cells, hxc_l_tmp_length_n1);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n145 = hxc_caxecraft_domain_CharacterPhysics_body(5.5, 1.0, 5.5);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_player = hxc_l_tmp_call_result_n145;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n146 = hxc_l_player;
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n147 = hxc_caxecraft_domain_World_coord(5, 1, 5);
  bool hxc_l_tmp_call_result_n148 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_load_result_n146, hxc_l_tmp_call_result_n147);
  if (hxc_l_tmp_call_result_n148)
  {
    return 37;
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n149 = hxc_l_player;
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n150 = hxc_caxecraft_domain_World_coord(7, 1, 5);
  bool hxc_l_tmp_call_result_n151 = hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(hxc_l_tmp_load_result_n149, hxc_l_tmp_call_result_n150);
  if (!hxc_l_tmp_call_result_n151)
  {
    return 39;
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n152 = hxc_l_player;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n153 = hxc_caxecraft_domain_CharacterPhysics_input(0.0, 0.0, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n154 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n152, hxc_l_tmp_call_result_n153);
  hxc_l_player = hxc_l_tmp_call_result_n154;
  bool hxc_l_tmp_record_field_load_result_n155 = hxc_l_player.hxc_grounded;
  bool hxc_l_tmp_short_circuit_result_n55 = !hxc_l_tmp_record_field_load_result_n155;
  if (!!hxc_l_tmp_record_field_load_result_n155)
  {
    bool hxc_l_tmp_call_result_n157 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_player.hxc_y, 1.0);
    hxc_l_tmp_short_circuit_result_n55 = !hxc_l_tmp_call_result_n157;
  }
  bool hxc_l_tmp_short_circuit_load_result_n158 = hxc_l_tmp_short_circuit_result_n55;
  bool hxc_l_tmp_short_circuit_result_n56 = hxc_l_tmp_short_circuit_load_result_n158;
  if (!hxc_l_tmp_short_circuit_load_result_n158)
  {
    bool hxc_l_tmp_call_result_n160 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_player.hxc_velocityY, 0.0);
    hxc_l_tmp_short_circuit_result_n56 = !hxc_l_tmp_call_result_n160;
  }
  if (hxc_l_tmp_short_circuit_result_n56)
  {
    return 30;
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n162 = hxc_l_player;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n163 = hxc_caxecraft_domain_CharacterPhysics_input(0.0, 0.0, true);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n164 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n162, hxc_l_tmp_call_result_n163);
  hxc_l_player = hxc_l_tmp_call_result_n164;
  bool hxc_l_tmp_record_field_load_result_n165 = hxc_l_player.hxc_grounded;
  bool hxc_l_tmp_short_circuit_result_n57 = hxc_l_tmp_record_field_load_result_n165;
  if (!hxc_l_tmp_record_field_load_result_n165)
  {
    hxc_l_tmp_short_circuit_result_n57 = hxc_l_player.hxc_velocityY <= 0.0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n167 = hxc_l_tmp_short_circuit_result_n57;
  bool hxc_l_tmp_short_circuit_result_n58 = hxc_l_tmp_short_circuit_load_result_n167;
  if (!hxc_l_tmp_short_circuit_load_result_n167)
  {
    hxc_l_tmp_short_circuit_result_n58 = hxc_l_player.hxc_y <= 1.0;
  }
  if (hxc_l_tmp_short_circuit_result_n58)
  {
    return 31;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n170 = hxc_caxecraft_domain_World_coord(6, 1, 5);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n170, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n172 = hxc_caxecraft_domain_World_coord(6, 2, 5);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n172, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n174 = hxc_caxecraft_domain_CharacterPhysics_body(5.7, 1.0, 5.2);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_slider = hxc_l_tmp_call_result_n174;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n175 = hxc_l_slider;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n176 = hxc_caxecraft_domain_CharacterPhysics_input(1.0, 1.0, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n177 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n175, hxc_l_tmp_call_result_n176);
  hxc_l_slider = hxc_l_tmp_call_result_n177;
  bool hxc_l_tmp_call_result_n179 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_slider.hxc_x, 5.7);
  bool hxc_l_tmp_short_circuit_result_n60 = !hxc_l_tmp_call_result_n179;
  if (!!hxc_l_tmp_call_result_n179)
  {
    hxc_l_tmp_short_circuit_result_n60 = hxc_l_slider.hxc_z <= 5.2;
  }
  bool hxc_l_tmp_short_circuit_load_result_n181 = hxc_l_tmp_short_circuit_result_n60;
  bool hxc_l_tmp_short_circuit_result_n61 = hxc_l_tmp_short_circuit_load_result_n181;
  if (!hxc_l_tmp_short_circuit_load_result_n181)
  {
    bool hxc_l_tmp_call_result_n183 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_slider.hxc_velocityX, 0.0);
    hxc_l_tmp_short_circuit_result_n61 = !hxc_l_tmp_call_result_n183;
  }
  bool hxc_l_tmp_short_circuit_load_result_n184 = hxc_l_tmp_short_circuit_result_n61;
  bool hxc_l_tmp_short_circuit_result_n62 = hxc_l_tmp_short_circuit_load_result_n184;
  if (!hxc_l_tmp_short_circuit_load_result_n184)
  {
    hxc_l_tmp_short_circuit_result_n62 = hxc_l_slider.hxc_velocityZ <= 0.0;
  }
  if (hxc_l_tmp_short_circuit_result_n62)
  {
    return 32;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n187 = hxc_caxecraft_domain_World_coord(5, 1, 6);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n187, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n189 = hxc_caxecraft_domain_World_coord(5, 2, 6);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n189, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n191 = hxc_caxecraft_domain_CharacterPhysics_body(5.7, 1.0, 5.7);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_corner = hxc_l_tmp_call_result_n191;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n192 = hxc_l_corner;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n193 = hxc_caxecraft_domain_CharacterPhysics_input(1.0, 1.0, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n194 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n192, hxc_l_tmp_call_result_n193);
  hxc_l_corner = hxc_l_tmp_call_result_n194;
  bool hxc_l_tmp_call_result_n196 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_corner.hxc_x, 5.7);
  bool hxc_l_tmp_short_circuit_result_n64 = !hxc_l_tmp_call_result_n196;
  if (!!hxc_l_tmp_call_result_n196)
  {
    bool hxc_l_tmp_call_result_n198 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_corner.hxc_z, 5.7);
    hxc_l_tmp_short_circuit_result_n64 = !hxc_l_tmp_call_result_n198;
  }
  bool hxc_l_tmp_short_circuit_load_result_n199 = hxc_l_tmp_short_circuit_result_n64;
  bool hxc_l_tmp_short_circuit_result_n65 = hxc_l_tmp_short_circuit_load_result_n199;
  if (!hxc_l_tmp_short_circuit_load_result_n199)
  {
    bool hxc_l_tmp_call_result_n201 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_corner.hxc_velocityX, 0.0);
    hxc_l_tmp_short_circuit_result_n65 = !hxc_l_tmp_call_result_n201;
  }
  bool hxc_l_tmp_short_circuit_load_result_n202 = hxc_l_tmp_short_circuit_result_n65;
  bool hxc_l_tmp_short_circuit_result_n66 = hxc_l_tmp_short_circuit_load_result_n202;
  if (!hxc_l_tmp_short_circuit_load_result_n202)
  {
    bool hxc_l_tmp_call_result_n204 = hxc_caxecraft_qa_DomainProbe_near(hxc_l_corner.hxc_velocityZ, 0.0);
    hxc_l_tmp_short_circuit_result_n66 = !hxc_l_tmp_call_result_n204;
  }
  if (hxc_l_tmp_short_circuit_result_n66)
  {
    return 38;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n206 = hxc_caxecraft_domain_World_coord(4, 3, 4);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n206, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n208 = hxc_caxecraft_domain_CharacterPhysics_body(4.5, 1.0, 4.5);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_ceiling = hxc_l_tmp_call_result_n208;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n209 = hxc_l_ceiling;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n210 = hxc_caxecraft_domain_CharacterPhysics_input(0.0, 0.0, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n211 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n209, hxc_l_tmp_call_result_n210);
  hxc_l_ceiling = hxc_l_tmp_call_result_n211;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n212 = hxc_l_ceiling;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n213 = hxc_caxecraft_domain_CharacterPhysics_input(0.0, 0.0, true);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n214 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n212, hxc_l_tmp_call_result_n213);
  hxc_l_ceiling = hxc_l_tmp_call_result_n214;
  int32_t hxc_l_ceilingTicks = 0;
  while (1)
  {
    if (!(hxc_l_ceilingTicks < 4))
    {
      break;
    }
    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n216 = hxc_l_ceiling;
    struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n217 = hxc_caxecraft_domain_CharacterPhysics_input(0.0, 0.0, false);
    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n218 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n216, hxc_l_tmp_call_result_n217);
    hxc_l_ceiling = hxc_l_tmp_call_result_n218;
    hxc_l_ceilingTicks = hxc_i32_add_wrapping(hxc_l_ceilingTicks, 1);
  }
  double hxc_l_tmp_record_field_load_result_n220 = hxc_l_ceiling.hxc_y;
  bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_record_field_load_result_n220 + 1.8 > 3.0001;
  if (!(hxc_l_tmp_record_field_load_result_n220 + 1.8 > 3.0001))
  {
    hxc_l_tmp_short_circuit_result_n69 = hxc_l_ceiling.hxc_velocityY > 0.0;
  }
  if (hxc_l_tmp_short_circuit_result_n69)
  {
    return 33;
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n223 = hxc_caxecraft_domain_CharacterPhysics_body(6.5, 1.0, 5.5);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_embedded = hxc_l_tmp_call_result_n223;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n225 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_embedded);
  hxc_l_embedded = hxc_l_tmp_call_result_n225;
  if (hxc_l_embedded.hxc_y < 3.0)
  {
    return 34;
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n227 = hxc_caxecraft_domain_CharacterPhysics_body(0.31, 1.0, 0.31);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_boundary = hxc_l_tmp_call_result_n227;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n228 = hxc_l_boundary;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n229 = hxc_caxecraft_domain_CharacterPhysics_input(-1.0, -1.0, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n230 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n228, hxc_l_tmp_call_result_n229);
  hxc_l_boundary = hxc_l_tmp_call_result_n230;
  double hxc_l_tmp_record_field_load_result_n231 = hxc_l_boundary.hxc_x;
  bool hxc_l_tmp_short_circuit_result_n72 = hxc_l_tmp_record_field_load_result_n231 < 0.29;
  if (!(hxc_l_tmp_record_field_load_result_n231 < 0.29))
  {
    hxc_l_tmp_short_circuit_result_n72 = hxc_l_boundary.hxc_z < 0.29;
  }
  if (hxc_l_tmp_short_circuit_result_n72)
  {
    return 35;
  }
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n234 = hxc_caxecraft_domain_CharacterPhysics_body(10.5, 1.0, 10.5);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tunnel = hxc_l_tmp_call_result_n234;
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n235 = hxc_caxecraft_domain_World_coord(11, 1, 10);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n235, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n237 = hxc_caxecraft_domain_World_coord(11, 2, 10);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n237, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n239 = hxc_l_tunnel;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n240 = hxc_caxecraft_domain_CharacterPhysics_input(20.0, 0.0, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n241 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n239, hxc_l_tmp_call_result_n240);
  hxc_l_tunnel = hxc_l_tmp_call_result_n241;
  if (!(hxc_l_tunnel.hxc_x > 10.701))
  {
    return 0;
  }
  return 36;
}
