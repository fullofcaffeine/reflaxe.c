#include "hxc/program.h"

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
    if (!(hxc_l_visited < 120))
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
