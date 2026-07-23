#include "hxc/program.h"

bool hxc_caxecraft_domain_World_contains(struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  bool hxc_tmp_short_circuit_result_n1 = hxc_coord.hxc_x >= 0;
  if (hxc_coord.hxc_x >= 0)
  {
    hxc_tmp_short_circuit_result_n1 = hxc_coord.hxc_x < 32;
  }
  bool hxc_tmp_short_circuit_load_result_n2 = hxc_tmp_short_circuit_result_n1;
  bool hxc_tmp_short_circuit_result_n2 = hxc_tmp_short_circuit_load_result_n2;
  if (hxc_tmp_short_circuit_load_result_n2)
  {
    hxc_tmp_short_circuit_result_n2 = hxc_coord.hxc_y >= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n4 = hxc_tmp_short_circuit_result_n2;
  bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_short_circuit_load_result_n4;
  if (hxc_tmp_short_circuit_load_result_n4)
  {
    hxc_tmp_short_circuit_result_n3 = hxc_coord.hxc_y < 16;
  }
  bool hxc_tmp_short_circuit_load_result_n6 = hxc_tmp_short_circuit_result_n3;
  bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_short_circuit_load_result_n6;
  if (hxc_tmp_short_circuit_load_result_n6)
  {
    hxc_tmp_short_circuit_result_n4 = hxc_coord.hxc_z >= 0;
  }
  bool hxc_tmp_short_circuit_load_result_n8 = hxc_tmp_short_circuit_result_n4;
  bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n8;
  if (hxc_tmp_short_circuit_load_result_n8)
  {
    hxc_tmp_short_circuit_result_n5 = hxc_coord.hxc_z < 32;
  }
  return hxc_tmp_short_circuit_result_n5;
}

struct hxc_caxecraft_domain_BlockCoord hxc_caxecraft_domain_World_coord(int32_t hxc_x, int32_t hxc_y, int32_t hxc_z)
{
  return (struct hxc_caxecraft_domain_BlockCoord){ .hxc_x = hxc_x, .hxc_y = hxc_y, .hxc_z = hxc_z };
}

int32_t hxc_caxecraft_domain_World_indexOf(struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  bool hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_contains(hxc_coord);
  if (!!hxc_tmp_call_result_n0)
  {
    return hxc_i32_add_wrapping(hxc_coord.hxc_x, hxc_i32_multiply_wrapping(32, hxc_i32_add_wrapping(hxc_coord.hxc_y, hxc_i32_multiply_wrapping(16, hxc_coord.hxc_z))));
  }
  return -1;
}

bool hxc_caxecraft_domain_World_isPlaceable(enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  switch (hxc_kind) {
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
  }
  return true;
}

bool hxc_caxecraft_domain_World_isSolid(enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  switch (hxc_kind) {
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
  }
  return false;
}

int32_t hxc_caxecraft_domain_World_kindCode(enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  switch (hxc_kind) {
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
  }
  return 0;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_kindFromCode(int32_t hxc_code)
{
  switch (hxc_code) {
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

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_query(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_indexOf(hxc_coord);
  int32_t hxc_index = hxc_tmp_call_result_n0;
  if (!(hxc_index < 0))
  {
    int32_t hxc_tmp_call_result_n4 = hxc_caxecraft_domain_WorldStorage_readCode(hxc_cells, hxc_length, hxc_index);
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n5 = hxc_caxecraft_domain_World_kindFromCode(hxc_tmp_call_result_n4);
    return hxc_tmp_call_result_n5;
  }
  return hxc_caxecraft_domain_BlockKind_Air;
}

bool hxc_caxecraft_domain_World_remove(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_coord);
  enum hxc_caxecraft_domain_BlockKind hxc_current = hxc_tmp_call_result_n0;
  switch (hxc_current) {
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
  }
  bool hxc_tmp_call_result_n3 = hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_coord, hxc_caxecraft_domain_BlockKind_Air);
  return hxc_tmp_call_result_n3;
}

bool hxc_caxecraft_domain_World_replace(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord, enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_indexOf(hxc_coord);
  int32_t hxc_index = hxc_tmp_call_result_n0;
  if (!(hxc_index < 0))
  {
    int32_t hxc_tmp_load_result_n2 = hxc_index;
    int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_World_kindCode(hxc_kind);
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_cells, hxc_length, hxc_tmp_load_result_n2, hxc_tmp_call_result_n3);
    return true;
  }
  return false;
}

int32_t hxc_caxecraft_domain_World_surfaceY(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_x, int32_t hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  bool hxc_tmp_short_circuit_result_n4 = hxc_x < 0;
  if (!(hxc_x < 0))
  {
    hxc_tmp_short_circuit_result_n4 = hxc_x >= 32;
  }
  bool hxc_tmp_short_circuit_load_result_n0 = hxc_tmp_short_circuit_result_n4;
  bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n0;
  if (!hxc_tmp_short_circuit_load_result_n0)
  {
    hxc_tmp_short_circuit_result_n5 = hxc_z < 0;
  }
  bool hxc_tmp_short_circuit_load_result_n1 = hxc_tmp_short_circuit_result_n5;
  bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n1;
  if (!hxc_tmp_short_circuit_load_result_n1)
  {
    hxc_tmp_short_circuit_result_n6 = hxc_z >= 32;
  }
  if (hxc_tmp_short_circuit_result_n6)
  {
    return -1;
  }
  int32_t hxc_y = 15;
  while (1)
  {
    if (!(hxc_y >= 0))
    {
      break;
    }
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n5 = hxc_caxecraft_domain_World_coord(hxc_x, hxc_y, hxc_z);
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n6 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n5);
    bool hxc_tmp_call_result_n7 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n6);
    if (!hxc_tmp_call_result_n7)
    {
      hxc_y = hxc_i32_subtract_wrapping(hxc_y, 1);
      continue;
    }
    return hxc_y;
  }
  return -1;
}
