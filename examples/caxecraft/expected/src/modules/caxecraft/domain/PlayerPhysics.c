#include "hxc/program.h"

double hxc_caxecraft_domain_PlayerPhysics_abs(double hxc_value)
{
  if (!(hxc_value < 0.0))
  {
    return hxc_value;
  }
  return -hxc_value;
}

int32_t hxc_caxecraft_domain_PlayerPhysics_floorToInt(double hxc_value)
{
  int32_t hxc_truncated = hxc_f64_to_i32_saturating(hxc_value);
  if (!(hxc_value < (double)hxc_truncated))
  {
    return hxc_truncated;
  }
  return hxc_i32_subtract_wrapping(hxc_truncated, 1);
}

struct hxc_caxecraft_domain_StepInput hxc_caxecraft_domain_PlayerPhysics_input(double hxc_moveX, double hxc_moveZ, bool hxc_jump)
{
  return (struct hxc_caxecraft_domain_StepInput){ .hxc_jump = hxc_jump, .hxc_moveX = hxc_moveX, .hxc_moveZ = hxc_moveZ };
}

struct hxc_caxecraft_domain_AxisMove hxc_caxecraft_domain_PlayerPhysics_moveAxis(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z, double hxc_delta, int32_t hxc_axis)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n6 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n6;
  double hxc_tmp_call_result_n0 = hxc_caxecraft_domain_PlayerPhysics_abs(hxc_delta);
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
    bool hxc_tmp_call_result_n26 = hxc_caxecraft_domain_PlayerPhysics_overlaps(hxc_cells, hxc_length, hxc_tmp_load_result_n23, hxc_tmp_load_result_n24, hxc_candidateZ);
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

bool hxc_caxecraft_domain_PlayerPhysics_overlaps(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_PlayerPhysics_floorToInt(hxc_x - 0.30 + 0.0001);
  int32_t hxc_minimumX = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_domain_PlayerPhysics_floorToInt(hxc_x + 0.30 - 0.0001);
  int32_t hxc_maximumX = hxc_tmp_call_result_n1;
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_domain_PlayerPhysics_floorToInt(hxc_y + 0.0001);
  int32_t hxc_minimumY = hxc_tmp_call_result_n2;
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_domain_PlayerPhysics_floorToInt(hxc_y + 1.80 - 0.0001);
  int32_t hxc_maximumY = hxc_tmp_call_result_n3;
  int32_t hxc_tmp_call_result_n4 = hxc_caxecraft_domain_PlayerPhysics_floorToInt(hxc_z - 0.30 + 0.0001);
  int32_t hxc_minimumZ = hxc_tmp_call_result_n4;
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_domain_PlayerPhysics_floorToInt(hxc_z + 0.30 - 0.0001);
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

struct hxc_caxecraft_domain_PlayerState hxc_caxecraft_domain_PlayerPhysics_player(double hxc_x, double hxc_y, double hxc_z)
{
  return (struct hxc_caxecraft_domain_PlayerState){ .hxc_grounded = false, .hxc_velocityX = 0.0, .hxc_velocityY = 0.0, .hxc_velocityZ = 0.0, .hxc_x = hxc_x, .hxc_y = hxc_y, .hxc_z = hxc_z };
}

struct hxc_caxecraft_domain_PlayerState hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_PlayerState hxc_state)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  double hxc_recoveredY = hxc_state.hxc_y;
  int32_t hxc_attempts = 0;
  while (1)
  {
    bool hxc_tmp_call_result_n4 = hxc_caxecraft_domain_PlayerPhysics_overlaps(hxc_cells, hxc_length, hxc_state.hxc_x, hxc_recoveredY, hxc_state.hxc_z);
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
  return (struct hxc_caxecraft_domain_PlayerState){ .hxc_grounded = false, .hxc_velocityX = hxc_state.hxc_velocityX, .hxc_velocityY = 0.0, .hxc_velocityZ = hxc_state.hxc_velocityZ, .hxc_x = hxc_state.hxc_x, .hxc_y = hxc_recoveredY, .hxc_z = hxc_state.hxc_z };
}

struct hxc_caxecraft_domain_PlayerState hxc_caxecraft_domain_PlayerPhysics_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_PlayerState hxc_original, struct hxc_caxecraft_domain_StepInput hxc_command)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  struct hxc_caxecraft_domain_PlayerState hxc_state = hxc_original;
  bool hxc_tmp_call_result_n3 = hxc_caxecraft_domain_PlayerPhysics_overlaps(hxc_cells, hxc_length, hxc_original.hxc_x, hxc_original.hxc_y, hxc_original.hxc_z);
  if (hxc_tmp_call_result_n3)
  {
    struct hxc_caxecraft_domain_PlayerState hxc_tmp_call_result_n4 = hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(hxc_cells, hxc_length, hxc_original);
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
  double hxc_tmp_record_field_load_result_n12 = hxc_state.hxc_x;
  double hxc_tmp_record_field_load_result_n13 = hxc_state.hxc_y;
  double hxc_tmp_record_field_load_result_n14 = hxc_state.hxc_z;
  struct hxc_caxecraft_domain_AxisMove hxc_tmp_call_result_n16 = hxc_caxecraft_domain_PlayerPhysics_moveAxis(hxc_cells, hxc_length, hxc_tmp_record_field_load_result_n12, hxc_tmp_record_field_load_result_n13, hxc_tmp_record_field_load_result_n14, hxc_velocityX * 0.05, 0);
  struct hxc_caxecraft_domain_AxisMove hxc_movedX = hxc_tmp_call_result_n16;
  double hxc_tmp_record_field_load_result_n17 = hxc_movedX.hxc_x;
  double hxc_tmp_record_field_load_result_n18 = hxc_movedX.hxc_y;
  double hxc_tmp_record_field_load_result_n19 = hxc_movedX.hxc_z;
  struct hxc_caxecraft_domain_AxisMove hxc_tmp_call_result_n21 = hxc_caxecraft_domain_PlayerPhysics_moveAxis(hxc_cells, hxc_length, hxc_tmp_record_field_load_result_n17, hxc_tmp_record_field_load_result_n18, hxc_tmp_record_field_load_result_n19, hxc_velocityZ * 0.05, 2);
  struct hxc_caxecraft_domain_AxisMove hxc_movedZ = hxc_tmp_call_result_n21;
  double hxc_tmp_record_field_load_result_n22 = hxc_movedZ.hxc_x;
  double hxc_tmp_record_field_load_result_n23 = hxc_movedZ.hxc_y;
  double hxc_tmp_record_field_load_result_n24 = hxc_movedZ.hxc_z;
  struct hxc_caxecraft_domain_AxisMove hxc_tmp_call_result_n26 = hxc_caxecraft_domain_PlayerPhysics_moveAxis(hxc_cells, hxc_length, hxc_tmp_record_field_load_result_n22, hxc_tmp_record_field_load_result_n23, hxc_tmp_record_field_load_result_n24, hxc_velocityY * 0.05, 1);
  struct hxc_caxecraft_domain_AxisMove hxc_movedY = hxc_tmp_call_result_n26;
  bool hxc_tmp_record_field_load_result_n27 = hxc_movedY.hxc_blocked;
  bool hxc_tmp_short_circuit_result_n14 = hxc_tmp_record_field_load_result_n27;
  if (hxc_tmp_record_field_load_result_n27)
  {
    hxc_tmp_short_circuit_result_n14 = hxc_velocityY <= 0.0;
  }
  bool hxc_grounded = hxc_tmp_short_circuit_result_n14;
  if (hxc_movedY.hxc_blocked)
  {
    hxc_velocityY = 0.0;
  }
  if (!hxc_grounded)
  {
    double hxc_tmp_record_field_load_result_n32 = hxc_movedY.hxc_x;
    double hxc_tmp_record_field_load_result_n33 = hxc_movedY.hxc_y;
    bool hxc_tmp_call_result_n35 = hxc_caxecraft_domain_PlayerPhysics_overlaps(hxc_cells, hxc_length, hxc_tmp_record_field_load_result_n32, hxc_tmp_record_field_load_result_n33 - 0.02, hxc_movedY.hxc_z);
    hxc_grounded = hxc_tmp_call_result_n35;
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
  double hxc_tmp_record_field_load_result_n40 = hxc_movedY.hxc_x;
  double hxc_tmp_record_field_load_result_n41 = hxc_movedY.hxc_y;
  double hxc_tmp_record_field_load_result_n42 = hxc_movedY.hxc_z;
  double hxc_tmp_load_result_n43 = hxc_finalVelocityX;
  double hxc_tmp_load_result_n44 = hxc_velocityY;
  double hxc_tmp_load_result_n45 = hxc_finalVelocityZ;
  return (struct hxc_caxecraft_domain_PlayerState){ .hxc_grounded = hxc_grounded, .hxc_velocityX = hxc_tmp_load_result_n43, .hxc_velocityY = hxc_tmp_load_result_n44, .hxc_velocityZ = hxc_tmp_load_result_n45, .hxc_x = hxc_tmp_record_field_load_result_n40, .hxc_y = hxc_tmp_record_field_load_result_n41, .hxc_z = hxc_tmp_record_field_load_result_n42 };
}
