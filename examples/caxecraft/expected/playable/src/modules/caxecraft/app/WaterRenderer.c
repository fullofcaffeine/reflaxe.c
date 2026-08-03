#include "hxc/program.h"

struct hxc_caxecraft_app_WaterRenderCounters hxc_caxecraft_app_WaterRenderer_draw(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct Texture hxc_l_texture, bool hxc_l_textureReady, int32_t hxc_l_presentationCell)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n4 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = !hxc_l_textureReady;
  if (!!hxc_l_textureReady)
  {
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_presentationCell < 0;
  }
  if (hxc_l_tmp_short_circuit_result_n5)
  {
    return (struct hxc_caxecraft_app_WaterRenderCounters){ .hxc_drawCalls = 0, .hxc_visible = 0 };
  }
  int32_t hxc_l_column = hxc_i32_modulo_zero_safe(hxc_l_presentationCell, 4);
  int32_t hxc_l_row = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_presentationCell, (double)4));
  double hxc_l_halfPixel = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_l_texture.width);
  int32_t hxc_l_tmp_load_result_n3 = hxc_l_column;
  double hxc_l_u0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n3, (double)4) + hxc_l_halfPixel;
  int32_t hxc_l_tmp_load_result_n5 = hxc_l_column;
  double hxc_l_u1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n5, 1), (double)4) - hxc_l_halfPixel;
  int32_t hxc_l_tmp_load_result_n7 = hxc_l_row;
  double hxc_l_v0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n7, (double)4) + hxc_l_halfPixel;
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_row;
  double hxc_l_v1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n9, 1), (double)4) - hxc_l_halfPixel;
  int32_t hxc_l_visible = 0;
  int32_t hxc_l_faces = 0;
  struct Texture hxc_l_raw = hxc_l_texture;
  uint32_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw.id;
  uint32_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
  rlSetTexture(hxc_l_tmp_imported_field_load_result_n12);
  rlBegin((int32_t)7);
  int32_t hxc_l_z = 0;
  while (1)
  {
    if (!(hxc_l_z < 32))
    {
      break;
    }
    int32_t hxc_l_y = 0;
    while (1)
    {
      if (!(hxc_l_y < 16))
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
        int32_t hxc_l_tmp_load_result_n16 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n17 = hxc_l_y;
        struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n19 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n16, hxc_l_tmp_load_result_n17, hxc_l_z);
        struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n20 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n19);
        struct hxc_caxecraft_domain_WaterCellState hxc_l_state = hxc_l_tmp_call_result_n20;
        bool hxc_l_tmp_call_result_n22 = hxc_caxecraft_app_WaterSurfaceGeometry_isWater(hxc_l_state);
        if (hxc_l_tmp_call_result_n22)
        {
          int32_t hxc_l_tmp_load_result_n23 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n24 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n26 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n23, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n24, 1), hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n27 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n26);
          bool hxc_l_tmp_call_result_n28 = hxc_caxecraft_app_WaterSurfaceGeometry_isOpenTop(hxc_l_tmp_call_result_n27);
          bool hxc_l_topOpen = hxc_l_tmp_call_result_n28;
          int32_t hxc_l_tmp_load_result_n29 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n30 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n32 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n29, hxc_l_tmp_load_result_n30, hxc_i32_subtract_wrapping(hxc_l_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n33 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n32);
          bool hxc_l_tmp_call_result_n34 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n33);
          bool hxc_l_northOpen = hxc_l_tmp_call_result_n34;
          int32_t hxc_l_tmp_load_result_n35 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n36 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n38 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n35, hxc_l_tmp_load_result_n36, hxc_i32_add_wrapping(hxc_l_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n39 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n38);
          bool hxc_l_tmp_call_result_n40 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n39);
          bool hxc_l_southOpen = hxc_l_tmp_call_result_n40;
          int32_t hxc_l_tmp_load_result_n41 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n42 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n44 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n41, 1), hxc_l_tmp_load_result_n42, hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n45 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n44);
          bool hxc_l_tmp_call_result_n46 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n45);
          bool hxc_l_eastOpen = hxc_l_tmp_call_result_n46;
          int32_t hxc_l_tmp_load_result_n47 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n48 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n50 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n47, 1), hxc_l_tmp_load_result_n48, hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n51 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n50);
          bool hxc_l_tmp_call_result_n52 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n51);
          bool hxc_l_westOpen = hxc_l_tmp_call_result_n52;
          bool hxc_l_tmp_load_result_n53 = hxc_l_topOpen;
          bool hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_load_result_n53;
          if (!hxc_l_tmp_load_result_n53)
          {
            hxc_l_tmp_short_circuit_result_n25 = hxc_l_northOpen;
          }
          bool hxc_l_tmp_short_circuit_load_result_n55 = hxc_l_tmp_short_circuit_result_n25;
          bool hxc_l_tmp_short_circuit_result_n26 = hxc_l_tmp_short_circuit_load_result_n55;
          if (!hxc_l_tmp_short_circuit_load_result_n55)
          {
            hxc_l_tmp_short_circuit_result_n26 = hxc_l_southOpen;
          }
          bool hxc_l_tmp_short_circuit_load_result_n57 = hxc_l_tmp_short_circuit_result_n26;
          bool hxc_l_tmp_short_circuit_result_n27 = hxc_l_tmp_short_circuit_load_result_n57;
          if (!hxc_l_tmp_short_circuit_load_result_n57)
          {
            hxc_l_tmp_short_circuit_result_n27 = hxc_l_eastOpen;
          }
          bool hxc_l_tmp_short_circuit_load_result_n59 = hxc_l_tmp_short_circuit_result_n27;
          bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_short_circuit_load_result_n59;
          if (!hxc_l_tmp_short_circuit_load_result_n59)
          {
            hxc_l_tmp_short_circuit_result_n28 = hxc_l_westOpen;
          }
          if (hxc_l_tmp_short_circuit_result_n28)
          {
            int32_t hxc_l_tmp_load_result_n62 = hxc_l_x;
            int32_t hxc_l_tmp_load_result_n63 = hxc_l_y;
            struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_call_result_n65 = hxc_caxecraft_app_WaterSurfaceGeometry_surfaceCorners(hxc_l_cells, hxc_l_length, hxc_l_tmp_load_result_n62, hxc_l_tmp_load_result_n63, hxc_l_z);
            struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_corners = hxc_l_tmp_call_result_n65;
            if (hxc_l_topOpen)
            {
              int32_t hxc_l_tmp_load_result_n67 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n68 = hxc_l_z;
              struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_load_result_n69 = hxc_l_corners;
              double hxc_l_tmp_load_result_n70 = hxc_l_u0;
              double hxc_l_tmp_load_result_n71 = hxc_l_u1;
              double hxc_l_tmp_load_result_n72 = hxc_l_v0;
              hxc_caxecraft_app_WaterRenderer_emitTop((double)hxc_l_tmp_load_result_n67, (double)hxc_l_tmp_load_result_n68, hxc_l_tmp_load_result_n69, hxc_l_tmp_load_result_n70, hxc_l_tmp_load_result_n71, hxc_l_tmp_load_result_n72, hxc_l_v1);
              hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            }
            if (hxc_l_northOpen)
            {
              int32_t hxc_l_tmp_load_result_n76 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n77 = hxc_l_y;
              double hxc_l_tmp_record_field_load_result_n78 = hxc_l_corners.hxc_northWest;
              double hxc_l_tmp_record_field_load_result_n79 = hxc_l_corners.hxc_northEast;
              int32_t hxc_l_tmp_load_result_n80 = hxc_l_z;
              double hxc_l_tmp_load_result_n81 = hxc_l_u0;
              double hxc_l_tmp_load_result_n82 = hxc_l_u1;
              double hxc_l_tmp_load_result_n83 = hxc_l_v0;
              hxc_caxecraft_app_WaterRenderer_emitNorth((double)hxc_l_tmp_load_result_n76, (double)hxc_l_tmp_load_result_n77, hxc_l_tmp_record_field_load_result_n78, hxc_l_tmp_record_field_load_result_n79, (double)hxc_l_tmp_load_result_n80, hxc_l_tmp_load_result_n81, hxc_l_tmp_load_result_n82, hxc_l_tmp_load_result_n83, hxc_l_v1);
              hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            }
            if (hxc_l_southOpen)
            {
              int32_t hxc_l_tmp_load_result_n87 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n88 = hxc_l_y;
              double hxc_l_tmp_record_field_load_result_n89 = hxc_l_corners.hxc_southWest;
              double hxc_l_tmp_record_field_load_result_n90 = hxc_l_corners.hxc_southEast;
              int32_t hxc_l_tmp_load_result_n91 = hxc_l_z;
              double hxc_l_tmp_load_result_n92 = hxc_l_u0;
              double hxc_l_tmp_load_result_n93 = hxc_l_u1;
              double hxc_l_tmp_load_result_n94 = hxc_l_v0;
              hxc_caxecraft_app_WaterRenderer_emitSouth((double)hxc_l_tmp_load_result_n87, (double)hxc_l_tmp_load_result_n88, hxc_l_tmp_record_field_load_result_n89, hxc_l_tmp_record_field_load_result_n90, (double)hxc_l_tmp_load_result_n91, hxc_l_tmp_load_result_n92, hxc_l_tmp_load_result_n93, hxc_l_tmp_load_result_n94, hxc_l_v1);
              hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            }
            if (hxc_l_eastOpen)
            {
              int32_t hxc_l_tmp_load_result_n98 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n99 = hxc_l_y;
              double hxc_l_tmp_record_field_load_result_n100 = hxc_l_corners.hxc_northEast;
              double hxc_l_tmp_record_field_load_result_n101 = hxc_l_corners.hxc_southEast;
              int32_t hxc_l_tmp_load_result_n102 = hxc_l_z;
              double hxc_l_tmp_load_result_n103 = hxc_l_u0;
              double hxc_l_tmp_load_result_n104 = hxc_l_u1;
              double hxc_l_tmp_load_result_n105 = hxc_l_v0;
              hxc_caxecraft_app_WaterRenderer_emitEast((double)hxc_l_tmp_load_result_n98, (double)hxc_l_tmp_load_result_n99, hxc_l_tmp_record_field_load_result_n100, hxc_l_tmp_record_field_load_result_n101, (double)hxc_l_tmp_load_result_n102, hxc_l_tmp_load_result_n103, hxc_l_tmp_load_result_n104, hxc_l_tmp_load_result_n105, hxc_l_v1);
              hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            }
            if (hxc_l_westOpen)
            {
              int32_t hxc_l_tmp_load_result_n109 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n110 = hxc_l_y;
              double hxc_l_tmp_record_field_load_result_n111 = hxc_l_corners.hxc_northWest;
              double hxc_l_tmp_record_field_load_result_n112 = hxc_l_corners.hxc_southWest;
              int32_t hxc_l_tmp_load_result_n113 = hxc_l_z;
              double hxc_l_tmp_load_result_n114 = hxc_l_u0;
              double hxc_l_tmp_load_result_n115 = hxc_l_u1;
              double hxc_l_tmp_load_result_n116 = hxc_l_v0;
              hxc_caxecraft_app_WaterRenderer_emitWest((double)hxc_l_tmp_load_result_n109, (double)hxc_l_tmp_load_result_n110, hxc_l_tmp_record_field_load_result_n111, hxc_l_tmp_record_field_load_result_n112, (double)hxc_l_tmp_load_result_n113, hxc_l_tmp_load_result_n114, hxc_l_tmp_load_result_n115, hxc_l_tmp_load_result_n116, hxc_l_v1);
              hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            }
            hxc_l_visible = hxc_i32_add_wrapping(hxc_l_visible, 1);
          }
        }
        hxc_l_x = hxc_i32_add_wrapping(hxc_l_x, 1);
      }
      hxc_l_y = hxc_i32_add_wrapping(hxc_l_y, 1);
    }
    hxc_l_z = hxc_i32_add_wrapping(hxc_l_z, 1);
  }
  rlEnd();
  rlSetTexture((uint32_t)0);
  int32_t hxc_l_drawCalls = 0;
  if (hxc_l_faces > 0)
  {
    hxc_l_drawCalls = 1;
  }
  int32_t hxc_l_tmp_load_result_n124 = hxc_l_visible;
  return (struct hxc_caxecraft_app_WaterRenderCounters){ .hxc_drawCalls = hxc_l_drawCalls, .hxc_visible = hxc_l_tmp_load_result_n124 };
}

void hxc_caxecraft_app_WaterRenderer_emitEast(double hxc_l_x, double hxc_l_bottom, double hxc_l_topNorth, double hxc_l_topSouth, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  struct Color hxc_l_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_l_tmp_imported_field_address_n1 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n2 = *hxc_l_tmp_imported_field_address_n1;
  uint8_t *hxc_l_tmp_imported_field_address_n3 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n4 = *hxc_l_tmp_imported_field_address_n3;
  uint8_t *hxc_l_tmp_imported_field_address_n5 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n6 = *hxc_l_tmp_imported_field_address_n5;
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n2, hxc_l_tmp_imported_field_load_result_n4, hxc_l_tmp_imported_field_load_result_n6, hxc_l_tmp_imported_field_load_result_n8);
  rlNormal3f((float)1.0, (float)0.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x + 1.0, hxc_l_bottom, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x + 1.0, hxc_l_topNorth, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_topSouth, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0, hxc_l_bottom, hxc_l_z + 1.0);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitNorth(double hxc_l_x, double hxc_l_bottom, double hxc_l_topWest, double hxc_l_topEast, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  struct Color hxc_l_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_l_tmp_imported_field_address_n1 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n2 = *hxc_l_tmp_imported_field_address_n1;
  uint8_t *hxc_l_tmp_imported_field_address_n3 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n4 = *hxc_l_tmp_imported_field_address_n3;
  uint8_t *hxc_l_tmp_imported_field_address_n5 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n6 = *hxc_l_tmp_imported_field_address_n5;
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n2, hxc_l_tmp_imported_field_load_result_n4, hxc_l_tmp_imported_field_load_result_n6, hxc_l_tmp_imported_field_load_result_n8);
  rlNormal3f((float)0.0, (float)0.0, (float)-1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x, hxc_l_bottom, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_topWest, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_topEast, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0, hxc_l_bottom, hxc_l_z);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitSouth(double hxc_l_x, double hxc_l_bottom, double hxc_l_topWest, double hxc_l_topEast, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  struct Color hxc_l_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_l_tmp_imported_field_address_n1 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n2 = *hxc_l_tmp_imported_field_address_n1;
  uint8_t *hxc_l_tmp_imported_field_address_n3 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n4 = *hxc_l_tmp_imported_field_address_n3;
  uint8_t *hxc_l_tmp_imported_field_address_n5 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n6 = *hxc_l_tmp_imported_field_address_n5;
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n2, hxc_l_tmp_imported_field_load_result_n4, hxc_l_tmp_imported_field_load_result_n6, hxc_l_tmp_imported_field_load_result_n8);
  rlNormal3f((float)0.0, (float)0.0, (float)1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x, hxc_l_bottom, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0, hxc_l_bottom, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_topEast, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_topWest, hxc_l_z + 1.0);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitTop(double hxc_l_x, double hxc_l_z, struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_corners, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  struct Color hxc_l_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_l_tmp_imported_field_address_n1 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n2 = *hxc_l_tmp_imported_field_address_n1;
  uint8_t *hxc_l_tmp_imported_field_address_n3 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n4 = *hxc_l_tmp_imported_field_address_n3;
  uint8_t *hxc_l_tmp_imported_field_address_n5 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n6 = *hxc_l_tmp_imported_field_address_n5;
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n2, hxc_l_tmp_imported_field_load_result_n4, hxc_l_tmp_imported_field_load_result_n6, hxc_l_tmp_imported_field_load_result_n8);
  rlNormal3f((float)0.0, (float)1.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_corners.hxc_northWest, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x, hxc_l_corners.hxc_southWest, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0, hxc_l_corners.hxc_southEast, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_corners.hxc_northEast, hxc_l_z);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitWest(double hxc_l_x, double hxc_l_bottom, double hxc_l_topNorth, double hxc_l_topSouth, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  struct Color hxc_l_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_l_tmp_imported_field_address_n1 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n2 = *hxc_l_tmp_imported_field_address_n1;
  uint8_t *hxc_l_tmp_imported_field_address_n3 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n4 = *hxc_l_tmp_imported_field_address_n3;
  uint8_t *hxc_l_tmp_imported_field_address_n5 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n6 = *hxc_l_tmp_imported_field_address_n5;
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n2, hxc_l_tmp_imported_field_load_result_n4, hxc_l_tmp_imported_field_load_result_n6, hxc_l_tmp_imported_field_load_result_n8);
  rlNormal3f((float)-1.0, (float)0.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x, hxc_l_bottom, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x, hxc_l_bottom, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x, hxc_l_topSouth, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_topNorth, hxc_l_z);
  return;
}

void hxc_caxecraft_app_WaterRenderer_vertex(double hxc_l_u, double hxc_l_v, double hxc_l_x, double hxc_l_y, double hxc_l_z)
{
  rlTexCoord2f((float)hxc_l_u, (float)hxc_l_v);
  rlVertex3f((float)hxc_l_x, (float)hxc_l_y, (float)hxc_l_z);
  return;
}
