#include "hxc/program.h"

bool hxc_caxecraft_domain_World_contains(struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
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

struct hxc_caxecraft_domain_BlockCoord hxc_caxecraft_domain_World_coord(int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z)
{
  return (struct hxc_caxecraft_domain_BlockCoord){ .hxc_x = hxc_l_x, .hxc_y = hxc_l_y, .hxc_z = hxc_l_z };
}

void hxc_caxecraft_domain_World_generate(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_seed)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  int32_t hxc_l_index = 0;
  while (1)
  {
    if (!(hxc_l_index < 16384))
    {
      break;
    }
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_l_cells, hxc_l_length, hxc_l_index, 0);
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  int32_t hxc_l_z = 0;
  while (1)
  {
    if (!(hxc_l_z < 32))
    {
      break;
    }
    int32_t hxc_l_x = 0;
    while (1)
    {
      if (!(hxc_l_x < 32))
      {
        break;
      }
      int32_t hxc_l_tmp_load_result_n5 = hxc_l_x;
      int32_t hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_World_terrainNoise(hxc_l_seed, hxc_l_tmp_load_result_n5, hxc_l_z);
      int32_t hxc_l_noise = hxc_i32_bit_and(hxc_l_tmp_call_result_n7, 2147483647);
      int32_t hxc_l_surface = hxc_i32_add_wrapping(3, hxc_i32_modulo_zero_safe(hxc_l_noise, 5));
      int32_t hxc_l_y = 0;
      while (1)
      {
        int32_t hxc_l_tmp_load_result_n9 = hxc_l_y;
        if (!(hxc_l_tmp_load_result_n9 <= hxc_l_surface))
        {
          break;
        }
        if (hxc_l_y == 0)
        {
          int32_t hxc_l_tmp_load_result_n12 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n13 = hxc_l_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n15 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n12, hxc_l_tmp_load_result_n13, hxc_l_z);
          hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n15, hxc_caxecraft_domain_BlockKind_Bedrock);
        }
        else
        {
          int32_t hxc_l_tmp_load_result_n17 = hxc_l_y;
          if (hxc_l_tmp_load_result_n17 == hxc_l_surface)
          {
            int32_t hxc_l_tmp_load_result_n19 = hxc_l_x;
            int32_t hxc_l_tmp_load_result_n20 = hxc_l_y;
            struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n22 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n19, hxc_l_tmp_load_result_n20, hxc_l_z);
            hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n22, hxc_caxecraft_domain_BlockKind_Grass);
          }
          else
          {
            int32_t hxc_l_tmp_load_result_n24 = hxc_l_y;
            if (hxc_i32_add_wrapping(hxc_l_tmp_load_result_n24, 2) >= hxc_l_surface)
            {
              int32_t hxc_l_tmp_load_result_n26 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n27 = hxc_l_y;
              struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n29 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n26, hxc_l_tmp_load_result_n27, hxc_l_z);
              hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n29, hxc_caxecraft_domain_BlockKind_Dirt);
            }
            else
            {
              int32_t hxc_l_tmp_load_result_n31 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n32 = hxc_l_y;
              struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n34 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n31, hxc_l_tmp_load_result_n32, hxc_l_z);
              hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n34, hxc_caxecraft_domain_BlockKind_Stone);
            }
          }
        }
        hxc_l_y = hxc_i32_add_wrapping(hxc_l_y, 1);
      }
      hxc_l_x = hxc_i32_add_wrapping(hxc_l_x, 1);
    }
    hxc_l_z = hxc_i32_add_wrapping(hxc_l_z, 1);
  }
  return;
}

int32_t hxc_caxecraft_domain_World_indexOf(struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
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

bool hxc_caxecraft_domain_World_place(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n3;
  bool hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_isPlaceable(hxc_l_kind);
  if (!hxc_l_tmp_call_result_n0)
  {
    return false;
  }
  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_World_query(hxc_l_cells, hxc_l_length, hxc_l_coord);
  switch (hxc_l_tmp_call_result_n1) {
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
        return false;
      }
    default:
      {
        abort();
      }
  }
  bool hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_coord, hxc_l_kind);
  return hxc_l_tmp_call_result_n2;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_query(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
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

bool hxc_caxecraft_domain_World_remove(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
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

bool hxc_caxecraft_domain_World_replace(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
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

int32_t hxc_caxecraft_domain_World_stateHash(uint8_t *hxc_l_cells, size_t hxc_l_length)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n1 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n1;
  int32_t hxc_l_hash = -2128831035;
  int32_t hxc_l_index = 0;
  while (1)
  {
    if (!(hxc_l_index < 16384))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_hash;
    int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_WorldStorage_readCode(hxc_l_cells, hxc_l_length, hxc_l_index);
    hxc_l_hash = hxc_i32_multiply_wrapping(hxc_i32_bit_xor(hxc_l_tmp_load_result_n1, hxc_l_tmp_call_result_n3), 16777619);
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  return hxc_l_hash;
}

int32_t hxc_caxecraft_domain_World_terrainNoise(int32_t hxc_l_seed, int32_t hxc_l_x, int32_t hxc_l_z)
{
  int32_t hxc_l_value = hxc_i32_bit_xor(hxc_i32_bit_xor(hxc_l_seed, hxc_i32_multiply_wrapping(hxc_l_x, 374761393)), hxc_i32_multiply_wrapping(hxc_l_z, 668265263));
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_value;
  hxc_l_value = hxc_i32_multiply_wrapping(hxc_i32_bit_xor(hxc_l_tmp_load_result_n0, hxc_i32_unsigned_shift_right_masked(hxc_l_value, 13)), 1274126177);
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_value;
  return hxc_i32_bit_xor(hxc_l_tmp_load_result_n2, hxc_i32_unsigned_shift_right_masked(hxc_l_value, 16));
}
