#include "hxc/program.h"

void hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(int32_t hxc_code, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, struct Texture hxc_adventureTexture, bool hxc_adventureTextureReady, int32_t hxc_x, int32_t hxc_y, int32_t hxc_size)
{
  bool hxc_tmp_short_circuit_result_n8 = hxc_code >= 0;
  if (hxc_code >= 0)
  {
    hxc_tmp_short_circuit_result_n8 = hxc_code < 9;
  }
  if (!!hxc_tmp_short_circuit_result_n8)
  {
    int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_code);
    struct hxc_caxecraft_content_ContentPresentation hxc_tmp_call_result_n2 = hxc_caxecraft_content_BaseContentPack_itemIcon(hxc_tmp_call_result_n1);
    struct hxc_caxecraft_content_ContentPresentation hxc_presentation = hxc_tmp_call_result_n2;
    int32_t hxc_tmp_record_field_load_result_n3 = hxc_presentation.hxc_asset;
    bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_record_field_load_result_n3 == 2;
    if (hxc_tmp_record_field_load_result_n3 == 2)
    {
      hxc_tmp_short_circuit_result_n10 = hxc_itemTextureReady;
    }
    if (hxc_tmp_short_circuit_result_n10)
    {
      int32_t hxc_column_hf1e54e4b4fbb = hxc_i32_modulo_zero_safe(hxc_presentation.hxc_cellIndex, 4);
      int32_t hxc_row_h231fa05c69ed = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_presentation.hxc_cellIndex, (double)4));
      struct Color hxc_this1_haa9524c098c0 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
      double hxc_sourceWidth_h08e2ecedb701 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_itemTexture.width, (double)4);
      double hxc_sourceHeight_ha2df9f5be6f6 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_itemTexture.height, (double)4);
      int32_t hxc_tmp_load_result_n10 = hxc_column_hf1e54e4b4fbb;
      double hxc_tmp_load_result_n11 = hxc_sourceWidth_h08e2ecedb701;
      int32_t hxc_tmp_load_result_n12 = hxc_row_h231fa05c69ed;
      double hxc_tmp_load_result_n13 = hxc_sourceHeight_ha2df9f5be6f6;
      double hxc_tmp_load_result_n14 = hxc_sourceWidth_h08e2ecedb701;
      double hxc_tmp_load_result_n15 = hxc_sourceHeight_ha2df9f5be6f6;
      DrawTexturePro(hxc_itemTexture, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n10 * hxc_tmp_load_result_n11), .y = (float)((double)hxc_tmp_load_result_n12 * hxc_tmp_load_result_n13), .width = (float)hxc_tmp_load_result_n14, .height = (float)hxc_tmp_load_result_n15 }, (struct Rectangle){ .x = (float)((double)hxc_x + 0.0), .y = (float)((double)hxc_y + 0.0), .width = (float)((double)hxc_size + 0.0), .height = (float)((double)hxc_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_haa9524c098c0);
    }
    else
    {
      int32_t hxc_tmp_record_field_load_result_n20 = hxc_presentation.hxc_asset;
      bool hxc_tmp_short_circuit_result_n16 = hxc_tmp_record_field_load_result_n20 == 0;
      if (hxc_tmp_record_field_load_result_n20 == 0)
      {
        hxc_tmp_short_circuit_result_n16 = hxc_adventureTextureReady;
      }
      if (hxc_tmp_short_circuit_result_n16)
      {
        int32_t hxc_column_heef4af09bea0 = hxc_i32_modulo_zero_safe(hxc_presentation.hxc_cellIndex, 4);
        int32_t hxc_row_h9795344089dc = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_presentation.hxc_cellIndex, (double)4));
        struct Color hxc_this1_hab22142602b8 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
        double hxc_sourceWidth_h364f7c10b0e6 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_adventureTexture.width, (double)4);
        double hxc_sourceHeight_h84d482298038 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_adventureTexture.height, (double)4);
        int32_t hxc_tmp_load_result_n27 = hxc_column_heef4af09bea0;
        double hxc_tmp_load_result_n28 = hxc_sourceWidth_h364f7c10b0e6;
        int32_t hxc_tmp_load_result_n29 = hxc_row_h9795344089dc;
        double hxc_tmp_load_result_n30 = hxc_sourceHeight_h84d482298038;
        double hxc_tmp_load_result_n31 = hxc_sourceWidth_h364f7c10b0e6;
        double hxc_tmp_load_result_n32 = hxc_sourceHeight_h84d482298038;
        DrawTexturePro(hxc_adventureTexture, (struct Rectangle){ .x = (float)((double)hxc_tmp_load_result_n27 * hxc_tmp_load_result_n28), .y = (float)((double)hxc_tmp_load_result_n29 * hxc_tmp_load_result_n30), .width = (float)hxc_tmp_load_result_n31, .height = (float)hxc_tmp_load_result_n32 }, (struct Rectangle){ .x = (float)((double)hxc_x + 0.0), .y = (float)((double)hxc_y + 0.0), .width = (float)((double)hxc_size + 0.0), .height = (float)((double)hxc_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_this1_hab22142602b8);
      }
    }
    return;
  }
  return;
}

void hxc_caxecraft_app_AuthoredItemRenderer_drawPresentation(struct Camera3D hxc_camera, int32_t hxc_asset, int32_t hxc_cellIndex, struct Vector3 hxc_position, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, struct Texture hxc_adventureTexture, bool hxc_adventureTextureReady, double hxc_width, double hxc_height)
{
  bool hxc_tmp_short_circuit_result_n10 = hxc_asset == 2;
  if (hxc_asset == 2)
  {
    hxc_tmp_short_circuit_result_n10 = hxc_itemTextureReady;
  }
  if (hxc_tmp_short_circuit_result_n10)
  {
    int32_t hxc_row_h81252ef7f0a7 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_cellIndex, (double)4));
    struct Color hxc_this1_h1b575f97dd8f = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    double hxc_sourceWidth_h46384f71c518 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_itemTexture.width, (double)4);
    double hxc_sourceHeight_hd77ac56d8f30 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_itemTexture.height, (double)4);
    double hxc_tmp_load_result_n4 = hxc_sourceWidth_h46384f71c518;
    int32_t hxc_tmp_load_result_n5 = hxc_row_h81252ef7f0a7;
    double hxc_tmp_load_result_n6 = hxc_sourceHeight_hd77ac56d8f30;
    double hxc_tmp_load_result_n7 = hxc_sourceWidth_h46384f71c518;
    double hxc_tmp_load_result_n8 = hxc_sourceHeight_hd77ac56d8f30;
    DrawBillboardRec(hxc_camera, hxc_itemTexture, (struct Rectangle){ .x = (float)((double)hxc_i32_modulo_zero_safe(hxc_cellIndex, 4) * hxc_tmp_load_result_n4), .y = (float)((double)hxc_tmp_load_result_n5 * hxc_tmp_load_result_n6), .width = (float)hxc_tmp_load_result_n7, .height = (float)hxc_tmp_load_result_n8 }, hxc_position, (struct Vector2){ .x = (float)hxc_width, .y = (float)hxc_height }, hxc_this1_h1b575f97dd8f);
  }
  else
  {
    bool hxc_tmp_short_circuit_result_n15 = hxc_asset == 0;
    if (hxc_asset == 0)
    {
      hxc_tmp_short_circuit_result_n15 = hxc_adventureTextureReady;
    }
    if (hxc_tmp_short_circuit_result_n15)
    {
      int32_t hxc_row_hf0aafdaaba57 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_cellIndex, (double)4));
      struct Color hxc_this1_hbb0c3574dafd = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
      double hxc_sourceWidth_haba66acfde60 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_adventureTexture.width, (double)4);
      double hxc_sourceHeight_hdecce6c0ea9c = hxc_f64_divide_zero_safe((double)(int32_t)hxc_adventureTexture.height, (double)4);
      double hxc_tmp_load_result_n16 = hxc_sourceWidth_haba66acfde60;
      int32_t hxc_tmp_load_result_n17 = hxc_row_hf0aafdaaba57;
      double hxc_tmp_load_result_n18 = hxc_sourceHeight_hdecce6c0ea9c;
      double hxc_tmp_load_result_n19 = hxc_sourceWidth_haba66acfde60;
      double hxc_tmp_load_result_n20 = hxc_sourceHeight_hdecce6c0ea9c;
      DrawBillboardRec(hxc_camera, hxc_adventureTexture, (struct Rectangle){ .x = (float)((double)hxc_i32_modulo_zero_safe(hxc_cellIndex, 4) * hxc_tmp_load_result_n16), .y = (float)((double)hxc_tmp_load_result_n17 * hxc_tmp_load_result_n18), .width = (float)hxc_tmp_load_result_n19, .height = (float)hxc_tmp_load_result_n20 }, hxc_position, (struct Vector2){ .x = (float)hxc_width, .y = (float)hxc_height }, hxc_this1_hbb0c3574dafd);
    }
  }
  return;
}

void hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(struct Camera3D hxc_camera, const int32_t *hxc_active, size_t hxc_length, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, struct Texture hxc_adventureTexture, bool hxc_adventureTextureReady)
{
  const int32_t *hxc_borrow = hxc_active;
  size_t hxc_tmp_length_n6 = hxc_length;
  int32_t hxc_inactive = 0;
  int32_t hxc_index = 0;
  while (1)
  {
    if (!(hxc_index < 1))
    {
      break;
    }
    int32_t hxc_tmp_load_result_n1 = hxc_index;
    if (hxc_tmp_load_result_n1 < 0 || (size_t)hxc_tmp_load_result_n1 >= hxc_tmp_length_n6)
    {
      abort();
    }
    int32_t hxc_tmp_collection_index_load_result_n2 = hxc_borrow[(size_t)hxc_tmp_load_result_n1];
    if (hxc_tmp_collection_index_load_result_n2 != hxc_inactive)
    {
      int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_content_FirstPlayableLevel_itemStorageCode(hxc_index);
      int32_t hxc_code = hxc_tmp_call_result_n5;
      int32_t hxc_tmp_load_result_n6 = hxc_code;
      bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_load_result_n6 >= 0;
      if (hxc_tmp_load_result_n6 >= 0)
      {
        hxc_tmp_short_circuit_result_n10 = hxc_code < 9;
      }
      if (hxc_tmp_short_circuit_result_n10)
      {
        int32_t hxc_tmp_call_result_n10 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_code);
        int32_t hxc_item = hxc_tmp_call_result_n10;
        struct hxc_caxecraft_content_ContentPresentation hxc_tmp_call_result_n12 = hxc_caxecraft_content_BaseContentPack_itemIcon(hxc_item);
        struct hxc_caxecraft_content_ContentPresentation hxc_presentation = hxc_tmp_call_result_n12;
        int32_t hxc_tmp_call_result_n14 = hxc_caxecraft_content_FirstPlayableLevel_itemXMilli(hxc_index);
        double hxc_x = hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n14, 1000.0);
        int32_t hxc_tmp_call_result_n16 = hxc_caxecraft_content_FirstPlayableLevel_itemYMilli(hxc_index);
        double hxc_y = hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n16, 1000.0) + 0.42;
        int32_t hxc_tmp_call_result_n18 = hxc_caxecraft_content_FirstPlayableLevel_itemZMilli(hxc_index);
        double hxc_z = hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n18, 1000.0);
        double hxc_tmp_load_result_n19 = hxc_x;
        double hxc_tmp_load_result_n20 = hxc_y;
        struct Vector3 hxc_position = (struct Vector3){ .x = (float)hxc_tmp_load_result_n19, .y = (float)hxc_tmp_load_result_n20, .z = (float)hxc_z };
        int32_t hxc_tmp_record_field_load_result_n23 = hxc_presentation.hxc_asset;
        int32_t hxc_tmp_record_field_load_result_n24 = hxc_presentation.hxc_cellIndex;
        hxc_caxecraft_app_AuthoredItemRenderer_drawPresentation(hxc_camera, hxc_tmp_record_field_load_result_n23, hxc_tmp_record_field_load_result_n24, hxc_position, hxc_itemTexture, hxc_itemTextureReady, hxc_adventureTexture, hxc_adventureTextureReady, 0.72, 0.72);
      }
    }
    hxc_index = hxc_i32_add_wrapping(hxc_index, 1);
  }
  return;
}
