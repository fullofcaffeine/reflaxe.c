#include "hxc/program.h"

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
    double hxc_l_tmp_load_result_n31 = hxc_l_currentX;
    double hxc_l_tmp_load_result_n32 = hxc_l_currentY;
    double hxc_l_tmp_load_result_n33 = hxc_l_currentZ;
    return (struct hxc_caxecraft_domain_AxisMove){ .hxc_blocked = hxc_l_blocked, .hxc_x = hxc_l_tmp_load_result_n31, .hxc_y = hxc_l_tmp_load_result_n32, .hxc_z = hxc_l_tmp_load_result_n33 };
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
