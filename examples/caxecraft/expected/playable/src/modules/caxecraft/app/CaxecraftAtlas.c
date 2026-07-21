#include "hxc/program.h"

void hxc_caxecraft_app_CaxecraftAtlas_drawHotbarFrame(struct Texture hxc_texture, enum hxc_caxecraft_app_HotbarFrame hxc_frame, int32_t hxc_x, int32_t hxc_y, int32_t hxc_size)
{
  int32_t hxc_column = 0;
  switch (hxc_frame) {
    case hxc_caxecraft_app_HotbarFrame_Normal:
      {
        break;
      }
    case hxc_caxecraft_app_HotbarFrame_Selected:
      {
        hxc_column = 1;
        break;
      }
  }
  struct Color hxc_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
  double hxc_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_texture.width, (double)4);
  double hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_texture.height, (double)4);
  int32_t hxc_tmp_load_result_n3 = hxc_column;
  double hxc_tmp_load_result_n4 = hxc_sourceWidth;
  double hxc_tmp_load_result_n5 = hxc_sourceHeight;
  double hxc_tmp_load_result_n6 = hxc_sourceWidth;
  double hxc_tmp_load_result_n7 = hxc_sourceHeight;
  DrawTexturePro(hxc_texture, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n3 * hxc_tmp_load_result_n4), .y = (float)((double)2 * hxc_tmp_load_result_n5), .width = (float)hxc_tmp_load_result_n6, .height = (float)hxc_tmp_load_result_n7 }, (struct Rectangle){ .x = (float)((double)hxc_x + 0.0), .y = (float)((double)hxc_y + 0.0), .width = (float)((double)hxc_size + 0.0), .height = (float)((double)hxc_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1);
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawHudGlyph(struct Texture hxc_texture, enum hxc_caxecraft_app_HudGlyph hxc_glyph, int32_t hxc_x, int32_t hxc_y, int32_t hxc_size)
{
  int32_t hxc_column = 0;
  switch (hxc_glyph) {
    case hxc_caxecraft_app_HudGlyph_HealthFull:
      {
        break;
      }
    case hxc_caxecraft_app_HudGlyph_HealthHalf:
      {
        hxc_column = 1;
        break;
      }
    case hxc_caxecraft_app_HudGlyph_HealthEmpty:
      {
        hxc_column = 2;
        break;
      }
  }
  struct Color hxc_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
  double hxc_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_texture.width, (double)4);
  double hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_texture.height, (double)4);
  int32_t hxc_tmp_load_result_n3 = hxc_column;
  double hxc_tmp_load_result_n4 = hxc_sourceWidth;
  double hxc_tmp_load_result_n5 = hxc_sourceHeight;
  double hxc_tmp_load_result_n6 = hxc_sourceWidth;
  double hxc_tmp_load_result_n7 = hxc_sourceHeight;
  DrawTexturePro(hxc_texture, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n3 * hxc_tmp_load_result_n4), .y = (float)((double)0 * hxc_tmp_load_result_n5), .width = (float)hxc_tmp_load_result_n6, .height = (float)hxc_tmp_load_result_n7 }, (struct Rectangle){ .x = (float)((double)hxc_x + 0.0), .y = (float)((double)hxc_y + 0.0), .width = (float)((double)hxc_size + 0.0), .height = (float)((double)hxc_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1);
  return;
}

void hxc_caxecraft_app_CaxecraftAtlas_drawItem(struct Texture hxc_texture, enum hxc_caxecraft_gameplay_ItemKind hxc_item, int32_t hxc_x, int32_t hxc_y, int32_t hxc_size)
{
  int32_t hxc_column = 0;
  int32_t hxc_row = 0;
  switch (hxc_item) {
    case hxc_caxecraft_gameplay_ItemKind_GrassBlock:
      {
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_DirtBlock:
      {
        hxc_column = 1;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_StoneBlock:
      {
        hxc_column = 2;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Haxeforge:
      {
        hxc_row = 1;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_CopperSword:
      {
        hxc_column = 3;
        hxc_row = 1;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Berries:
      {
        hxc_column = 2;
        hxc_row = 2;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Bread:
      {
        hxc_column = 3;
        hxc_row = 2;
        break;
      }
    case hxc_caxecraft_gameplay_ItemKind_Lantern:
      {
        hxc_column = 1;
        hxc_row = 3;
        break;
      }
  }
  struct Color hxc_this1 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
  double hxc_sourceWidth = hxc_f64_divide_zero_safe((double)(int32_t)hxc_texture.width, (double)4);
  double hxc_sourceHeight = hxc_f64_divide_zero_safe((double)(int32_t)hxc_texture.height, (double)4);
  int32_t hxc_tmp_load_result_n3 = hxc_column;
  double hxc_tmp_load_result_n4 = hxc_sourceWidth;
  int32_t hxc_tmp_load_result_n5 = hxc_row;
  double hxc_tmp_load_result_n6 = hxc_sourceHeight;
  double hxc_tmp_load_result_n7 = hxc_sourceWidth;
  double hxc_tmp_load_result_n8 = hxc_sourceHeight;
  DrawTexturePro(hxc_texture, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n3 * hxc_tmp_load_result_n4), .y = (float)((double)hxc_tmp_load_result_n5 * hxc_tmp_load_result_n6), .width = (float)hxc_tmp_load_result_n7, .height = (float)hxc_tmp_load_result_n8 }, (struct Rectangle){ .x = (float)((double)hxc_x + 0.0), .y = (float)((double)hxc_y + 0.0), .width = (float)((double)hxc_size + 0.0), .height = (float)((double)hxc_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1);
  return;
}
