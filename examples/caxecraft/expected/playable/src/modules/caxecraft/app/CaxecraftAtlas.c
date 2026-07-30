#include "hxc/program.h"

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

void hxc_caxecraft_app_CaxecraftAtlas_drawWorldSprite(struct Camera3D hxc_l_camera, struct Texture hxc_l_texture, enum hxc_caxecraft_app_WorldSprite hxc_l_sprite, struct Vector3 hxc_l_position, double hxc_l_width, double hxc_l_height)
{
  int32_t hxc_l_row = 1;
  switch (hxc_l_sprite) {
    case hxc_caxecraft_app_WorldSprite_NiaFront:
      {
        break;
      }
    case hxc_caxecraft_app_WorldSprite_MosslingFront:
      {
        hxc_l_row = 2;
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
  double hxc_l_tmp_load_result_n3 = hxc_l_sourceWidth;
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_row;
  double hxc_l_tmp_load_result_n5 = hxc_l_sourceHeight;
  double hxc_l_tmp_load_result_n6 = hxc_l_sourceWidth;
  double hxc_l_tmp_load_result_n7 = hxc_l_sourceHeight;
  DrawBillboardRec(hxc_l_camera, hxc_l_texture, (struct Rectangle){ .x = (float)((double)0 * hxc_l_tmp_load_result_n3), .y = (float)((double)hxc_l_tmp_load_result_n4 * hxc_l_tmp_load_result_n5), .width = (float)hxc_l_tmp_load_result_n6, .height = (float)hxc_l_tmp_load_result_n7 }, hxc_l_position, (struct Vector2){ .x = (float)hxc_l_width, .y = (float)hxc_l_height }, hxc_l_this1);
  return;
}
