#include "hxc/program.h"

double hxc_caxecraft_app_WaterSurfaceGeometry_admittedSurface(struct hxc_caxecraft_domain_WaterCellState hxc_l_state, int32_t hxc_l_y)
{
  switch (hxc_l_state.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        double hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_WaterSurfaceGeometry_surfaceTop(hxc_l_state, hxc_l_y);
        return hxc_l_tmp_call_result_n0;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        double hxc_l_tmp_call_result_n1 = hxc_caxecraft_app_WaterSurfaceGeometry_surfaceTop(hxc_l_state, hxc_l_y);
        return hxc_l_tmp_call_result_n1;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return -1.0;
      }
    default:
      {
        abort();
      }
  }
  return -1.0;
}

double hxc_caxecraft_app_WaterSurfaceGeometry_cornerHeight(int32_t hxc_l_y, struct hxc_caxecraft_domain_WaterCellState hxc_l_first, struct hxc_caxecraft_domain_WaterCellState hxc_l_second, struct hxc_caxecraft_domain_WaterCellState hxc_l_third, struct hxc_caxecraft_domain_WaterCellState hxc_l_fourth)
{
  double hxc_l_sum = 0.0;
  int32_t hxc_l_count = 0;
  double hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_WaterSurfaceGeometry_admittedSurface(hxc_l_first, hxc_l_y);
  double hxc_l_firstHeight = hxc_l_tmp_call_result_n0;
  if (hxc_l_firstHeight >= 0.0)
  {
    double hxc_l_tmp_compound_load_result_n2 = hxc_l_sum;
    hxc_l_sum = hxc_l_tmp_compound_load_result_n2 + hxc_l_firstHeight;
    hxc_l_count = hxc_i32_add_wrapping(hxc_l_count, 1);
  }
  double hxc_l_tmp_call_result_n5 = hxc_caxecraft_app_WaterSurfaceGeometry_admittedSurface(hxc_l_second, hxc_l_y);
  double hxc_l_secondHeight = hxc_l_tmp_call_result_n5;
  if (hxc_l_secondHeight >= 0.0)
  {
    double hxc_l_tmp_compound_load_result_n7 = hxc_l_sum;
    hxc_l_sum = hxc_l_tmp_compound_load_result_n7 + hxc_l_secondHeight;
    hxc_l_count = hxc_i32_add_wrapping(hxc_l_count, 1);
  }
  double hxc_l_tmp_call_result_n10 = hxc_caxecraft_app_WaterSurfaceGeometry_admittedSurface(hxc_l_third, hxc_l_y);
  double hxc_l_thirdHeight = hxc_l_tmp_call_result_n10;
  if (hxc_l_thirdHeight >= 0.0)
  {
    double hxc_l_tmp_compound_load_result_n12 = hxc_l_sum;
    hxc_l_sum = hxc_l_tmp_compound_load_result_n12 + hxc_l_thirdHeight;
    hxc_l_count = hxc_i32_add_wrapping(hxc_l_count, 1);
  }
  double hxc_l_tmp_call_result_n15 = hxc_caxecraft_app_WaterSurfaceGeometry_admittedSurface(hxc_l_fourth, hxc_l_y);
  double hxc_l_fourthHeight = hxc_l_tmp_call_result_n15;
  if (hxc_l_fourthHeight >= 0.0)
  {
    double hxc_l_tmp_compound_load_result_n17 = hxc_l_sum;
    hxc_l_sum = hxc_l_tmp_compound_load_result_n17 + hxc_l_fourthHeight;
    hxc_l_count = hxc_i32_add_wrapping(hxc_l_count, 1);
  }
  if (!(hxc_l_count == 0))
  {
    double hxc_l_tmp_load_result_n21 = hxc_l_sum;
    return hxc_f64_divide_zero_safe(hxc_l_tmp_load_result_n21, (double)hxc_l_count);
  }
  return (double)hxc_l_y;
}

bool hxc_caxecraft_app_WaterSurfaceGeometry_isOpenTop(struct hxc_caxecraft_domain_WaterCellState hxc_l_state)
{
  switch (hxc_l_state.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Blocked:
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return false;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        return false;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
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

bool hxc_caxecraft_app_WaterSurfaceGeometry_isWater(struct hxc_caxecraft_domain_WaterCellState hxc_l_state)
{
  switch (hxc_l_state.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return true;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        return true;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return false;
      }
    default:
      {
        abort();
      }
  }
  return false;
}

bool hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(struct hxc_caxecraft_domain_WaterCellState hxc_l_state)
{
  switch (hxc_l_state.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Blocked:
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return false;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        return false;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
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

struct hxc_caxecraft_app_WaterSurfaceCorners hxc_caxecraft_app_WaterSurfaceGeometry_surfaceCorners(const uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n4 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n4;
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n0);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_l_x, 1), hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n2);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_i32_subtract_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n4);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n6 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_l_x, 1), hxc_l_y, hxc_i32_subtract_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n7 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n6);
  double hxc_l_tmp_call_result_n8 = hxc_caxecraft_app_WaterSurfaceGeometry_cornerHeight(hxc_l_y, hxc_l_tmp_call_result_n1, hxc_l_tmp_call_result_n3, hxc_l_tmp_call_result_n5, hxc_l_tmp_call_result_n7);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n9 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n10 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n9);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n11 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_l_x, 1), hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n12 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n11);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_i32_subtract_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n14 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n13);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n15 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_l_x, 1), hxc_l_y, hxc_i32_subtract_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n16 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n15);
  double hxc_l_tmp_call_result_n17 = hxc_caxecraft_app_WaterSurfaceGeometry_cornerHeight(hxc_l_y, hxc_l_tmp_call_result_n10, hxc_l_tmp_call_result_n12, hxc_l_tmp_call_result_n14, hxc_l_tmp_call_result_n16);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n18 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n19 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n18);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n20 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_l_x, 1), hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n21 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n20);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n22 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_i32_add_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n23 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n22);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n24 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_l_x, 1), hxc_l_y, hxc_i32_add_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n25 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n24);
  double hxc_l_tmp_call_result_n26 = hxc_caxecraft_app_WaterSurfaceGeometry_cornerHeight(hxc_l_y, hxc_l_tmp_call_result_n19, hxc_l_tmp_call_result_n21, hxc_l_tmp_call_result_n23, hxc_l_tmp_call_result_n25);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n27 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n28 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n27);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n29 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_l_x, 1), hxc_l_y, hxc_l_z);
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n30 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n29);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n31 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_i32_add_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n32 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n31);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n33 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_l_x, 1), hxc_l_y, hxc_i32_add_wrapping(hxc_l_z, 1));
  struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n34 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n33);
  double hxc_l_tmp_call_result_n35 = hxc_caxecraft_app_WaterSurfaceGeometry_cornerHeight(hxc_l_y, hxc_l_tmp_call_result_n28, hxc_l_tmp_call_result_n30, hxc_l_tmp_call_result_n32, hxc_l_tmp_call_result_n34);
  return (struct hxc_caxecraft_app_WaterSurfaceCorners){ .hxc_northEast = hxc_l_tmp_call_result_n17, .hxc_northWest = hxc_l_tmp_call_result_n8, .hxc_southEast = hxc_l_tmp_call_result_n26, .hxc_southWest = hxc_l_tmp_call_result_n35 };
}

double hxc_caxecraft_app_WaterSurfaceGeometry_surfaceTop(struct hxc_caxecraft_domain_WaterCellState hxc_l_state, int32_t hxc_l_y)
{
  int32_t hxc_l_distance = { 0 };
  switch (hxc_l_state.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        return (double)hxc_l_y;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return (double)hxc_l_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return (double)hxc_l_y;
      }
    default:
      {
        abort();
      }
  }
  if (hxc_l_state.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
  {
    abort();
  }
  int32_t hxc_l_tmp_enum_payload_project_n0 = hxc_l_state.hxc_payload.hxc_Flowing.hxc_level;
  int32_t hxc_l_level = hxc_l_tmp_enum_payload_project_n0;
  if (hxc_l_state.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
  {
    abort();
  }
  bool hxc_l_tmp_enum_payload_project_n1 = hxc_l_state.hxc_payload.hxc_Flowing.hxc_falling;
  bool hxc_l_falling = hxc_l_tmp_enum_payload_project_n1;
  if (!hxc_l_falling)
  {
    hxc_l_distance = hxc_l_level;
    return (double)hxc_l_y + hxc_f64_divide_zero_safe(8.0 - (double)hxc_l_distance, 8.0);
  }
  return (double)hxc_l_y + 1.0;
}
