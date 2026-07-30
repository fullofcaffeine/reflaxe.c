#include "hxc/program.h"

bool hxc_caxecraft_domain_World_contains(struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  bool hxc_l_tmp_short_circuit_result_n1 = hxc_l_coord.hxc_x >= 0;
  if (hxc_l_coord.hxc_x >= 0)
  {
    hxc_l_tmp_short_circuit_result_n1 = hxc_l_coord.hxc_x < 32;
  }
  bool hxc_l_tmp_short_circuit_load_result_n2 = hxc_l_tmp_short_circuit_result_n1;
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_tmp_short_circuit_load_result_n2;
  if (hxc_l_tmp_short_circuit_load_result_n2)
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_coord.hxc_y >= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n4 = hxc_l_tmp_short_circuit_result_n2;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_short_circuit_load_result_n4;
  if (hxc_l_tmp_short_circuit_load_result_n4)
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_coord.hxc_y < 16;
  }
  bool hxc_l_tmp_short_circuit_load_result_n6 = hxc_l_tmp_short_circuit_result_n3;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_short_circuit_load_result_n6;
  if (hxc_l_tmp_short_circuit_load_result_n6)
  {
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_coord.hxc_z >= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n8 = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_short_circuit_load_result_n8;
  if (hxc_l_tmp_short_circuit_load_result_n8)
  {
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_coord.hxc_z < 32;
  }
  return hxc_l_tmp_short_circuit_result_n5;
}

struct hxc_caxecraft_scenario_VoxelPoint hxc_caxecraft_domain_World_coord(int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z)
{
  return (struct hxc_caxecraft_scenario_VoxelPoint){ .hxc_x = hxc_l_x, .hxc_y = hxc_l_y, .hxc_z = hxc_l_z };
}

int32_t hxc_caxecraft_domain_World_indexOf(struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  bool hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_contains(hxc_l_coord);
  if (!!hxc_l_tmp_call_result_n0)
  {
    return hxc_i32_add_wrapping(hxc_l_coord.hxc_x, hxc_i32_multiply_wrapping(32, hxc_i32_add_wrapping(hxc_l_coord.hxc_y, hxc_i32_multiply_wrapping(16, hxc_l_coord.hxc_z))));
  }
  return -1;
}

bool hxc_caxecraft_domain_World_isPlaceable(enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  switch (hxc_l_kind) {
    case hxc_caxecraft_domain_BlockKind_Grass:
    case hxc_caxecraft_domain_BlockKind_Dirt:
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
    case hxc_caxecraft_domain_BlockKind_Sand:
    case hxc_caxecraft_domain_BlockKind_Wood:
    case hxc_caxecraft_domain_BlockKind_Leaves:
    case hxc_caxecraft_domain_BlockKind_Snow:
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        return false;
      }
    default:
      {
        abort();
      }
  }
  return true;
}

bool hxc_caxecraft_domain_World_isSolid(enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  switch (hxc_l_kind) {
    case hxc_caxecraft_domain_BlockKind_Air:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Grass:
    case hxc_caxecraft_domain_BlockKind_Dirt:
    case hxc_caxecraft_domain_BlockKind_Stone:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
    case hxc_caxecraft_domain_BlockKind_Sand:
    case hxc_caxecraft_domain_BlockKind_Wood:
    case hxc_caxecraft_domain_BlockKind_Leaves:
    case hxc_caxecraft_domain_BlockKind_Snow:
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        return true;
      }
    default:
      {
        abort();
      }
  }
  return false;
}

int32_t hxc_caxecraft_domain_World_kindCode(enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  switch (hxc_l_kind) {
    case hxc_caxecraft_domain_BlockKind_Air:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        return 1;
      }
    case hxc_caxecraft_domain_BlockKind_Dirt:
      {
        return 2;
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        return 3;
      }
    case hxc_caxecraft_domain_BlockKind_Bedrock:
      {
        return 4;
      }
    case hxc_caxecraft_domain_BlockKind_Sand:
      {
        return 5;
      }
    case hxc_caxecraft_domain_BlockKind_Wood:
      {
        return 6;
      }
    case hxc_caxecraft_domain_BlockKind_Leaves:
      {
        return 7;
      }
    case hxc_caxecraft_domain_BlockKind_Snow:
      {
        return 8;
      }
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        return 9;
      }
    default:
      {
        abort();
      }
  }
  return 0;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_kindFromCode(int32_t hxc_l_code)
{
  switch (hxc_l_code) {
    case 1:
      {
        break;
      }
    case 2:
      {
        return hxc_caxecraft_domain_BlockKind_Dirt;
      }
    case 3:
      {
        return hxc_caxecraft_domain_BlockKind_Stone;
      }
    case 4:
      {
        return hxc_caxecraft_domain_BlockKind_Bedrock;
      }
    case 5:
      {
        return hxc_caxecraft_domain_BlockKind_Sand;
      }
    case 6:
      {
        return hxc_caxecraft_domain_BlockKind_Wood;
      }
    case 7:
      {
        return hxc_caxecraft_domain_BlockKind_Leaves;
      }
    case 8:
      {
        return hxc_caxecraft_domain_BlockKind_Snow;
      }
    case 9:
      {
        return hxc_caxecraft_domain_BlockKind_Ash;
      }
    default:
      {
        return hxc_caxecraft_domain_BlockKind_Air;
      }
  }
  return hxc_caxecraft_domain_BlockKind_Grass;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_query(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_indexOf(hxc_l_coord);
  int32_t hxc_l_index = hxc_l_tmp_call_result_n0;
  if (!(hxc_l_index < 0))
  {
    int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_WorldStorage_readCode(hxc_l_cells, hxc_l_length, hxc_l_index);
    enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_World_kindFromCode(hxc_l_tmp_call_result_n4);
    return hxc_l_tmp_call_result_n5;
  }
  return hxc_caxecraft_domain_BlockKind_Air;
}

bool hxc_caxecraft_domain_World_remove(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_query(hxc_l_cells, hxc_l_length, hxc_l_coord);
  enum hxc_caxecraft_domain_BlockKind hxc_l_current = hxc_l_tmp_call_result_n0;
  switch (hxc_l_current) {
    case hxc_caxecraft_domain_BlockKind_Grass:
    case hxc_caxecraft_domain_BlockKind_Dirt:
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
    case hxc_caxecraft_domain_BlockKind_Sand:
    case hxc_caxecraft_domain_BlockKind_Wood:
    case hxc_caxecraft_domain_BlockKind_Leaves:
    case hxc_caxecraft_domain_BlockKind_Snow:
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        return false;
      }
    default:
      {
        abort();
      }
  }
  bool hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_coord, hxc_caxecraft_domain_BlockKind_Air);
  return hxc_l_tmp_call_result_n3;
}

bool hxc_caxecraft_domain_World_replace(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_VoxelPoint hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n3;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_indexOf(hxc_l_coord);
  int32_t hxc_l_index = hxc_l_tmp_call_result_n0;
  if (!(hxc_l_index < 0))
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_index;
    int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_World_kindCode(hxc_l_kind);
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_l_cells, hxc_l_length, hxc_l_tmp_load_result_n2, hxc_l_tmp_call_result_n3);
    return true;
  }
  return false;
}
