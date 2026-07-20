#include "hxc/program.h"

void hxc_caxecraft_qa_DomainProbe_clear(uint8_t *hxc_cells, size_t hxc_length)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n1 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n1;
  int32_t hxc_index = 0;
  while (1)
  {
    if (!(hxc_index < 16384))
    {
      break;
    }
    int32_t hxc_tmp_load_result_n1 = hxc_index;
    int32_t hxc_tmp_load_result_n2 = hxc_index;
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n4 = hxc_caxecraft_domain_World_coord(hxc_i32_bit_and(hxc_tmp_load_result_n1, 31), hxc_i32_bit_and(hxc_i32_unsigned_shift_right_masked(hxc_tmp_load_result_n2, 5), 15), hxc_i32_unsigned_shift_right_masked(hxc_index, 9));
    hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_tmp_call_result_n4, hxc_caxecraft_domain_BlockKind_Air);
    hxc_index = hxc_i32_add_wrapping(hxc_index, 1);
  }
  return;
}

void hxc_caxecraft_qa_DomainProbe_main(void)
{
  hxc_caxecraft_qa_DomainProbe_selfCheck();
  hxc_caxecraft_domain_CaxecraftTrace_runTrace();
  return;
}

bool hxc_caxecraft_qa_DomainProbe_near(double hxc_left, double hxc_right)
{
  double hxc_tmp_conditional_result_n3 = 0.0;
  if (hxc_left < hxc_right)
  {
    hxc_tmp_conditional_result_n3 = hxc_right - hxc_left;
  }
  else
  {
    hxc_tmp_conditional_result_n3 = hxc_left - hxc_right;
  }
  double hxc_difference = hxc_tmp_conditional_result_n3;
  return hxc_difference < 0.00001;
}

int32_t hxc_caxecraft_qa_DomainProbe_selfCheck(void)
{
  uint8_t hxc_storage[16384] = { 0 };
  uint8_t *hxc_cells = hxc_storage;
  size_t hxc_tmp_length_n1 = sizeof(hxc_storage) / sizeof(hxc_storage[0]);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(0, 0, 0);
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_domain_World_indexOf(hxc_tmp_call_result_n0);
  if (hxc_tmp_call_result_n1 != 0)
  {
    return 1;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n2 = hxc_caxecraft_domain_World_coord(31, 15, 31);
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_World_indexOf(hxc_tmp_call_result_n2);
  if (hxc_tmp_call_result_n3 != 16383)
  {
    return 2;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n4 = hxc_caxecraft_domain_World_coord(-2147483647, 15, 31);
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_domain_World_indexOf(hxc_tmp_call_result_n4);
  if (hxc_tmp_call_result_n5 != -1)
  {
    return 3;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n6 = hxc_caxecraft_domain_World_coord(32, 0, 0);
  bool hxc_tmp_call_result_n8 = hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n6, hxc_caxecraft_domain_BlockKind_Stone);
  if (hxc_tmp_call_result_n8)
  {
    return 4;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n9 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_tmp_call_result_n11 = hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n9, hxc_caxecraft_domain_BlockKind_Stone);
  if (!hxc_tmp_call_result_n11)
  {
    return 5;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n12 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_axis = hxc_tmp_call_result_n12;
  bool hxc_tmp_record_field_load_result_n13 = hxc_axis.hxc_hit;
  bool hxc_tmp_short_circuit_result_n3 = !hxc_tmp_record_field_load_result_n13;
  if (!!hxc_tmp_record_field_load_result_n13)
  {
    hxc_tmp_short_circuit_result_n3 = hxc_axis.hxc_cellX != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n15 = hxc_tmp_short_circuit_result_n3;
  bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_short_circuit_load_result_n15;
  if (!hxc_tmp_short_circuit_load_result_n15)
  {
    hxc_tmp_short_circuit_result_n4 = hxc_axis.hxc_cellY != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n17 = hxc_tmp_short_circuit_result_n4;
  bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n17;
  if (!hxc_tmp_short_circuit_load_result_n17)
  {
    hxc_tmp_short_circuit_result_n5 = hxc_axis.hxc_cellZ != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n19 = hxc_tmp_short_circuit_result_n5;
  bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n19;
  if (!hxc_tmp_short_circuit_load_result_n19)
  {
    hxc_tmp_short_circuit_result_n6 = hxc_axis.hxc_previousX != 7;
  }
  bool hxc_tmp_short_circuit_load_result_n21 = hxc_tmp_short_circuit_result_n6;
  bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_short_circuit_load_result_n21;
  if (!hxc_tmp_short_circuit_load_result_n21)
  {
    hxc_tmp_short_circuit_result_n7 = hxc_axis.hxc_normalX != -1;
  }
  bool hxc_tmp_short_circuit_load_result_n23 = hxc_tmp_short_circuit_result_n7;
  bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n23;
  if (!hxc_tmp_short_circuit_load_result_n23)
  {
    bool hxc_tmp_call_result_n25 = hxc_caxecraft_qa_DomainProbe_near(hxc_axis.hxc_distance, 5.5);
    hxc_tmp_short_circuit_result_n8 = !hxc_tmp_call_result_n25;
  }
  bool hxc_tmp_short_circuit_load_result_n26 = hxc_tmp_short_circuit_result_n8;
  bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n26;
  if (!hxc_tmp_short_circuit_load_result_n26)
  {
    hxc_tmp_short_circuit_result_n9 = hxc_axis.hxc_visited != 7;
  }
  if (hxc_tmp_short_circuit_result_n9)
  {
    return 10;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n29 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 12.5, 8.5, 8.5, -1.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_negative = hxc_tmp_call_result_n29;
  bool hxc_tmp_record_field_load_result_n30 = hxc_negative.hxc_hit;
  bool hxc_tmp_short_circuit_result_n11 = !hxc_tmp_record_field_load_result_n30;
  if (!!hxc_tmp_record_field_load_result_n30)
  {
    hxc_tmp_short_circuit_result_n11 = hxc_negative.hxc_previousX != 9;
  }
  bool hxc_tmp_short_circuit_load_result_n32 = hxc_tmp_short_circuit_result_n11;
  bool hxc_tmp_short_circuit_result_n12 = hxc_tmp_short_circuit_load_result_n32;
  if (!hxc_tmp_short_circuit_load_result_n32)
  {
    hxc_tmp_short_circuit_result_n12 = hxc_negative.hxc_normalX != 1;
  }
  bool hxc_tmp_short_circuit_load_result_n34 = hxc_tmp_short_circuit_result_n12;
  bool hxc_tmp_short_circuit_result_n13 = hxc_tmp_short_circuit_load_result_n34;
  if (!hxc_tmp_short_circuit_load_result_n34)
  {
    bool hxc_tmp_call_result_n36 = hxc_caxecraft_qa_DomainProbe_near(hxc_negative.hxc_distance, 3.5);
    hxc_tmp_short_circuit_result_n13 = !hxc_tmp_call_result_n36;
  }
  if (hxc_tmp_short_circuit_result_n13)
  {
    return 11;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n38 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n38, hxc_caxecraft_domain_BlockKind_Air);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n40 = hxc_caxecraft_domain_World_coord(10, 10, 10);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n40, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n42 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_diagonal = hxc_tmp_call_result_n42;
  bool hxc_tmp_record_field_load_result_n43 = hxc_diagonal.hxc_hit;
  bool hxc_tmp_short_circuit_result_n15 = !hxc_tmp_record_field_load_result_n43;
  if (!!hxc_tmp_record_field_load_result_n43)
  {
    hxc_tmp_short_circuit_result_n15 = hxc_diagonal.hxc_cellX != 10;
  }
  bool hxc_tmp_short_circuit_load_result_n45 = hxc_tmp_short_circuit_result_n15;
  bool hxc_tmp_short_circuit_result_n16 = hxc_tmp_short_circuit_load_result_n45;
  if (!hxc_tmp_short_circuit_load_result_n45)
  {
    hxc_tmp_short_circuit_result_n16 = hxc_diagonal.hxc_cellY != 10;
  }
  bool hxc_tmp_short_circuit_load_result_n47 = hxc_tmp_short_circuit_result_n16;
  bool hxc_tmp_short_circuit_result_n17 = hxc_tmp_short_circuit_load_result_n47;
  if (!hxc_tmp_short_circuit_load_result_n47)
  {
    hxc_tmp_short_circuit_result_n17 = hxc_diagonal.hxc_cellZ != 10;
  }
  bool hxc_tmp_short_circuit_load_result_n49 = hxc_tmp_short_circuit_result_n17;
  bool hxc_tmp_short_circuit_result_n18 = hxc_tmp_short_circuit_load_result_n49;
  if (!hxc_tmp_short_circuit_load_result_n49)
  {
    hxc_tmp_short_circuit_result_n18 = hxc_diagonal.hxc_visited != 10;
  }
  if (hxc_tmp_short_circuit_result_n18)
  {
    return 12;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n52 = hxc_caxecraft_domain_World_coord(8, 7, 7);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n52, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n54 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_tieX = hxc_tmp_call_result_n54;
  bool hxc_tmp_record_field_load_result_n55 = hxc_tieX.hxc_hit;
  bool hxc_tmp_short_circuit_result_n20 = !hxc_tmp_record_field_load_result_n55;
  if (!!hxc_tmp_record_field_load_result_n55)
  {
    hxc_tmp_short_circuit_result_n20 = hxc_tieX.hxc_cellX != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n57 = hxc_tmp_short_circuit_result_n20;
  bool hxc_tmp_short_circuit_result_n21 = hxc_tmp_short_circuit_load_result_n57;
  if (!hxc_tmp_short_circuit_load_result_n57)
  {
    hxc_tmp_short_circuit_result_n21 = hxc_tieX.hxc_cellY != 7;
  }
  bool hxc_tmp_short_circuit_load_result_n59 = hxc_tmp_short_circuit_result_n21;
  bool hxc_tmp_short_circuit_result_n22 = hxc_tmp_short_circuit_load_result_n59;
  if (!hxc_tmp_short_circuit_load_result_n59)
  {
    hxc_tmp_short_circuit_result_n22 = hxc_tieX.hxc_cellZ != 7;
  }
  bool hxc_tmp_short_circuit_load_result_n61 = hxc_tmp_short_circuit_result_n22;
  bool hxc_tmp_short_circuit_result_n23 = hxc_tmp_short_circuit_load_result_n61;
  if (!hxc_tmp_short_circuit_load_result_n61)
  {
    hxc_tmp_short_circuit_result_n23 = hxc_tieX.hxc_normalX != -1;
  }
  bool hxc_tmp_short_circuit_load_result_n63 = hxc_tmp_short_circuit_result_n23;
  bool hxc_tmp_short_circuit_result_n24 = hxc_tmp_short_circuit_load_result_n63;
  if (!hxc_tmp_short_circuit_load_result_n63)
  {
    hxc_tmp_short_circuit_result_n24 = hxc_tieX.hxc_visited != 2;
  }
  if (hxc_tmp_short_circuit_result_n24)
  {
    return 18;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n66 = hxc_caxecraft_domain_World_coord(8, 7, 7);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n66, hxc_caxecraft_domain_BlockKind_Air);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n68 = hxc_caxecraft_domain_World_coord(8, 8, 7);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n68, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n70 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_tieY = hxc_tmp_call_result_n70;
  bool hxc_tmp_record_field_load_result_n71 = hxc_tieY.hxc_hit;
  bool hxc_tmp_short_circuit_result_n26 = !hxc_tmp_record_field_load_result_n71;
  if (!!hxc_tmp_record_field_load_result_n71)
  {
    hxc_tmp_short_circuit_result_n26 = hxc_tieY.hxc_cellX != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n73 = hxc_tmp_short_circuit_result_n26;
  bool hxc_tmp_short_circuit_result_n27 = hxc_tmp_short_circuit_load_result_n73;
  if (!hxc_tmp_short_circuit_load_result_n73)
  {
    hxc_tmp_short_circuit_result_n27 = hxc_tieY.hxc_cellY != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n75 = hxc_tmp_short_circuit_result_n27;
  bool hxc_tmp_short_circuit_result_n28 = hxc_tmp_short_circuit_load_result_n75;
  if (!hxc_tmp_short_circuit_load_result_n75)
  {
    hxc_tmp_short_circuit_result_n28 = hxc_tieY.hxc_cellZ != 7;
  }
  bool hxc_tmp_short_circuit_load_result_n77 = hxc_tmp_short_circuit_result_n28;
  bool hxc_tmp_short_circuit_result_n29 = hxc_tmp_short_circuit_load_result_n77;
  if (!hxc_tmp_short_circuit_load_result_n77)
  {
    hxc_tmp_short_circuit_result_n29 = hxc_tieY.hxc_normalY != -1;
  }
  bool hxc_tmp_short_circuit_load_result_n79 = hxc_tmp_short_circuit_result_n29;
  bool hxc_tmp_short_circuit_result_n30 = hxc_tmp_short_circuit_load_result_n79;
  if (!hxc_tmp_short_circuit_load_result_n79)
  {
    hxc_tmp_short_circuit_result_n30 = hxc_tieY.hxc_visited != 3;
  }
  if (hxc_tmp_short_circuit_result_n30)
  {
    return 19;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n82 = hxc_caxecraft_domain_World_coord(8, 8, 7);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n82, hxc_caxecraft_domain_BlockKind_Air);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n84 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n84, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n86 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_tieZ = hxc_tmp_call_result_n86;
  bool hxc_tmp_record_field_load_result_n87 = hxc_tieZ.hxc_hit;
  bool hxc_tmp_short_circuit_result_n32 = !hxc_tmp_record_field_load_result_n87;
  if (!!hxc_tmp_record_field_load_result_n87)
  {
    hxc_tmp_short_circuit_result_n32 = hxc_tieZ.hxc_cellX != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n89 = hxc_tmp_short_circuit_result_n32;
  bool hxc_tmp_short_circuit_result_n33 = hxc_tmp_short_circuit_load_result_n89;
  if (!hxc_tmp_short_circuit_load_result_n89)
  {
    hxc_tmp_short_circuit_result_n33 = hxc_tieZ.hxc_cellY != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n91 = hxc_tmp_short_circuit_result_n33;
  bool hxc_tmp_short_circuit_result_n34 = hxc_tmp_short_circuit_load_result_n91;
  if (!hxc_tmp_short_circuit_load_result_n91)
  {
    hxc_tmp_short_circuit_result_n34 = hxc_tieZ.hxc_cellZ != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n93 = hxc_tmp_short_circuit_result_n34;
  bool hxc_tmp_short_circuit_result_n35 = hxc_tmp_short_circuit_load_result_n93;
  if (!hxc_tmp_short_circuit_load_result_n93)
  {
    hxc_tmp_short_circuit_result_n35 = hxc_tieZ.hxc_normalZ != -1;
  }
  bool hxc_tmp_short_circuit_load_result_n95 = hxc_tmp_short_circuit_result_n35;
  bool hxc_tmp_short_circuit_result_n36 = hxc_tmp_short_circuit_load_result_n95;
  if (!hxc_tmp_short_circuit_load_result_n95)
  {
    hxc_tmp_short_circuit_result_n36 = hxc_tieZ.hxc_visited != 4;
  }
  if (hxc_tmp_short_circuit_result_n36)
  {
    return 23;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n98 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 1.5, 1.5, 1.5, 0.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_zeroDirection = hxc_tmp_call_result_n98;
  bool hxc_tmp_record_field_load_result_n99 = hxc_zeroDirection.hxc_hit;
  bool hxc_tmp_short_circuit_result_n38 = hxc_tmp_record_field_load_result_n99;
  if (!hxc_tmp_record_field_load_result_n99)
  {
    hxc_tmp_short_circuit_result_n38 = hxc_zeroDirection.hxc_visited != 1;
  }
  if (hxc_tmp_short_circuit_result_n38)
  {
    return 13;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n102 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 9.0, 8.5, 8.5, -1.0, 0.0, 0.0, 2.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_exactBoundary = hxc_tmp_call_result_n102;
  bool hxc_tmp_record_field_load_result_n103 = hxc_exactBoundary.hxc_hit;
  bool hxc_tmp_short_circuit_result_n40 = !hxc_tmp_record_field_load_result_n103;
  if (!!hxc_tmp_record_field_load_result_n103)
  {
    hxc_tmp_short_circuit_result_n40 = hxc_exactBoundary.hxc_cellX != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n105 = hxc_tmp_short_circuit_result_n40;
  bool hxc_tmp_short_circuit_result_n41 = hxc_tmp_short_circuit_load_result_n105;
  if (!hxc_tmp_short_circuit_load_result_n105)
  {
    bool hxc_tmp_call_result_n107 = hxc_caxecraft_qa_DomainProbe_near(hxc_exactBoundary.hxc_distance, 0.0);
    hxc_tmp_short_circuit_result_n41 = !hxc_tmp_call_result_n107;
  }
  if (hxc_tmp_short_circuit_result_n41)
  {
    return 14;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n109 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 8.5, 8.5, 8.5, 0.0, 1.0, 0.0, 2.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_inside = hxc_tmp_call_result_n109;
  bool hxc_tmp_record_field_load_result_n110 = hxc_inside.hxc_hit;
  bool hxc_tmp_short_circuit_result_n43 = !hxc_tmp_record_field_load_result_n110;
  if (!!hxc_tmp_record_field_load_result_n110)
  {
    hxc_tmp_short_circuit_result_n43 = hxc_inside.hxc_previousX != 8;
  }
  bool hxc_tmp_short_circuit_load_result_n112 = hxc_tmp_short_circuit_result_n43;
  bool hxc_tmp_short_circuit_result_n44 = hxc_tmp_short_circuit_load_result_n112;
  if (!hxc_tmp_short_circuit_load_result_n112)
  {
    bool hxc_tmp_call_result_n114 = hxc_caxecraft_qa_DomainProbe_near(hxc_inside.hxc_distance, 0.0);
    hxc_tmp_short_circuit_result_n44 = !hxc_tmp_call_result_n114;
  }
  bool hxc_tmp_short_circuit_load_result_n115 = hxc_tmp_short_circuit_result_n44;
  bool hxc_tmp_short_circuit_result_n45 = hxc_tmp_short_circuit_load_result_n115;
  if (!hxc_tmp_short_circuit_load_result_n115)
  {
    hxc_tmp_short_circuit_result_n45 = hxc_inside.hxc_visited != 1;
  }
  if (hxc_tmp_short_circuit_result_n45)
  {
    return 15;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n118 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 5.49);
  struct hxc_caxecraft_domain_RaycastHit hxc_tooShort = hxc_tmp_call_result_n118;
  bool hxc_tmp_record_field_load_result_n119 = hxc_tooShort.hxc_hit;
  bool hxc_tmp_short_circuit_result_n47 = hxc_tmp_record_field_load_result_n119;
  if (!hxc_tmp_record_field_load_result_n119)
  {
    hxc_tmp_short_circuit_result_n47 = hxc_tooShort.hxc_previousX != 7;
  }
  if (hxc_tmp_short_circuit_result_n47)
  {
    return 16;
  }
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n122 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 30.5, 14.5, 30.5, 1.0, 0.0, 0.0, 8.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_worldEdge = hxc_tmp_call_result_n122;
  bool hxc_tmp_record_field_load_result_n123 = hxc_worldEdge.hxc_hit;
  bool hxc_tmp_short_circuit_result_n49 = hxc_tmp_record_field_load_result_n123;
  if (!hxc_tmp_record_field_load_result_n123)
  {
    hxc_tmp_short_circuit_result_n49 = hxc_worldEdge.hxc_previousX != 31;
  }
  bool hxc_tmp_short_circuit_load_result_n125 = hxc_tmp_short_circuit_result_n49;
  bool hxc_tmp_short_circuit_result_n50 = hxc_tmp_short_circuit_load_result_n125;
  if (!hxc_tmp_short_circuit_load_result_n125)
  {
    bool hxc_tmp_call_result_n127 = hxc_caxecraft_qa_DomainProbe_near(hxc_worldEdge.hxc_distance, 1.5);
    hxc_tmp_short_circuit_result_n50 = !hxc_tmp_call_result_n127;
  }
  if (hxc_tmp_short_circuit_result_n50)
  {
    return 17;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n129 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_tmp_call_result_n130 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n129);
  bool hxc_tmp_short_circuit_result_n51 = !hxc_tmp_call_result_n130;
  if (!!hxc_tmp_call_result_n130)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n131 = hxc_caxecraft_domain_World_coord(8, 8, 8);
    bool hxc_tmp_call_result_n132 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n131);
    hxc_tmp_short_circuit_result_n51 = hxc_tmp_call_result_n132;
  }
  if (hxc_tmp_short_circuit_result_n51)
  {
    return 20;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n134 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_tmp_call_result_n136 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n134, hxc_caxecraft_domain_BlockKind_Dirt);
  bool hxc_tmp_short_circuit_result_n52 = !hxc_tmp_call_result_n136;
  if (!!hxc_tmp_call_result_n136)
  {
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n137 = hxc_caxecraft_domain_World_coord(8, 8, 8);
    bool hxc_tmp_call_result_n139 = hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n137, hxc_caxecraft_domain_BlockKind_Stone);
    hxc_tmp_short_circuit_result_n52 = hxc_tmp_call_result_n139;
  }
  if (hxc_tmp_short_circuit_result_n52)
  {
    return 21;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n141 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n141, hxc_caxecraft_domain_BlockKind_Bedrock);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n143 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  bool hxc_tmp_call_result_n144 = hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n143);
  if (hxc_tmp_call_result_n144)
  {
    return 22;
  }
  hxc_caxecraft_qa_DomainProbe_clear(hxc_cells, hxc_tmp_length_n1);
  hxc_caxecraft_domain_CaxecraftTrace_makeFloor(hxc_cells, hxc_tmp_length_n1);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n145 = hxc_caxecraft_domain_PlayerPhysics_player(5.5, 1.0, 5.5);
  struct hxc_caxecraft_domain_PlayerState hxc_player = hxc_tmp_call_result_n145;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n146 = hxc_player;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n147 = hxc_caxecraft_domain_PlayerPhysics_input(0.0, 0.0, false);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n148 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n146, hxc_tmp_call_result_n147);
  hxc_player = hxc_tmp_call_result_n148;
  bool hxc_tmp_record_field_load_result_n149 = hxc_player.hxc_grounded;
  bool hxc_tmp_short_circuit_result_n54 = !hxc_tmp_record_field_load_result_n149;
  if (!!hxc_tmp_record_field_load_result_n149)
  {
    bool hxc_tmp_call_result_n151 = hxc_caxecraft_qa_DomainProbe_near(hxc_player.hxc_y, 1.0);
    hxc_tmp_short_circuit_result_n54 = !hxc_tmp_call_result_n151;
  }
  bool hxc_tmp_short_circuit_load_result_n152 = hxc_tmp_short_circuit_result_n54;
  bool hxc_tmp_short_circuit_result_n55 = hxc_tmp_short_circuit_load_result_n152;
  if (!hxc_tmp_short_circuit_load_result_n152)
  {
    bool hxc_tmp_call_result_n154 = hxc_caxecraft_qa_DomainProbe_near(hxc_player.hxc_velocityY, 0.0);
    hxc_tmp_short_circuit_result_n55 = !hxc_tmp_call_result_n154;
  }
  if (hxc_tmp_short_circuit_result_n55)
  {
    return 30;
  }
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n156 = hxc_player;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n157 = hxc_caxecraft_domain_PlayerPhysics_input(0.0, 0.0, true);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n158 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n156, hxc_tmp_call_result_n157);
  hxc_player = hxc_tmp_call_result_n158;
  bool hxc_tmp_record_field_load_result_n159 = hxc_player.hxc_grounded;
  bool hxc_tmp_short_circuit_result_n56 = hxc_tmp_record_field_load_result_n159;
  if (!hxc_tmp_record_field_load_result_n159)
  {
    hxc_tmp_short_circuit_result_n56 = hxc_player.hxc_velocityY <= 0.0;
  }
  bool hxc_tmp_short_circuit_load_result_n161 = hxc_tmp_short_circuit_result_n56;
  bool hxc_tmp_short_circuit_result_n57 = hxc_tmp_short_circuit_load_result_n161;
  if (!hxc_tmp_short_circuit_load_result_n161)
  {
    hxc_tmp_short_circuit_result_n57 = hxc_player.hxc_y <= 1.0;
  }
  if (hxc_tmp_short_circuit_result_n57)
  {
    return 31;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n164 = hxc_caxecraft_domain_World_coord(6, 1, 5);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n164, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n166 = hxc_caxecraft_domain_World_coord(6, 2, 5);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n166, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n168 = hxc_caxecraft_domain_PlayerPhysics_player(5.7, 1.0, 5.2);
  struct hxc_caxecraft_domain_PlayerState hxc_slider = hxc_tmp_call_result_n168;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n169 = hxc_slider;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n170 = hxc_caxecraft_domain_PlayerPhysics_input(1.0, 1.0, false);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n171 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n169, hxc_tmp_call_result_n170);
  hxc_slider = hxc_tmp_call_result_n171;
  bool hxc_tmp_call_result_n173 = hxc_caxecraft_qa_DomainProbe_near(hxc_slider.hxc_x, 5.7);
  bool hxc_tmp_short_circuit_result_n59 = !hxc_tmp_call_result_n173;
  if (!!hxc_tmp_call_result_n173)
  {
    hxc_tmp_short_circuit_result_n59 = hxc_slider.hxc_z <= 5.2;
  }
  bool hxc_tmp_short_circuit_load_result_n175 = hxc_tmp_short_circuit_result_n59;
  bool hxc_tmp_short_circuit_result_n60 = hxc_tmp_short_circuit_load_result_n175;
  if (!hxc_tmp_short_circuit_load_result_n175)
  {
    bool hxc_tmp_call_result_n177 = hxc_caxecraft_qa_DomainProbe_near(hxc_slider.hxc_velocityX, 0.0);
    hxc_tmp_short_circuit_result_n60 = !hxc_tmp_call_result_n177;
  }
  bool hxc_tmp_short_circuit_load_result_n178 = hxc_tmp_short_circuit_result_n60;
  bool hxc_tmp_short_circuit_result_n61 = hxc_tmp_short_circuit_load_result_n178;
  if (!hxc_tmp_short_circuit_load_result_n178)
  {
    hxc_tmp_short_circuit_result_n61 = hxc_slider.hxc_velocityZ <= 0.0;
  }
  if (hxc_tmp_short_circuit_result_n61)
  {
    return 32;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n181 = hxc_caxecraft_domain_World_coord(5, 1, 6);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n181, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n183 = hxc_caxecraft_domain_World_coord(5, 2, 6);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n183, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n185 = hxc_caxecraft_domain_PlayerPhysics_player(5.7, 1.0, 5.7);
  struct hxc_caxecraft_domain_PlayerState hxc_corner = hxc_tmp_call_result_n185;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n186 = hxc_corner;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n187 = hxc_caxecraft_domain_PlayerPhysics_input(1.0, 1.0, false);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n188 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n186, hxc_tmp_call_result_n187);
  hxc_corner = hxc_tmp_call_result_n188;
  bool hxc_tmp_call_result_n190 = hxc_caxecraft_qa_DomainProbe_near(hxc_corner.hxc_x, 5.7);
  bool hxc_tmp_short_circuit_result_n63 = !hxc_tmp_call_result_n190;
  if (!!hxc_tmp_call_result_n190)
  {
    bool hxc_tmp_call_result_n192 = hxc_caxecraft_qa_DomainProbe_near(hxc_corner.hxc_z, 5.7);
    hxc_tmp_short_circuit_result_n63 = !hxc_tmp_call_result_n192;
  }
  bool hxc_tmp_short_circuit_load_result_n193 = hxc_tmp_short_circuit_result_n63;
  bool hxc_tmp_short_circuit_result_n64 = hxc_tmp_short_circuit_load_result_n193;
  if (!hxc_tmp_short_circuit_load_result_n193)
  {
    bool hxc_tmp_call_result_n195 = hxc_caxecraft_qa_DomainProbe_near(hxc_corner.hxc_velocityX, 0.0);
    hxc_tmp_short_circuit_result_n64 = !hxc_tmp_call_result_n195;
  }
  bool hxc_tmp_short_circuit_load_result_n196 = hxc_tmp_short_circuit_result_n64;
  bool hxc_tmp_short_circuit_result_n65 = hxc_tmp_short_circuit_load_result_n196;
  if (!hxc_tmp_short_circuit_load_result_n196)
  {
    bool hxc_tmp_call_result_n198 = hxc_caxecraft_qa_DomainProbe_near(hxc_corner.hxc_velocityZ, 0.0);
    hxc_tmp_short_circuit_result_n65 = !hxc_tmp_call_result_n198;
  }
  if (hxc_tmp_short_circuit_result_n65)
  {
    return 38;
  }
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n200 = hxc_caxecraft_domain_World_coord(4, 3, 4);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n200, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n202 = hxc_caxecraft_domain_PlayerPhysics_player(4.5, 1.0, 4.5);
  struct hxc_caxecraft_domain_PlayerState hxc_ceiling = hxc_tmp_call_result_n202;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n203 = hxc_ceiling;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n204 = hxc_caxecraft_domain_PlayerPhysics_input(0.0, 0.0, false);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n205 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n203, hxc_tmp_call_result_n204);
  hxc_ceiling = hxc_tmp_call_result_n205;
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n206 = hxc_ceiling;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n207 = hxc_caxecraft_domain_PlayerPhysics_input(0.0, 0.0, true);
  struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n208 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n206, hxc_tmp_call_result_n207);
  hxc_ceiling = hxc_tmp_call_result_n208;
  int32_t hxc_ceilingTicks = 0;
  while (1)
  {
    if (!(hxc_ceilingTicks < 4))
    {
      break;
    }
    struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n210 = hxc_ceiling;
    struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n211 = hxc_caxecraft_domain_PlayerPhysics_input(0.0, 0.0, false);
    struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n212 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n210, hxc_tmp_call_result_n211);
    hxc_ceiling = hxc_tmp_call_result_n212;
    hxc_ceilingTicks = hxc_i32_add_wrapping(hxc_ceilingTicks, 1);
  }
  double hxc_tmp_record_field_load_result_n214 = hxc_ceiling.hxc_y;
  bool hxc_tmp_short_circuit_result_n68 = hxc_tmp_record_field_load_result_n214 + 1.8 > 3.0001;
  if (!(hxc_tmp_record_field_load_result_n214 + 1.8 > 3.0001))
  {
    hxc_tmp_short_circuit_result_n68 = hxc_ceiling.hxc_velocityY > 0.0;
  }
  if (!hxc_tmp_short_circuit_result_n68)
  {
    struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n217 = hxc_caxecraft_domain_PlayerPhysics_player(6.5, 1.0, 5.5);
    struct hxc_caxecraft_domain_PlayerState hxc_embedded = hxc_tmp_call_result_n217;
    struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n219 = hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(hxc_cells, hxc_tmp_length_n1, hxc_embedded);
    hxc_embedded = hxc_tmp_call_result_n219;
    if (!(hxc_embedded.hxc_y < 3.0))
    {
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n221 = hxc_caxecraft_domain_PlayerPhysics_player(0.31, 1.0, 0.31);
      struct hxc_caxecraft_domain_PlayerState hxc_boundary = hxc_tmp_call_result_n221;
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n222 = hxc_boundary;
      struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n223 = hxc_caxecraft_domain_PlayerPhysics_input(-1.0, -1.0, false);
      struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n224 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n222, hxc_tmp_call_result_n223);
      hxc_boundary = hxc_tmp_call_result_n224;
      double hxc_tmp_record_field_load_result_n225 = hxc_boundary.hxc_x;
      bool hxc_tmp_short_circuit_result_n71 = hxc_tmp_record_field_load_result_n225 < 0.29;
      if (!(hxc_tmp_record_field_load_result_n225 < 0.29))
      {
        hxc_tmp_short_circuit_result_n71 = hxc_boundary.hxc_z < 0.29;
      }
      if (!hxc_tmp_short_circuit_result_n71)
      {
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n228 = hxc_caxecraft_domain_PlayerPhysics_player(10.5, 1.0, 10.5);
        struct hxc_caxecraft_domain_PlayerState hxc_tunnel = hxc_tmp_call_result_n228;
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n229 = hxc_caxecraft_domain_World_coord(11, 1, 10);
        hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n229, hxc_caxecraft_domain_BlockKind_Stone);
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n231 = hxc_caxecraft_domain_World_coord(11, 2, 10);
        hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n231, hxc_caxecraft_domain_BlockKind_Stone);
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_load_result_n233 = hxc_tunnel;
        struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n234 = hxc_caxecraft_domain_PlayerPhysics_input(20.0, 0.0, false);
        struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n235 = hxc_caxecraft_domain_PlayerPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n233, hxc_tmp_call_result_n234);
        hxc_tunnel = hxc_tmp_call_result_n235;
        if (!(hxc_tunnel.hxc_x > 10.701))
        {
          return 0;
        }
        return 36;
      }
      return 35;
    }
    return 34;
  }
  return 33;
}
