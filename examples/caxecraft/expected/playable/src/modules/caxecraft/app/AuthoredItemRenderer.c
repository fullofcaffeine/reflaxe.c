#include "hxc/program.h"

void hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(int32_t hxc_l_code, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureTextureReady, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_size)
{
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_code >= 0;
  if (hxc_l_code >= 0)
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_code < 9;
  }
  if (!hxc_l_tmp_short_circuit_result_n8)
  {
    return;
  }
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_l_code);
  struct hxc_caxecraft_content_ContentPresentation hxc_l_tmp_call_result_n2 = hxc_caxecraft_content_BaseContentPack_itemIcon(hxc_l_tmp_call_result_n1);
  struct hxc_caxecraft_content_ContentPresentation hxc_l_presentation = hxc_l_tmp_call_result_n2;
  int32_t hxc_l_tmp_record_field_load_result_n3 = hxc_l_presentation.hxc_asset;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_record_field_load_result_n3 == 2;
  if (hxc_l_tmp_record_field_load_result_n3 == 2)
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_itemTextureReady;
  }
  if (hxc_l_tmp_short_circuit_result_n10)
  {
    int32_t hxc_l_column_hf1e54e4b4fbb = hxc_i32_modulo_zero_safe(hxc_l_presentation.hxc_cellIndex, 4);
    int32_t hxc_l_row_h231fa05c69ed = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_presentation.hxc_cellIndex, (double)4));
    struct Color hxc_l_this1_haa9524c098c0 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    double hxc_l_sourceWidth_h08e2ecedb701 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.width, (double)4);
    double hxc_l_sourceHeight_ha2df9f5be6f6 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.height, (double)4);
    int32_t hxc_l_tmp_load_result_n10 = hxc_l_column_hf1e54e4b4fbb;
    double hxc_l_tmp_load_result_n11 = hxc_l_sourceWidth_h08e2ecedb701;
    int32_t hxc_l_tmp_load_result_n12 = hxc_l_row_h231fa05c69ed;
    double hxc_l_tmp_load_result_n13 = hxc_l_sourceHeight_ha2df9f5be6f6;
    double hxc_l_tmp_load_result_n14 = hxc_l_sourceWidth_h08e2ecedb701;
    double hxc_l_tmp_load_result_n15 = hxc_l_sourceHeight_ha2df9f5be6f6;
    DrawTexturePro(hxc_l_itemTexture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n10 * hxc_l_tmp_load_result_n11), .y = (float)((double)hxc_l_tmp_load_result_n12 * hxc_l_tmp_load_result_n13), .width = (float)hxc_l_tmp_load_result_n14, .height = (float)hxc_l_tmp_load_result_n15 }, (struct Rectangle){ .x = (float)((double)hxc_l_x + 0.0), .y = (float)((double)hxc_l_y + 0.0), .width = (float)((double)hxc_l_size + 0.0), .height = (float)((double)hxc_l_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_haa9524c098c0);
  }
  else
  {
    int32_t hxc_l_tmp_record_field_load_result_n20 = hxc_l_presentation.hxc_asset;
    bool hxc_l_tmp_short_circuit_result_n16 = hxc_l_tmp_record_field_load_result_n20 == 0;
    if (hxc_l_tmp_record_field_load_result_n20 == 0)
    {
      hxc_l_tmp_short_circuit_result_n16 = hxc_l_adventureTextureReady;
    }
    if (hxc_l_tmp_short_circuit_result_n16)
    {
      int32_t hxc_l_column_heef4af09bea0 = hxc_i32_modulo_zero_safe(hxc_l_presentation.hxc_cellIndex, 4);
      int32_t hxc_l_row_h9795344089dc = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_presentation.hxc_cellIndex, (double)4));
      struct Color hxc_l_this1_hab22142602b8 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
      double hxc_l_sourceWidth_h364f7c10b0e6 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.width, (double)4);
      double hxc_l_sourceHeight_h84d482298038 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.height, (double)4);
      int32_t hxc_l_tmp_load_result_n27 = hxc_l_column_heef4af09bea0;
      double hxc_l_tmp_load_result_n28 = hxc_l_sourceWidth_h364f7c10b0e6;
      int32_t hxc_l_tmp_load_result_n29 = hxc_l_row_h9795344089dc;
      double hxc_l_tmp_load_result_n30 = hxc_l_sourceHeight_h84d482298038;
      double hxc_l_tmp_load_result_n31 = hxc_l_sourceWidth_h364f7c10b0e6;
      double hxc_l_tmp_load_result_n32 = hxc_l_sourceHeight_h84d482298038;
      DrawTexturePro(hxc_l_adventureTexture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n27 * hxc_l_tmp_load_result_n28), .y = (float)((double)hxc_l_tmp_load_result_n29 * hxc_l_tmp_load_result_n30), .width = (float)hxc_l_tmp_load_result_n31, .height = (float)hxc_l_tmp_load_result_n32 }, (struct Rectangle){ .x = (float)((double)hxc_l_x + 0.0), .y = (float)((double)hxc_l_y + 0.0), .width = (float)((double)hxc_l_size + 0.0), .height = (float)((double)hxc_l_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_hab22142602b8);
    }
  }
  return;
}

void hxc_caxecraft_app_AuthoredItemRenderer_drawPresentation(struct Camera3D hxc_l_camera, int32_t hxc_l_asset, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_position, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureTextureReady, double hxc_l_width, double hxc_l_height)
{
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_asset == 2;
  if (hxc_l_asset == 2)
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_itemTextureReady;
  }
  if (hxc_l_tmp_short_circuit_result_n10)
  {
    int32_t hxc_l_row_h81252ef7f0a7 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_cellIndex, (double)4));
    struct Color hxc_l_this1_h1b575f97dd8f = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    double hxc_l_sourceWidth_h46384f71c518 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.width, (double)4);
    double hxc_l_sourceHeight_hd77ac56d8f30 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.height, (double)4);
    double hxc_l_tmp_load_result_n4 = hxc_l_sourceWidth_h46384f71c518;
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_row_h81252ef7f0a7;
    double hxc_l_tmp_load_result_n6 = hxc_l_sourceHeight_hd77ac56d8f30;
    double hxc_l_tmp_load_result_n7 = hxc_l_sourceWidth_h46384f71c518;
    double hxc_l_tmp_load_result_n8 = hxc_l_sourceHeight_hd77ac56d8f30;
    DrawBillboardRec(hxc_l_camera, hxc_l_itemTexture, (struct Rectangle){ .x = (float)((double)hxc_i32_modulo_zero_safe(hxc_l_cellIndex, 4) * hxc_l_tmp_load_result_n4), .y = (float)((double)hxc_l_tmp_load_result_n5 * hxc_l_tmp_load_result_n6), .width = (float)hxc_l_tmp_load_result_n7, .height = (float)hxc_l_tmp_load_result_n8 }, hxc_l_position, (struct Vector2){ .x = (float)hxc_l_width, .y = (float)hxc_l_height }, hxc_l_this1_h1b575f97dd8f);
  }
  else
  {
    bool hxc_l_tmp_short_circuit_result_n15 = hxc_l_asset == 0;
    if (hxc_l_asset == 0)
    {
      hxc_l_tmp_short_circuit_result_n15 = hxc_l_adventureTextureReady;
    }
    if (hxc_l_tmp_short_circuit_result_n15)
    {
      int32_t hxc_l_row_hf0aafdaaba57 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_cellIndex, (double)4));
      struct Color hxc_l_this1_hbb0c3574dafd = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
      double hxc_l_sourceWidth_haba66acfde60 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.width, (double)4);
      double hxc_l_sourceHeight_hdecce6c0ea9c = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.height, (double)4);
      double hxc_l_tmp_load_result_n16 = hxc_l_sourceWidth_haba66acfde60;
      int32_t hxc_l_tmp_load_result_n17 = hxc_l_row_hf0aafdaaba57;
      double hxc_l_tmp_load_result_n18 = hxc_l_sourceHeight_hdecce6c0ea9c;
      double hxc_l_tmp_load_result_n19 = hxc_l_sourceWidth_haba66acfde60;
      double hxc_l_tmp_load_result_n20 = hxc_l_sourceHeight_hdecce6c0ea9c;
      DrawBillboardRec(hxc_l_camera, hxc_l_adventureTexture, (struct Rectangle){ .x = (float)((double)hxc_i32_modulo_zero_safe(hxc_l_cellIndex, 4) * hxc_l_tmp_load_result_n16), .y = (float)((double)hxc_l_tmp_load_result_n17 * hxc_l_tmp_load_result_n18), .width = (float)hxc_l_tmp_load_result_n19, .height = (float)hxc_l_tmp_load_result_n20 }, hxc_l_position, (struct Vector2){ .x = (float)hxc_l_width, .y = (float)hxc_l_height }, hxc_l_this1_hbb0c3574dafd);
    }
  }
  return;
}

void hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(struct Camera3D hxc_l_camera, const int32_t *hxc_l_active, size_t hxc_l_length, struct hxc_array_ref *hxc_l_items, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureTextureReady)
{
  const int32_t *hxc_l_borrow = hxc_l_active;
  size_t hxc_l_tmp_length_n7 = hxc_l_length;
  int32_t hxc_l_inactive = 0;
  int32_t hxc_l_index = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_index;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_items, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_index;
    if (hxc_l_tmp_load_result_n2 < 0 || (size_t)hxc_l_tmp_load_result_n2 >= hxc_l_tmp_length_n7)
    {
      abort();
    }
    int32_t hxc_l_tmp_collection_index_load_result_n3 = hxc_l_borrow[(size_t)hxc_l_tmp_load_result_n2];
    if (hxc_l_tmp_collection_index_load_result_n3 != hxc_l_inactive)
    {
      struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n6;
      if (hxc_array_ref_get_copy(hxc_l_items, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loaded = hxc_l_tmp_array_get_result_n6;
      int32_t hxc_l_code = hxc_l_loaded.hxc_storageCode;
      int32_t hxc_l_tmp_load_result_n8 = hxc_l_code;
      bool hxc_l_tmp_short_circuit_result_n12 = hxc_l_tmp_load_result_n8 >= 0;
      if (hxc_l_tmp_load_result_n8 >= 0)
      {
        hxc_l_tmp_short_circuit_result_n12 = hxc_l_code < 9;
      }
      if (hxc_l_tmp_short_circuit_result_n12)
      {
        int32_t hxc_l_tmp_call_result_n12 = hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(hxc_l_code);
        int32_t hxc_l_item = hxc_l_tmp_call_result_n12;
        struct hxc_caxecraft_content_ContentPresentation hxc_l_tmp_call_result_n14 = hxc_caxecraft_content_BaseContentPack_itemIcon(hxc_l_item);
        struct hxc_caxecraft_content_ContentPresentation hxc_l_presentation = hxc_l_tmp_call_result_n14;
        double hxc_l_y = hxc_f64_divide_zero_safe((double)hxc_l_loaded.hxc_yMilli, 1000.0) + 0.42;
        double hxc_l_z = hxc_f64_divide_zero_safe((double)hxc_l_loaded.hxc_zMilli, 1000.0);
        int32_t hxc_l_tmp_record_field_load_result_n17 = hxc_l_loaded.hxc_xMilli;
        double hxc_l_tmp_load_result_n18 = hxc_l_y;
        struct Vector3 hxc_l_position = (struct Vector3){ .x = (float)hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n17, 1000.0), .y = (float)hxc_l_tmp_load_result_n18, .z = (float)hxc_l_z };
        int32_t hxc_l_tmp_record_field_load_result_n21 = hxc_l_presentation.hxc_asset;
        int32_t hxc_l_tmp_record_field_load_result_n22 = hxc_l_presentation.hxc_cellIndex;
        hxc_caxecraft_app_AuthoredItemRenderer_drawPresentation(hxc_l_camera, hxc_l_tmp_record_field_load_result_n21, hxc_l_tmp_record_field_load_result_n22, hxc_l_position, hxc_l_itemTexture, hxc_l_itemTextureReady, hxc_l_adventureTexture, hxc_l_adventureTextureReady, 0.72, 0.72);
      }
    }
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  return;
}
