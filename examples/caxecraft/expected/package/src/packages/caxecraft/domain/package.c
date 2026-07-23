#include "hxc/program.h"

int32_t hxc_caxecraft_domain_CaxecraftTrace_collisionTrace(void)
{
  uint8_t hxc_storage[16384] = { 0 };
  uint8_t *hxc_cells = hxc_storage;
  size_t hxc_tmp_length_n1 = sizeof(hxc_storage) / sizeof(hxc_storage[0]);
  hxc_caxecraft_domain_CaxecraftTrace_makeFloor(hxc_cells, hxc_tmp_length_n1);
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n0 = hxc_caxecraft_domain_CharacterPhysics_body(5.5, 1.0, 5.5);
  struct hxc_caxecraft_domain_CharacterBody hxc_state = hxc_tmp_call_result_n0;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_load_result_n1 = hxc_state;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n2 = hxc_caxecraft_domain_CharacterPhysics_input(1.0, 0.5, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n1, hxc_tmp_call_result_n2);
  hxc_state = hxc_tmp_call_result_n3;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_load_result_n4 = hxc_state;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n5 = hxc_caxecraft_domain_CharacterPhysics_input(0.0, 0.0, true);
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n6 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_cells, hxc_tmp_length_n1, hxc_tmp_load_result_n4, hxc_tmp_call_result_n5);
  hxc_state = hxc_tmp_call_result_n6;
  int32_t hxc_hash = hxc_f64_to_i32_saturating(hxc_state.hxc_x * 1000.0);
  int32_t hxc_tmp_load_result_n8 = hxc_hash;
  int32_t hxc_tmp_call_result_n10 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n8, hxc_f64_to_i32_saturating(hxc_state.hxc_y * 1000.0));
  hxc_hash = hxc_tmp_call_result_n10;
  int32_t hxc_tmp_load_result_n11 = hxc_hash;
  int32_t hxc_tmp_call_result_n13 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n11, hxc_f64_to_i32_saturating(hxc_state.hxc_z * 1000.0));
  hxc_hash = hxc_tmp_call_result_n13;
  int32_t hxc_tmp_load_result_n14 = hxc_hash;
  int32_t hxc_tmp_call_result_n16 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n14, hxc_f64_to_i32_saturating(hxc_state.hxc_velocityY * 1000.0));
  hxc_hash = hxc_tmp_call_result_n16;
  int32_t hxc_groundedCode = 0;
  if (hxc_state.hxc_grounded)
  {
    hxc_groundedCode = 1;
  }
  int32_t hxc_tmp_load_result_n18 = hxc_hash;
  int32_t hxc_tmp_call_result_n20 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n18, hxc_groundedCode);
  return hxc_tmp_call_result_n20;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_editTrace(void)
{
  uint8_t hxc_storage[16384] = { 0 };
  uint8_t *hxc_cells = hxc_storage;
  size_t hxc_tmp_length_n1 = sizeof(hxc_storage) / sizeof(hxc_storage[0]);
  hxc_caxecraft_domain_World_generate(hxc_cells, hxc_tmp_length_n1, 324508639);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(4, 10, 4);
  struct hxc_caxecraft_domain_BlockCoord hxc_target = hxc_tmp_call_result_n0;
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n1 = hxc_caxecraft_domain_World_coord(4, 9, 4);
  struct hxc_caxecraft_domain_BlockCoord hxc_base = hxc_tmp_call_result_n1;
  hxc_caxecraft_domain_World_remove(hxc_cells, hxc_tmp_length_n1, hxc_base);
  hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_target, hxc_caxecraft_domain_BlockKind_Stone);
  hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n1, hxc_target, hxc_caxecraft_domain_BlockKind_Dirt);
  int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_domain_World_stateHash(hxc_cells, hxc_tmp_length_n1);
  enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n9 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_tmp_length_n1, hxc_target);
  int32_t hxc_tmp_call_result_n10 = hxc_caxecraft_domain_World_kindCode(hxc_tmp_call_result_n9);
  int32_t hxc_tmp_call_result_n11 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_call_result_n7, hxc_tmp_call_result_n10);
  return hxc_tmp_call_result_n11;
}

void hxc_caxecraft_domain_CaxecraftTrace_makeFloor(uint8_t *hxc_cells, size_t hxc_length)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n1 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n1;
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
      int32_t hxc_tmp_load_result_n2 = hxc_x;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n4 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n2, 0, hxc_z);
      hxc_caxecraft_domain_World_replace(hxc_cells, hxc_length, hxc_tmp_call_result_n4, hxc_caxecraft_domain_BlockKind_Stone);
      hxc_x = hxc_i32_add_wrapping(hxc_x, 1);
    }
    hxc_z = hxc_i32_add_wrapping(hxc_z, 1);
  }
  return;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_mix(int32_t hxc_hash, int32_t hxc_value)
{
  return hxc_i32_multiply_wrapping(hxc_i32_bit_xor(hxc_hash, hxc_value), 16777619);
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_propertyTrace(int32_t hxc_seed)
{
  uint8_t hxc_storage[16384] = { 0 };
  uint8_t *hxc_cells = hxc_storage;
  size_t hxc_tmp_length_n2 = sizeof(hxc_storage) / sizeof(hxc_storage[0]);
  hxc_caxecraft_domain_World_generate(hxc_cells, hxc_tmp_length_n2, hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_seed, 1103515245), 12345));
  int32_t hxc_x = hxc_i32_bit_and(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_seed, 17), 5), 31);
  int32_t hxc_z = hxc_i32_bit_and(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_seed, 29), 11), 31);
  int32_t hxc_tmp_load_result_n0 = hxc_x;
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n2 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n0, 14, hxc_z);
  struct hxc_caxecraft_domain_BlockCoord hxc_edit = hxc_tmp_call_result_n2;
  if (hxc_i32_bit_and(hxc_seed, 1) == 0)
  {
    hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n2, hxc_edit, hxc_caxecraft_domain_BlockKind_Dirt);
  }
  else
  {
    hxc_caxecraft_domain_World_place(hxc_cells, hxc_tmp_length_n2, hxc_edit, hxc_caxecraft_domain_BlockKind_Stone);
  }
  int32_t hxc_tmp_load_result_n7 = hxc_x;
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n9 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n2, (double)hxc_tmp_load_result_n7 + 0.5, 15.5, (double)hxc_z + 0.5, 0.25, -1.0, 0.5, 24.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_ray = hxc_tmp_call_result_n9;
  double hxc_tmp_conditional_result_n8 = 0.0;
  if (hxc_i32_bit_and(hxc_seed, 1) == 0)
  {
    hxc_tmp_conditional_result_n8 = 1.0;
  }
  else
  {
    hxc_tmp_conditional_result_n8 = -1.0;
  }
  double hxc_moveX = hxc_tmp_conditional_result_n8;
  double hxc_tmp_conditional_result_n10 = 0.0;
  if (hxc_i32_bit_and(hxc_seed, 2) == 0)
  {
    hxc_tmp_conditional_result_n10 = 0.5;
  }
  else
  {
    hxc_tmp_conditional_result_n10 = -0.5;
  }
  double hxc_moveZ = hxc_tmp_conditional_result_n10;
  bool hxc_jump = hxc_i32_bit_and(hxc_seed, 4) != 0;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n12 = hxc_caxecraft_domain_CharacterPhysics_body(1.5 + (double)hxc_i32_bit_and(hxc_seed, 3), 14.0, 1.5 + (double)hxc_i32_bit_and(hxc_i32_unsigned_shift_right_masked(hxc_seed, 2), 3));
  struct hxc_caxecraft_domain_CharacterBody hxc_state = hxc_tmp_call_result_n12;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n14 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_cells, hxc_tmp_length_n2, hxc_state);
  hxc_state = hxc_tmp_call_result_n14;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_load_result_n15 = hxc_state;
  double hxc_tmp_load_result_n16 = hxc_moveX;
  double hxc_tmp_load_result_n17 = hxc_moveZ;
  struct hxc_caxecraft_domain_StepInput hxc_tmp_call_result_n19 = hxc_caxecraft_domain_CharacterPhysics_input(hxc_tmp_load_result_n16, hxc_tmp_load_result_n17, hxc_jump);
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n20 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_cells, hxc_tmp_length_n2, hxc_tmp_load_result_n15, hxc_tmp_call_result_n19);
  hxc_state = hxc_tmp_call_result_n20;
  int32_t hxc_tmp_call_result_n21 = hxc_caxecraft_domain_World_stateHash(hxc_cells, hxc_tmp_length_n2);
  int32_t hxc_hash = hxc_tmp_call_result_n21;
  int32_t hxc_hitCode = 0;
  if (hxc_ray.hxc_hit)
  {
    hxc_hitCode = 1;
  }
  int32_t hxc_tmp_load_result_n23 = hxc_hash;
  int32_t hxc_tmp_call_result_n25 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n23, hxc_hitCode);
  hxc_hash = hxc_tmp_call_result_n25;
  int32_t hxc_tmp_load_result_n26 = hxc_hash;
  int32_t hxc_tmp_call_result_n28 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n26, hxc_ray.hxc_cellX);
  hxc_hash = hxc_tmp_call_result_n28;
  int32_t hxc_tmp_load_result_n29 = hxc_hash;
  int32_t hxc_tmp_call_result_n31 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n29, hxc_ray.hxc_cellY);
  hxc_hash = hxc_tmp_call_result_n31;
  int32_t hxc_tmp_load_result_n32 = hxc_hash;
  int32_t hxc_tmp_call_result_n34 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n32, hxc_ray.hxc_cellZ);
  hxc_hash = hxc_tmp_call_result_n34;
  int32_t hxc_tmp_load_result_n35 = hxc_hash;
  int32_t hxc_tmp_call_result_n37 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n35, hxc_ray.hxc_visited);
  hxc_hash = hxc_tmp_call_result_n37;
  int32_t hxc_tmp_load_result_n38 = hxc_hash;
  int32_t hxc_tmp_call_result_n40 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n38, hxc_f64_to_i32_saturating(hxc_state.hxc_x * 1000.0));
  hxc_hash = hxc_tmp_call_result_n40;
  int32_t hxc_tmp_load_result_n41 = hxc_hash;
  int32_t hxc_tmp_call_result_n43 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n41, hxc_f64_to_i32_saturating(hxc_state.hxc_y * 1000.0));
  hxc_hash = hxc_tmp_call_result_n43;
  int32_t hxc_tmp_load_result_n44 = hxc_hash;
  int32_t hxc_tmp_call_result_n46 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n44, hxc_f64_to_i32_saturating(hxc_state.hxc_z * 1000.0));
  return hxc_tmp_call_result_n46;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_rayTrace(void)
{
  uint8_t hxc_storage[16384] = { 0 };
  uint8_t *hxc_cells = hxc_storage;
  size_t hxc_tmp_length_n1 = sizeof(hxc_storage) / sizeof(hxc_storage[0]);
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_cells, hxc_tmp_length_n1, hxc_tmp_call_result_n0, hxc_caxecraft_domain_BlockKind_Stone);
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n2 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_cells, hxc_tmp_length_n1, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_hit = hxc_tmp_call_result_n2;
  bool hxc_tmp_record_field_load_result_n3 = hxc_hit.hxc_hit;
  int32_t hxc_tmp_conditional_result_n4 = 0;
  if (hxc_tmp_record_field_load_result_n3)
  {
    hxc_tmp_conditional_result_n4 = 1;
  }
  else
  {
    hxc_tmp_conditional_result_n4 = 0;
  }
  int32_t hxc_hash = hxc_tmp_conditional_result_n4;
  int32_t hxc_tmp_load_result_n5 = hxc_hash;
  int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n5, hxc_hit.hxc_cellX);
  hxc_hash = hxc_tmp_call_result_n7;
  int32_t hxc_tmp_load_result_n8 = hxc_hash;
  int32_t hxc_tmp_call_result_n10 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n8, hxc_hit.hxc_previousX);
  hxc_hash = hxc_tmp_call_result_n10;
  int32_t hxc_tmp_load_result_n11 = hxc_hash;
  int32_t hxc_tmp_call_result_n13 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n11, hxc_hit.hxc_normalX);
  hxc_hash = hxc_tmp_call_result_n13;
  int32_t hxc_tmp_load_result_n14 = hxc_hash;
  int32_t hxc_tmp_call_result_n16 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n14, hxc_f64_to_i32_saturating(hxc_hit.hxc_distance * 1000.0));
  hxc_hash = hxc_tmp_call_result_n16;
  int32_t hxc_tmp_load_result_n17 = hxc_hash;
  int32_t hxc_tmp_call_result_n19 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n17, hxc_hit.hxc_visited);
  return hxc_tmp_call_result_n19;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_runTrace(void)
{
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_CaxecraftTrace_terrainTrace();
  int32_t hxc_hash = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_load_result_n1 = hxc_hash;
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_domain_CaxecraftTrace_editTrace();
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n1, hxc_tmp_call_result_n2);
  hxc_hash = hxc_tmp_call_result_n3;
  int32_t hxc_tmp_load_result_n4 = hxc_hash;
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_domain_CaxecraftTrace_rayTrace();
  int32_t hxc_tmp_call_result_n6 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n4, hxc_tmp_call_result_n5);
  hxc_hash = hxc_tmp_call_result_n6;
  int32_t hxc_tmp_load_result_n7 = hxc_hash;
  int32_t hxc_tmp_call_result_n8 = hxc_caxecraft_domain_CaxecraftTrace_collisionTrace();
  int32_t hxc_tmp_call_result_n9 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n7, hxc_tmp_call_result_n8);
  hxc_hash = hxc_tmp_call_result_n9;
  int32_t hxc_seed = 0;
  while (1)
  {
    if (!(hxc_seed < 32))
    {
      break;
    }
    int32_t hxc_tmp_load_result_n11 = hxc_hash;
    int32_t hxc_tmp_call_result_n13 = hxc_caxecraft_domain_CaxecraftTrace_propertyTrace(hxc_seed);
    int32_t hxc_tmp_call_result_n14 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_tmp_load_result_n11, hxc_tmp_call_result_n13);
    hxc_hash = hxc_tmp_call_result_n14;
    hxc_seed = hxc_i32_add_wrapping(hxc_seed, 1);
  }
  return hxc_hash;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_terrainTrace(void)
{
  uint8_t hxc_storage[16384] = { 0 };
  uint8_t *hxc_cells = hxc_storage;
  size_t hxc_tmp_length_n1 = sizeof(hxc_storage) / sizeof(hxc_storage[0]);
  hxc_caxecraft_domain_World_generate(hxc_cells, hxc_tmp_length_n1, 324508639);
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_stateHash(hxc_cells, hxc_tmp_length_n1);
  return hxc_tmp_call_result_n0;
}

double hxc_caxecraft_domain_VoxelRaycast_abs(double hxc_value)
{
  if (!(hxc_value < 0.0))
  {
    return hxc_value;
  }
  return -hxc_value;
}

int32_t hxc_caxecraft_domain_VoxelRaycast_floorToInt(double hxc_value)
{
  int32_t hxc_truncated = hxc_f64_to_i32_saturating(hxc_value);
  if (!(hxc_value < (double)hxc_truncated))
  {
    return hxc_truncated;
  }
  return hxc_i32_subtract_wrapping(hxc_truncated, 1);
}

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_hit(int32_t hxc_cellX, int32_t hxc_cellY, int32_t hxc_cellZ, int32_t hxc_previousX, int32_t hxc_previousY, int32_t hxc_previousZ, int32_t hxc_normalX, int32_t hxc_normalY, int32_t hxc_normalZ, double hxc_distance, int32_t hxc_visited)
{
  return (struct hxc_caxecraft_domain_RaycastHit){ .hxc_cellX = hxc_cellX, .hxc_cellY = hxc_cellY, .hxc_cellZ = hxc_cellZ, .hxc_distance = hxc_distance, .hxc_hit = true, .hxc_normalX = hxc_normalX, .hxc_normalY = hxc_normalY, .hxc_normalZ = hxc_normalZ, .hxc_previousX = hxc_previousX, .hxc_previousY = hxc_previousY, .hxc_previousZ = hxc_previousZ, .hxc_visited = hxc_visited };
}

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_miss(int32_t hxc_previousX, int32_t hxc_previousY, int32_t hxc_previousZ, double hxc_distance, int32_t hxc_visited)
{
  return (struct hxc_caxecraft_domain_RaycastHit){ .hxc_cellX = 0, .hxc_cellY = 0, .hxc_cellZ = 0, .hxc_distance = hxc_distance, .hxc_hit = false, .hxc_normalX = 0, .hxc_normalY = 0, .hxc_normalZ = 0, .hxc_previousX = hxc_previousX, .hxc_previousY = hxc_previousY, .hxc_previousZ = hxc_previousZ, .hxc_visited = hxc_visited };
}

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_trace(uint8_t *hxc_cells, size_t hxc_length, double hxc_originX, double hxc_originY, double hxc_originZ, double hxc_directionX, double hxc_directionY, double hxc_directionZ, double hxc_maximumDistance)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n8 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n8;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_VoxelRaycast_floorToInt(hxc_originX);
  int32_t hxc_cellX = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_domain_VoxelRaycast_floorToInt(hxc_originY);
  int32_t hxc_cellY = hxc_tmp_call_result_n1;
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_domain_VoxelRaycast_floorToInt(hxc_originZ);
  int32_t hxc_cellZ = hxc_tmp_call_result_n2;
  bool hxc_tmp_short_circuit_result_n12 = hxc_maximumDistance < 0.0;
  if (!(hxc_maximumDistance < 0.0))
  {
    int32_t hxc_tmp_load_result_n3 = hxc_cellX;
    int32_t hxc_tmp_load_result_n4 = hxc_cellY;
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n6 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n3, hxc_tmp_load_result_n4, hxc_cellZ);
    bool hxc_tmp_call_result_n7 = hxc_caxecraft_domain_World_contains(hxc_tmp_call_result_n6);
    hxc_tmp_short_circuit_result_n12 = !hxc_tmp_call_result_n7;
  }
  if (hxc_tmp_short_circuit_result_n12)
  {
    int32_t hxc_tmp_load_result_n9 = hxc_cellX;
    int32_t hxc_tmp_load_result_n10 = hxc_cellY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n12 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_tmp_load_result_n9, hxc_tmp_load_result_n10, hxc_cellZ, 0.0, 0);
    return hxc_tmp_call_result_n12;
  }
  int32_t hxc_tmp_load_result_n13 = hxc_cellX;
  int32_t hxc_tmp_load_result_n14 = hxc_cellY;
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n16 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n13, hxc_tmp_load_result_n14, hxc_cellZ);
  enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n17 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n16);
  bool hxc_tmp_call_result_n18 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n17);
  if (hxc_tmp_call_result_n18)
  {
    int32_t hxc_tmp_load_result_n19 = hxc_cellX;
    int32_t hxc_tmp_load_result_n20 = hxc_cellY;
    int32_t hxc_tmp_load_result_n21 = hxc_cellZ;
    int32_t hxc_tmp_load_result_n22 = hxc_cellX;
    int32_t hxc_tmp_load_result_n23 = hxc_cellY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n25 = hxc_caxecraft_domain_VoxelRaycast_hit(hxc_tmp_load_result_n19, hxc_tmp_load_result_n20, hxc_tmp_load_result_n21, hxc_tmp_load_result_n22, hxc_tmp_load_result_n23, hxc_cellZ, 0, 0, 0, 0.0, 1);
    return hxc_tmp_call_result_n25;
  }
  int32_t hxc_tmp_conditional_result_n14 = 0;
  if (hxc_directionX > 0.0)
  {
    hxc_tmp_conditional_result_n14 = 1;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n15 = 0;
    if (hxc_directionX < 0.0)
    {
      hxc_tmp_conditional_result_n15 = -1;
    }
    else
    {
      hxc_tmp_conditional_result_n15 = 0;
    }
    hxc_tmp_conditional_result_n14 = hxc_tmp_conditional_result_n15;
  }
  int32_t hxc_stepX = hxc_tmp_conditional_result_n14;
  int32_t hxc_tmp_conditional_result_n17 = 0;
  if (hxc_directionY > 0.0)
  {
    hxc_tmp_conditional_result_n17 = 1;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n18 = 0;
    if (hxc_directionY < 0.0)
    {
      hxc_tmp_conditional_result_n18 = -1;
    }
    else
    {
      hxc_tmp_conditional_result_n18 = 0;
    }
    hxc_tmp_conditional_result_n17 = hxc_tmp_conditional_result_n18;
  }
  int32_t hxc_stepY = hxc_tmp_conditional_result_n17;
  int32_t hxc_tmp_conditional_result_n20 = 0;
  if (hxc_directionZ > 0.0)
  {
    hxc_tmp_conditional_result_n20 = 1;
  }
  else
  {
    int32_t hxc_tmp_conditional_result_n21 = 0;
    if (hxc_directionZ < 0.0)
    {
      hxc_tmp_conditional_result_n21 = -1;
    }
    else
    {
      hxc_tmp_conditional_result_n21 = 0;
    }
    hxc_tmp_conditional_result_n20 = hxc_tmp_conditional_result_n21;
  }
  int32_t hxc_stepZ = hxc_tmp_conditional_result_n20;
  int32_t hxc_tmp_load_result_n32 = hxc_stepX;
  bool hxc_tmp_short_circuit_result_n22 = hxc_tmp_load_result_n32 == 0;
  if (hxc_tmp_load_result_n32 == 0)
  {
    hxc_tmp_short_circuit_result_n22 = hxc_stepY == 0;
  }
  bool hxc_tmp_short_circuit_load_result_n34 = hxc_tmp_short_circuit_result_n22;
  bool hxc_tmp_short_circuit_result_n23 = hxc_tmp_short_circuit_load_result_n34;
  if (hxc_tmp_short_circuit_load_result_n34)
  {
    hxc_tmp_short_circuit_result_n23 = hxc_stepZ == 0;
  }
  if (hxc_tmp_short_circuit_result_n23)
  {
    int32_t hxc_tmp_load_result_n37 = hxc_cellX;
    int32_t hxc_tmp_load_result_n38 = hxc_cellY;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n40 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_tmp_load_result_n37, hxc_tmp_load_result_n38, hxc_cellZ, 0.0, 1);
    return hxc_tmp_call_result_n40;
  }
  int32_t hxc_tmp_load_result_n41 = hxc_stepX;
  double hxc_tmp_conditional_result_n25 = 0.0;
  if (hxc_tmp_load_result_n41 == 0)
  {
    hxc_tmp_conditional_result_n25 = 0.0;
  }
  else
  {
    double hxc_tmp_call_result_n42 = hxc_caxecraft_domain_VoxelRaycast_abs(hxc_f64_divide_zero_safe(1.0, hxc_directionX));
    hxc_tmp_conditional_result_n25 = hxc_tmp_call_result_n42;
  }
  double hxc_deltaX = hxc_tmp_conditional_result_n25;
  int32_t hxc_tmp_load_result_n44 = hxc_stepY;
  double hxc_tmp_conditional_result_n27 = 0.0;
  if (hxc_tmp_load_result_n44 == 0)
  {
    hxc_tmp_conditional_result_n27 = 0.0;
  }
  else
  {
    double hxc_tmp_call_result_n45 = hxc_caxecraft_domain_VoxelRaycast_abs(hxc_f64_divide_zero_safe(1.0, hxc_directionY));
    hxc_tmp_conditional_result_n27 = hxc_tmp_call_result_n45;
  }
  double hxc_deltaY = hxc_tmp_conditional_result_n27;
  int32_t hxc_tmp_load_result_n47 = hxc_stepZ;
  double hxc_tmp_conditional_result_n29 = 0.0;
  if (hxc_tmp_load_result_n47 == 0)
  {
    hxc_tmp_conditional_result_n29 = 0.0;
  }
  else
  {
    double hxc_tmp_call_result_n48 = hxc_caxecraft_domain_VoxelRaycast_abs(hxc_f64_divide_zero_safe(1.0, hxc_directionZ));
    hxc_tmp_conditional_result_n29 = hxc_tmp_call_result_n48;
  }
  double hxc_deltaZ = hxc_tmp_conditional_result_n29;
  int32_t hxc_tmp_load_result_n50 = hxc_stepX;
  double hxc_tmp_conditional_result_n31 = 0.0;
  if (hxc_tmp_load_result_n50 == 0)
  {
    hxc_tmp_conditional_result_n31 = 0.0;
  }
  else
  {
    int32_t hxc_tmp_load_result_n51 = hxc_stepX;
    int32_t hxc_tmp_conditional_result_n32 = 0;
    if (hxc_tmp_load_result_n51 > 0)
    {
      hxc_tmp_conditional_result_n32 = hxc_i32_add_wrapping(hxc_cellX, 1);
    }
    else
    {
      hxc_tmp_conditional_result_n32 = hxc_cellX;
    }
    hxc_tmp_conditional_result_n31 = hxc_f64_divide_zero_safe((double)hxc_tmp_conditional_result_n32 - hxc_originX, hxc_directionX);
  }
  double hxc_maximumX = hxc_tmp_conditional_result_n31;
  int32_t hxc_tmp_load_result_n56 = hxc_stepY;
  double hxc_tmp_conditional_result_n34 = 0.0;
  if (hxc_tmp_load_result_n56 == 0)
  {
    hxc_tmp_conditional_result_n34 = 0.0;
  }
  else
  {
    int32_t hxc_tmp_load_result_n57 = hxc_stepY;
    int32_t hxc_tmp_conditional_result_n35 = 0;
    if (hxc_tmp_load_result_n57 > 0)
    {
      hxc_tmp_conditional_result_n35 = hxc_i32_add_wrapping(hxc_cellY, 1);
    }
    else
    {
      hxc_tmp_conditional_result_n35 = hxc_cellY;
    }
    hxc_tmp_conditional_result_n34 = hxc_f64_divide_zero_safe((double)hxc_tmp_conditional_result_n35 - hxc_originY, hxc_directionY);
  }
  double hxc_maximumY = hxc_tmp_conditional_result_n34;
  int32_t hxc_tmp_load_result_n62 = hxc_stepZ;
  double hxc_tmp_conditional_result_n37 = 0.0;
  if (hxc_tmp_load_result_n62 == 0)
  {
    hxc_tmp_conditional_result_n37 = 0.0;
  }
  else
  {
    int32_t hxc_tmp_load_result_n63 = hxc_stepZ;
    int32_t hxc_tmp_conditional_result_n38 = 0;
    if (hxc_tmp_load_result_n63 > 0)
    {
      hxc_tmp_conditional_result_n38 = hxc_i32_add_wrapping(hxc_cellZ, 1);
    }
    else
    {
      hxc_tmp_conditional_result_n38 = hxc_cellZ;
    }
    hxc_tmp_conditional_result_n37 = hxc_f64_divide_zero_safe((double)hxc_tmp_conditional_result_n38 - hxc_originZ, hxc_directionZ);
  }
  double hxc_maximumZ = hxc_tmp_conditional_result_n37;
  int32_t hxc_previousX = hxc_cellX;
  int32_t hxc_previousY = hxc_cellY;
  int32_t hxc_previousZ = hxc_cellZ;
  int32_t hxc_normalX = 0;
  int32_t hxc_normalY = 0;
  int32_t hxc_normalZ = 0;
  double hxc_distance = 0.0;
  int32_t hxc_visited = 1;
  while (1)
  {
    if (!(hxc_visited < 88))
    {
      break;
    }
    hxc_previousX = hxc_cellX;
    hxc_previousY = hxc_cellY;
    hxc_previousZ = hxc_cellZ;
    int32_t hxc_tmp_load_result_n75 = hxc_stepX;
    bool hxc_tmp_short_circuit_result_n47 = hxc_tmp_load_result_n75 != 0;
    if (hxc_tmp_load_result_n75 != 0)
    {
      int32_t hxc_tmp_load_result_n76 = hxc_stepY;
      bool hxc_tmp_short_circuit_result_n48 = hxc_tmp_load_result_n76 == 0;
      if (!(hxc_tmp_load_result_n76 == 0))
      {
        double hxc_tmp_load_result_n77 = hxc_maximumX;
        hxc_tmp_short_circuit_result_n48 = hxc_tmp_load_result_n77 <= hxc_maximumY;
      }
      hxc_tmp_short_circuit_result_n47 = hxc_tmp_short_circuit_result_n48;
    }
    bool hxc_tmp_short_circuit_load_result_n80 = hxc_tmp_short_circuit_result_n47;
    bool hxc_tmp_short_circuit_result_n49 = hxc_tmp_short_circuit_load_result_n80;
    if (hxc_tmp_short_circuit_load_result_n80)
    {
      int32_t hxc_tmp_load_result_n81 = hxc_stepZ;
      bool hxc_tmp_short_circuit_result_n50 = hxc_tmp_load_result_n81 == 0;
      if (!(hxc_tmp_load_result_n81 == 0))
      {
        double hxc_tmp_load_result_n82 = hxc_maximumX;
        hxc_tmp_short_circuit_result_n50 = hxc_tmp_load_result_n82 <= hxc_maximumZ;
      }
      hxc_tmp_short_circuit_result_n49 = hxc_tmp_short_circuit_result_n50;
    }
    if (hxc_tmp_short_circuit_result_n49)
    {
      int32_t hxc_tmp_compound_load_result_n86 = hxc_cellX;
      hxc_cellX = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n86, hxc_stepX);
      hxc_distance = hxc_maximumX;
      double hxc_tmp_compound_load_result_n89 = hxc_maximumX;
      hxc_maximumX = hxc_tmp_compound_load_result_n89 + hxc_deltaX;
      hxc_normalX = hxc_i32_negate_wrapping(hxc_stepX);
      hxc_normalY = 0;
      hxc_normalZ = 0;
    }
    else
    {
      int32_t hxc_tmp_load_result_n92 = hxc_stepY;
      bool hxc_tmp_short_circuit_result_n51 = hxc_tmp_load_result_n92 != 0;
      if (hxc_tmp_load_result_n92 != 0)
      {
        int32_t hxc_tmp_load_result_n93 = hxc_stepZ;
        bool hxc_tmp_short_circuit_result_n52 = hxc_tmp_load_result_n93 == 0;
        if (!(hxc_tmp_load_result_n93 == 0))
        {
          double hxc_tmp_load_result_n94 = hxc_maximumY;
          hxc_tmp_short_circuit_result_n52 = hxc_tmp_load_result_n94 <= hxc_maximumZ;
        }
        hxc_tmp_short_circuit_result_n51 = hxc_tmp_short_circuit_result_n52;
      }
      if (hxc_tmp_short_circuit_result_n51)
      {
        int32_t hxc_tmp_compound_load_result_n98 = hxc_cellY;
        hxc_cellY = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n98, hxc_stepY);
        hxc_distance = hxc_maximumY;
        double hxc_tmp_compound_load_result_n101 = hxc_maximumY;
        hxc_maximumY = hxc_tmp_compound_load_result_n101 + hxc_deltaY;
        hxc_normalX = 0;
        hxc_normalY = hxc_i32_negate_wrapping(hxc_stepY);
        hxc_normalZ = 0;
      }
      else
      {
        int32_t hxc_tmp_compound_load_result_n104 = hxc_cellZ;
        hxc_cellZ = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n104, hxc_stepZ);
        hxc_distance = hxc_maximumZ;
        double hxc_tmp_compound_load_result_n107 = hxc_maximumZ;
        hxc_maximumZ = hxc_tmp_compound_load_result_n107 + hxc_deltaZ;
        hxc_normalX = 0;
        hxc_normalY = 0;
        hxc_normalZ = hxc_i32_negate_wrapping(hxc_stepZ);
      }
    }
    hxc_visited = hxc_i32_add_wrapping(hxc_visited, 1);
    if (!(hxc_distance > hxc_maximumDistance))
    {
      int32_t hxc_tmp_load_result_n118 = hxc_cellX;
      int32_t hxc_tmp_load_result_n119 = hxc_cellY;
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n121 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n118, hxc_tmp_load_result_n119, hxc_cellZ);
      struct hxc_caxecraft_domain_BlockCoord hxc_coord = hxc_tmp_call_result_n121;
      bool hxc_tmp_call_result_n123 = hxc_caxecraft_domain_World_contains(hxc_coord);
      if (!hxc_tmp_call_result_n123)
      {
        int32_t hxc_tmp_load_result_n124 = hxc_previousX;
        int32_t hxc_tmp_load_result_n125 = hxc_previousY;
        int32_t hxc_tmp_load_result_n126 = hxc_previousZ;
        double hxc_tmp_load_result_n127 = hxc_distance;
        struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n129 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_tmp_load_result_n124, hxc_tmp_load_result_n125, hxc_tmp_load_result_n126, hxc_tmp_load_result_n127, hxc_visited);
        return hxc_tmp_call_result_n129;
      }
      else
      {
        enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n131 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_coord);
        bool hxc_tmp_call_result_n132 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n131);
        if (hxc_tmp_call_result_n132)
        {
          int32_t hxc_tmp_load_result_n133 = hxc_cellX;
          int32_t hxc_tmp_load_result_n134 = hxc_cellY;
          int32_t hxc_tmp_load_result_n135 = hxc_cellZ;
          int32_t hxc_tmp_load_result_n136 = hxc_previousX;
          int32_t hxc_tmp_load_result_n137 = hxc_previousY;
          int32_t hxc_tmp_load_result_n138 = hxc_previousZ;
          int32_t hxc_tmp_load_result_n139 = hxc_normalX;
          int32_t hxc_tmp_load_result_n140 = hxc_normalY;
          int32_t hxc_tmp_load_result_n141 = hxc_normalZ;
          double hxc_tmp_load_result_n142 = hxc_distance;
          struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n144 = hxc_caxecraft_domain_VoxelRaycast_hit(hxc_tmp_load_result_n133, hxc_tmp_load_result_n134, hxc_tmp_load_result_n135, hxc_tmp_load_result_n136, hxc_tmp_load_result_n137, hxc_tmp_load_result_n138, hxc_tmp_load_result_n139, hxc_tmp_load_result_n140, hxc_tmp_load_result_n141, hxc_tmp_load_result_n142, hxc_visited);
          return hxc_tmp_call_result_n144;
        }
        else
        {
          continue;
        }
      }
    }
    int32_t hxc_tmp_load_result_n112 = hxc_previousX;
    int32_t hxc_tmp_load_result_n113 = hxc_previousY;
    int32_t hxc_tmp_load_result_n114 = hxc_previousZ;
    double hxc_tmp_load_result_n115 = hxc_distance;
    struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n117 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_tmp_load_result_n112, hxc_tmp_load_result_n113, hxc_tmp_load_result_n114, hxc_tmp_load_result_n115, hxc_visited);
    return hxc_tmp_call_result_n117;
  }
  int32_t hxc_tmp_load_result_n145 = hxc_previousX;
  int32_t hxc_tmp_load_result_n146 = hxc_previousY;
  int32_t hxc_tmp_load_result_n147 = hxc_previousZ;
  double hxc_tmp_load_result_n148 = hxc_distance;
  struct hxc_caxecraft_domain_RaycastHit hxc_tmp_call_result_n150 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_tmp_load_result_n145, hxc_tmp_load_result_n146, hxc_tmp_load_result_n147, hxc_tmp_load_result_n148, hxc_visited);
  return hxc_tmp_call_result_n150;
}

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
      case hxc_caxecraft_domain_BlockKind_Sand:
      case hxc_caxecraft_domain_BlockKind_Wood:
      case hxc_caxecraft_domain_BlockKind_Leaves:
      case hxc_caxecraft_domain_BlockKind_Snow:
      case hxc_caxecraft_domain_BlockKind_Ash:
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

int32_t hxc_caxecraft_domain_WorldStorage_readCode(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_index)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n0 = hxc_borrow[(size_t)hxc_index];
  return (int32_t)hxc_tmp_collection_index_load_result_n0;
}

void hxc_caxecraft_domain_WorldStorage_writeCode(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_index, int32_t hxc_code)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n3)
  {
    abort();
  }
  hxc_borrow[(size_t)hxc_index] = (uint8_t)hxc_code;
  return;
}

double hxc_caxecraft_domain_CharacterPhysics_abs(double hxc_value)
{
  if (!(hxc_value < 0.0))
  {
    return hxc_value;
  }
  return -hxc_value;
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_body(double hxc_x, double hxc_y, double hxc_z)
{
  return (struct hxc_caxecraft_domain_CharacterBody){ .hxc_grounded = false, .hxc_velocityX = 0.0, .hxc_velocityY = 0.0, .hxc_velocityZ = 0.0, .hxc_x = hxc_x, .hxc_y = hxc_y, .hxc_z = hxc_z };
}

bool hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(struct hxc_caxecraft_domain_CharacterBody hxc_state, struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  double hxc_bodyMinimumX = hxc_state.hxc_x - 0.30;
  double hxc_bodyMaximumX = hxc_state.hxc_x + 0.30;
  double hxc_bodyMinimumY = hxc_state.hxc_y;
  double hxc_bodyMaximumY = hxc_state.hxc_y + 1.80;
  double hxc_bodyMinimumZ = hxc_state.hxc_z - 0.30;
  double hxc_bodyMaximumZ = hxc_state.hxc_z + 0.30;
  bool hxc_separated = false;
  if (hxc_bodyMaximumX <= (double)hxc_coord.hxc_x)
  {
    hxc_separated = true;
  }
  if (hxc_bodyMinimumX >= (double)hxc_i32_add_wrapping(hxc_coord.hxc_x, 1))
  {
    hxc_separated = true;
  }
  if (hxc_bodyMaximumY <= (double)hxc_coord.hxc_y)
  {
    hxc_separated = true;
  }
  if (hxc_bodyMinimumY >= (double)hxc_i32_add_wrapping(hxc_coord.hxc_y, 1))
  {
    hxc_separated = true;
  }
  if (hxc_bodyMaximumZ <= (double)hxc_coord.hxc_z)
  {
    hxc_separated = true;
  }
  if (hxc_bodyMinimumZ >= (double)hxc_i32_add_wrapping(hxc_coord.hxc_z, 1))
  {
    hxc_separated = true;
  }
  return hxc_separated;
}

int32_t hxc_caxecraft_domain_CharacterPhysics_floorToInt(double hxc_value)
{
  int32_t hxc_truncated = hxc_f64_to_i32_saturating(hxc_value);
  if (!(hxc_value < (double)hxc_truncated))
  {
    return hxc_truncated;
  }
  return hxc_i32_subtract_wrapping(hxc_truncated, 1);
}

struct hxc_caxecraft_domain_StepInput hxc_caxecraft_domain_CharacterPhysics_input(double hxc_moveX, double hxc_moveZ, bool hxc_jump)
{
  return (struct hxc_caxecraft_domain_StepInput){ .hxc_jump = hxc_jump, .hxc_moveX = hxc_moveX, .hxc_moveZ = hxc_moveZ };
}

struct hxc_caxecraft_domain_AxisMove hxc_caxecraft_domain_CharacterPhysics_moveAxis(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z, double hxc_delta, int32_t hxc_axis)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n6 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n6;
  double hxc_tmp_call_result_n0 = hxc_caxecraft_domain_CharacterPhysics_abs(hxc_delta);
  double hxc_magnitude = hxc_tmp_call_result_n0;
  if (hxc_magnitude == 0.0)
  {
    return (struct hxc_caxecraft_domain_AxisMove){ .hxc_blocked = false, .hxc_x = hxc_x, .hxc_y = hxc_y, .hxc_z = hxc_z };
  }
  int32_t hxc_count = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe(hxc_magnitude, 0.20));
  int32_t hxc_tmp_load_result_n4 = hxc_count;
  if ((double)hxc_tmp_load_result_n4 * 0.20 < hxc_magnitude)
  {
    hxc_count = hxc_i32_add_wrapping(hxc_count, 1);
  }
  if (hxc_count < 1)
  {
    hxc_count = 1;
  }
  double hxc_increment = hxc_f64_divide_zero_safe(hxc_delta, (double)hxc_count);
  double hxc_currentX = hxc_x;
  double hxc_currentY = hxc_y;
  double hxc_currentZ = hxc_z;
  bool hxc_blocked = false;
  int32_t hxc_stepIndex = 0;
  while (1)
  {
    int32_t hxc_tmp_load_result_n9 = hxc_stepIndex;
    if (!(hxc_tmp_load_result_n9 < hxc_count))
    {
      break;
    }
    double hxc_tmp_conditional_result_n16 = 0.0;
    if (hxc_axis == 0)
    {
      double hxc_tmp_load_result_n11 = hxc_currentX;
      hxc_tmp_conditional_result_n16 = hxc_tmp_load_result_n11 + hxc_increment;
    }
    else
    {
      hxc_tmp_conditional_result_n16 = hxc_currentX;
    }
    double hxc_candidateX = hxc_tmp_conditional_result_n16;
    double hxc_tmp_conditional_result_n18 = 0.0;
    if (hxc_axis == 1)
    {
      double hxc_tmp_load_result_n15 = hxc_currentY;
      hxc_tmp_conditional_result_n18 = hxc_tmp_load_result_n15 + hxc_increment;
    }
    else
    {
      hxc_tmp_conditional_result_n18 = hxc_currentY;
    }
    double hxc_candidateY = hxc_tmp_conditional_result_n18;
    double hxc_tmp_conditional_result_n20 = 0.0;
    if (hxc_axis == 2)
    {
      double hxc_tmp_load_result_n19 = hxc_currentZ;
      hxc_tmp_conditional_result_n20 = hxc_tmp_load_result_n19 + hxc_increment;
    }
    else
    {
      hxc_tmp_conditional_result_n20 = hxc_currentZ;
    }
    double hxc_candidateZ = hxc_tmp_conditional_result_n20;
    double hxc_tmp_load_result_n23 = hxc_candidateX;
    double hxc_tmp_load_result_n24 = hxc_candidateY;
    bool hxc_tmp_call_result_n26 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_cells, hxc_length, hxc_tmp_load_result_n23, hxc_tmp_load_result_n24, hxc_candidateZ);
    if (!hxc_tmp_call_result_n26)
    {
      hxc_currentX = hxc_candidateX;
      hxc_currentY = hxc_candidateY;
      hxc_currentZ = hxc_candidateZ;
      hxc_stepIndex = hxc_i32_add_wrapping(hxc_stepIndex, 1);
      continue;
    }
    hxc_blocked = true;
    break;
  }
  double hxc_tmp_load_result_n31 = hxc_currentX;
  double hxc_tmp_load_result_n32 = hxc_currentY;
  double hxc_tmp_load_result_n33 = hxc_currentZ;
  return (struct hxc_caxecraft_domain_AxisMove){ .hxc_blocked = hxc_blocked, .hxc_x = hxc_tmp_load_result_n31, .hxc_y = hxc_tmp_load_result_n32, .hxc_z = hxc_tmp_load_result_n33 };
}

bool hxc_caxecraft_domain_CharacterPhysics_overlaps(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_x - 0.30 + 0.0001);
  int32_t hxc_minimumX = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_x + 0.30 - 0.0001);
  int32_t hxc_maximumX = hxc_tmp_call_result_n1;
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_y + 0.0001);
  int32_t hxc_minimumY = hxc_tmp_call_result_n2;
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_y + 1.80 - 0.0001);
  int32_t hxc_maximumY = hxc_tmp_call_result_n3;
  int32_t hxc_tmp_call_result_n4 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_z - 0.30 + 0.0001);
  int32_t hxc_minimumZ = hxc_tmp_call_result_n4;
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_z + 0.30 - 0.0001);
  int32_t hxc_maximumZ = hxc_tmp_call_result_n5;
  int32_t hxc_blockY = hxc_minimumY;
  while (1)
  {
    int32_t hxc_tmp_load_result_n7 = hxc_blockY;
    if (!(hxc_tmp_load_result_n7 <= hxc_maximumY))
    {
      break;
    }
    int32_t hxc_blockZ = hxc_minimumZ;
    while (1)
    {
      int32_t hxc_tmp_load_result_n10 = hxc_blockZ;
      if (!(hxc_tmp_load_result_n10 <= hxc_maximumZ))
      {
        break;
      }
      int32_t hxc_blockX = hxc_minimumX;
      while (1)
      {
        int32_t hxc_tmp_load_result_n13 = hxc_blockX;
        if (!(hxc_tmp_load_result_n13 <= hxc_maximumX))
        {
          break;
        }
        int32_t hxc_tmp_load_result_n15 = hxc_blockX;
        int32_t hxc_tmp_load_result_n16 = hxc_blockY;
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n18 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n15, hxc_tmp_load_result_n16, hxc_blockZ);
        struct hxc_caxecraft_domain_BlockCoord hxc_coord = hxc_tmp_call_result_n18;
        bool hxc_tmp_call_result_n20 = hxc_caxecraft_domain_World_contains(hxc_coord);
        bool hxc_tmp_short_circuit_result_n15 = !hxc_tmp_call_result_n20;
        if (!!hxc_tmp_call_result_n20)
        {
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n22 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_coord);
          bool hxc_tmp_call_result_n23 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n22);
          hxc_tmp_short_circuit_result_n15 = hxc_tmp_call_result_n23;
        }
        if (!hxc_tmp_short_circuit_result_n15)
        {
          hxc_blockX = hxc_i32_add_wrapping(hxc_blockX, 1);
          continue;
        }
        return true;
      }
      hxc_blockZ = hxc_i32_add_wrapping(hxc_blockZ, 1);
    }
    hxc_blockY = hxc_i32_add_wrapping(hxc_blockY, 1);
  }
  return false;
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_state)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  double hxc_recoveredY = hxc_state.hxc_y;
  int32_t hxc_attempts = 0;
  while (1)
  {
    bool hxc_tmp_call_result_n4 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_cells, hxc_length, hxc_state.hxc_x, hxc_recoveredY, hxc_state.hxc_z);
    bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_call_result_n4;
    if (hxc_tmp_call_result_n4)
    {
      hxc_tmp_short_circuit_result_n5 = hxc_attempts < 16;
    }
    if (!hxc_tmp_short_circuit_result_n5)
    {
      break;
    }
    hxc_recoveredY = hxc_recoveredY + 1.0;
    hxc_attempts = hxc_i32_add_wrapping(hxc_attempts, 1);
  }
  return (struct hxc_caxecraft_domain_CharacterBody){ .hxc_grounded = false, .hxc_velocityX = hxc_state.hxc_velocityX, .hxc_velocityY = 0.0, .hxc_velocityZ = hxc_state.hxc_velocityZ, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_recoveredY, .hxc_z = hxc_state.hxc_z };
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_resolveVelocity(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_state, double hxc_velocityX, double hxc_requestedVelocityY, double hxc_velocityZ)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n5 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n5;
  double hxc_velocityY = hxc_requestedVelocityY;
  struct hxc_caxecraft_domain_AxisMove hxc_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_moveAxis(hxc_cells, hxc_length, hxc_state.hxc_x, hxc_state.hxc_y, hxc_state.hxc_z, hxc_velocityX * 0.05, 0);
  struct hxc_caxecraft_domain_AxisMove hxc_movedX = hxc_tmp_call_result_n3;
  double hxc_tmp_record_field_load_result_n4 = hxc_movedX.hxc_x;
  double hxc_tmp_record_field_load_result_n5 = hxc_movedX.hxc_y;
  struct hxc_caxecraft_domain_AxisMove hxc_tmp_call_result_n7 = hxc_caxecraft_domain_CharacterPhysics_moveAxis(hxc_cells, hxc_length, hxc_tmp_record_field_load_result_n4, hxc_tmp_record_field_load_result_n5, hxc_movedX.hxc_z, hxc_velocityZ * 0.05, 2);
  struct hxc_caxecraft_domain_AxisMove hxc_movedZ = hxc_tmp_call_result_n7;
  double hxc_tmp_record_field_load_result_n8 = hxc_movedZ.hxc_x;
  double hxc_tmp_record_field_load_result_n9 = hxc_movedZ.hxc_y;
  double hxc_tmp_record_field_load_result_n10 = hxc_movedZ.hxc_z;
  struct hxc_caxecraft_domain_AxisMove hxc_tmp_call_result_n12 = hxc_caxecraft_domain_CharacterPhysics_moveAxis(hxc_cells, hxc_length, hxc_tmp_record_field_load_result_n8, hxc_tmp_record_field_load_result_n9, hxc_tmp_record_field_load_result_n10, hxc_velocityY * 0.05, 1);
  struct hxc_caxecraft_domain_AxisMove hxc_movedY = hxc_tmp_call_result_n12;
  bool hxc_tmp_record_field_load_result_n13 = hxc_movedY.hxc_blocked;
  bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_record_field_load_result_n13;
  if (hxc_tmp_record_field_load_result_n13)
  {
    hxc_tmp_short_circuit_result_n11 = hxc_velocityY <= 0.0;
  }
  bool hxc_grounded = hxc_tmp_short_circuit_result_n11;
  if (hxc_movedY.hxc_blocked)
  {
    hxc_velocityY = 0.0;
  }
  if (!hxc_grounded)
  {
    double hxc_tmp_record_field_load_result_n18 = hxc_movedY.hxc_x;
    double hxc_tmp_record_field_load_result_n19 = hxc_movedY.hxc_y;
    bool hxc_tmp_call_result_n21 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_cells, hxc_length, hxc_tmp_record_field_load_result_n18, hxc_tmp_record_field_load_result_n19 - 0.02, hxc_movedY.hxc_z);
    hxc_grounded = hxc_tmp_call_result_n21;
  }
  double hxc_finalVelocityX = hxc_velocityX;
  if (hxc_movedX.hxc_blocked)
  {
    hxc_finalVelocityX = 0.0;
  }
  double hxc_finalVelocityZ = hxc_velocityZ;
  if (hxc_movedZ.hxc_blocked)
  {
    hxc_finalVelocityZ = 0.0;
  }
  double hxc_tmp_record_field_load_result_n24 = hxc_movedY.hxc_x;
  double hxc_tmp_record_field_load_result_n25 = hxc_movedY.hxc_y;
  double hxc_tmp_record_field_load_result_n26 = hxc_movedY.hxc_z;
  double hxc_tmp_load_result_n27 = hxc_finalVelocityX;
  double hxc_tmp_load_result_n28 = hxc_velocityY;
  double hxc_tmp_load_result_n29 = hxc_finalVelocityZ;
  return (struct hxc_caxecraft_domain_CharacterBody){ .hxc_grounded = hxc_grounded, .hxc_velocityX = hxc_tmp_load_result_n27, .hxc_velocityY = hxc_tmp_load_result_n28, .hxc_velocityZ = hxc_tmp_load_result_n29, .hxc_x = hxc_tmp_record_field_load_result_n24, .hxc_y = hxc_tmp_record_field_load_result_n25, .hxc_z = hxc_tmp_record_field_load_result_n26 };
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_original, struct hxc_caxecraft_domain_StepInput hxc_command)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  struct hxc_caxecraft_domain_CharacterBody hxc_state = hxc_original;
  bool hxc_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_cells, hxc_length, hxc_original.hxc_x, hxc_original.hxc_y, hxc_original.hxc_z);
  if (hxc_tmp_call_result_n3)
  {
    struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n4 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_cells, hxc_length, hxc_original);
    hxc_state = hxc_tmp_call_result_n4;
  }
  double hxc_velocityX = hxc_command.hxc_moveX * 4.0;
  double hxc_velocityZ = hxc_command.hxc_moveZ * 4.0;
  bool hxc_tmp_record_field_project_n7 = hxc_command.hxc_jump;
  bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_record_field_project_n7;
  if (hxc_tmp_record_field_project_n7)
  {
    hxc_tmp_short_circuit_result_n8 = hxc_state.hxc_grounded;
  }
  bool hxc_tmp_short_circuit_load_result_n9 = hxc_tmp_short_circuit_result_n8;
  double hxc_tmp_conditional_result_n9 = 0.0;
  if (hxc_tmp_short_circuit_load_result_n9)
  {
    hxc_tmp_conditional_result_n9 = 6.5;
  }
  else
  {
    hxc_tmp_conditional_result_n9 = hxc_state.hxc_velocityY + -0.9;
  }
  double hxc_velocityY = hxc_tmp_conditional_result_n9;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_load_result_n12 = hxc_state;
  double hxc_tmp_load_result_n13 = hxc_velocityX;
  double hxc_tmp_load_result_n14 = hxc_velocityY;
  struct hxc_caxecraft_domain_CharacterBody hxc_tmp_call_result_n16 = hxc_caxecraft_domain_CharacterPhysics_resolveVelocity(hxc_cells, hxc_length, hxc_tmp_load_result_n12, hxc_tmp_load_result_n13, hxc_tmp_load_result_n14, hxc_velocityZ);
  return hxc_tmp_call_result_n16;
}
