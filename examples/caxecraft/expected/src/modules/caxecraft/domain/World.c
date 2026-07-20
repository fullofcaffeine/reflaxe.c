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

void hxc_caxecraft_domain_World_generate(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_seed)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  int32_t hxc_index = 0;
  while (1)
  {
    if (!(hxc_index < 16384))
    {
      break;
    }
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_cells, hxc_length, hxc_index, 0);
    hxc_index = hxc_i32_add_wrapping(hxc_index, 1);
  }
  int32_t hxc_z = 0;
  while (1)
  {
    if (!(hxc_z < 32))
    {
      break;
    }
    int32_t hxc_x = 0;
    while (1)
    {
      if (!(hxc_x < 32))
      {
        break;
      }
      int32_t hxc_tmp_load_result_n5 = hxc_x;
      int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_domain_World_terrainNoise(hxc_seed, hxc_tmp_load_result_n5, hxc_z);
      int32_t hxc_noise = hxc_i32_bit_and(hxc_tmp_call_result_n7, 2147483647);
      int32_t hxc_surface = hxc_i32_add_wrapping(3, hxc_i32_modulo_zero_safe(hxc_noise, 5));
      int32_t hxc_y = 0;
      while (1)
      {
        int32_t hxc_tmp_load_result_n9 = hxc_y;
        if (!(hxc_tmp_load_result_n9 <= hxc_surface))
        {
          break;
        }
        if (hxc_y == 0)
        {
          int32_t hxc_tmp_load_result_n12 = hxc_x;
          int32_t hxc_tmp_load_result_n13 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n15 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n12, hxc_tmp_load_result_n13, hxc_z);
          hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_tmp_call_result_n15, hxc_caxecraft_domain_BlockKind_Bedrock);
        }
        else
        {
          int32_t hxc_tmp_load_result_n17 = hxc_y;
          if (hxc_tmp_load_result_n17 == hxc_surface)
          {
            int32_t hxc_tmp_load_result_n19 = hxc_x;
            int32_t hxc_tmp_load_result_n20 = hxc_y;
            struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n22 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n19, hxc_tmp_load_result_n20, hxc_z);
            hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_tmp_call_result_n22, hxc_caxecraft_domain_BlockKind_Grass);
          }
          else
          {
            int32_t hxc_tmp_load_result_n24 = hxc_y;
            if (hxc_i32_add_wrapping(hxc_tmp_load_result_n24, 2) >= hxc_surface)
            {
              int32_t hxc_tmp_load_result_n26 = hxc_x;
              int32_t hxc_tmp_load_result_n27 = hxc_y;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n29 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n26, hxc_tmp_load_result_n27, hxc_z);
              hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_tmp_call_result_n29, hxc_caxecraft_domain_BlockKind_Dirt);
            }
            else
            {
              int32_t hxc_tmp_load_result_n31 = hxc_x;
              int32_t hxc_tmp_load_result_n32 = hxc_y;
              struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n34 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n31, hxc_tmp_load_result_n32, hxc_z);
              hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_tmp_call_result_n34, hxc_caxecraft_domain_BlockKind_Stone);
            }
          }
        }
        hxc_y = hxc_i32_add_wrapping(hxc_y, 1);
      }
      hxc_x = hxc_i32_add_wrapping(hxc_x, 1);
    }
    hxc_z = hxc_i32_add_wrapping(hxc_z, 1);
  }
  return;
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
    default:
      {
        return hxc_caxecraft_domain_BlockKind_Air;
      }
  }
  return hxc_caxecraft_domain_BlockKind_Grass;
}

bool hxc_caxecraft_domain_World_place(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord, enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  bool hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_isPlaceable(hxc_kind);
  if (!!hxc_tmp_call_result_n0)
  {
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n1 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_coord);
    switch (hxc_tmp_call_result_n1) {
      case hxc_caxecraft_domain_BlockKind_Air:
        {
          break;
        }
      case hxc_caxecraft_domain_BlockKind_Grass:
      case hxc_caxecraft_domain_BlockKind_Dirt:
      case hxc_caxecraft_domain_BlockKind_Stone:
      case hxc_caxecraft_domain_BlockKind_Bedrock:
        {
          return false;
        }
    }
    bool hxc_tmp_call_result_n2 = hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_coord, hxc_kind);
    return hxc_tmp_call_result_n2;
  }
  return false;
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

int32_t hxc_caxecraft_domain_World_stateHash(uint8_t *hxc_cells, size_t hxc_length)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n1 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n1;
  int32_t hxc_hash = -2128831035;
  int32_t hxc_index = 0;
  while (1)
  {
    if (!(hxc_index < 16384))
    {
      break;
    }
    int32_t hxc_tmp_load_result_n1 = hxc_hash;
    int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_WorldStorage_readCode(hxc_cells, hxc_length, hxc_index);
    hxc_hash = hxc_i32_multiply_wrapping(hxc_i32_bit_xor(hxc_tmp_load_result_n1, hxc_tmp_call_result_n3), 16777619);
    hxc_index = hxc_i32_add_wrapping(hxc_index, 1);
  }
  return hxc_hash;
}

int32_t hxc_caxecraft_domain_World_terrainNoise(int32_t hxc_seed, int32_t hxc_x, int32_t hxc_z)
{
  int32_t hxc_value = hxc_i32_bit_xor(hxc_i32_bit_xor(hxc_seed, hxc_i32_multiply_wrapping(hxc_x, 374761393)), hxc_i32_multiply_wrapping(hxc_z, 668265263));
  int32_t hxc_tmp_load_result_n0 = hxc_value;
  hxc_value = hxc_i32_multiply_wrapping(hxc_i32_bit_xor(hxc_tmp_load_result_n0, hxc_i32_unsigned_shift_right_masked(hxc_value, 13)), 1274126177);
  int32_t hxc_tmp_load_result_n2 = hxc_value;
  return hxc_i32_bit_xor(hxc_tmp_load_result_n2, hxc_i32_unsigned_shift_right_masked(hxc_value, 16));
}
