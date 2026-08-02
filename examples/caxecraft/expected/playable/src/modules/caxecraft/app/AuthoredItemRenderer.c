#include "hxc/program.h"

void hxc_caxecraft_app_AuthoredItemRenderer_drawEquippedIcon(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_registry, int32_t hxc_l_code, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureTextureReady, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_size)
{
  const void *hxc_l_gc_roots[10] = { (const void *)hxc_l_registry, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 10, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_registry == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_itemPresentation(hxc_l_registry, hxc_l_code);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_presentation;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_presentation == NULL)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return;
  }
  struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n2 = hxc_l_presentation;
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
  if (hxc_l_tmp_load_result_n2 == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_asset;
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_tmp_class_field_load_result_n3.data != NULL && (hxc_l_tmp_class_field_load_result_n3.byte_length == (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.byte_length && (hxc_l_tmp_class_field_load_result_n3.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n3.data, (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.data, hxc_l_tmp_class_field_load_result_n3.byte_length) == 0));
  if (hxc_l_tmp_class_field_load_result_n3.data != NULL && (hxc_l_tmp_class_field_load_result_n3.byte_length == (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.byte_length && (hxc_l_tmp_class_field_load_result_n3.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n3.data, (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.data, hxc_l_tmp_class_field_load_result_n3.byte_length) == 0)))
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_itemTextureReady;
  }
  if (hxc_l_tmp_short_circuit_result_n10)
  {
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n6 = hxc_l_presentation;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n6;
    if (hxc_l_tmp_load_result_n6 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_tmp_load_result_n6).hxc_cellIndex;
    int32_t hxc_l_column_hfbe196e87250 = hxc_i32_modulo_zero_safe(hxc_l_tmp_class_field_load_result_n7, 4);
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n8 = hxc_l_presentation;
    hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_cellIndex;
    int32_t hxc_l_row_h302e3bbfb5b4 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_tmp_class_field_load_result_n9, (double)4));
    struct Color hxc_l_this1_h67fe5237afb7 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    double hxc_l_sourceWidth_h34f5ace42d36 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.width, (double)4);
    double hxc_l_sourceHeight_h1c30737c3470 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.height, (double)4);
    int32_t hxc_l_tmp_load_result_n13 = hxc_l_column_hfbe196e87250;
    double hxc_l_tmp_load_result_n14 = hxc_l_sourceWidth_h34f5ace42d36;
    int32_t hxc_l_tmp_load_result_n15 = hxc_l_row_h302e3bbfb5b4;
    double hxc_l_tmp_load_result_n16 = hxc_l_sourceHeight_h1c30737c3470;
    double hxc_l_tmp_load_result_n17 = hxc_l_sourceWidth_h34f5ace42d36;
    double hxc_l_tmp_load_result_n18 = hxc_l_sourceHeight_h1c30737c3470;
    DrawTexturePro(hxc_l_itemTexture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n13 * hxc_l_tmp_load_result_n14), .y = (float)((double)hxc_l_tmp_load_result_n15 * hxc_l_tmp_load_result_n16), .width = (float)hxc_l_tmp_load_result_n17, .height = (float)hxc_l_tmp_load_result_n18 }, (struct Rectangle){ .x = (float)((double)hxc_l_x + 0.0), .y = (float)((double)hxc_l_y + 0.0), .width = (float)((double)hxc_l_size + 0.0), .height = (float)((double)hxc_l_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_h67fe5237afb7);
  }
  else
  {
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n23 = hxc_l_presentation;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n23;
    if (hxc_l_tmp_load_result_n23 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n24 = (*hxc_l_tmp_load_result_n23).hxc_asset;
    bool hxc_l_tmp_short_circuit_result_n16 = hxc_l_tmp_class_field_load_result_n24.data != NULL && (hxc_l_tmp_class_field_load_result_n24.byte_length == (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.byte_length && (hxc_l_tmp_class_field_load_result_n24.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n24.data, (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.data, hxc_l_tmp_class_field_load_result_n24.byte_length) == 0));
    if (hxc_l_tmp_class_field_load_result_n24.data != NULL && (hxc_l_tmp_class_field_load_result_n24.byte_length == (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.byte_length && (hxc_l_tmp_class_field_load_result_n24.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n24.data, (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.data, hxc_l_tmp_class_field_load_result_n24.byte_length) == 0)))
    {
      hxc_l_tmp_short_circuit_result_n16 = hxc_l_adventureTextureReady;
    }
    if (hxc_l_tmp_short_circuit_result_n16)
    {
      struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n27 = hxc_l_presentation;
      hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n27;
      if (hxc_l_tmp_load_result_n27 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n28 = (*hxc_l_tmp_load_result_n27).hxc_cellIndex;
      int32_t hxc_l_column_h5ab467015112 = hxc_i32_modulo_zero_safe(hxc_l_tmp_class_field_load_result_n28, 4);
      struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n29 = hxc_l_presentation;
      hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n29;
      if (hxc_l_tmp_load_result_n29 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n30 = (*hxc_l_tmp_load_result_n29).hxc_cellIndex;
      int32_t hxc_l_row_h4f150f15728a = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_tmp_class_field_load_result_n30, (double)4));
      struct Color hxc_l_this1_ha0f055ac5391 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
      double hxc_l_sourceWidth_h9cf52348a458 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.width, (double)4);
      double hxc_l_sourceHeight_h2f32ea188898 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.height, (double)4);
      int32_t hxc_l_tmp_load_result_n34 = hxc_l_column_h5ab467015112;
      double hxc_l_tmp_load_result_n35 = hxc_l_sourceWidth_h9cf52348a458;
      int32_t hxc_l_tmp_load_result_n36 = hxc_l_row_h4f150f15728a;
      double hxc_l_tmp_load_result_n37 = hxc_l_sourceHeight_h2f32ea188898;
      double hxc_l_tmp_load_result_n38 = hxc_l_sourceWidth_h9cf52348a458;
      double hxc_l_tmp_load_result_n39 = hxc_l_sourceHeight_h2f32ea188898;
      DrawTexturePro(hxc_l_adventureTexture, (struct Rectangle){ .x = (float)((double)hxc_l_tmp_load_result_n34 * hxc_l_tmp_load_result_n35), .y = (float)((double)hxc_l_tmp_load_result_n36 * hxc_l_tmp_load_result_n37), .width = (float)hxc_l_tmp_load_result_n38, .height = (float)hxc_l_tmp_load_result_n39 }, (struct Rectangle){ .x = (float)((double)hxc_l_x + 0.0), .y = (float)((double)hxc_l_y + 0.0), .width = (float)((double)hxc_l_size + 0.0), .height = (float)((double)hxc_l_size + 0.0) }, (struct Vector2){ .x = (float)0.0, .y = (float)0.0 }, (float)0.0, hxc_l_this1_ha0f055ac5391);
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_caxecraft_app_AuthoredItemRenderer_drawPresentation(struct Camera3D hxc_l_camera, hxc_string hxc_l_asset, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_position, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureTextureReady, double hxc_l_width, double hxc_l_height)
{
  bool hxc_l_tmp_short_circuit_result_n10 = hxc_l_asset.data != NULL && (hxc_l_asset.byte_length == (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.byte_length && (hxc_l_asset.byte_length == 0 || memcmp(hxc_l_asset.data, (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.data, hxc_l_asset.byte_length) == 0));
  if (hxc_l_asset.data != NULL && (hxc_l_asset.byte_length == (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.byte_length && (hxc_l_asset.byte_length == 0 || memcmp(hxc_l_asset.data, (hxc_string){ (const uint8_t *)"items", 5, true, NULL }.data, hxc_l_asset.byte_length) == 0)))
  {
    hxc_l_tmp_short_circuit_result_n10 = hxc_l_itemTextureReady;
  }
  if (hxc_l_tmp_short_circuit_result_n10)
  {
    int32_t hxc_l_row_h626da02fe064 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_cellIndex, (double)4));
    struct Color hxc_l_this1_h31d8ff5dcce9 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    double hxc_l_sourceWidth_h188946618782 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.width, (double)4);
    double hxc_l_sourceHeight_h5fa14bbf9bdd = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_itemTexture.height, (double)4);
    double hxc_l_tmp_load_result_n5 = hxc_l_sourceWidth_h188946618782;
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_row_h626da02fe064;
    double hxc_l_tmp_load_result_n7 = hxc_l_sourceHeight_h5fa14bbf9bdd;
    double hxc_l_tmp_load_result_n8 = hxc_l_sourceWidth_h188946618782;
    double hxc_l_tmp_load_result_n9 = hxc_l_sourceHeight_h5fa14bbf9bdd;
    DrawBillboardRec(hxc_l_camera, hxc_l_itemTexture, (struct Rectangle){ .x = (float)((double)hxc_i32_modulo_zero_safe(hxc_l_cellIndex, 4) * hxc_l_tmp_load_result_n5), .y = (float)((double)hxc_l_tmp_load_result_n6 * hxc_l_tmp_load_result_n7), .width = (float)hxc_l_tmp_load_result_n8, .height = (float)hxc_l_tmp_load_result_n9 }, hxc_l_position, (struct Vector2){ .x = (float)hxc_l_width, .y = (float)hxc_l_height }, hxc_l_this1_h31d8ff5dcce9);
  }
  else
  {
    bool hxc_l_tmp_short_circuit_result_n15 = hxc_l_asset.data != NULL && (hxc_l_asset.byte_length == (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.byte_length && (hxc_l_asset.byte_length == 0 || memcmp(hxc_l_asset.data, (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.data, hxc_l_asset.byte_length) == 0));
    if (hxc_l_asset.data != NULL && (hxc_l_asset.byte_length == (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.byte_length && (hxc_l_asset.byte_length == 0 || memcmp(hxc_l_asset.data, (hxc_string){ (const uint8_t *)"adventure-items", 15, true, NULL }.data, hxc_l_asset.byte_length) == 0)))
    {
      hxc_l_tmp_short_circuit_result_n15 = hxc_l_adventureTextureReady;
    }
    if (hxc_l_tmp_short_circuit_result_n15)
    {
      int32_t hxc_l_row_hac8a494e2713 = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_cellIndex, (double)4));
      struct Color hxc_l_this1_hef2bd57bc9a0 = (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
      double hxc_l_sourceWidth_haa176c227024 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.width, (double)4);
      double hxc_l_sourceHeight_hb0d1d89fcf25 = hxc_f64_divide_zero_safe((double)(int32_t)hxc_l_adventureTexture.height, (double)4);
      double hxc_l_tmp_load_result_n18 = hxc_l_sourceWidth_haa176c227024;
      int32_t hxc_l_tmp_load_result_n19 = hxc_l_row_hac8a494e2713;
      double hxc_l_tmp_load_result_n20 = hxc_l_sourceHeight_hb0d1d89fcf25;
      double hxc_l_tmp_load_result_n21 = hxc_l_sourceWidth_haa176c227024;
      double hxc_l_tmp_load_result_n22 = hxc_l_sourceHeight_hb0d1d89fcf25;
      DrawBillboardRec(hxc_l_camera, hxc_l_adventureTexture, (struct Rectangle){ .x = (float)((double)hxc_i32_modulo_zero_safe(hxc_l_cellIndex, 4) * hxc_l_tmp_load_result_n18), .y = (float)((double)hxc_l_tmp_load_result_n19 * hxc_l_tmp_load_result_n20), .width = (float)hxc_l_tmp_load_result_n21, .height = (float)hxc_l_tmp_load_result_n22 }, hxc_l_position, (struct Vector2){ .x = (float)hxc_l_width, .y = (float)hxc_l_height }, hxc_l_this1_hef2bd57bc9a0);
    }
  }
  return;
}

void hxc_caxecraft_app_AuthoredItemRenderer_drawWorldItems(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_registry, struct Camera3D hxc_l_camera, const int32_t *hxc_l_active, size_t hxc_l_length, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureTextureReady)
{
  const void *hxc_l_gc_roots[7] = { (const void *)hxc_l_registry, (const void *)hxc_l_level, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  const int32_t *hxc_l_borrow = hxc_l_active;
  size_t hxc_l_tmp_length_n8 = hxc_l_length;
  int32_t hxc_l_inactive = 0;
  int32_t hxc_l_index = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_index;
    if (hxc_l_level == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_level).hxc_items;
    int32_t hxc_l_tmp_array_length_result_n2;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n1, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n2))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_index;
    if (hxc_l_tmp_load_result_n3 < 0 || (size_t)hxc_l_tmp_load_result_n3 >= hxc_l_tmp_length_n8)
    {
      abort();
    }
    int32_t hxc_l_tmp_collection_index_load_result_n4 = hxc_l_borrow[(size_t)hxc_l_tmp_load_result_n3];
    if (hxc_l_tmp_collection_index_load_result_n4 != hxc_l_inactive)
    {
      struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_level).hxc_items;
      struct hxc_caxecraft_app_LoadedWorldItem hxc_l_tmp_array_get_result_n8;
      if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n6, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_app_LoadedWorldItem hxc_l_loaded = hxc_l_tmp_array_get_result_n8;
      int32_t hxc_l_code = hxc_l_loaded.hxc_storageCode;
      if (hxc_l_registry == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_content_RuntimeContentRegistry_itemPresentation(hxc_l_registry, hxc_l_code);
      hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_instance_call_result_n11;
      struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_instance_call_result_n11;
      hxc_l_gc_roots[3] = (const void *)hxc_l_presentation;
      hxc_l_gc_roots[4] = (const void *)NULL;
      if (hxc_l_presentation != NULL)
      {
        double hxc_l_y = hxc_f64_divide_zero_safe((double)hxc_l_loaded.hxc_yMilli, 1000.0) + 0.42;
        double hxc_l_z = hxc_f64_divide_zero_safe((double)hxc_l_loaded.hxc_zMilli, 1000.0);
        int32_t hxc_l_tmp_record_field_load_result_n15 = hxc_l_loaded.hxc_xMilli;
        double hxc_l_tmp_load_result_n16 = hxc_l_y;
        struct Vector3 hxc_l_position = (struct Vector3){ .x = (float)hxc_f64_divide_zero_safe((double)hxc_l_tmp_record_field_load_result_n15, 1000.0), .y = (float)hxc_l_tmp_load_result_n16, .z = (float)hxc_l_z };
        struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n19 = hxc_l_presentation;
        hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n19;
        if (hxc_l_tmp_load_result_n19 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n20 = (*hxc_l_tmp_load_result_n19).hxc_asset;
        struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n21 = hxc_l_presentation;
        hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n21;
        if (hxc_l_tmp_load_result_n21 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_tmp_load_result_n21).hxc_cellIndex;
        hxc_caxecraft_app_AuthoredItemRenderer_drawPresentation(hxc_l_camera, hxc_l_tmp_class_field_load_result_n20, hxc_l_tmp_class_field_load_result_n22, hxc_l_position, hxc_l_itemTexture, hxc_l_itemTextureReady, hxc_l_adventureTexture, hxc_l_adventureTextureReady, 0.72, 0.72);
      }
    }
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
