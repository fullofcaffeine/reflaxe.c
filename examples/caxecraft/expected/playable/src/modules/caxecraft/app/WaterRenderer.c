#include "hxc/program.h"

struct hxc_caxecraft_app_TerrainRenderCounters hxc_caxecraft_app_WaterRenderer_draw(uint8_t *hxc_cells, size_t hxc_length, struct Texture hxc_texture, bool hxc_textureReady, int32_t hxc_presentationCell)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  bool hxc_tmp_short_circuit_result_n5 = !hxc_textureReady;
  if (!!hxc_textureReady)
  {
    hxc_tmp_short_circuit_result_n5 = hxc_presentationCell < 0;
  }
  if (hxc_tmp_short_circuit_result_n5)
  {
    return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_drawCalls = 0, .hxc_visible = 0 };
  }
  int32_t hxc_column = hxc_i32_modulo_zero_safe(hxc_presentationCell, 4);
  int32_t hxc_row = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_presentationCell, (double)4));
  double hxc_halfPixel = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_texture.width);
  int32_t hxc_tmp_load_result_n3 = hxc_column;
  double hxc_u0 = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n3, (double)4) + hxc_halfPixel;
  int32_t hxc_tmp_load_result_n5 = hxc_column;
  double hxc_u1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_tmp_load_result_n5, 1), (double)4) - hxc_halfPixel;
  int32_t hxc_tmp_load_result_n7 = hxc_row;
  double hxc_v0 = hxc_f64_divide_zero_safe((double)hxc_tmp_load_result_n7, (double)4) + hxc_halfPixel;
  int32_t hxc_tmp_load_result_n9 = hxc_row;
  double hxc_v1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_tmp_load_result_n9, 1), (double)4) - hxc_halfPixel;
  int32_t hxc_visible = 0;
  int32_t hxc_faces = 0;
  struct Texture hxc_raw = hxc_texture;
  uint32_t *hxc_tmp_imported_field_address_n11 = &hxc_raw.id;
  uint32_t hxc_tmp_imported_field_load_result_n12 = *hxc_tmp_imported_field_address_n11;
  rlSetTexture(hxc_tmp_imported_field_load_result_n12);
  rlBegin((int32_t)7);
  int32_t hxc_z = 0;
  while (1)
  {
    if (!(hxc_z < 32))
    {
      break;
    }
    int32_t hxc_y = 0;
    while (1)
    {
      if (!(hxc_y < 16))
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
        int32_t hxc_tmp_load_result_n16 = hxc_x;
        int32_t hxc_tmp_load_result_n17 = hxc_y;
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n19 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n16, hxc_tmp_load_result_n17, hxc_z);
        struct hxc_caxecraft_domain_WaterCellState hxc_tmp_call_result_n20 = hxc_caxecraft_domain_WaterCellCodec_stateAt(hxc_cells, hxc_length, hxc_tmp_call_result_n19);
        struct hxc_caxecraft_domain_WaterCellState hxc_state = hxc_tmp_call_result_n20;
        bool hxc_tmp_call_result_n22 = hxc_caxecraft_app_WaterRenderer_isWater(hxc_state);
        if (hxc_tmp_call_result_n22)
        {
          struct hxc_caxecraft_domain_WaterCellState hxc_tmp_load_result_n23 = hxc_state;
          double hxc_tmp_call_result_n25 = hxc_caxecraft_app_WaterRenderer_surfaceTop(hxc_tmp_load_result_n23, hxc_y);
          double hxc_top = hxc_tmp_call_result_n25;
          bool hxc_cellVisible = false;
          int32_t hxc_tmp_load_result_n26 = hxc_x;
          int32_t hxc_tmp_load_result_n27 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n29 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n26, hxc_i32_add_wrapping(hxc_tmp_load_result_n27, 1), hxc_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_tmp_call_result_n30 = hxc_caxecraft_domain_WaterCellCodec_stateAt(hxc_cells, hxc_length, hxc_tmp_call_result_n29);
          bool hxc_tmp_call_result_n31 = hxc_caxecraft_app_WaterRenderer_isOpenAir(hxc_tmp_call_result_n30);
          if (hxc_tmp_call_result_n31)
          {
            int32_t hxc_tmp_load_result_n32 = hxc_x;
            double hxc_tmp_load_result_n33 = hxc_top;
            int32_t hxc_tmp_load_result_n34 = hxc_z;
            double hxc_tmp_load_result_n35 = hxc_u0;
            double hxc_tmp_load_result_n36 = hxc_u1;
            double hxc_tmp_load_result_n37 = hxc_v0;
            hxc_caxecraft_app_WaterRenderer_emitTop((double)hxc_tmp_load_result_n32, hxc_tmp_load_result_n33, (double)hxc_tmp_load_result_n34, hxc_tmp_load_result_n35, hxc_tmp_load_result_n36, hxc_tmp_load_result_n37, hxc_v1);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_cellVisible = true;
          }
          int32_t hxc_tmp_load_result_n40 = hxc_x;
          int32_t hxc_tmp_load_result_n41 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n43 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n40, hxc_tmp_load_result_n41, hxc_i32_subtract_wrapping(hxc_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_tmp_call_result_n44 = hxc_caxecraft_domain_WaterCellCodec_stateAt(hxc_cells, hxc_length, hxc_tmp_call_result_n43);
          int32_t hxc_tmp_load_result_n45 = hxc_y;
          double hxc_tmp_call_result_n47 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_tmp_call_result_n44, hxc_tmp_load_result_n45, hxc_top);
          double hxc_northBottom = hxc_tmp_call_result_n47;
          double hxc_tmp_load_result_n48 = hxc_northBottom;
          if (hxc_tmp_load_result_n48 < hxc_top)
          {
            int32_t hxc_tmp_load_result_n50 = hxc_x;
            double hxc_tmp_load_result_n51 = hxc_northBottom;
            double hxc_tmp_load_result_n52 = hxc_top;
            int32_t hxc_tmp_load_result_n53 = hxc_z;
            double hxc_tmp_load_result_n54 = hxc_u0;
            double hxc_tmp_load_result_n55 = hxc_u1;
            double hxc_tmp_load_result_n56 = hxc_v0;
            hxc_caxecraft_app_WaterRenderer_emitNorth((double)hxc_tmp_load_result_n50, hxc_tmp_load_result_n51, hxc_tmp_load_result_n52, (double)hxc_tmp_load_result_n53, hxc_tmp_load_result_n54, hxc_tmp_load_result_n55, hxc_tmp_load_result_n56, hxc_v1);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_cellVisible = true;
          }
          int32_t hxc_tmp_load_result_n59 = hxc_x;
          int32_t hxc_tmp_load_result_n60 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n62 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n59, hxc_tmp_load_result_n60, hxc_i32_add_wrapping(hxc_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_tmp_call_result_n63 = hxc_caxecraft_domain_WaterCellCodec_stateAt(hxc_cells, hxc_length, hxc_tmp_call_result_n62);
          int32_t hxc_tmp_load_result_n64 = hxc_y;
          double hxc_tmp_call_result_n66 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_tmp_call_result_n63, hxc_tmp_load_result_n64, hxc_top);
          double hxc_southBottom = hxc_tmp_call_result_n66;
          double hxc_tmp_load_result_n67 = hxc_southBottom;
          if (hxc_tmp_load_result_n67 < hxc_top)
          {
            int32_t hxc_tmp_load_result_n69 = hxc_x;
            double hxc_tmp_load_result_n70 = hxc_southBottom;
            double hxc_tmp_load_result_n71 = hxc_top;
            int32_t hxc_tmp_load_result_n72 = hxc_z;
            double hxc_tmp_load_result_n73 = hxc_u0;
            double hxc_tmp_load_result_n74 = hxc_u1;
            double hxc_tmp_load_result_n75 = hxc_v0;
            hxc_caxecraft_app_WaterRenderer_emitSouth((double)hxc_tmp_load_result_n69, hxc_tmp_load_result_n70, hxc_tmp_load_result_n71, (double)hxc_tmp_load_result_n72, hxc_tmp_load_result_n73, hxc_tmp_load_result_n74, hxc_tmp_load_result_n75, hxc_v1);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_cellVisible = true;
          }
          int32_t hxc_tmp_load_result_n78 = hxc_x;
          int32_t hxc_tmp_load_result_n79 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n81 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_tmp_load_result_n78, 1), hxc_tmp_load_result_n79, hxc_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_tmp_call_result_n82 = hxc_caxecraft_domain_WaterCellCodec_stateAt(hxc_cells, hxc_length, hxc_tmp_call_result_n81);
          int32_t hxc_tmp_load_result_n83 = hxc_y;
          double hxc_tmp_call_result_n85 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_tmp_call_result_n82, hxc_tmp_load_result_n83, hxc_top);
          double hxc_eastBottom = hxc_tmp_call_result_n85;
          double hxc_tmp_load_result_n86 = hxc_eastBottom;
          if (hxc_tmp_load_result_n86 < hxc_top)
          {
            int32_t hxc_tmp_load_result_n88 = hxc_x;
            double hxc_tmp_load_result_n89 = hxc_eastBottom;
            double hxc_tmp_load_result_n90 = hxc_top;
            int32_t hxc_tmp_load_result_n91 = hxc_z;
            double hxc_tmp_load_result_n92 = hxc_u0;
            double hxc_tmp_load_result_n93 = hxc_u1;
            double hxc_tmp_load_result_n94 = hxc_v0;
            hxc_caxecraft_app_WaterRenderer_emitEast((double)hxc_tmp_load_result_n88, hxc_tmp_load_result_n89, hxc_tmp_load_result_n90, (double)hxc_tmp_load_result_n91, hxc_tmp_load_result_n92, hxc_tmp_load_result_n93, hxc_tmp_load_result_n94, hxc_v1);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_cellVisible = true;
          }
          int32_t hxc_tmp_load_result_n97 = hxc_x;
          int32_t hxc_tmp_load_result_n98 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n100 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_tmp_load_result_n97, 1), hxc_tmp_load_result_n98, hxc_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_tmp_call_result_n101 = hxc_caxecraft_domain_WaterCellCodec_stateAt(hxc_cells, hxc_length, hxc_tmp_call_result_n100);
          int32_t hxc_tmp_load_result_n102 = hxc_y;
          double hxc_tmp_call_result_n104 = hxc_caxecraft_app_WaterRenderer_exposedSideBottom(hxc_tmp_call_result_n101, hxc_tmp_load_result_n102, hxc_top);
          double hxc_westBottom = hxc_tmp_call_result_n104;
          double hxc_tmp_load_result_n105 = hxc_westBottom;
          if (hxc_tmp_load_result_n105 < hxc_top)
          {
            int32_t hxc_tmp_load_result_n107 = hxc_x;
            double hxc_tmp_load_result_n108 = hxc_westBottom;
            double hxc_tmp_load_result_n109 = hxc_top;
            int32_t hxc_tmp_load_result_n110 = hxc_z;
            double hxc_tmp_load_result_n111 = hxc_u0;
            double hxc_tmp_load_result_n112 = hxc_u1;
            double hxc_tmp_load_result_n113 = hxc_v0;
            hxc_caxecraft_app_WaterRenderer_emitWest((double)hxc_tmp_load_result_n107, hxc_tmp_load_result_n108, hxc_tmp_load_result_n109, (double)hxc_tmp_load_result_n110, hxc_tmp_load_result_n111, hxc_tmp_load_result_n112, hxc_tmp_load_result_n113, hxc_v1);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_cellVisible = true;
          }
          if (hxc_cellVisible)
          {
            hxc_visible = hxc_i32_add_wrapping(hxc_visible, 1);
          }
        }
        hxc_x = hxc_i32_add_wrapping(hxc_x, 1);
      }
      hxc_y = hxc_i32_add_wrapping(hxc_y, 1);
    }
    hxc_z = hxc_i32_add_wrapping(hxc_z, 1);
  }
  rlEnd();
  rlSetTexture((uint32_t)0);
  int32_t hxc_drawCalls = 0;
  if (hxc_faces > 0)
  {
    hxc_drawCalls = 1;
  }
  int32_t hxc_tmp_load_result_n122 = hxc_visible;
  return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_drawCalls = hxc_drawCalls, .hxc_visible = hxc_tmp_load_result_n122 };
}

void hxc_caxecraft_app_WaterRenderer_emitEast(double hxc_x, double hxc_bottom, double hxc_top, double hxc_z, double hxc_u0, double hxc_u1, double hxc_v0, double hxc_v1)
{
  struct Color hxc_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_tmp_imported_field_address_n1 = &hxc_raw.r;
  uint8_t hxc_tmp_imported_field_load_result_n2 = *hxc_tmp_imported_field_address_n1;
  uint8_t *hxc_tmp_imported_field_address_n3 = &hxc_raw.g;
  uint8_t hxc_tmp_imported_field_load_result_n4 = *hxc_tmp_imported_field_address_n3;
  uint8_t *hxc_tmp_imported_field_address_n5 = &hxc_raw.b;
  uint8_t hxc_tmp_imported_field_load_result_n6 = *hxc_tmp_imported_field_address_n5;
  uint8_t *hxc_tmp_imported_field_address_n7 = &hxc_raw.a;
  uint8_t hxc_tmp_imported_field_load_result_n8 = *hxc_tmp_imported_field_address_n7;
  rlColor4ub(hxc_tmp_imported_field_load_result_n2, hxc_tmp_imported_field_load_result_n4, hxc_tmp_imported_field_load_result_n6, hxc_tmp_imported_field_load_result_n8);
  rlNormal3f((float)1.0, (float)0.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v1, hxc_x + 1.0, hxc_bottom, hxc_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v0, hxc_x + 1.0, hxc_top, hxc_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v0, hxc_x + 1.0, hxc_top, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v1, hxc_x + 1.0, hxc_bottom, hxc_z + 1.0);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitNorth(double hxc_x, double hxc_bottom, double hxc_top, double hxc_z, double hxc_u0, double hxc_u1, double hxc_v0, double hxc_v1)
{
  struct Color hxc_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_tmp_imported_field_address_n1 = &hxc_raw.r;
  uint8_t hxc_tmp_imported_field_load_result_n2 = *hxc_tmp_imported_field_address_n1;
  uint8_t *hxc_tmp_imported_field_address_n3 = &hxc_raw.g;
  uint8_t hxc_tmp_imported_field_load_result_n4 = *hxc_tmp_imported_field_address_n3;
  uint8_t *hxc_tmp_imported_field_address_n5 = &hxc_raw.b;
  uint8_t hxc_tmp_imported_field_load_result_n6 = *hxc_tmp_imported_field_address_n5;
  uint8_t *hxc_tmp_imported_field_address_n7 = &hxc_raw.a;
  uint8_t hxc_tmp_imported_field_load_result_n8 = *hxc_tmp_imported_field_address_n7;
  rlColor4ub(hxc_tmp_imported_field_load_result_n2, hxc_tmp_imported_field_load_result_n4, hxc_tmp_imported_field_load_result_n6, hxc_tmp_imported_field_load_result_n8);
  rlNormal3f((float)0.0, (float)0.0, (float)-1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v1, hxc_x, hxc_bottom, hxc_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v0, hxc_x, hxc_top, hxc_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v0, hxc_x + 1.0, hxc_top, hxc_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v1, hxc_x + 1.0, hxc_bottom, hxc_z);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitSouth(double hxc_x, double hxc_bottom, double hxc_top, double hxc_z, double hxc_u0, double hxc_u1, double hxc_v0, double hxc_v1)
{
  struct Color hxc_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_tmp_imported_field_address_n1 = &hxc_raw.r;
  uint8_t hxc_tmp_imported_field_load_result_n2 = *hxc_tmp_imported_field_address_n1;
  uint8_t *hxc_tmp_imported_field_address_n3 = &hxc_raw.g;
  uint8_t hxc_tmp_imported_field_load_result_n4 = *hxc_tmp_imported_field_address_n3;
  uint8_t *hxc_tmp_imported_field_address_n5 = &hxc_raw.b;
  uint8_t hxc_tmp_imported_field_load_result_n6 = *hxc_tmp_imported_field_address_n5;
  uint8_t *hxc_tmp_imported_field_address_n7 = &hxc_raw.a;
  uint8_t hxc_tmp_imported_field_load_result_n8 = *hxc_tmp_imported_field_address_n7;
  rlColor4ub(hxc_tmp_imported_field_load_result_n2, hxc_tmp_imported_field_load_result_n4, hxc_tmp_imported_field_load_result_n6, hxc_tmp_imported_field_load_result_n8);
  rlNormal3f((float)0.0, (float)0.0, (float)1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v1, hxc_x, hxc_bottom, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v1, hxc_x + 1.0, hxc_bottom, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v0, hxc_x + 1.0, hxc_top, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v0, hxc_x, hxc_top, hxc_z + 1.0);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitTop(double hxc_x, double hxc_y, double hxc_z, double hxc_u0, double hxc_u1, double hxc_v0, double hxc_v1)
{
  struct Color hxc_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_tmp_imported_field_address_n1 = &hxc_raw.r;
  uint8_t hxc_tmp_imported_field_load_result_n2 = *hxc_tmp_imported_field_address_n1;
  uint8_t *hxc_tmp_imported_field_address_n3 = &hxc_raw.g;
  uint8_t hxc_tmp_imported_field_load_result_n4 = *hxc_tmp_imported_field_address_n3;
  uint8_t *hxc_tmp_imported_field_address_n5 = &hxc_raw.b;
  uint8_t hxc_tmp_imported_field_load_result_n6 = *hxc_tmp_imported_field_address_n5;
  uint8_t *hxc_tmp_imported_field_address_n7 = &hxc_raw.a;
  uint8_t hxc_tmp_imported_field_load_result_n8 = *hxc_tmp_imported_field_address_n7;
  rlColor4ub(hxc_tmp_imported_field_load_result_n2, hxc_tmp_imported_field_load_result_n4, hxc_tmp_imported_field_load_result_n6, hxc_tmp_imported_field_load_result_n8);
  rlNormal3f((float)0.0, (float)1.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v0, hxc_x, hxc_y, hxc_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v1, hxc_x, hxc_y, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v1, hxc_x + 1.0, hxc_y, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v0, hxc_x + 1.0, hxc_y, hxc_z);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitWest(double hxc_x, double hxc_bottom, double hxc_top, double hxc_z, double hxc_u0, double hxc_u1, double hxc_v0, double hxc_v1)
{
  struct Color hxc_raw = (struct Color){ .r = 150, .g = 226, .b = 242, .a = 178 };
  uint8_t *hxc_tmp_imported_field_address_n1 = &hxc_raw.r;
  uint8_t hxc_tmp_imported_field_load_result_n2 = *hxc_tmp_imported_field_address_n1;
  uint8_t *hxc_tmp_imported_field_address_n3 = &hxc_raw.g;
  uint8_t hxc_tmp_imported_field_load_result_n4 = *hxc_tmp_imported_field_address_n3;
  uint8_t *hxc_tmp_imported_field_address_n5 = &hxc_raw.b;
  uint8_t hxc_tmp_imported_field_load_result_n6 = *hxc_tmp_imported_field_address_n5;
  uint8_t *hxc_tmp_imported_field_address_n7 = &hxc_raw.a;
  uint8_t hxc_tmp_imported_field_load_result_n8 = *hxc_tmp_imported_field_address_n7;
  rlColor4ub(hxc_tmp_imported_field_load_result_n2, hxc_tmp_imported_field_load_result_n4, hxc_tmp_imported_field_load_result_n6, hxc_tmp_imported_field_load_result_n8);
  rlNormal3f((float)-1.0, (float)0.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v1, hxc_x, hxc_bottom, hxc_z);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v1, hxc_x, hxc_bottom, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u1, hxc_v0, hxc_x, hxc_top, hxc_z + 1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_u0, hxc_v0, hxc_x, hxc_top, hxc_z);
  return;
}

double hxc_caxecraft_app_WaterRenderer_exposedSideBottom(struct hxc_caxecraft_domain_WaterCellState hxc_neighbor, int32_t hxc_y, double hxc_top)
{
  switch (hxc_neighbor.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        return hxc_top;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return (double)hxc_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        double hxc_tmp_call_result_n0 = hxc_caxecraft_app_WaterRenderer_surfaceTop(hxc_neighbor, hxc_y);
        return hxc_tmp_call_result_n0;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return hxc_top;
      }
  }
  return (double)hxc_y;
}

bool hxc_caxecraft_app_WaterRenderer_isOpenAir(struct hxc_caxecraft_domain_WaterCellState hxc_state)
{
  switch (hxc_state.hxc_tag) {
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
  }
  return true;
}

bool hxc_caxecraft_app_WaterRenderer_isWater(struct hxc_caxecraft_domain_WaterCellState hxc_state)
{
  switch (hxc_state.hxc_tag) {
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
  }
  return false;
}

double hxc_caxecraft_app_WaterRenderer_surfaceTop(struct hxc_caxecraft_domain_WaterCellState hxc_state, int32_t hxc_y)
{
  switch (hxc_state.hxc_tag) {
    case hxc_caxecraft_domain_WaterCellState_Empty:
    case hxc_caxecraft_domain_WaterCellState_Blocked:
      {
        break;
      }
    case hxc_caxecraft_domain_WaterCellState_Source:
      {
        return (double)hxc_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_Flowing:
      {
        if (hxc_state.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n0 = hxc_state.hxc_payload.hxc_Flowing.hxc_level;
        int32_t hxc_level = hxc_tmp_enum_payload_project_n0;
        if (hxc_state.hxc_tag != hxc_caxecraft_domain_WaterCellState_Flowing)
        {
          abort();
        }
        bool hxc_tmp_enum_payload_project_n1 = hxc_state.hxc_payload.hxc_Flowing.hxc_falling;
        bool hxc_falling = hxc_tmp_enum_payload_project_n1;
        if (!hxc_falling)
        {
          int32_t hxc_distance = hxc_level;
          return (double)hxc_y + hxc_f64_divide_zero_safe(8.0 - (double)hxc_distance, 8.0);
        }
        return (double)hxc_y + 1.0;
      }
    case hxc_caxecraft_domain_WaterCellState_InvalidStorage:
      {
        return (double)hxc_y;
      }
  }
  return (double)hxc_y;
}

void hxc_caxecraft_app_WaterRenderer_vertex(double hxc_u, double hxc_v, double hxc_x, double hxc_y, double hxc_z)
{
  rlTexCoord2f((float)hxc_u, (float)hxc_v);
  rlVertex3f((float)hxc_x, (float)hxc_y, (float)hxc_z);
  return;
}
