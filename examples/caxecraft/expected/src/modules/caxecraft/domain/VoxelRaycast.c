#include "hxc/program.h"

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
