#include "hxc/program.h"

struct hxc_caxecraft_app_TerrainRenderCounters hxc_caxecraft_app_TerrainRenderer_draw(uint8_t *hxc_cells, size_t hxc_length, struct Texture hxc_texture, bool hxc_textureReady, double hxc_playerX, double hxc_playerZ)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n5 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n5;
  if (!hxc_textureReady)
  {
    return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_drawCalls = 0, .hxc_visible = 0 };
  }
  double hxc_halfPixel = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_texture.width);
  int32_t hxc_playerCellX = hxc_f64_to_i32_saturating(hxc_playerX);
  int32_t hxc_playerCellZ = hxc_f64_to_i32_saturating(hxc_playerZ);
  int32_t hxc_visible = 0;
  int32_t hxc_faces = 0;
  struct Texture hxc_raw = hxc_texture;
  uint32_t *hxc_tmp_imported_field_address_n2 = &hxc_raw.id;
  uint32_t hxc_tmp_imported_field_load_result_n3 = *hxc_tmp_imported_field_address_n2;
  rlSetTexture(hxc_tmp_imported_field_load_result_n3);
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
        int32_t hxc_tmp_load_result_n7 = hxc_x;
        int32_t hxc_tmp_load_result_n8 = hxc_y;
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n10 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n7, hxc_tmp_load_result_n8, hxc_z);
        enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n11 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n10);
        enum hxc_caxecraft_domain_BlockKind hxc_kind = hxc_tmp_call_result_n11;
        bool hxc_tmp_call_result_n13 = hxc_caxecraft_domain_World_isSolid(hxc_kind);
        if (hxc_tmp_call_result_n13)
        {
          bool hxc_blockVisible = false;
          int32_t hxc_tmp_load_result_n14 = hxc_x;
          int32_t hxc_dx = hxc_i32_subtract_wrapping(hxc_tmp_load_result_n14, hxc_playerCellX);
          if (hxc_dx < 0)
          {
            hxc_dx = hxc_i32_negate_wrapping(hxc_dx);
          }
          int32_t hxc_tmp_load_result_n18 = hxc_z;
          int32_t hxc_dz = hxc_i32_subtract_wrapping(hxc_tmp_load_result_n18, hxc_playerCellZ);
          if (hxc_dz < 0)
          {
            hxc_dz = hxc_i32_negate_wrapping(hxc_dz);
          }
          int32_t hxc_tmp_load_result_n22 = hxc_dx;
          int32_t hxc_tmp_load_result_n23 = hxc_dz;
          int32_t hxc_tmp_conditional_result_n20 = 0;
          if (hxc_tmp_load_result_n22 > hxc_tmp_load_result_n23)
          {
            hxc_tmp_conditional_result_n20 = hxc_dx;
          }
          else
          {
            hxc_tmp_conditional_result_n20 = hxc_dz;
          }
          int32_t hxc_distance = hxc_tmp_conditional_result_n20;
          int32_t hxc_tmp_load_result_n27 = hxc_x;
          int32_t hxc_tmp_load_result_n28 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n30 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n27, hxc_i32_add_wrapping(hxc_tmp_load_result_n28, 1), hxc_z);
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n31 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n30);
          bool hxc_tmp_call_result_n32 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n31);
          if (!hxc_tmp_call_result_n32)
          {
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n33 = hxc_kind;
            int32_t hxc_tmp_load_result_n35 = hxc_x;
            int32_t hxc_tmp_load_result_n36 = hxc_y;
            int32_t hxc_tmp_load_result_n37 = hxc_z;
            double hxc_tmp_load_result_n38 = hxc_halfPixel;
            hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_tmp_load_result_n33, hxc_caxecraft_app_VoxelFace_Top, hxc_tmp_load_result_n35, hxc_tmp_load_result_n36, hxc_tmp_load_result_n37, hxc_tmp_load_result_n38, hxc_distance);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_blockVisible = true;
          }
          int32_t hxc_tmp_load_result_n41 = hxc_x;
          int32_t hxc_tmp_load_result_n42 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n44 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n41, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n42, 1), hxc_z);
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n45 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n44);
          bool hxc_tmp_call_result_n46 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n45);
          if (!hxc_tmp_call_result_n46)
          {
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n47 = hxc_kind;
            int32_t hxc_tmp_load_result_n49 = hxc_x;
            int32_t hxc_tmp_load_result_n50 = hxc_y;
            int32_t hxc_tmp_load_result_n51 = hxc_z;
            double hxc_tmp_load_result_n52 = hxc_halfPixel;
            hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_tmp_load_result_n47, hxc_caxecraft_app_VoxelFace_Bottom, hxc_tmp_load_result_n49, hxc_tmp_load_result_n50, hxc_tmp_load_result_n51, hxc_tmp_load_result_n52, hxc_distance);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_blockVisible = true;
          }
          int32_t hxc_tmp_load_result_n55 = hxc_x;
          int32_t hxc_tmp_load_result_n56 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n58 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n55, hxc_tmp_load_result_n56, hxc_i32_subtract_wrapping(hxc_z, 1));
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n59 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n58);
          bool hxc_tmp_call_result_n60 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n59);
          if (!hxc_tmp_call_result_n60)
          {
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n61 = hxc_kind;
            int32_t hxc_tmp_load_result_n63 = hxc_x;
            int32_t hxc_tmp_load_result_n64 = hxc_y;
            int32_t hxc_tmp_load_result_n65 = hxc_z;
            double hxc_tmp_load_result_n66 = hxc_halfPixel;
            hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_tmp_load_result_n61, hxc_caxecraft_app_VoxelFace_North, hxc_tmp_load_result_n63, hxc_tmp_load_result_n64, hxc_tmp_load_result_n65, hxc_tmp_load_result_n66, hxc_distance);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_blockVisible = true;
          }
          int32_t hxc_tmp_load_result_n69 = hxc_x;
          int32_t hxc_tmp_load_result_n70 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n72 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n69, hxc_tmp_load_result_n70, hxc_i32_add_wrapping(hxc_z, 1));
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n73 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n72);
          bool hxc_tmp_call_result_n74 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n73);
          if (!hxc_tmp_call_result_n74)
          {
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n75 = hxc_kind;
            int32_t hxc_tmp_load_result_n77 = hxc_x;
            int32_t hxc_tmp_load_result_n78 = hxc_y;
            int32_t hxc_tmp_load_result_n79 = hxc_z;
            double hxc_tmp_load_result_n80 = hxc_halfPixel;
            hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_tmp_load_result_n75, hxc_caxecraft_app_VoxelFace_South, hxc_tmp_load_result_n77, hxc_tmp_load_result_n78, hxc_tmp_load_result_n79, hxc_tmp_load_result_n80, hxc_distance);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_blockVisible = true;
          }
          int32_t hxc_tmp_load_result_n83 = hxc_x;
          int32_t hxc_tmp_load_result_n84 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n86 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_tmp_load_result_n83, 1), hxc_tmp_load_result_n84, hxc_z);
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n87 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n86);
          bool hxc_tmp_call_result_n88 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n87);
          if (!hxc_tmp_call_result_n88)
          {
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n89 = hxc_kind;
            int32_t hxc_tmp_load_result_n91 = hxc_x;
            int32_t hxc_tmp_load_result_n92 = hxc_y;
            int32_t hxc_tmp_load_result_n93 = hxc_z;
            double hxc_tmp_load_result_n94 = hxc_halfPixel;
            hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_tmp_load_result_n89, hxc_caxecraft_app_VoxelFace_East, hxc_tmp_load_result_n91, hxc_tmp_load_result_n92, hxc_tmp_load_result_n93, hxc_tmp_load_result_n94, hxc_distance);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_blockVisible = true;
          }
          int32_t hxc_tmp_load_result_n97 = hxc_x;
          int32_t hxc_tmp_load_result_n98 = hxc_y;
          struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n100 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_tmp_load_result_n97, 1), hxc_tmp_load_result_n98, hxc_z);
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n101 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n100);
          bool hxc_tmp_call_result_n102 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n101);
          if (!hxc_tmp_call_result_n102)
          {
            enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n103 = hxc_kind;
            int32_t hxc_tmp_load_result_n105 = hxc_x;
            int32_t hxc_tmp_load_result_n106 = hxc_y;
            int32_t hxc_tmp_load_result_n107 = hxc_z;
            double hxc_tmp_load_result_n108 = hxc_halfPixel;
            hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_tmp_load_result_n103, hxc_caxecraft_app_VoxelFace_West, hxc_tmp_load_result_n105, hxc_tmp_load_result_n106, hxc_tmp_load_result_n107, hxc_tmp_load_result_n108, hxc_distance);
            hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
            hxc_blockVisible = true;
          }
          if (hxc_blockVisible)
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
  int32_t hxc_tmp_load_result_n117 = hxc_visible;
  return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_drawCalls = hxc_drawCalls, .hxc_visible = hxc_tmp_load_result_n117 };
}

void hxc_caxecraft_app_TerrainRenderer_emitFace(enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_app_VoxelFace hxc_face, int32_t hxc_x, int32_t hxc_y, int32_t hxc_z, double hxc_halfPixel, int32_t hxc_distance)
{
  enum hxc_caxecraft_app_TerrainTile hxc_tmp_call_result_n0 = hxc_caxecraft_app_TerrainAtlas_tile(hxc_kind, hxc_face);
  enum hxc_caxecraft_app_TerrainTile hxc_tile = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_app_TerrainAtlas_column(hxc_tile);
  double hxc_u0 = hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n2, (double)4) + hxc_halfPixel;
  int32_t hxc_tmp_call_result_n4 = hxc_caxecraft_app_TerrainAtlas_column(hxc_tile);
  double hxc_u1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_tmp_call_result_n4, 1), (double)4) - hxc_halfPixel;
  int32_t hxc_tmp_call_result_n6 = hxc_caxecraft_app_TerrainAtlas_row(hxc_tile);
  double hxc_v0 = hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n6, (double)4) + hxc_halfPixel;
  int32_t hxc_tmp_call_result_n8 = hxc_caxecraft_app_TerrainAtlas_row(hxc_tile);
  double hxc_v1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_tmp_call_result_n8, 1), (double)4) - hxc_halfPixel;
  struct Color hxc_tmp_call_result_n9 = hxc_caxecraft_app_TerrainRenderer_faceTint(hxc_face, hxc_distance);
  struct Color hxc_raw = hxc_tmp_call_result_n9;
  uint8_t *hxc_tmp_imported_field_address_n10 = &hxc_raw.r;
  uint8_t hxc_tmp_imported_field_load_result_n11 = *hxc_tmp_imported_field_address_n10;
  uint8_t *hxc_tmp_imported_field_address_n12 = &hxc_raw.g;
  uint8_t hxc_tmp_imported_field_load_result_n13 = *hxc_tmp_imported_field_address_n12;
  uint8_t *hxc_tmp_imported_field_address_n14 = &hxc_raw.b;
  uint8_t hxc_tmp_imported_field_load_result_n15 = *hxc_tmp_imported_field_address_n14;
  uint8_t *hxc_tmp_imported_field_address_n16 = &hxc_raw.a;
  uint8_t hxc_tmp_imported_field_load_result_n17 = *hxc_tmp_imported_field_address_n16;
  rlColor4ub(hxc_tmp_imported_field_load_result_n11, hxc_tmp_imported_field_load_result_n13, hxc_tmp_imported_field_load_result_n15, hxc_tmp_imported_field_load_result_n17);
  switch (hxc_face) {
    case hxc_caxecraft_app_VoxelFace_Top:
      {
        rlNormal3f((float)0.0, (float)1.0, (float)0.0);
        double hxc_tmp_load_result_n18 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n18, hxc_v0, (double)hxc_x, (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_z);
        double hxc_tmp_load_result_n20 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n20, hxc_v1, (double)hxc_x, (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n22 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n22, hxc_v1, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n24 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n24, hxc_v0, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_z);
        break;
      }
    case hxc_caxecraft_app_VoxelFace_Bottom:
      {
        rlNormal3f((float)0.0, (float)-1.0, (float)0.0);
        double hxc_tmp_load_result_n26 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n26, hxc_v0, (double)hxc_x, (double)hxc_y, (double)hxc_z);
        double hxc_tmp_load_result_n28 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n28, hxc_v0, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_y, (double)hxc_z);
        double hxc_tmp_load_result_n30 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n30, hxc_v1, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_y, (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n32 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n32, hxc_v1, (double)hxc_x, (double)hxc_y, (double)hxc_i32_add_wrapping(hxc_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_North:
      {
        rlNormal3f((float)0.0, (float)0.0, (float)-1.0);
        double hxc_tmp_load_result_n34 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n34, hxc_v1, (double)hxc_x, (double)hxc_y, (double)hxc_z);
        double hxc_tmp_load_result_n36 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n36, hxc_v0, (double)hxc_x, (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_z);
        double hxc_tmp_load_result_n38 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n38, hxc_v0, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_z);
        double hxc_tmp_load_result_n40 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n40, hxc_v1, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_y, (double)hxc_z);
        break;
      }
    case hxc_caxecraft_app_VoxelFace_South:
      {
        rlNormal3f((float)0.0, (float)0.0, (float)1.0);
        double hxc_tmp_load_result_n42 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n42, hxc_v1, (double)hxc_x, (double)hxc_y, (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n44 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n44, hxc_v1, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_y, (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n46 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n46, hxc_v0, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n48 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n48, hxc_v0, (double)hxc_x, (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_i32_add_wrapping(hxc_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_East:
      {
        rlNormal3f((float)1.0, (float)0.0, (float)0.0);
        double hxc_tmp_load_result_n50 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n50, hxc_v1, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_y, (double)hxc_z);
        double hxc_tmp_load_result_n52 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n52, hxc_v0, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_z);
        double hxc_tmp_load_result_n54 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n54, hxc_v0, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n56 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n56, hxc_v1, (double)hxc_i32_add_wrapping(hxc_x, 1), (double)hxc_y, (double)hxc_i32_add_wrapping(hxc_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_West:
      {
        rlNormal3f((float)-1.0, (float)0.0, (float)0.0);
        double hxc_tmp_load_result_n58 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n58, hxc_v1, (double)hxc_x, (double)hxc_y, (double)hxc_z);
        double hxc_tmp_load_result_n60 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n60, hxc_v1, (double)hxc_x, (double)hxc_y, (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n62 = hxc_u1;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n62, hxc_v0, (double)hxc_x, (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n64 = hxc_u0;
        hxc_caxecraft_app_TerrainRenderer_vertex(hxc_tmp_load_result_n64, hxc_v0, (double)hxc_x, (double)hxc_i32_add_wrapping(hxc_y, 1), (double)hxc_z);
        break;
      }
  }
  return;
}

struct Color hxc_caxecraft_app_TerrainRenderer_faceTint(enum hxc_caxecraft_app_VoxelFace hxc_face, int32_t hxc_distance)
{
  if (!(hxc_distance >= 22))
  {
    if (!(hxc_distance >= 12))
    {
      switch (hxc_face) {
        case hxc_caxecraft_app_VoxelFace_Top:
          {
            break;
          }
        case hxc_caxecraft_app_VoxelFace_Bottom:
          {
            return (struct Color){ .r = 178, .g = 191, .b = 188, .a = 255 };
          }
        case hxc_caxecraft_app_VoxelFace_South:
        case hxc_caxecraft_app_VoxelFace_East:
          {
            return (struct Color){ .r = 235, .g = 241, .b = 230, .a = 255 };
          }
        case hxc_caxecraft_app_VoxelFace_North:
        case hxc_caxecraft_app_VoxelFace_West:
          {
            return (struct Color){ .r = 211, .g = 225, .b = 220, .a = 255 };
          }
      }
      return (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    }
    switch (hxc_face) {
      case hxc_caxecraft_app_VoxelFace_Top:
        {
          break;
        }
      case hxc_caxecraft_app_VoxelFace_Bottom:
        {
          return (struct Color){ .r = 173, .g = 190, .b = 188, .a = 255 };
        }
      case hxc_caxecraft_app_VoxelFace_South:
      case hxc_caxecraft_app_VoxelFace_East:
        {
          return (struct Color){ .r = 219, .g = 231, .b = 222, .a = 255 };
        }
      case hxc_caxecraft_app_VoxelFace_North:
      case hxc_caxecraft_app_VoxelFace_West:
        {
          return (struct Color){ .r = 199, .g = 216, .b = 211, .a = 255 };
        }
    }
    return (struct Color){ .r = 240, .g = 246, .b = 239, .a = 255 };
  }
  switch (hxc_face) {
    case hxc_caxecraft_app_VoxelFace_Top:
      {
        break;
      }
    case hxc_caxecraft_app_VoxelFace_Bottom:
      {
        return (struct Color){ .r = 158, .g = 182, .b = 185, .a = 255 };
      }
    case hxc_caxecraft_app_VoxelFace_South:
    case hxc_caxecraft_app_VoxelFace_East:
      {
        return (struct Color){ .r = 197, .g = 218, .b = 212, .a = 255 };
      }
    case hxc_caxecraft_app_VoxelFace_North:
    case hxc_caxecraft_app_VoxelFace_West:
      {
        return (struct Color){ .r = 181, .g = 205, .b = 204, .a = 255 };
      }
  }
  return (struct Color){ .r = 220, .g = 235, .b = 226, .a = 255 };
}

void hxc_caxecraft_app_TerrainRenderer_vertex(double hxc_u, double hxc_v, double hxc_x, double hxc_y, double hxc_z)
{
  rlTexCoord2f((float)hxc_u, (float)hxc_v);
  rlVertex3f((float)hxc_x, (float)hxc_y, (float)hxc_z);
  return;
}
