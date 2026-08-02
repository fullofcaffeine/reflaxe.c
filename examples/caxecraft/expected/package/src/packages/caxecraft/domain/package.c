#include "hxc/program.h"

int32_t hxc_caxecraft_domain_CaxecraftTrace_collisionTrace(void)
{
  uint8_t hxc_l_storage[16384] = { 0 };
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
  uint8_t hxc_l_storage[16384] = { 0 };
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
      if (!(hxc_l_x < 32))
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
  uint8_t hxc_l_storage[16384] = { 0 };
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
  uint8_t hxc_l_storage[16384] = { 0 };
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
  uint8_t hxc_l_storage[16384] = { 0 };
  uint8_t *hxc_l_cells = hxc_l_storage;
  size_t hxc_l_tmp_length_n1 = sizeof(hxc_l_storage) / sizeof(hxc_l_storage[0]);
  hxc_caxecraft_domain_World_generate(hxc_l_cells, hxc_l_tmp_length_n1, 324508639);
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_stateHash(hxc_l_cells, hxc_l_tmp_length_n1);
  return hxc_l_tmp_call_result_n0;
}

double hxc_caxecraft_domain_VoxelRaycast_abs(double hxc_l_value)
{
  if (!(hxc_l_value < 0.0))
  {
    return hxc_l_value;
  }
  return -hxc_l_value;
}

int32_t hxc_caxecraft_domain_VoxelRaycast_floorToInt(double hxc_l_value)
{
  int32_t hxc_l_truncated = hxc_f64_to_i32_saturating(hxc_l_value);
  if (!(hxc_l_value < (double)hxc_l_truncated))
  {
    return hxc_l_truncated;
  }
  return hxc_i32_subtract_wrapping(hxc_l_truncated, 1);
}

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_hit(int32_t hxc_l_cellX, int32_t hxc_l_cellY, int32_t hxc_l_cellZ, int32_t hxc_l_previousX, int32_t hxc_l_previousY, int32_t hxc_l_previousZ, int32_t hxc_l_normalX, int32_t hxc_l_normalY, int32_t hxc_l_normalZ, double hxc_l_distance, int32_t hxc_l_visited)
{
  return (struct hxc_caxecraft_domain_RaycastHit){ .hxc_cellX = hxc_l_cellX, .hxc_cellY = hxc_l_cellY, .hxc_cellZ = hxc_l_cellZ, .hxc_distance = hxc_l_distance, .hxc_hit = true, .hxc_normalX = hxc_l_normalX, .hxc_normalY = hxc_l_normalY, .hxc_normalZ = hxc_l_normalZ, .hxc_previousX = hxc_l_previousX, .hxc_previousY = hxc_l_previousY, .hxc_previousZ = hxc_l_previousZ, .hxc_visited = hxc_l_visited };
}

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_miss(int32_t hxc_l_previousX, int32_t hxc_l_previousY, int32_t hxc_l_previousZ, double hxc_l_distance, int32_t hxc_l_visited)
{
  return (struct hxc_caxecraft_domain_RaycastHit){ .hxc_cellX = 0, .hxc_cellY = 0, .hxc_cellZ = 0, .hxc_distance = hxc_l_distance, .hxc_hit = false, .hxc_normalX = 0, .hxc_normalY = 0, .hxc_normalZ = 0, .hxc_previousX = hxc_l_previousX, .hxc_previousY = hxc_l_previousY, .hxc_previousZ = hxc_l_previousZ, .hxc_visited = hxc_l_visited };
}

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_trace(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_originX, double hxc_l_originY, double hxc_l_originZ, double hxc_l_directionX, double hxc_l_directionY, double hxc_l_directionZ, double hxc_l_maximumDistance)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n8 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n8;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_VoxelRaycast_floorToInt(hxc_l_originX);
  int32_t hxc_l_cellX = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_VoxelRaycast_floorToInt(hxc_l_originY);
  int32_t hxc_l_cellY = hxc_l_tmp_call_result_n1;
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_VoxelRaycast_floorToInt(hxc_l_originZ);
  int32_t hxc_l_cellZ = hxc_l_tmp_call_result_n2;
  bool hxc_l_tmp_short_circuit_result_n12 = hxc_l_maximumDistance < 0.0;
  if (!(hxc_l_maximumDistance < 0.0))
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_cellX;
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_cellY;
    struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n3, hxc_l_tmp_load_result_n4, hxc_l_cellZ);
    bool hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_World_contains(hxc_l_tmp_call_result_n6);
    hxc_l_tmp_short_circuit_result_n12 = !hxc_l_tmp_call_result_n7;
  }
  if (hxc_l_tmp_short_circuit_result_n12)
  {
    int32_t hxc_l_tmp_load_result_n9 = hxc_l_cellX;
    int32_t hxc_l_tmp_load_result_n10 = hxc_l_cellY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n12 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_l_tmp_load_result_n9, hxc_l_tmp_load_result_n10, hxc_l_cellZ, 0.0, 0);
    return hxc_l_tmp_call_result_n12;
  }
  int32_t hxc_l_tmp_load_result_n13 = hxc_l_cellX;
  int32_t hxc_l_tmp_load_result_n14 = hxc_l_cellY;
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n16 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n13, hxc_l_tmp_load_result_n14, hxc_l_cellZ);
  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n17 = hxc_caxecraft_domain_WorldRead_query(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n16);
  bool hxc_l_tmp_call_result_n18 = hxc_caxecraft_domain_World_isSolid(hxc_l_tmp_call_result_n17);
  if (hxc_l_tmp_call_result_n18)
  {
    int32_t hxc_l_tmp_load_result_n19 = hxc_l_cellX;
    int32_t hxc_l_tmp_load_result_n20 = hxc_l_cellY;
    int32_t hxc_l_tmp_load_result_n21 = hxc_l_cellZ;
    int32_t hxc_l_tmp_load_result_n22 = hxc_l_cellX;
    int32_t hxc_l_tmp_load_result_n23 = hxc_l_cellY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n25 = hxc_caxecraft_domain_VoxelRaycast_hit(hxc_l_tmp_load_result_n19, hxc_l_tmp_load_result_n20, hxc_l_tmp_load_result_n21, hxc_l_tmp_load_result_n22, hxc_l_tmp_load_result_n23, hxc_l_cellZ, 0, 0, 0, 0.0, 1);
    return hxc_l_tmp_call_result_n25;
  }
  int32_t hxc_l_tmp_conditional_result_n14 = 0;
  if (hxc_l_directionX > 0.0)
  {
    hxc_l_tmp_conditional_result_n14 = 1;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n15 = 0;
    if (hxc_l_directionX < 0.0)
    {
      hxc_l_tmp_conditional_result_n15 = -1;
    }
    else
    {
      hxc_l_tmp_conditional_result_n15 = 0;
    }
    hxc_l_tmp_conditional_result_n14 = hxc_l_tmp_conditional_result_n15;
  }
  int32_t hxc_l_stepX = hxc_l_tmp_conditional_result_n14;
  int32_t hxc_l_tmp_conditional_result_n17 = 0;
  if (hxc_l_directionY > 0.0)
  {
    hxc_l_tmp_conditional_result_n17 = 1;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n18 = 0;
    if (hxc_l_directionY < 0.0)
    {
      hxc_l_tmp_conditional_result_n18 = -1;
    }
    else
    {
      hxc_l_tmp_conditional_result_n18 = 0;
    }
    hxc_l_tmp_conditional_result_n17 = hxc_l_tmp_conditional_result_n18;
  }
  int32_t hxc_l_stepY = hxc_l_tmp_conditional_result_n17;
  int32_t hxc_l_tmp_conditional_result_n20 = 0;
  if (hxc_l_directionZ > 0.0)
  {
    hxc_l_tmp_conditional_result_n20 = 1;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n21 = 0;
    if (hxc_l_directionZ < 0.0)
    {
      hxc_l_tmp_conditional_result_n21 = -1;
    }
    else
    {
      hxc_l_tmp_conditional_result_n21 = 0;
    }
    hxc_l_tmp_conditional_result_n20 = hxc_l_tmp_conditional_result_n21;
  }
  int32_t hxc_l_stepZ = hxc_l_tmp_conditional_result_n20;
  int32_t hxc_l_tmp_load_result_n32 = hxc_l_stepX;
  bool hxc_l_tmp_short_circuit_result_n22 = hxc_l_tmp_load_result_n32 == 0;
  if (hxc_l_tmp_load_result_n32 == 0)
  {
    hxc_l_tmp_short_circuit_result_n22 = hxc_l_stepY == 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n34 = hxc_l_tmp_short_circuit_result_n22;
  bool hxc_l_tmp_short_circuit_result_n23 = hxc_l_tmp_short_circuit_load_result_n34;
  if (hxc_l_tmp_short_circuit_load_result_n34)
  {
    hxc_l_tmp_short_circuit_result_n23 = hxc_l_stepZ == 0;
  }
  if (hxc_l_tmp_short_circuit_result_n23)
  {
    int32_t hxc_l_tmp_load_result_n37 = hxc_l_cellX;
    int32_t hxc_l_tmp_load_result_n38 = hxc_l_cellY;
    struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n40 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_l_tmp_load_result_n37, hxc_l_tmp_load_result_n38, hxc_l_cellZ, 0.0, 1);
    return hxc_l_tmp_call_result_n40;
  }
  int32_t hxc_l_tmp_load_result_n41 = hxc_l_stepX;
  double hxc_l_tmp_conditional_result_n25 = 0.0;
  if (hxc_l_tmp_load_result_n41 == 0)
  {
    hxc_l_tmp_conditional_result_n25 = 0.0;
  }
  else
  {
    double hxc_l_tmp_call_result_n42 = hxc_caxecraft_domain_VoxelRaycast_abs(hxc_f64_divide_zero_safe(1.0, hxc_l_directionX));
    hxc_l_tmp_conditional_result_n25 = hxc_l_tmp_call_result_n42;
  }
  double hxc_l_deltaX = hxc_l_tmp_conditional_result_n25;
  int32_t hxc_l_tmp_load_result_n44 = hxc_l_stepY;
  double hxc_l_tmp_conditional_result_n27 = 0.0;
  if (hxc_l_tmp_load_result_n44 == 0)
  {
    hxc_l_tmp_conditional_result_n27 = 0.0;
  }
  else
  {
    double hxc_l_tmp_call_result_n45 = hxc_caxecraft_domain_VoxelRaycast_abs(hxc_f64_divide_zero_safe(1.0, hxc_l_directionY));
    hxc_l_tmp_conditional_result_n27 = hxc_l_tmp_call_result_n45;
  }
  double hxc_l_deltaY = hxc_l_tmp_conditional_result_n27;
  int32_t hxc_l_tmp_load_result_n47 = hxc_l_stepZ;
  double hxc_l_tmp_conditional_result_n29 = 0.0;
  if (hxc_l_tmp_load_result_n47 == 0)
  {
    hxc_l_tmp_conditional_result_n29 = 0.0;
  }
  else
  {
    double hxc_l_tmp_call_result_n48 = hxc_caxecraft_domain_VoxelRaycast_abs(hxc_f64_divide_zero_safe(1.0, hxc_l_directionZ));
    hxc_l_tmp_conditional_result_n29 = hxc_l_tmp_call_result_n48;
  }
  double hxc_l_deltaZ = hxc_l_tmp_conditional_result_n29;
  int32_t hxc_l_tmp_load_result_n50 = hxc_l_stepX;
  double hxc_l_tmp_conditional_result_n31 = 0.0;
  if (hxc_l_tmp_load_result_n50 == 0)
  {
    hxc_l_tmp_conditional_result_n31 = 0.0;
  }
  else
  {
    int32_t hxc_l_tmp_load_result_n51 = hxc_l_stepX;
    int32_t hxc_l_tmp_conditional_result_n32 = 0;
    if (hxc_l_tmp_load_result_n51 > 0)
    {
      hxc_l_tmp_conditional_result_n32 = hxc_i32_add_wrapping(hxc_l_cellX, 1);
    }
    else
    {
      hxc_l_tmp_conditional_result_n32 = hxc_l_cellX;
    }
    hxc_l_tmp_conditional_result_n31 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_conditional_result_n32 - hxc_l_originX, hxc_l_directionX);
  }
  double hxc_l_maximumX = hxc_l_tmp_conditional_result_n31;
  int32_t hxc_l_tmp_load_result_n56 = hxc_l_stepY;
  double hxc_l_tmp_conditional_result_n34 = 0.0;
  if (hxc_l_tmp_load_result_n56 == 0)
  {
    hxc_l_tmp_conditional_result_n34 = 0.0;
  }
  else
  {
    int32_t hxc_l_tmp_load_result_n57 = hxc_l_stepY;
    int32_t hxc_l_tmp_conditional_result_n35 = 0;
    if (hxc_l_tmp_load_result_n57 > 0)
    {
      hxc_l_tmp_conditional_result_n35 = hxc_i32_add_wrapping(hxc_l_cellY, 1);
    }
    else
    {
      hxc_l_tmp_conditional_result_n35 = hxc_l_cellY;
    }
    hxc_l_tmp_conditional_result_n34 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_conditional_result_n35 - hxc_l_originY, hxc_l_directionY);
  }
  double hxc_l_maximumY = hxc_l_tmp_conditional_result_n34;
  int32_t hxc_l_tmp_load_result_n62 = hxc_l_stepZ;
  double hxc_l_tmp_conditional_result_n37 = 0.0;
  if (hxc_l_tmp_load_result_n62 == 0)
  {
    hxc_l_tmp_conditional_result_n37 = 0.0;
  }
  else
  {
    int32_t hxc_l_tmp_load_result_n63 = hxc_l_stepZ;
    int32_t hxc_l_tmp_conditional_result_n38 = 0;
    if (hxc_l_tmp_load_result_n63 > 0)
    {
      hxc_l_tmp_conditional_result_n38 = hxc_i32_add_wrapping(hxc_l_cellZ, 1);
    }
    else
    {
      hxc_l_tmp_conditional_result_n38 = hxc_l_cellZ;
    }
    hxc_l_tmp_conditional_result_n37 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_conditional_result_n38 - hxc_l_originZ, hxc_l_directionZ);
  }
  double hxc_l_maximumZ = hxc_l_tmp_conditional_result_n37;
  int32_t hxc_l_previousX = hxc_l_cellX;
  int32_t hxc_l_previousY = hxc_l_cellY;
  int32_t hxc_l_previousZ = hxc_l_cellZ;
  int32_t hxc_l_normalX = 0;
  int32_t hxc_l_normalY = 0;
  int32_t hxc_l_normalZ = 0;
  double hxc_l_distance = 0.0;
  int32_t hxc_l_visited = 1;
  while (1)
  {
    if (!(hxc_l_visited < 88))
    {
      break;
    }
    hxc_l_previousX = hxc_l_cellX;
    hxc_l_previousY = hxc_l_cellY;
    hxc_l_previousZ = hxc_l_cellZ;
    int32_t hxc_l_tmp_load_result_n75 = hxc_l_stepX;
    bool hxc_l_tmp_short_circuit_result_n47 = hxc_l_tmp_load_result_n75 != 0;
    if (hxc_l_tmp_load_result_n75 != 0)
    {
      int32_t hxc_l_tmp_load_result_n76 = hxc_l_stepY;
      bool hxc_l_tmp_short_circuit_result_n48 = hxc_l_tmp_load_result_n76 == 0;
      if (!(hxc_l_tmp_load_result_n76 == 0))
      {
        double hxc_l_tmp_load_result_n77 = hxc_l_maximumX;
        hxc_l_tmp_short_circuit_result_n48 = hxc_l_tmp_load_result_n77 <= hxc_l_maximumY;
      }
      hxc_l_tmp_short_circuit_result_n47 = hxc_l_tmp_short_circuit_result_n48;
    }
    bool hxc_l_tmp_short_circuit_load_result_n80 = hxc_l_tmp_short_circuit_result_n47;
    bool hxc_l_tmp_short_circuit_result_n49 = hxc_l_tmp_short_circuit_load_result_n80;
    if (hxc_l_tmp_short_circuit_load_result_n80)
    {
      int32_t hxc_l_tmp_load_result_n81 = hxc_l_stepZ;
      bool hxc_l_tmp_short_circuit_result_n50 = hxc_l_tmp_load_result_n81 == 0;
      if (!(hxc_l_tmp_load_result_n81 == 0))
      {
        double hxc_l_tmp_load_result_n82 = hxc_l_maximumX;
        hxc_l_tmp_short_circuit_result_n50 = hxc_l_tmp_load_result_n82 <= hxc_l_maximumZ;
      }
      hxc_l_tmp_short_circuit_result_n49 = hxc_l_tmp_short_circuit_result_n50;
    }
    if (hxc_l_tmp_short_circuit_result_n49)
    {
      int32_t hxc_l_tmp_compound_load_result_n86 = hxc_l_cellX;
      hxc_l_cellX = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n86, hxc_l_stepX);
      hxc_l_distance = hxc_l_maximumX;
      double hxc_l_tmp_compound_load_result_n89 = hxc_l_maximumX;
      hxc_l_maximumX = hxc_l_tmp_compound_load_result_n89 + hxc_l_deltaX;
      hxc_l_normalX = hxc_i32_negate_wrapping(hxc_l_stepX);
      hxc_l_normalY = 0;
      hxc_l_normalZ = 0;
    }
    else
    {
      int32_t hxc_l_tmp_load_result_n92 = hxc_l_stepY;
      bool hxc_l_tmp_short_circuit_result_n51 = hxc_l_tmp_load_result_n92 != 0;
      if (hxc_l_tmp_load_result_n92 != 0)
      {
        int32_t hxc_l_tmp_load_result_n93 = hxc_l_stepZ;
        bool hxc_l_tmp_short_circuit_result_n52 = hxc_l_tmp_load_result_n93 == 0;
        if (!(hxc_l_tmp_load_result_n93 == 0))
        {
          double hxc_l_tmp_load_result_n94 = hxc_l_maximumY;
          hxc_l_tmp_short_circuit_result_n52 = hxc_l_tmp_load_result_n94 <= hxc_l_maximumZ;
        }
        hxc_l_tmp_short_circuit_result_n51 = hxc_l_tmp_short_circuit_result_n52;
      }
      if (hxc_l_tmp_short_circuit_result_n51)
      {
        int32_t hxc_l_tmp_compound_load_result_n98 = hxc_l_cellY;
        hxc_l_cellY = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n98, hxc_l_stepY);
        hxc_l_distance = hxc_l_maximumY;
        double hxc_l_tmp_compound_load_result_n101 = hxc_l_maximumY;
        hxc_l_maximumY = hxc_l_tmp_compound_load_result_n101 + hxc_l_deltaY;
        hxc_l_normalX = 0;
        hxc_l_normalY = hxc_i32_negate_wrapping(hxc_l_stepY);
        hxc_l_normalZ = 0;
      }
      else
      {
        int32_t hxc_l_tmp_compound_load_result_n104 = hxc_l_cellZ;
        hxc_l_cellZ = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n104, hxc_l_stepZ);
        hxc_l_distance = hxc_l_maximumZ;
        double hxc_l_tmp_compound_load_result_n107 = hxc_l_maximumZ;
        hxc_l_maximumZ = hxc_l_tmp_compound_load_result_n107 + hxc_l_deltaZ;
        hxc_l_normalX = 0;
        hxc_l_normalY = 0;
        hxc_l_normalZ = hxc_i32_negate_wrapping(hxc_l_stepZ);
      }
    }
    hxc_l_visited = hxc_i32_add_wrapping(hxc_l_visited, 1);
    if (hxc_l_distance > hxc_l_maximumDistance)
    {
      int32_t hxc_l_tmp_load_result_n112 = hxc_l_previousX;
      int32_t hxc_l_tmp_load_result_n113 = hxc_l_previousY;
      int32_t hxc_l_tmp_load_result_n114 = hxc_l_previousZ;
      double hxc_l_tmp_load_result_n115 = hxc_l_distance;
      struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n117 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_l_tmp_load_result_n112, hxc_l_tmp_load_result_n113, hxc_l_tmp_load_result_n114, hxc_l_tmp_load_result_n115, hxc_l_visited);
      return hxc_l_tmp_call_result_n117;
    }
    int32_t hxc_l_tmp_load_result_n118 = hxc_l_cellX;
    int32_t hxc_l_tmp_load_result_n119 = hxc_l_cellY;
    struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n121 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n118, hxc_l_tmp_load_result_n119, hxc_l_cellZ);
    struct hxc_caxecraft_domain_BlockCoord hxc_l_coord = hxc_l_tmp_call_result_n121;
    bool hxc_l_tmp_call_result_n123 = hxc_caxecraft_domain_World_contains(hxc_l_coord);
    if (!hxc_l_tmp_call_result_n123)
    {
      int32_t hxc_l_tmp_load_result_n124 = hxc_l_previousX;
      int32_t hxc_l_tmp_load_result_n125 = hxc_l_previousY;
      int32_t hxc_l_tmp_load_result_n126 = hxc_l_previousZ;
      double hxc_l_tmp_load_result_n127 = hxc_l_distance;
      struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n129 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_l_tmp_load_result_n124, hxc_l_tmp_load_result_n125, hxc_l_tmp_load_result_n126, hxc_l_tmp_load_result_n127, hxc_l_visited);
      return hxc_l_tmp_call_result_n129;
    }
    enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n131 = hxc_caxecraft_domain_WorldRead_query(hxc_l_cells, hxc_l_length, hxc_l_coord);
    bool hxc_l_tmp_call_result_n132 = hxc_caxecraft_domain_World_isSolid(hxc_l_tmp_call_result_n131);
    if (hxc_l_tmp_call_result_n132)
    {
      int32_t hxc_l_tmp_load_result_n133 = hxc_l_cellX;
      int32_t hxc_l_tmp_load_result_n134 = hxc_l_cellY;
      int32_t hxc_l_tmp_load_result_n135 = hxc_l_cellZ;
      int32_t hxc_l_tmp_load_result_n136 = hxc_l_previousX;
      int32_t hxc_l_tmp_load_result_n137 = hxc_l_previousY;
      int32_t hxc_l_tmp_load_result_n138 = hxc_l_previousZ;
      int32_t hxc_l_tmp_load_result_n139 = hxc_l_normalX;
      int32_t hxc_l_tmp_load_result_n140 = hxc_l_normalY;
      int32_t hxc_l_tmp_load_result_n141 = hxc_l_normalZ;
      double hxc_l_tmp_load_result_n142 = hxc_l_distance;
      struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n144 = hxc_caxecraft_domain_VoxelRaycast_hit(hxc_l_tmp_load_result_n133, hxc_l_tmp_load_result_n134, hxc_l_tmp_load_result_n135, hxc_l_tmp_load_result_n136, hxc_l_tmp_load_result_n137, hxc_l_tmp_load_result_n138, hxc_l_tmp_load_result_n139, hxc_l_tmp_load_result_n140, hxc_l_tmp_load_result_n141, hxc_l_tmp_load_result_n142, hxc_l_visited);
      return hxc_l_tmp_call_result_n144;
    }
  }
  int32_t hxc_l_tmp_load_result_n145 = hxc_l_previousX;
  int32_t hxc_l_tmp_load_result_n146 = hxc_l_previousY;
  int32_t hxc_l_tmp_load_result_n147 = hxc_l_previousZ;
  double hxc_l_tmp_load_result_n148 = hxc_l_distance;
  struct hxc_caxecraft_domain_RaycastHit hxc_l_tmp_call_result_n150 = hxc_caxecraft_domain_VoxelRaycast_miss(hxc_l_tmp_load_result_n145, hxc_l_tmp_load_result_n146, hxc_l_tmp_load_result_n147, hxc_l_tmp_load_result_n148, hxc_l_visited);
  return hxc_l_tmp_call_result_n150;
}

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
    case hxc_caxecraft_domain_BlockKind_Sand:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
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
    case hxc_caxecraft_domain_BlockKind_Sand:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
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

int32_t hxc_caxecraft_domain_WorldStorage_readCode(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_index)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_borrow[(size_t)hxc_l_index];
  return (int32_t)hxc_l_tmp_collection_index_load_result_n0;
}

void hxc_caxecraft_domain_WorldStorage_writeCode(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_index, int32_t hxc_l_code)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n3)
  {
    abort();
  }
  hxc_l_borrow[(size_t)hxc_l_index] = (uint8_t)hxc_l_code;
  return;
}

double hxc_caxecraft_domain_CharacterPhysics_abs(double hxc_l_value)
{
  if (!(hxc_l_value < 0.0))
  {
    return hxc_l_value;
  }
  return -hxc_l_value;
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_body(double hxc_l_x, double hxc_l_y, double hxc_l_z)
{
  return (struct hxc_caxecraft_domain_CharacterBody){ .hxc_grounded = false, .hxc_velocityX = 0.0, .hxc_velocityY = 0.0, .hxc_velocityZ = 0.0, .hxc_x = hxc_l_x, .hxc_y = hxc_l_y, .hxc_z = hxc_l_z };
}

bool hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(struct hxc_caxecraft_domain_CharacterBody hxc_l_state, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
{
  double hxc_l_bodyMinimumX = hxc_l_state.hxc_x - 0.30;
  double hxc_l_bodyMaximumX = hxc_l_state.hxc_x + 0.30;
  double hxc_l_bodyMinimumY = hxc_l_state.hxc_y;
  double hxc_l_bodyMaximumY = hxc_l_state.hxc_y + 1.80;
  double hxc_l_bodyMinimumZ = hxc_l_state.hxc_z - 0.30;
  double hxc_l_bodyMaximumZ = hxc_l_state.hxc_z + 0.30;
  bool hxc_l_separated = false;
  if (hxc_l_bodyMaximumX <= (double)hxc_l_coord.hxc_x)
  {
    hxc_l_separated = true;
  }
  if (hxc_l_bodyMinimumX >= (double)hxc_i32_add_wrapping(hxc_l_coord.hxc_x, 1))
  {
    hxc_l_separated = true;
  }
  if (hxc_l_bodyMaximumY <= (double)hxc_l_coord.hxc_y)
  {
    hxc_l_separated = true;
  }
  if (hxc_l_bodyMinimumY >= (double)hxc_i32_add_wrapping(hxc_l_coord.hxc_y, 1))
  {
    hxc_l_separated = true;
  }
  if (hxc_l_bodyMaximumZ <= (double)hxc_l_coord.hxc_z)
  {
    hxc_l_separated = true;
  }
  if (hxc_l_bodyMinimumZ >= (double)hxc_i32_add_wrapping(hxc_l_coord.hxc_z, 1))
  {
    hxc_l_separated = true;
  }
  return hxc_l_separated;
}

int32_t hxc_caxecraft_domain_CharacterPhysics_floorToInt(double hxc_l_value)
{
  int32_t hxc_l_truncated = hxc_f64_to_i32_saturating(hxc_l_value);
  if (!(hxc_l_value < (double)hxc_l_truncated))
  {
    return hxc_l_truncated;
  }
  return hxc_i32_subtract_wrapping(hxc_l_truncated, 1);
}

struct hxc_caxecraft_domain_StepInput hxc_caxecraft_domain_CharacterPhysics_input(double hxc_l_moveX, double hxc_l_moveZ, bool hxc_l_jump)
{
  return (struct hxc_caxecraft_domain_StepInput){ .hxc_jump = hxc_l_jump, .hxc_moveX = hxc_l_moveX, .hxc_moveZ = hxc_l_moveZ };
}

struct hxc_caxecraft_domain_AxisMove hxc_caxecraft_domain_CharacterPhysics_moveAxis(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_x, double hxc_l_y, double hxc_l_z, double hxc_l_delta, int32_t hxc_l_axis)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n6 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n6;
  double hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_CharacterPhysics_abs(hxc_l_delta);
  double hxc_l_magnitude = hxc_l_tmp_call_result_n0;
  if (hxc_l_magnitude == 0.0)
  {
    return (struct hxc_caxecraft_domain_AxisMove){ .hxc_blocked = false, .hxc_x = hxc_l_x, .hxc_y = hxc_l_y, .hxc_z = hxc_l_z };
  }
  int32_t hxc_l_count = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe(hxc_l_magnitude, 0.20));
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_count;
  if ((double)hxc_l_tmp_load_result_n4 * 0.20 < hxc_l_magnitude)
  {
    hxc_l_count = hxc_i32_add_wrapping(hxc_l_count, 1);
  }
  if (hxc_l_count < 1)
  {
    hxc_l_count = 1;
  }
  double hxc_l_increment = hxc_f64_divide_zero_safe(hxc_l_delta, (double)hxc_l_count);
  double hxc_l_currentX = hxc_l_x;
  double hxc_l_currentY = hxc_l_y;
  double hxc_l_currentZ = hxc_l_z;
  bool hxc_l_blocked = false;
  int32_t hxc_l_stepIndex = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n9 = hxc_l_stepIndex;
    if (!(hxc_l_tmp_load_result_n9 < hxc_l_count))
    {
      break;
    }
    double hxc_l_tmp_conditional_result_n16 = 0.0;
    if (hxc_l_axis == 0)
    {
      double hxc_l_tmp_load_result_n11 = hxc_l_currentX;
      hxc_l_tmp_conditional_result_n16 = hxc_l_tmp_load_result_n11 + hxc_l_increment;
    }
    else
    {
      hxc_l_tmp_conditional_result_n16 = hxc_l_currentX;
    }
    double hxc_l_candidateX = hxc_l_tmp_conditional_result_n16;
    double hxc_l_tmp_conditional_result_n18 = 0.0;
    if (hxc_l_axis == 1)
    {
      double hxc_l_tmp_load_result_n15 = hxc_l_currentY;
      hxc_l_tmp_conditional_result_n18 = hxc_l_tmp_load_result_n15 + hxc_l_increment;
    }
    else
    {
      hxc_l_tmp_conditional_result_n18 = hxc_l_currentY;
    }
    double hxc_l_candidateY = hxc_l_tmp_conditional_result_n18;
    double hxc_l_tmp_conditional_result_n20 = 0.0;
    if (hxc_l_axis == 2)
    {
      double hxc_l_tmp_load_result_n19 = hxc_l_currentZ;
      hxc_l_tmp_conditional_result_n20 = hxc_l_tmp_load_result_n19 + hxc_l_increment;
    }
    else
    {
      hxc_l_tmp_conditional_result_n20 = hxc_l_currentZ;
    }
    double hxc_l_candidateZ = hxc_l_tmp_conditional_result_n20;
    double hxc_l_tmp_load_result_n23 = hxc_l_candidateX;
    double hxc_l_tmp_load_result_n24 = hxc_l_candidateY;
    bool hxc_l_tmp_call_result_n26 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_l_cells, hxc_l_length, hxc_l_tmp_load_result_n23, hxc_l_tmp_load_result_n24, hxc_l_candidateZ);
    if (!hxc_l_tmp_call_result_n26)
    {
      hxc_l_currentX = hxc_l_candidateX;
      hxc_l_currentY = hxc_l_candidateY;
      hxc_l_currentZ = hxc_l_candidateZ;
      hxc_l_stepIndex = hxc_i32_add_wrapping(hxc_l_stepIndex, 1);
      continue;
    }
    hxc_l_blocked = true;
    break;
  }
  double hxc_l_tmp_load_result_n31 = hxc_l_currentX;
  double hxc_l_tmp_load_result_n32 = hxc_l_currentY;
  double hxc_l_tmp_load_result_n33 = hxc_l_currentZ;
  return (struct hxc_caxecraft_domain_AxisMove){ .hxc_blocked = hxc_l_blocked, .hxc_x = hxc_l_tmp_load_result_n31, .hxc_y = hxc_l_tmp_load_result_n32, .hxc_z = hxc_l_tmp_load_result_n33 };
}

bool hxc_caxecraft_domain_CharacterPhysics_overlaps(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_x, double hxc_l_y, double hxc_l_z)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n4 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n4;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_l_x - 0.30 + 0.0001);
  int32_t hxc_l_minimumX = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_l_x + 0.30 - 0.0001);
  int32_t hxc_l_maximumX = hxc_l_tmp_call_result_n1;
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_l_y + 0.0001);
  int32_t hxc_l_minimumY = hxc_l_tmp_call_result_n2;
  int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_l_y + 1.80 - 0.0001);
  int32_t hxc_l_maximumY = hxc_l_tmp_call_result_n3;
  int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_l_z - 0.30 + 0.0001);
  int32_t hxc_l_minimumZ = hxc_l_tmp_call_result_n4;
  int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_CharacterPhysics_floorToInt(hxc_l_z + 0.30 - 0.0001);
  int32_t hxc_l_maximumZ = hxc_l_tmp_call_result_n5;
  int32_t hxc_l_blockY = hxc_l_minimumY;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n7 = hxc_l_blockY;
    if (!(hxc_l_tmp_load_result_n7 <= hxc_l_maximumY))
    {
      break;
    }
    int32_t hxc_l_blockZ = hxc_l_minimumZ;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n10 = hxc_l_blockZ;
      if (!(hxc_l_tmp_load_result_n10 <= hxc_l_maximumZ))
      {
        break;
      }
      int32_t hxc_l_blockX = hxc_l_minimumX;
      while (1)
      {
        int32_t hxc_l_tmp_load_result_n13 = hxc_l_blockX;
        if (!(hxc_l_tmp_load_result_n13 <= hxc_l_maximumX))
        {
          break;
        }
        int32_t hxc_l_tmp_load_result_n15 = hxc_l_blockX;
        int32_t hxc_l_tmp_load_result_n16 = hxc_l_blockY;
        struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n18 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n15, hxc_l_tmp_load_result_n16, hxc_l_blockZ);
        struct hxc_caxecraft_domain_BlockCoord hxc_l_coord = hxc_l_tmp_call_result_n18;
        bool hxc_l_tmp_call_result_n20 = hxc_caxecraft_domain_World_contains(hxc_l_coord);
        bool hxc_l_tmp_short_circuit_result_n15 = !hxc_l_tmp_call_result_n20;
        if (!!hxc_l_tmp_call_result_n20)
        {
          enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n22 = hxc_caxecraft_domain_WorldRead_query(hxc_l_cells, hxc_l_length, hxc_l_coord);
          bool hxc_l_tmp_call_result_n23 = hxc_caxecraft_domain_World_isSolid(hxc_l_tmp_call_result_n22);
          hxc_l_tmp_short_circuit_result_n15 = hxc_l_tmp_call_result_n23;
        }
        if (hxc_l_tmp_short_circuit_result_n15)
        {
          return true;
        }
        hxc_l_blockX = hxc_i32_add_wrapping(hxc_l_blockX, 1);
      }
      hxc_l_blockZ = hxc_i32_add_wrapping(hxc_l_blockZ, 1);
    }
    hxc_l_blockY = hxc_i32_add_wrapping(hxc_l_blockY, 1);
  }
  return false;
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_CharacterBody hxc_l_state)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  double hxc_l_recoveredY = hxc_l_state.hxc_y;
  int32_t hxc_l_attempts = 0;
  while (1)
  {
    bool hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_l_cells, hxc_l_length, hxc_l_state.hxc_x, hxc_l_recoveredY, hxc_l_state.hxc_z);
    bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_call_result_n4;
    if (hxc_l_tmp_call_result_n4)
    {
      hxc_l_tmp_short_circuit_result_n5 = hxc_l_attempts < 16;
    }
    if (!hxc_l_tmp_short_circuit_result_n5)
    {
      break;
    }
    hxc_l_recoveredY = hxc_l_recoveredY + 1.0;
    hxc_l_attempts = hxc_i32_add_wrapping(hxc_l_attempts, 1);
  }
  return (struct hxc_caxecraft_domain_CharacterBody){ .hxc_grounded = false, .hxc_velocityX = hxc_l_state.hxc_velocityX, .hxc_velocityY = 0.0, .hxc_velocityZ = hxc_l_state.hxc_velocityZ, .hxc_x = hxc_l_state.hxc_x, .hxc_y = hxc_l_recoveredY, .hxc_z = hxc_l_state.hxc_z };
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_resolveVelocity(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_CharacterBody hxc_l_state, double hxc_l_velocityX, double hxc_l_requestedVelocityY, double hxc_l_velocityZ)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n5 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n5;
  double hxc_l_velocityY = hxc_l_requestedVelocityY;
  struct hxc_caxecraft_domain_AxisMove hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_moveAxis(hxc_l_cells, hxc_l_length, hxc_l_state.hxc_x, hxc_l_state.hxc_y, hxc_l_state.hxc_z, hxc_l_velocityX * 0.05, 0);
  struct hxc_caxecraft_domain_AxisMove hxc_l_movedX = hxc_l_tmp_call_result_n3;
  double hxc_l_tmp_record_field_load_result_n4 = hxc_l_movedX.hxc_x;
  double hxc_l_tmp_record_field_load_result_n5 = hxc_l_movedX.hxc_y;
  struct hxc_caxecraft_domain_AxisMove hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_CharacterPhysics_moveAxis(hxc_l_cells, hxc_l_length, hxc_l_tmp_record_field_load_result_n4, hxc_l_tmp_record_field_load_result_n5, hxc_l_movedX.hxc_z, hxc_l_velocityZ * 0.05, 2);
  struct hxc_caxecraft_domain_AxisMove hxc_l_movedZ = hxc_l_tmp_call_result_n7;
  double hxc_l_tmp_record_field_load_result_n8 = hxc_l_movedZ.hxc_x;
  double hxc_l_tmp_record_field_load_result_n9 = hxc_l_movedZ.hxc_y;
  double hxc_l_tmp_record_field_load_result_n10 = hxc_l_movedZ.hxc_z;
  struct hxc_caxecraft_domain_AxisMove hxc_l_tmp_call_result_n12 = hxc_caxecraft_domain_CharacterPhysics_moveAxis(hxc_l_cells, hxc_l_length, hxc_l_tmp_record_field_load_result_n8, hxc_l_tmp_record_field_load_result_n9, hxc_l_tmp_record_field_load_result_n10, hxc_l_velocityY * 0.05, 1);
  struct hxc_caxecraft_domain_AxisMove hxc_l_movedY = hxc_l_tmp_call_result_n12;
  bool hxc_l_tmp_record_field_load_result_n13 = hxc_l_movedY.hxc_blocked;
  bool hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_record_field_load_result_n13;
  if (hxc_l_tmp_record_field_load_result_n13)
  {
    hxc_l_tmp_short_circuit_result_n11 = hxc_l_velocityY <= 0.0;
  }
  bool hxc_l_grounded = hxc_l_tmp_short_circuit_result_n11;
  if (hxc_l_movedY.hxc_blocked)
  {
    hxc_l_velocityY = 0.0;
  }
  if (!hxc_l_grounded)
  {
    double hxc_l_tmp_record_field_load_result_n18 = hxc_l_movedY.hxc_x;
    double hxc_l_tmp_record_field_load_result_n19 = hxc_l_movedY.hxc_y;
    bool hxc_l_tmp_call_result_n21 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_l_cells, hxc_l_length, hxc_l_tmp_record_field_load_result_n18, hxc_l_tmp_record_field_load_result_n19 - 0.02, hxc_l_movedY.hxc_z);
    hxc_l_grounded = hxc_l_tmp_call_result_n21;
  }
  double hxc_l_finalVelocityX = hxc_l_velocityX;
  if (hxc_l_movedX.hxc_blocked)
  {
    hxc_l_finalVelocityX = 0.0;
  }
  double hxc_l_finalVelocityZ = hxc_l_velocityZ;
  if (hxc_l_movedZ.hxc_blocked)
  {
    hxc_l_finalVelocityZ = 0.0;
  }
  double hxc_l_tmp_record_field_load_result_n24 = hxc_l_movedY.hxc_x;
  double hxc_l_tmp_record_field_load_result_n25 = hxc_l_movedY.hxc_y;
  double hxc_l_tmp_record_field_load_result_n26 = hxc_l_movedY.hxc_z;
  double hxc_l_tmp_load_result_n27 = hxc_l_finalVelocityX;
  double hxc_l_tmp_load_result_n28 = hxc_l_velocityY;
  double hxc_l_tmp_load_result_n29 = hxc_l_finalVelocityZ;
  return (struct hxc_caxecraft_domain_CharacterBody){ .hxc_grounded = hxc_l_grounded, .hxc_velocityX = hxc_l_tmp_load_result_n27, .hxc_velocityY = hxc_l_tmp_load_result_n28, .hxc_velocityZ = hxc_l_tmp_load_result_n29, .hxc_x = hxc_l_tmp_record_field_load_result_n24, .hxc_y = hxc_l_tmp_record_field_load_result_n25, .hxc_z = hxc_l_tmp_record_field_load_result_n26 };
}

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_step(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_CharacterBody hxc_l_original, struct hxc_caxecraft_domain_StepInput hxc_l_command)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n3;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_state = hxc_l_original;
  bool hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_CharacterPhysics_overlaps(hxc_l_cells, hxc_l_length, hxc_l_original.hxc_x, hxc_l_original.hxc_y, hxc_l_original.hxc_z);
  if (hxc_l_tmp_call_result_n3)
  {
    struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(hxc_l_cells, hxc_l_length, hxc_l_original);
    hxc_l_state = hxc_l_tmp_call_result_n4;
  }
  double hxc_l_velocityX = hxc_l_command.hxc_moveX * 4.0;
  double hxc_l_velocityZ = hxc_l_command.hxc_moveZ * 4.0;
  bool hxc_l_tmp_record_field_project_n7 = hxc_l_command.hxc_jump;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_record_field_project_n7;
  if (hxc_l_tmp_record_field_project_n7)
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_state.hxc_grounded;
  }
  bool hxc_l_tmp_short_circuit_load_result_n9 = hxc_l_tmp_short_circuit_result_n8;
  double hxc_l_tmp_conditional_result_n9 = 0.0;
  if (hxc_l_tmp_short_circuit_load_result_n9)
  {
    hxc_l_tmp_conditional_result_n9 = 6.5;
  }
  else
  {
    hxc_l_tmp_conditional_result_n9 = hxc_l_state.hxc_velocityY + -0.9;
  }
  double hxc_l_velocityY = hxc_l_tmp_conditional_result_n9;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_load_result_n12 = hxc_l_state;
  double hxc_l_tmp_load_result_n13 = hxc_l_velocityX;
  double hxc_l_tmp_load_result_n14 = hxc_l_velocityY;
  struct hxc_caxecraft_domain_CharacterBody hxc_l_tmp_call_result_n16 = hxc_caxecraft_domain_CharacterPhysics_resolveVelocity(hxc_l_cells, hxc_l_length, hxc_l_tmp_load_result_n12, hxc_l_tmp_load_result_n13, hxc_l_tmp_load_result_n14, hxc_l_velocityZ);
  return hxc_l_tmp_call_result_n16;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_WorldRead_query(const uint8_t *hxc_l_view, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
{
  const uint8_t *hxc_l_borrow = hxc_l_view;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_indexOf(hxc_l_coord);
  int32_t hxc_l_index = hxc_l_tmp_call_result_n0;
  if (!(hxc_l_index < 0))
  {
    int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_WorldRead_storageCodeAt(hxc_l_view, hxc_l_length, hxc_l_index);
    enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_World_kindFromCode(hxc_l_tmp_call_result_n4);
    return hxc_l_tmp_call_result_n5;
  }
  return hxc_caxecraft_domain_BlockKind_Air;
}

int32_t hxc_caxecraft_domain_WorldRead_storageCodeAt(const uint8_t *hxc_l_view, size_t hxc_l_length, int32_t hxc_l_index)
{
  const uint8_t *hxc_l_borrow = hxc_l_view;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_borrow[(size_t)hxc_l_index];
  return (int32_t)hxc_l_tmp_collection_index_load_result_n0;
}
