#include "hxc/program.h"

int32_t hxc_caxecraft_domain_CaxecraftTrace_collisionTrace(void)
{
  uint8_t hxc_l_storage[32768] = { 0 };
  uint8_t *hxc_l_cells = hxc_l_storage;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  hxc_caxecraft_domain_CaxecraftTrace_makeFloor(hxc_l_cells, hxc_l_tmp_length_n1);
  const uint8_t *hxc_l_view = hxc_l_storage;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_CharacterPhysics_body(5.5, 1.0, 5.5);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_state = hxc_l_tmp_call_result_n0;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n1 = hxc_l_state;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_CharacterPhysics_input(1.0, 0.5, false);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n1, hxc_l_tmp_call_result_n2);
  hxc_l_state = hxc_l_tmp_call_result_n3;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n4 = hxc_l_state;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_CharacterPhysics_input(0.0, 0.0, true);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n2, hxc_l_tmp_load_result_n4, hxc_l_tmp_call_result_n5);
  hxc_l_state = hxc_l_tmp_call_result_n6;
  int32_t hxc_l_hash = hxc_f64_to_i32_saturating(hxc_l_state.hxc_x * 1000.0);
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n10 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n8, hxc_f64_to_i32_saturating(hxc_l_state.hxc_y * 1000.0));
  hxc_l_hash = hxc_l_tmp_call_result_n10;
  int32_t hxc_l_tmp_load_result_n11 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n11, hxc_f64_to_i32_saturating(hxc_l_state.hxc_z * 1000.0));
  hxc_l_hash = hxc_l_tmp_call_result_n13;
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n16 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n14, hxc_f64_to_i32_saturating(hxc_l_state.hxc_velocityY * 1000.0));
  hxc_l_hash = hxc_l_tmp_call_result_n16;
  int32_t hxc_l_groundedCode = 0;
  if (hxc_l_state.hxc_grounded)
  {
    hxc_l_groundedCode = 1;
  }
  int32_t hxc_l_tmp_load_result_n18 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n20 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n18, hxc_l_groundedCode);
  return hxc_l_tmp_call_result_n20;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_editTrace(void)
{
  uint8_t hxc_l_storage[32768] = { 0 };
  uint8_t *hxc_l_cells = hxc_l_storage;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  hxc_caxecraft_domain_World_generate(hxc_l_cells, hxc_l_tmp_length_n1, 324508639);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(4, 10, 4);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_target = hxc_l_tmp_call_result_n0;
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_World_coord(4, 9, 4);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_base = hxc_l_tmp_call_result_n1;
  hxc_caxecraft_domain_World_remove(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_base);
  hxc_caxecraft_domain_World_place(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_target, hxc_caxecraft_domain_BlockKind_Stone);
  hxc_caxecraft_domain_World_place(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_target, hxc_caxecraft_domain_BlockKind_Dirt);
  int32_t hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_World_stateHash(hxc_l_cells, hxc_l_tmp_length_n1);
  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_World_query(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_target);
  int32_t hxc_l_tmp_call_result_n10 = hxc_caxecraft_domain_World_kindCode(hxc_l_tmp_call_result_n9);
  int32_t hxc_l_tmp_call_result_n11 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_call_result_n7, hxc_l_tmp_call_result_n10);
  return hxc_l_tmp_call_result_n11;
}

void hxc_caxecraft_domain_CaxecraftTrace_makeFloor(uint8_t *hxc_l_cells, size_t hxc_l_length)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n1 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n1;
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
      if (!(hxc_l_x < 64))
      {
        break;
      }
      int32_t hxc_l_tmp_load_result_n2 = hxc_l_x;
      struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n2, 0, hxc_l_z);
      hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n4, hxc_caxecraft_domain_BlockKind_Stone);
      hxc_l_x = hxc_i32_add_wrapping(hxc_l_x, 1);
    }
    hxc_l_z = hxc_i32_add_wrapping(hxc_l_z, 1);
  }
  return;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_mix(int32_t hxc_l_hash, int32_t hxc_l_value)
{
  return hxc_i32_multiply_wrapping(hxc_i32_bit_xor(hxc_l_hash, hxc_l_value), 16777619);
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_propertyTrace(int32_t hxc_l_seed)
{
  uint8_t hxc_l_storage[32768] = { 0 };
  uint8_t *hxc_l_cells = hxc_l_storage;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  hxc_caxecraft_domain_World_generate(hxc_l_cells, hxc_l_tmp_length_n2, hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_seed, 1103515245), 12345));
  int32_t hxc_l_x = hxc_i32_bit_and(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_seed, 17), 5), 31);
  int32_t hxc_l_z = hxc_i32_bit_and(hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_seed, 29), 11), 31);
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_x;
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n0, 14, hxc_l_z);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_edit = hxc_l_tmp_call_result_n2;
  if (hxc_i32_bit_and(hxc_l_seed, 1) == 0)
  {
    hxc_caxecraft_domain_World_place(hxc_l_cells, hxc_l_tmp_length_n2, hxc_l_edit, hxc_caxecraft_domain_BlockKind_Dirt);
  }
  else
  {
    hxc_caxecraft_domain_World_place(hxc_l_cells, hxc_l_tmp_length_n2, hxc_l_edit, hxc_caxecraft_domain_BlockKind_Stone);
  }
  const uint8_t *hxc_l_view = hxc_l_storage;
  size_t hxc_l_tmp_length_n6 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_x;
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n6, (double)hxc_l_tmp_load_result_n7 + 0.5, 15.5, (double)hxc_l_z + 0.5, 0.25, -1.0, 0.5, 24.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_ray = hxc_l_tmp_call_result_n9;
  double hxc_l_tmp_conditional_result_n9 = 0.0;
  if (hxc_i32_bit_and(hxc_l_seed, 1) == 0)
  {
    hxc_l_tmp_conditional_result_n9 = 1.0;
  }
  else
  {
    hxc_l_tmp_conditional_result_n9 = -1.0;
  }
  double hxc_l_moveX = hxc_l_tmp_conditional_result_n9;
  double hxc_l_tmp_conditional_result_n11 = 0.0;
  if (hxc_i32_bit_and(hxc_l_seed, 2) == 0)
  {
    hxc_l_tmp_conditional_result_n11 = 0.5;
  }
  else
  {
    hxc_l_tmp_conditional_result_n11 = -0.5;
  }
  double hxc_l_moveZ = hxc_l_tmp_conditional_result_n11;
  bool hxc_l_jump = hxc_i32_bit_and(hxc_l_seed, 4) != 0;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n12 = hxc_caxecraft_domain_CharacterPhysics_body(1.5 + (double)hxc_i32_bit_and(hxc_l_seed, 3), 14.0, 1.5 + (double)hxc_i32_bit_and(hxc_i32_unsigned_shift_right_masked(hxc_l_seed, 2), 3));
  struct hxc_caxecraft_domain_CharacterBody hxc_l_state = hxc_l_tmp_call_result_n12;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n14 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_l_view, hxc_l_tmp_length_n6, hxc_l_state);
  hxc_l_state = hxc_l_tmp_call_result_n14;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n15 = hxc_l_state;
  double hxc_l_tmp_load_result_n16 = hxc_l_moveX;
  double hxc_l_tmp_load_result_n17 = hxc_l_moveZ;
  struct hxc_caxecraft_domain_StepInput hxc_l_tmp_call_result_n19 = hxc_caxecraft_domain_CharacterPhysics_input(hxc_l_tmp_load_result_n16, hxc_l_tmp_load_result_n17, hxc_l_jump);
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n20 = hxc_caxecraft_domain_CharacterPhysics_step(hxc_l_view, hxc_l_tmp_length_n6, hxc_l_tmp_load_result_n15, hxc_l_tmp_call_result_n19);
  hxc_l_state = hxc_l_tmp_call_result_n20;
  int32_t hxc_l_tmp_call_result_n21 = hxc_caxecraft_domain_World_stateHash(hxc_l_cells, hxc_l_tmp_length_n2);
  int32_t hxc_l_hash = hxc_l_tmp_call_result_n21;
  int32_t hxc_l_hitCode = 0;
  if (hxc_l_ray.hxc_hit)
  {
    hxc_l_hitCode = 1;
  }
  int32_t hxc_l_tmp_load_result_n23 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n25 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n23, hxc_l_hitCode);
  hxc_l_hash = hxc_l_tmp_call_result_n25;
  int32_t hxc_l_tmp_load_result_n26 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n28 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n26, hxc_l_ray.hxc_cellX);
  hxc_l_hash = hxc_l_tmp_call_result_n28;
  int32_t hxc_l_tmp_load_result_n29 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n31 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n29, hxc_l_ray.hxc_cellY);
  hxc_l_hash = hxc_l_tmp_call_result_n31;
  int32_t hxc_l_tmp_load_result_n32 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n34 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n32, hxc_l_ray.hxc_cellZ);
  hxc_l_hash = hxc_l_tmp_call_result_n34;
  int32_t hxc_l_tmp_load_result_n35 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n37 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n35, hxc_l_ray.hxc_visited);
  hxc_l_hash = hxc_l_tmp_call_result_n37;
  int32_t hxc_l_tmp_load_result_n38 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n40 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n38, hxc_f64_to_i32_saturating(hxc_l_state.hxc_x * 1000.0));
  hxc_l_hash = hxc_l_tmp_call_result_n40;
  int32_t hxc_l_tmp_load_result_n41 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n43 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n41, hxc_f64_to_i32_saturating(hxc_l_state.hxc_y * 1000.0));
  hxc_l_hash = hxc_l_tmp_call_result_n43;
  int32_t hxc_l_tmp_load_result_n44 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n46 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n44, hxc_f64_to_i32_saturating(hxc_l_state.hxc_z * 1000.0));
  return hxc_l_tmp_call_result_n46;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_rayTrace(void)
{
  uint8_t hxc_l_storage[32768] = { 0 };
  uint8_t *hxc_l_cells = hxc_l_storage;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(8, 8, 8);
  hxc_caxecraft_domain_World_replace(hxc_l_cells, hxc_l_tmp_length_n1, hxc_l_tmp_call_result_n0, hxc_caxecraft_domain_BlockKind_Stone);
  const uint8_t *hxc_l_view = hxc_l_storage;
  size_t hxc_l_tmp_length_n2 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_VoxelRaycast_trace(hxc_l_view, hxc_l_tmp_length_n2, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 12.0);
  struct hxc_caxecraft_domain_RaycastHit hxc_l_hit = hxc_l_tmp_call_result_n2;
  bool hxc_l_tmp_record_field_load_result_n3 = hxc_l_hit.hxc_hit;
  int32_t hxc_l_tmp_conditional_result_n5 = 0;
  if (hxc_l_tmp_record_field_load_result_n3)
  {
    hxc_l_tmp_conditional_result_n5 = 1;
  }
  else
  {
    hxc_l_tmp_conditional_result_n5 = 0;
  }
  int32_t hxc_l_hash = hxc_l_tmp_conditional_result_n5;
  int32_t hxc_l_tmp_load_result_n5 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n5, hxc_l_hit.hxc_cellX);
  hxc_l_hash = hxc_l_tmp_call_result_n7;
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n10 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n8, hxc_l_hit.hxc_previousX);
  hxc_l_hash = hxc_l_tmp_call_result_n10;
  int32_t hxc_l_tmp_load_result_n11 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n11, hxc_l_hit.hxc_normalX);
  hxc_l_hash = hxc_l_tmp_call_result_n13;
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n16 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n14, hxc_f64_to_i32_saturating(hxc_l_hit.hxc_distance * 1000.0));
  hxc_l_hash = hxc_l_tmp_call_result_n16;
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n19 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n17, hxc_l_hit.hxc_visited);
  return hxc_l_tmp_call_result_n19;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_runTrace(void)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_CaxecraftTrace_terrainTrace();
  int32_t hxc_l_hash = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_CaxecraftTrace_editTrace();
  int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n1, hxc_l_tmp_call_result_n2);
  hxc_l_hash = hxc_l_tmp_call_result_n3;
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_CaxecraftTrace_rayTrace();
  int32_t hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n4, hxc_l_tmp_call_result_n5);
  hxc_l_hash = hxc_l_tmp_call_result_n6;
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_hash;
  int32_t hxc_l_tmp_call_result_n8 = hxc_caxecraft_domain_CaxecraftTrace_collisionTrace();
  int32_t hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n7, hxc_l_tmp_call_result_n8);
  hxc_l_hash = hxc_l_tmp_call_result_n9;
  int32_t hxc_l_seed = 0;
  while (1)
  {
    if (!(hxc_l_seed < 32))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n11 = hxc_l_hash;
    int32_t hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_CaxecraftTrace_propertyTrace(hxc_l_seed);
    int32_t hxc_l_tmp_call_result_n14 = hxc_caxecraft_domain_CaxecraftTrace_mix(hxc_l_tmp_load_result_n11, hxc_l_tmp_call_result_n13);
    hxc_l_hash = hxc_l_tmp_call_result_n14;
    hxc_l_seed = hxc_i32_add_wrapping(hxc_l_seed, 1);
  }
  return hxc_l_hash;
}

int32_t hxc_caxecraft_domain_CaxecraftTrace_terrainTrace(void)
{
  uint8_t hxc_l_storage[32768] = { 0 };
  uint8_t *hxc_l_cells = hxc_l_storage;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  hxc_caxecraft_domain_World_generate(hxc_l_cells, hxc_l_tmp_length_n1, 324508639);
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_stateHash(hxc_l_cells, hxc_l_tmp_length_n1);
  return hxc_l_tmp_call_result_n0;
}
