#include "hxc/program.h"

bool hxc_caxecraft_app_CaxecraftAtlas_drawAtlasBox(struct Texture hxc_l_texture, int32_t hxc_l_cellIndex, int32_t hxc_l_columns, int32_t hxc_l_rows, struct Vector3 hxc_l_center, double hxc_l_width, double hxc_l_height, double hxc_l_depth)
{
  int32_t hxc_l_column = { 0 };
  int32_t hxc_l_row = { 0 };
  double hxc_l_halfPixelU = { 0 };
  double hxc_l_halfPixelV = { 0 };
  double hxc_l_u0 = { 0 };
  double hxc_l_u1 = { 0 };
  double hxc_l_v0 = { 0 };
  double hxc_l_v1 = { 0 };
  double hxc_l_centerX = { 0 };
  double hxc_l_centerY = { 0 };
  double hxc_l_centerZ = { 0 };
  double hxc_l_x0 = { 0 };
  double hxc_l_x1 = { 0 };
  double hxc_l_y0 = { 0 };
  double hxc_l_y1 = { 0 };
  double hxc_l_z0 = { 0 };
  double hxc_l_z1 = { 0 };
  struct Texture hxc_l_raw = { 0 };
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_columns <= 0;
  if (!(hxc_l_columns <= 0))
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_rows <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n0 = hxc_l_tmp_short_circuit_result_n8;
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_short_circuit_load_result_n0;
  if (!hxc_l_tmp_short_circuit_load_result_n0)
  {
    hxc_l_tmp_short_circuit_result_n9 = hxc_l_cellIndex < 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n1 = hxc_l_tmp_short_circuit_result_n9;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_short_circuit_load_result_n1;
  if (!hxc_l_tmp_short_circuit_load_result_n1)
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_cellIndex >= hxc_i32_multiply_wrapping(hxc_l_columns, hxc_l_rows);
  }
  if (!hxc_l_tmp_short_circuit_result_n10)
  {
    hxc_l_column = hxc_i32_modulo_zero_safe(hxc_l_cellIndex, hxc_l_columns);
    hxc_l_row = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_cellIndex, (double)hxc_l_columns));
    hxc_l_halfPixelU = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_l_texture.width);
    hxc_l_halfPixelV = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_l_texture.height);
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_column;
    hxc_l_u0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n5, (double)hxc_l_columns + 0.0) + hxc_l_halfPixelU;
    int32_t hxc_l_tmp_load_result_n7 = hxc_l_column;
    hxc_l_u1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n7, 1), (double)hxc_l_columns + 0.0) - hxc_l_halfPixelU;
    int32_t hxc_l_tmp_load_result_n9 = hxc_l_row;
    hxc_l_v0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n9, (double)hxc_l_rows + 0.0) + hxc_l_halfPixelV;
    int32_t hxc_l_tmp_load_result_n11 = hxc_l_row;
    hxc_l_v1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n11, 1), (double)hxc_l_rows + 0.0) - hxc_l_halfPixelV;
    hxc_l_centerX = (double)hxc_l_center.x;
    hxc_l_centerY = (double)hxc_l_center.y;
    hxc_l_centerZ = (double)hxc_l_center.z;
    hxc_l_x0 = hxc_l_centerX - hxc_l_width * 0.5;
    hxc_l_x1 = hxc_l_centerX + hxc_l_width * 0.5;
    hxc_l_y0 = hxc_l_centerY - hxc_l_height * 0.5;
    hxc_l_y1 = hxc_l_centerY + hxc_l_height * 0.5;
    hxc_l_z0 = hxc_l_centerZ - hxc_l_depth * 0.5;
    hxc_l_z1 = hxc_l_centerZ + hxc_l_depth * 0.5;
    hxc_l_raw = hxc_l_texture;
    uint32_t *hxc_l_tmp_imported_field_address_n22 = &hxc_l_raw.id;
    uint32_t hxc_l_tmp_imported_field_load_result_n23 = *hxc_l_tmp_imported_field_address_n22;
    rlSetTexture(hxc_l_tmp_imported_field_load_result_n23);
    rlBegin((int32_t)7);
    double hxc_l_tmp_load_result_n24 = hxc_l_x0;
    double hxc_l_tmp_load_result_n25 = hxc_l_x1;
    double hxc_l_tmp_load_result_n26 = hxc_l_y0;
    double hxc_l_tmp_load_result_n27 = hxc_l_y1;
    double hxc_l_tmp_load_result_n28 = hxc_l_z0;
    double hxc_l_tmp_load_result_n29 = hxc_l_z1;
    double hxc_l_tmp_load_result_n30 = hxc_l_u0;
    double hxc_l_tmp_load_result_n31 = hxc_l_u1;
    double hxc_l_tmp_load_result_n32 = hxc_l_v0;
    hxc_caxecraft_app_CaxecraftAtlas_emitTiledNorthSouth(hxc_l_tmp_load_result_n24, hxc_l_tmp_load_result_n25, hxc_l_tmp_load_result_n26, hxc_l_tmp_load_result_n27, hxc_l_tmp_load_result_n28, hxc_l_tmp_load_result_n29, hxc_l_tmp_load_result_n30, hxc_l_tmp_load_result_n31, hxc_l_tmp_load_result_n32, hxc_l_v1);
    double hxc_l_tmp_load_result_n34 = hxc_l_x0;
    double hxc_l_tmp_load_result_n35 = hxc_l_x1;
    double hxc_l_tmp_load_result_n36 = hxc_l_y0;
    double hxc_l_tmp_load_result_n37 = hxc_l_y1;
    double hxc_l_tmp_load_result_n38 = hxc_l_z0;
    double hxc_l_tmp_load_result_n39 = hxc_l_z1;
    double hxc_l_tmp_load_result_n40 = hxc_l_u0;
    double hxc_l_tmp_load_result_n41 = hxc_l_u1;
    double hxc_l_tmp_load_result_n42 = hxc_l_v0;
    hxc_caxecraft_app_CaxecraftAtlas_emitTiledEastWest(hxc_l_tmp_load_result_n34, hxc_l_tmp_load_result_n35, hxc_l_tmp_load_result_n36, hxc_l_tmp_load_result_n37, hxc_l_tmp_load_result_n38, hxc_l_tmp_load_result_n39, hxc_l_tmp_load_result_n40, hxc_l_tmp_load_result_n41, hxc_l_tmp_load_result_n42, hxc_l_v1);
    double hxc_l_tmp_load_result_n44 = hxc_l_x0;
    double hxc_l_tmp_load_result_n45 = hxc_l_x1;
    double hxc_l_tmp_load_result_n46 = hxc_l_y0;
    double hxc_l_tmp_load_result_n47 = hxc_l_y1;
    double hxc_l_tmp_load_result_n48 = hxc_l_z0;
    double hxc_l_tmp_load_result_n49 = hxc_l_z1;
    double hxc_l_tmp_load_result_n50 = hxc_l_u0;
    double hxc_l_tmp_load_result_n51 = hxc_l_u1;
    double hxc_l_tmp_load_result_n52 = hxc_l_v0;
    hxc_caxecraft_app_CaxecraftAtlas_emitTiledTopBottom(hxc_l_tmp_load_result_n44, hxc_l_tmp_load_result_n45, hxc_l_tmp_load_result_n46, hxc_l_tmp_load_result_n47, hxc_l_tmp_load_result_n48, hxc_l_tmp_load_result_n49, hxc_l_tmp_load_result_n50, hxc_l_tmp_load_result_n51, hxc_l_tmp_load_result_n52, hxc_l_v1);
    rlEnd();
    rlSetTexture((uint32_t)0);
    return true;
  }
  return false;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawAtlasSprite(struct Camera3D hxc_l_camera, struct Texture hxc_l_texture, int32_t hxc_l_cellIndex, int32_t hxc_l_columns, int32_t hxc_l_rows, struct Vector3 hxc_l_position, double hxc_l_width, double hxc_l_height)
{
  int32_t hxc_l_column = { 0 };
  int32_t hxc_l_row = { 0 };
  struct Color hxc_l_this1 = { 0 };
  double hxc_l_sourceWidth = { 0 };
  double hxc_l_sourceHeight = { 0 };
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_columns <= 0;
  if (!(hxc_l_columns <= 0))
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_rows <= 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n0 = hxc_l_tmp_short_circuit_result_n8;
  bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_short_circuit_load_result_n0;
  if (!hxc_l_tmp_short_circuit_load_result_n0)
  {
    hxc_l_tmp_short_circuit_result_n9 = hxc_l_cellIndex < 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n1 = hxc_l_tmp_short_circuit_result_n9;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_short_circuit_load_result_n1;
  if (!hxc_l_tmp_short_circuit_load_result_n1)
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_cellIndex >= hxc_i32_multiply_wrapping(hxc_l_columns, hxc_l_rows);
  }
  if (!hxc_l_tmp_short_circuit_result_n10)
  {
    hxc_l_column = hxc_i32_modulo_zero_safe(hxc_l_cellIndex, hxc_l_columns);
    hxc_l_row = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_cellIndex, (double)hxc_l_columns));
    hxc_l_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    hxc_l_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.width, (double)hxc_l_columns);
    hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.height, (double)hxc_l_rows);
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_column;
    double hxc_l_tmp_load_result_n7 = hxc_l_sourceWidth;
    int32_t hxc_l_tmp_load_result_n8 = hxc_l_row;
    double hxc_l_tmp_load_result_n9 = hxc_l_sourceHeight;
    double hxc_l_tmp_load_result_n10 = hxc_l_sourceWidth;
    double hxc_l_tmp_load_result_n11 = hxc_l_sourceHeight;
    DrawBillboardRec(hxc_l_camera, hxc_l_texture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n6 * hxc_l_tmp_load_result_n7), .y = (float)((double)hxc_l_tmp_load_result_n8 * hxc_l_tmp_load_result_n9), .width = (float)hxc_l_tmp_load_result_n10, .height = (float)hxc_l_tmp_load_result_n11 }, hxc_l_position, (struct Vector2){ .x = (float)hxc_l_width, .y = (float)hxc_l_height }, hxc_l_this1);
    return;
  }
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawEntitySprite(struct Camera3D hxc_l_camera, struct Texture hxc_l_texture, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_position, double hxc_l_width, double hxc_l_height)
{
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_cellIndex >= 0;
  if (hxc_l_cellIndex >= 0)
  {
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_cellIndex < 20;
  }
  if (!hxc_l_tmp_short_circuit_result_n6)
  {
    return;
  }
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_cellIndex >= 0;
  if (hxc_l_cellIndex >= 0)
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_cellIndex < 20;
  }
  bool hxc_l_tmp_short_circuit_load_result_n1 = hxc_l_tmp_short_circuit_result_n8;
  int32_t hxc_l_tmp_conditional_result_n9 = 0;
  if (hxc_l_tmp_short_circuit_load_result_n1)
  {
    hxc_l_tmp_conditional_result_n9 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_cellIndex, (double)4));
  }
  else
  {
    hxc_l_tmp_conditional_result_n9 = -1;
  }
  int32_t hxc_l_row = hxc_l_tmp_conditional_result_n9;
  struct Color hxc_l_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
  double hxc_l_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.width, (double)4);
  double hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.height, (double)5);
  struct Camera3D hxc_l_tmp_native_call_argument_0_n13 = hxc_l_camera;
  struct Texture hxc_l_tmp_native_call_argument_1_n14 = hxc_l_texture;
  bool hxc_l_tmp_short_circuit_result_n15 = hxc_l_cellIndex >= 0;
  if (hxc_l_cellIndex >= 0)
  {
    hxc_l_tmp_short_circuit_result_n15 = hxc_l_cellIndex < 20;
  }
  bool hxc_l_tmp_short_circuit_load_result_n6 = hxc_l_tmp_short_circuit_result_n15;
  int32_t hxc_l_tmp_conditional_result_n16 = 0;
  if (hxc_l_tmp_short_circuit_load_result_n6)
  {
    hxc_l_tmp_conditional_result_n16 = hxc_i32_modulo_zero_safe(hxc_l_cellIndex, 4);
  }
  else
  {
    hxc_l_tmp_conditional_result_n16 = -1;
  }
  int32_t hxc_l_tmp_conditional_load_result_n7 = hxc_l_tmp_conditional_result_n16;
  double hxc_l_tmp_load_result_n8 = hxc_l_sourceWidth;
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_row;
  double hxc_l_tmp_load_result_n10 = hxc_l_sourceHeight;
  double hxc_l_tmp_load_result_n11 = hxc_l_sourceWidth;
  double hxc_l_tmp_load_result_n12 = hxc_l_sourceHeight;
  struct Color hxc_l_tmp_load_result_n15 = hxc_l_this1;
  struct Camera3D hxc_l_tmp_native_call_argument_0_load_result_n16 = hxc_l_tmp_native_call_argument_0_n13;
  DrawBillboardRec(hxc_l_tmp_native_call_argument_0_load_result_n16, hxc_l_tmp_native_call_argument_1_n14, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_conditional_load_result_n7 * hxc_l_tmp_load_result_n8), .y = (float)((double)hxc_l_tmp_load_result_n9 * hxc_l_tmp_load_result_n10), .width = (float)hxc_l_tmp_load_result_n11, .height = (float)hxc_l_tmp_load_result_n12 }, hxc_l_position, (struct Vector2){ .x = (float)hxc_l_width, .y = (float)hxc_l_height }, hxc_l_tmp_load_result_n15);
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(struct Texture hxc_l_texture, enum hxc_caxecraft_app_HotbarFrame hxc_l_frame, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_size)
{
  int32_t hxc_l_column = 0;
  switch (hxc_l_frame) {
    case hxc_caxecraft_app_HotbarFrame_Normal:
      {
        break;
      }
    case hxc_caxecraft_app_HotbarFrame_Selected:
      {
        hxc_l_column = 1;
        break;
      }
    default:
      {
        abort();
      }
  }
  struct Color hxc_l_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
  double hxc_l_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.width, (double)4);
  double hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.height, (double)4);
  int32_t hxc_l_tmp_load_result_n3 = hxc_l_column;
  double hxc_l_tmp_load_result_n4 = hxc_l_sourceWidth;
  double hxc_l_tmp_load_result_n5 = hxc_l_sourceHeight;
  double hxc_l_tmp_load_result_n6 = hxc_l_sourceWidth;
  double hxc_l_tmp_load_result_n7 = hxc_l_sourceHeight;
  DrawTexturePro(hxc_l_texture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n3 * hxc_l_tmp_load_result_n4), .y = (float)((double)2 * hxc_l_tmp_load_result_n5), .width = (float)hxc_l_tmp_load_result_n6, .height = (float)hxc_l_tmp_load_result_n7 }, (struct Rectangle){ .x = (float)((double)hxc_l_x + 0.0), .y = (float)((double)hxc_l_y + 0.0), .width = (float)((double)hxc_l_size + 0.0), .height = (float)((double)hxc_l_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1);
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(struct Texture hxc_l_texture, enum hxc_caxecraft_app_HudGlyph hxc_l_glyph, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_size)
{
  int32_t hxc_l_column = 0;
  switch (hxc_l_glyph) {
    case hxc_caxecraft_app_HudGlyph_HealthFull:
      {
        break;
      }
    case hxc_caxecraft_app_HudGlyph_HealthHalf:
      {
        hxc_l_column = 1;
        break;
      }
    case hxc_caxecraft_app_HudGlyph_HealthEmpty:
      {
        hxc_l_column = 2;
        break;
      }
    default:
      {
        abort();
      }
  }
  struct Color hxc_l_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
  double hxc_l_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.width, (double)4);
  double hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.height, (double)4);
  int32_t hxc_l_tmp_load_result_n3 = hxc_l_column;
  double hxc_l_tmp_load_result_n4 = hxc_l_sourceWidth;
  double hxc_l_tmp_load_result_n5 = hxc_l_sourceHeight;
  double hxc_l_tmp_load_result_n6 = hxc_l_sourceWidth;
  double hxc_l_tmp_load_result_n7 = hxc_l_sourceHeight;
  DrawTexturePro(hxc_l_texture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n3 * hxc_l_tmp_load_result_n4), .y = (float)((double)0 * hxc_l_tmp_load_result_n5), .width = (float)hxc_l_tmp_load_result_n6, .height = (float)hxc_l_tmp_load_result_n7 }, (struct Rectangle){ .x = (float)((double)hxc_l_x + 0.0), .y = (float)((double)hxc_l_y + 0.0), .width = (float)((double)hxc_l_size + 0.0), .height = (float)((double)hxc_l_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1);
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawItem(struct Texture hxc_l_texture, enum hxc_caxecraft_gameplay_ItemKind hxc_l_item, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_size)
{
  int32_t hxc_l_column = 0;
  int32_t hxc_l_row = 0;
  switch (hxc_l_item) {
    case hxc_caxecraft_gameplay_ItemKind_GrassBlock:
      {
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_DirtBlock:
      {
        hxc_l_column = 1;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_StoneBlock:
      {
        hxc_l_column = 2;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Haxeforge:
      {
        hxc_l_row = 1;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_CopperSword:
      {
        hxc_l_column = 3;
        hxc_l_row = 1;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Berries:
      {
        hxc_l_column = 2;
        hxc_l_row = 2;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Bread:
      {
        hxc_l_column = 3;
        hxc_l_row = 2;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Lantern:
      {
        hxc_l_column = 1;
        hxc_l_row = 3;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_SandBlock:
      {
        hxc_l_column = 2;
        hxc_l_row = 3;
        break;
      }
    default:
      {
        abort();
      }
  }
  struct Color hxc_l_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
  double hxc_l_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.width, (double)4);
  double hxc_l_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_texture.height, (double)4);
  int32_t hxc_l_tmp_load_result_n3 = hxc_l_column;
  double hxc_l_tmp_load_result_n4 = hxc_l_sourceWidth;
  int32_t hxc_l_tmp_load_result_n5 = hxc_l_row;
  double hxc_l_tmp_load_result_n6 = hxc_l_sourceHeight;
  double hxc_l_tmp_load_result_n7 = hxc_l_sourceWidth;
  double hxc_l_tmp_load_result_n8 = hxc_l_sourceHeight;
  DrawTexturePro(hxc_l_texture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n3 * hxc_l_tmp_load_result_n4), .y = (float)((double)hxc_l_tmp_load_result_n5 * hxc_l_tmp_load_result_n6), .width = (float)hxc_l_tmp_load_result_n7, .height = (float)hxc_l_tmp_load_result_n8 }, (struct Rectangle){ .x = (float)((double)hxc_l_x + 0.0), .y = (float)((double)hxc_l_y + 0.0), .width = (float)((double)hxc_l_size + 0.0), .height = (float)((double)hxc_l_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1);
  return;
}

bool hxc_caxecraft_app_CaxecraftAtlas_drawWorldBox(struct Texture hxc_l_texture, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_center, double hxc_l_width, double hxc_l_height, double hxc_l_depth)
{
  bool hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_CaxecraftAtlas_drawAtlasBox(hxc_l_texture, hxc_l_cellIndex, 4, 4, hxc_l_center, hxc_l_width, hxc_l_height, hxc_l_depth);
  return hxc_l_tmp_call_result_n0;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(struct Camera3D hxc_l_camera, struct Texture hxc_l_texture, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_position, double hxc_l_width, double hxc_l_height)
{
  hxc_caxecraft_app_CaxecraftAtlas_drawAtlasSprite(hxc_l_camera, hxc_l_texture, hxc_l_cellIndex, 4, 4, hxc_l_position, hxc_l_width, hxc_l_height);
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_emitTiledEastWest(double hxc_l_x0, double hxc_l_x1, double hxc_l_y0, double hxc_l_y1, double hxc_l_z0, double hxc_l_z1, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  double hxc_l_z = hxc_l_z0;
  while (1)
  {
    if (!(hxc_l_z < hxc_l_z1))
    {
      break;
    }
    double hxc_l_tmp_load_result_n1 = hxc_l_z;
    double hxc_l_tmp_conditional_result_n12 = 0.0;
    if (hxc_l_tmp_load_result_n1 + 1.0 < hxc_l_z1)
    {
      hxc_l_tmp_conditional_result_n12 = hxc_l_z + 1.0;
    }
    else
    {
      hxc_l_tmp_conditional_result_n12 = hxc_l_z1;
    }
    double hxc_l_nextZ = hxc_l_tmp_conditional_result_n12;
    double hxc_l_y = hxc_l_y0;
    while (1)
    {
      if (!(hxc_l_y < hxc_l_y1))
      {
        break;
      }
      double hxc_l_tmp_load_result_n5 = hxc_l_y;
      double hxc_l_tmp_conditional_result_n15 = 0.0;
      if (hxc_l_tmp_load_result_n5 + 1.0 < hxc_l_y1)
      {
        hxc_l_tmp_conditional_result_n15 = hxc_l_y + 1.0;
      }
      else
      {
        hxc_l_tmp_conditional_result_n15 = hxc_l_y1;
      }
      double hxc_l_nextY = hxc_l_tmp_conditional_result_n15;
      struct Color hxc_l_raw_h2805f16948c3 = (struct Color){ .r = 235, .g = 241, .b = 230, .a = 255 };
      uint8_t *hxc_l_tmp_imported_field_address_n9 = &hxc_l_raw_h2805f16948c3.r;
      uint8_t hxc_l_tmp_imported_field_load_result_n10 = *hxc_l_tmp_imported_field_address_n9;
      uint8_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw_h2805f16948c3.g;
      uint8_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
      uint8_t *hxc_l_tmp_imported_field_address_n13 = &hxc_l_raw_h2805f16948c3.b;
      uint8_t hxc_l_tmp_imported_field_load_result_n14 = *hxc_l_tmp_imported_field_address_n13;
      uint8_t *hxc_l_tmp_imported_field_address_n15 = &hxc_l_raw_h2805f16948c3.a;
      uint8_t hxc_l_tmp_imported_field_load_result_n16 = *hxc_l_tmp_imported_field_address_n15;
      rlColor4ub(hxc_l_tmp_imported_field_load_result_n10, hxc_l_tmp_imported_field_load_result_n12, hxc_l_tmp_imported_field_load_result_n14, hxc_l_tmp_imported_field_load_result_n16);
      rlNormal3f((float)1.0, (float)0.0, (float)0.0);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n17 = hxc_l_y;
      rlVertex3f((float)hxc_l_x1, (float)hxc_l_tmp_load_result_n17, (float)hxc_l_z);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n19 = hxc_l_nextY;
      rlVertex3f((float)hxc_l_x1, (float)hxc_l_tmp_load_result_n19, (float)hxc_l_z);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n21 = hxc_l_nextY;
      rlVertex3f((float)hxc_l_x1, (float)hxc_l_tmp_load_result_n21, (float)hxc_l_nextZ);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n23 = hxc_l_y;
      rlVertex3f((float)hxc_l_x1, (float)hxc_l_tmp_load_result_n23, (float)hxc_l_nextZ);
      struct Color hxc_l_raw_hed704932c93a = (struct Color){ .r = 211, .g = 225, .b = 220, .a = 255 };
      uint8_t *hxc_l_tmp_imported_field_address_n26 = &hxc_l_raw_hed704932c93a.r;
      uint8_t hxc_l_tmp_imported_field_load_result_n27 = *hxc_l_tmp_imported_field_address_n26;
      uint8_t *hxc_l_tmp_imported_field_address_n28 = &hxc_l_raw_hed704932c93a.g;
      uint8_t hxc_l_tmp_imported_field_load_result_n29 = *hxc_l_tmp_imported_field_address_n28;
      uint8_t *hxc_l_tmp_imported_field_address_n30 = &hxc_l_raw_hed704932c93a.b;
      uint8_t hxc_l_tmp_imported_field_load_result_n31 = *hxc_l_tmp_imported_field_address_n30;
      uint8_t *hxc_l_tmp_imported_field_address_n32 = &hxc_l_raw_hed704932c93a.a;
      uint8_t hxc_l_tmp_imported_field_load_result_n33 = *hxc_l_tmp_imported_field_address_n32;
      rlColor4ub(hxc_l_tmp_imported_field_load_result_n27, hxc_l_tmp_imported_field_load_result_n29, hxc_l_tmp_imported_field_load_result_n31, hxc_l_tmp_imported_field_load_result_n33);
      rlNormal3f((float)-1.0, (float)0.0, (float)0.0);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n34 = hxc_l_y;
      rlVertex3f((float)hxc_l_x0, (float)hxc_l_tmp_load_result_n34, (float)hxc_l_z);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n36 = hxc_l_y;
      rlVertex3f((float)hxc_l_x0, (float)hxc_l_tmp_load_result_n36, (float)hxc_l_nextZ);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n38 = hxc_l_nextY;
      rlVertex3f((float)hxc_l_x0, (float)hxc_l_tmp_load_result_n38, (float)hxc_l_nextZ);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n40 = hxc_l_nextY;
      rlVertex3f((float)hxc_l_x0, (float)hxc_l_tmp_load_result_n40, (float)hxc_l_z);
      hxc_l_y = hxc_l_nextY;
    }
    hxc_l_z = hxc_l_nextZ;
  }
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_emitTiledNorthSouth(double hxc_l_x0, double hxc_l_x1, double hxc_l_y0, double hxc_l_y1, double hxc_l_z0, double hxc_l_z1, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  double hxc_l_x = hxc_l_x0;
  while (1)
  {
    if (!(hxc_l_x < hxc_l_x1))
    {
      break;
    }
    double hxc_l_tmp_load_result_n1 = hxc_l_x;
    double hxc_l_tmp_conditional_result_n12 = 0.0;
    if (hxc_l_tmp_load_result_n1 + 1.0 < hxc_l_x1)
    {
      hxc_l_tmp_conditional_result_n12 = hxc_l_x + 1.0;
    }
    else
    {
      hxc_l_tmp_conditional_result_n12 = hxc_l_x1;
    }
    double hxc_l_nextX = hxc_l_tmp_conditional_result_n12;
    double hxc_l_y = hxc_l_y0;
    while (1)
    {
      if (!(hxc_l_y < hxc_l_y1))
      {
        break;
      }
      double hxc_l_tmp_load_result_n5 = hxc_l_y;
      double hxc_l_tmp_conditional_result_n15 = 0.0;
      if (hxc_l_tmp_load_result_n5 + 1.0 < hxc_l_y1)
      {
        hxc_l_tmp_conditional_result_n15 = hxc_l_y + 1.0;
      }
      else
      {
        hxc_l_tmp_conditional_result_n15 = hxc_l_y1;
      }
      double hxc_l_nextY = hxc_l_tmp_conditional_result_n15;
      struct Color hxc_l_raw_hfce52b8a1f2e = (struct Color){ .r = 235, .g = 241, .b = 230, .a = 255 };
      uint8_t *hxc_l_tmp_imported_field_address_n9 = &hxc_l_raw_hfce52b8a1f2e.r;
      uint8_t hxc_l_tmp_imported_field_load_result_n10 = *hxc_l_tmp_imported_field_address_n9;
      uint8_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw_hfce52b8a1f2e.g;
      uint8_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
      uint8_t *hxc_l_tmp_imported_field_address_n13 = &hxc_l_raw_hfce52b8a1f2e.b;
      uint8_t hxc_l_tmp_imported_field_load_result_n14 = *hxc_l_tmp_imported_field_address_n13;
      uint8_t *hxc_l_tmp_imported_field_address_n15 = &hxc_l_raw_hfce52b8a1f2e.a;
      uint8_t hxc_l_tmp_imported_field_load_result_n16 = *hxc_l_tmp_imported_field_address_n15;
      rlColor4ub(hxc_l_tmp_imported_field_load_result_n10, hxc_l_tmp_imported_field_load_result_n12, hxc_l_tmp_imported_field_load_result_n14, hxc_l_tmp_imported_field_load_result_n16);
      rlNormal3f((float)0.0, (float)0.0, (float)-1.0);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n17 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n17, (float)hxc_l_y, (float)hxc_l_z0);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n19 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n19, (float)hxc_l_nextY, (float)hxc_l_z0);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n21 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n21, (float)hxc_l_nextY, (float)hxc_l_z0);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n23 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n23, (float)hxc_l_y, (float)hxc_l_z0);
      struct Color hxc_l_raw_h137e9b2af151 = (struct Color){ .r = 211, .g = 225, .b = 220, .a = 255 };
      uint8_t *hxc_l_tmp_imported_field_address_n26 = &hxc_l_raw_h137e9b2af151.r;
      uint8_t hxc_l_tmp_imported_field_load_result_n27 = *hxc_l_tmp_imported_field_address_n26;
      uint8_t *hxc_l_tmp_imported_field_address_n28 = &hxc_l_raw_h137e9b2af151.g;
      uint8_t hxc_l_tmp_imported_field_load_result_n29 = *hxc_l_tmp_imported_field_address_n28;
      uint8_t *hxc_l_tmp_imported_field_address_n30 = &hxc_l_raw_h137e9b2af151.b;
      uint8_t hxc_l_tmp_imported_field_load_result_n31 = *hxc_l_tmp_imported_field_address_n30;
      uint8_t *hxc_l_tmp_imported_field_address_n32 = &hxc_l_raw_h137e9b2af151.a;
      uint8_t hxc_l_tmp_imported_field_load_result_n33 = *hxc_l_tmp_imported_field_address_n32;
      rlColor4ub(hxc_l_tmp_imported_field_load_result_n27, hxc_l_tmp_imported_field_load_result_n29, hxc_l_tmp_imported_field_load_result_n31, hxc_l_tmp_imported_field_load_result_n33);
      rlNormal3f((float)0.0, (float)0.0, (float)1.0);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n34 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n34, (float)hxc_l_y, (float)hxc_l_z1);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n36 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n36, (float)hxc_l_y, (float)hxc_l_z1);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n38 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n38, (float)hxc_l_nextY, (float)hxc_l_z1);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n40 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n40, (float)hxc_l_nextY, (float)hxc_l_z1);
      hxc_l_y = hxc_l_nextY;
    }
    hxc_l_x = hxc_l_nextX;
  }
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_emitTiledTopBottom(double hxc_l_x0, double hxc_l_x1, double hxc_l_y0, double hxc_l_y1, double hxc_l_z0, double hxc_l_z1, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1)
{
  double hxc_l_x = hxc_l_x0;
  while (1)
  {
    if (!(hxc_l_x < hxc_l_x1))
    {
      break;
    }
    double hxc_l_tmp_load_result_n1 = hxc_l_x;
    double hxc_l_tmp_conditional_result_n12 = 0.0;
    if (hxc_l_tmp_load_result_n1 + 1.0 < hxc_l_x1)
    {
      hxc_l_tmp_conditional_result_n12 = hxc_l_x + 1.0;
    }
    else
    {
      hxc_l_tmp_conditional_result_n12 = hxc_l_x1;
    }
    double hxc_l_nextX = hxc_l_tmp_conditional_result_n12;
    double hxc_l_z = hxc_l_z0;
    while (1)
    {
      if (!(hxc_l_z < hxc_l_z1))
      {
        break;
      }
      double hxc_l_tmp_load_result_n5 = hxc_l_z;
      double hxc_l_tmp_conditional_result_n15 = 0.0;
      if (hxc_l_tmp_load_result_n5 + 1.0 < hxc_l_z1)
      {
        hxc_l_tmp_conditional_result_n15 = hxc_l_z + 1.0;
      }
      else
      {
        hxc_l_tmp_conditional_result_n15 = hxc_l_z1;
      }
      double hxc_l_nextZ = hxc_l_tmp_conditional_result_n15;
      struct Color hxc_l_raw_h56b208e5795e = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
      uint8_t *hxc_l_tmp_imported_field_address_n9 = &hxc_l_raw_h56b208e5795e.r;
      uint8_t hxc_l_tmp_imported_field_load_result_n10 = *hxc_l_tmp_imported_field_address_n9;
      uint8_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw_h56b208e5795e.g;
      uint8_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
      uint8_t *hxc_l_tmp_imported_field_address_n13 = &hxc_l_raw_h56b208e5795e.b;
      uint8_t hxc_l_tmp_imported_field_load_result_n14 = *hxc_l_tmp_imported_field_address_n13;
      uint8_t *hxc_l_tmp_imported_field_address_n15 = &hxc_l_raw_h56b208e5795e.a;
      uint8_t hxc_l_tmp_imported_field_load_result_n16 = *hxc_l_tmp_imported_field_address_n15;
      rlColor4ub(hxc_l_tmp_imported_field_load_result_n10, hxc_l_tmp_imported_field_load_result_n12, hxc_l_tmp_imported_field_load_result_n14, hxc_l_tmp_imported_field_load_result_n16);
      rlNormal3f((float)0.0, (float)1.0, (float)0.0);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n17 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n17, (float)hxc_l_y1, (float)hxc_l_z);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n19 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n19, (float)hxc_l_y1, (float)hxc_l_nextZ);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n21 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n21, (float)hxc_l_y1, (float)hxc_l_nextZ);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n23 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n23, (float)hxc_l_y1, (float)hxc_l_z);
      struct Color hxc_l_raw_h058c6ba10874 = (struct Color){ .r = 178, .g = 191, .b = 188, .a = 255 };
      uint8_t *hxc_l_tmp_imported_field_address_n26 = &hxc_l_raw_h058c6ba10874.r;
      uint8_t hxc_l_tmp_imported_field_load_result_n27 = *hxc_l_tmp_imported_field_address_n26;
      uint8_t *hxc_l_tmp_imported_field_address_n28 = &hxc_l_raw_h058c6ba10874.g;
      uint8_t hxc_l_tmp_imported_field_load_result_n29 = *hxc_l_tmp_imported_field_address_n28;
      uint8_t *hxc_l_tmp_imported_field_address_n30 = &hxc_l_raw_h058c6ba10874.b;
      uint8_t hxc_l_tmp_imported_field_load_result_n31 = *hxc_l_tmp_imported_field_address_n30;
      uint8_t *hxc_l_tmp_imported_field_address_n32 = &hxc_l_raw_h058c6ba10874.a;
      uint8_t hxc_l_tmp_imported_field_load_result_n33 = *hxc_l_tmp_imported_field_address_n32;
      rlColor4ub(hxc_l_tmp_imported_field_load_result_n27, hxc_l_tmp_imported_field_load_result_n29, hxc_l_tmp_imported_field_load_result_n31, hxc_l_tmp_imported_field_load_result_n33);
      rlNormal3f((float)0.0, (float)-1.0, (float)0.0);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n34 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n34, (float)hxc_l_y0, (float)hxc_l_z);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v0);
      double hxc_l_tmp_load_result_n36 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n36, (float)hxc_l_y0, (float)hxc_l_z);
      rlTexCoord2f((float)hxc_l_u1, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n38 = hxc_l_nextX;
      rlVertex3f((float)hxc_l_tmp_load_result_n38, (float)hxc_l_y0, (float)hxc_l_nextZ);
      rlTexCoord2f((float)hxc_l_u0, (float)hxc_l_v1);
      double hxc_l_tmp_load_result_n40 = hxc_l_x;
      rlVertex3f((float)hxc_l_tmp_load_result_n40, (float)hxc_l_y0, (float)hxc_l_nextZ);
      hxc_l_z = hxc_l_nextZ;
    }
    hxc_l_x = hxc_l_nextX;
  }
  return;
}
