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
        bool hxc_l_tmp_call_result_n22 = hxc_caxecraft_app_WaterRenderer_isWater(hxc_l_state);
        if (hxc_l_tmp_call_result_n22)
        {
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_load_result_n23 = hxc_l_state;
          double hxc_l_tmp_call_result_n25 = hxc_caxecraft_app_WaterRenderer_surfaceTop(hxc_l_tmp_load_result_n23, hxc_l_y);
          double hxc_l_top = hxc_l_tmp_call_result_n25;
          bool hxc_l_cellVisible = false;
          int32_t hxc_l_tmp_load_result_n26 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n27 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n29 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n26, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n27, 1), hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n30 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n29);
          bool hxc_l_tmp_call_result_n31 = hxc_caxecraft_app_WaterRenderer_isOpenAir(hxc_l_tmp_call_result_n30);
          if (hxc_l_tmp_call_result_n31)
          {
            int32_t hxc_l_tmp_load_result_n32 = hxc_l_x;
            double hxc_l_tmp_load_result_n33 = hxc_l_top;
            int32_t hxc_l_tmp_load_result_n34 = hxc_l_z;
            double hxc_l_tmp_load_result_n35 = hxc_l_u0;
            double hxc_l_tmp_load_result_n36 = hxc_l_u1;
            double hxc_l_tmp_load_result_n37 = hxc_l_v0;
            hxc_caxecraft_app_WaterRenderer_emitTop((double)hxc_l_tmp_load_result_n32, hxc_l_tmp_load_result_n33, (double)hxc_l_tmp_load_result_n34, hxc_l_tmp_load_result_n35, hxc_l_tmp_load_result_n36, hxc_l_tmp_load_result_n37, hxc_l_v1);
            hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            hxc_l_cellVisible = true;
          }
          int32_t hxc_l_tmp_load_result_n40 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n41 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n43 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n40, hxc_l_tmp_load_result_n41, hxc_i32_subtract_wrapping(hxc_l_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n44 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n43);
          int32_t hxc_l_tmp_load_result_n45 = hxc_l_y;
          double hxc_l_tmp_call_result_n47 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_l_tmp_call_result_n44, hxc_l_tmp_load_result_n45, hxc_l_top);
          double hxc_l_northBottom = hxc_l_tmp_call_result_n47;
          double hxc_l_tmp_load_result_n48 = hxc_l_northBottom;
          if (hxc_l_tmp_load_result_n48 < hxc_l_top)
          {
            int32_t hxc_l_tmp_load_result_n50 = hxc_l_x;
            double hxc_l_tmp_load_result_n51 = hxc_l_northBottom;
            double hxc_l_tmp_load_result_n52 = hxc_l_top;
            int32_t hxc_l_tmp_load_result_n53 = hxc_l_z;
            double hxc_l_tmp_load_result_n54 = hxc_l_u0;
            double hxc_l_tmp_load_result_n55 = hxc_l_u1;
            double hxc_l_tmp_load_result_n56 = hxc_l_v0;
            hxc_caxecraft_app_WaterRenderer_emitNorth((double)hxc_l_tmp_load_result_n50, hxc_l_tmp_load_result_n51, hxc_l_tmp_load_result_n52, (double)hxc_l_tmp_load_result_n53, hxc_l_tmp_load_result_n54, hxc_l_tmp_load_result_n55, hxc_l_tmp_load_result_n56, hxc_l_v1);
            hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            hxc_l_cellVisible = true;
          }
          int32_t hxc_l_tmp_load_result_n59 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n60 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n62 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n59, hxc_l_tmp_load_result_n60, hxc_i32_add_wrapping(hxc_l_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n63 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n62);
          int32_t hxc_l_tmp_load_result_n64 = hxc_l_y;
          double hxc_l_tmp_call_result_n66 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_l_tmp_call_result_n63, hxc_l_tmp_load_result_n64, hxc_l_top);
          double hxc_l_southBottom = hxc_l_tmp_call_result_n66;
          double hxc_l_tmp_load_result_n67 = hxc_l_southBottom;
          if (hxc_l_tmp_load_result_n67 < hxc_l_top)
          {
            int32_t hxc_l_tmp_load_result_n69 = hxc_l_x;
            double hxc_l_tmp_load_result_n70 = hxc_l_southBottom;
            double hxc_l_tmp_load_result_n71 = hxc_l_top;
            int32_t hxc_l_tmp_load_result_n72 = hxc_l_z;
            double hxc_l_tmp_load_result_n73 = hxc_l_u0;
            double hxc_l_tmp_load_result_n74 = hxc_l_u1;
            double hxc_l_tmp_load_result_n75 = hxc_l_v0;
            hxc_caxecraft_app_WaterRenderer_emitSouth((double)hxc_l_tmp_load_result_n69, hxc_l_tmp_load_result_n70, hxc_l_tmp_load_result_n71, (double)hxc_l_tmp_load_result_n72, hxc_l_tmp_load_result_n73, hxc_l_tmp_load_result_n74, hxc_l_tmp_load_result_n75, hxc_l_v1);
            hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            hxc_l_cellVisible = true;
          }
          int32_t hxc_l_tmp_load_result_n78 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n79 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n81 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n78, 1), hxc_l_tmp_load_result_n79, hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n82 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n81);
          int32_t hxc_l_tmp_load_result_n83 = hxc_l_y;
          double hxc_l_tmp_call_result_n85 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_l_tmp_call_result_n82, hxc_l_tmp_load_result_n83, hxc_l_top);
          double hxc_l_eastBottom = hxc_l_tmp_call_result_n85;
          double hxc_l_tmp_load_result_n86 = hxc_l_eastBottom;
          if (hxc_l_tmp_load_result_n86 < hxc_l_top)
          {
            int32_t hxc_l_tmp_load_result_n88 = hxc_l_x;
            double hxc_l_tmp_load_result_n89 = hxc_l_eastBottom;
            double hxc_l_tmp_load_result_n90 = hxc_l_top;
            int32_t hxc_l_tmp_load_result_n91 = hxc_l_z;
            double hxc_l_tmp_load_result_n92 = hxc_l_u0;
            double hxc_l_tmp_load_result_n93 = hxc_l_u1;
            double hxc_l_tmp_load_result_n94 = hxc_l_v0;
            hxc_caxecraft_app_WaterRenderer_emitEast((double)hxc_l_tmp_load_result_n88, hxc_l_tmp_load_result_n89, hxc_l_tmp_load_result_n90, (double)hxc_l_tmp_load_result_n91, hxc_l_tmp_load_result_n92, hxc_l_tmp_load_result_n93, hxc_l_tmp_load_result_n94, hxc_l_v1);
            hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            hxc_l_cellVisible = true;
          }
          int32_t hxc_l_tmp_load_result_n97 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n98 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n100 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n97, 1), hxc_l_tmp_load_result_n98, hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n101 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n100);
          int32_t hxc_l_tmp_load_result_n102 = hxc_l_y;
          double hxc_l_tmp_call_result_n104 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_l_tmp_call_result_n101, hxc_l_tmp_load_result_n102, hxc_l_top);
          double hxc_l_westBottom = hxc_l_tmp_call_result_n104;
          double hxc_l_tmp_load_result_n105 = hxc_l_westBottom;
          if (hxc_l_tmp_load_result_n105 < hxc_l_top)
          {
            int32_t hxc_l_tmp_load_result_n107 = hxc_l_x;
            double hxc_l_tmp_load_result_n108 = hxc_l_westBottom;
            double hxc_l_tmp_load_result_n109 = hxc_l_top;
            int32_t hxc_l_tmp_load_result_n110 = hxc_l_z;
            double hxc_l_tmp_load_result_n111 = hxc_l_u0;
            double hxc_l_tmp_load_result_n112 = hxc_l_u1;
            double hxc_l_tmp_load_result_n113 = hxc_l_v0;
            hxc_caxecraft_app_WaterRenderer_emitWest((double)hxc_l_tmp_load_result_n107, hxc_l_tmp_load_result_n108, hxc_l_tmp_load_result_n109, (double)hxc_l_tmp_load_result_n110, hxc_l_tmp_load_result_n111, hxc_l_tmp_load_result_n112, hxc_l_tmp_load_result_n113, hxc_l_v1);
            hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
            hxc_l_cellVisible = true;
          }
          if (hxc_l_cellVisible)
          {
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
  int32_t hxc_l_tmp_load_result_n122 = hxc_l_visible;
  return (struct hxc_caxecraft_app_WaterRenderCounters){ .hxc_drawCalls = hxc_l_drawCalls, .hxc_visible = hxc_l_tmp_load_result_n122 };
}

void hxc_caxecraft_app_WaterRenderer_emitEast(double hxc_l_x, double hxc_l_bottom, double hxc_l_top, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
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
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x + 1.0, hxc_l_top, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_top, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0, hxc_l_bottom, hxc_l_z + 1.0);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitNorth(double hxc_l_x, double hxc_l_bottom, double hxc_l_top, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
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
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_top, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_top, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0, hxc_l_bottom, hxc_l_z);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitSouth(double hxc_l_x, double hxc_l_bottom, double hxc_l_top, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
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
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_top, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_top, hxc_l_z + 1.0);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitTop(double hxc_l_x, double hxc_l_y, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
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
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_y, hxc_l_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x, hxc_l_y, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0, hxc_l_y, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0, hxc_l_y, hxc_l_z);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitWest(double hxc_l_x, double hxc_l_bottom, double hxc_l_top, double hxc_l_z, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
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
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x, hxc_l_top, hxc_l_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x, hxc_l_top, hxc_l_z);
  return;
}

double hxc_caxecraft_app_WaterRenderer_exposedSideBottom(struct hxc_caxecraft_domain_WaterCellState hxc_l_neighbor, int32_t hxc_l_y, double hxc_l_top)
{
  switch (hxc_l_neighbor.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        return hxc_l_top;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return (double)hxc_l_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        double hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_WaterRenderer_surfaceTop(hxc_l_neighbor, hxc_l_y);
        return hxc_l_tmp_call_result_n0;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return hxc_l_top;
      }
    default:
      {
        abort();
      }
  }
  return (double)hxc_l_y;
}

bool hxc_caxecraft_app_WaterRenderer_isOpenAir(struct hxc_caxecraft_domain_WaterCellState hxc_l_state)
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

bool hxc_caxecraft_app_WaterRenderer_isWater(struct hxc_caxecraft_domain_WaterCellState hxc_l_state)
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

double hxc_caxecraft_app_WaterRenderer_surfaceTop(struct hxc_caxecraft_domain_WaterCellState hxc_l_state, int32_t hxc_l_y)
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

void hxc_caxecraft_app_WaterRenderer_vertex(double hxc_l_u, double hxc_l_v, double hxc_l_x, double hxc_l_y, double hxc_l_z)
{
  rlTexCoord2f((float)hxc_l_u, (float)hxc_l_v);
  rlVertex3f((float)hxc_l_x, (float)hxc_l_y, (float)hxc_l_z);
  return;
}
