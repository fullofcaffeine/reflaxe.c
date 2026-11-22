#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_WaterRenderer(struct hxc_caxecraft_app_WaterRenderer *hxc_l_self)
{
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace), _Alignof(struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace), &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_faces = hxc_l_tmp_array_create_result_n0;
  return;
}

int32_t hxc_caxecraft_app_WaterRenderer_compareFaces(struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_left, struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_right)
{
  int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_app_WaterRenderOrder_compareDepth(hxc_l_left.hxc_depth, hxc_l_left.hxc_stableKey, hxc_l_right.hxc_depth, hxc_l_right.hxc_stableKey);
  return hxc_l_tmp_call_result_n4;
}

void hxc_caxecraft_app_WaterRenderer_emitEast(double hxc_l_x, double hxc_l_bottom, double hxc_l_topNorth, double hxc_l_topSouth, double hxc_l_z, int32_t hxc_l_alpha_hb3ba56626890, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  uint8_t hxc_l_red = (uint8_t)180;
  uint8_t hxc_l_green = (uint8_t)235;
  uint8_t hxc_l_blue = (uint8_t)255;
  int32_t hxc_l_tmp_conditional_result_n17 = 0;
  if (hxc_l_alpha_hb3ba56626890 < 0)
  {
    hxc_l_tmp_conditional_result_n17 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n18 = 0;
    if (hxc_l_alpha_hb3ba56626890 > 255)
    {
      hxc_l_tmp_conditional_result_n18 = 255;
    }
    else
    {
      hxc_l_tmp_conditional_result_n18 = hxc_l_alpha_hb3ba56626890;
    }
    hxc_l_tmp_conditional_result_n17 = hxc_l_tmp_conditional_result_n18;
  }
  uint8_t hxc_l_alpha_h7fc4fe253df9 = (uint8_t)hxc_l_tmp_conditional_result_n17;
  uint8_t hxc_l_tmp_load_result_n2 = hxc_l_red;
  uint8_t hxc_l_tmp_load_result_n3 = hxc_l_green;
  uint8_t hxc_l_tmp_load_result_n4 = hxc_l_blue;
  struct Color hxc_l_raw = (struct Color){ .r = hxc_l_tmp_load_result_n2, .g = hxc_l_tmp_load_result_n3, .b = hxc_l_tmp_load_result_n4, .a = hxc_l_alpha_h7fc4fe253df9 };
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  uint8_t *hxc_l_tmp_imported_field_address_n9 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n10 = *hxc_l_tmp_imported_field_address_n9;
  uint8_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
  uint8_t *hxc_l_tmp_imported_field_address_n13 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n14 = *hxc_l_tmp_imported_field_address_n13;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n8, hxc_l_tmp_imported_field_load_result_n10, hxc_l_tmp_imported_field_load_result_n12, hxc_l_tmp_imported_field_load_result_n14);
  rlNormal3f((float)1.0, (float)0.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_topNorth + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_topSouth + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitFace(struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_face, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  switch (hxc_l_face.hxc_kind) {
    case 0:
      {
        hxc_caxecraft_app_WaterRenderer_emitTop((double)hxc_l_face.hxc_x, (double)hxc_l_face.hxc_z, hxc_l_face.hxc_corners, hxc_l_u0, hxc_l_u1, hxc_l_v0, hxc_l_v1, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
        break;
      }
    case 1:
      {
        hxc_caxecraft_app_WaterRenderer_emitNorth((double)hxc_l_face.hxc_x, (double)hxc_l_face.hxc_y, hxc_l_face.hxc_corners.hxc_northWest, hxc_l_face.hxc_corners.hxc_northEast, (double)hxc_l_face.hxc_z, hxc_l_face.hxc_alpha, hxc_l_u0, hxc_l_u1, hxc_l_v0, hxc_l_v1, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
        break;
      }
    case 2:
      {
        hxc_caxecraft_app_WaterRenderer_emitSouth((double)hxc_l_face.hxc_x, (double)hxc_l_face.hxc_y, hxc_l_face.hxc_corners.hxc_southWest, hxc_l_face.hxc_corners.hxc_southEast, (double)hxc_l_face.hxc_z, hxc_l_face.hxc_alpha, hxc_l_u0, hxc_l_u1, hxc_l_v0, hxc_l_v1, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
        break;
      }
    case 3:
      {
        hxc_caxecraft_app_WaterRenderer_emitEast((double)hxc_l_face.hxc_x, (double)hxc_l_face.hxc_y, hxc_l_face.hxc_corners.hxc_northEast, hxc_l_face.hxc_corners.hxc_southEast, (double)hxc_l_face.hxc_z, hxc_l_face.hxc_alpha, hxc_l_u0, hxc_l_u1, hxc_l_v0, hxc_l_v1, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
        break;
      }
    case 4:
      {
        hxc_caxecraft_app_WaterRenderer_emitWest((double)hxc_l_face.hxc_x, (double)hxc_l_face.hxc_y, hxc_l_face.hxc_corners.hxc_northWest, hxc_l_face.hxc_corners.hxc_southWest, (double)hxc_l_face.hxc_z, hxc_l_face.hxc_alpha, hxc_l_u0, hxc_l_u1, hxc_l_v0, hxc_l_v1, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
        break;
      }
    default:
      {
        break;
      }
  }
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitNorth(double hxc_l_x, double hxc_l_bottom, double hxc_l_topWest, double hxc_l_topEast, double hxc_l_z, int32_t hxc_l_alpha_h95c819dc65aa, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  uint8_t hxc_l_red = (uint8_t)180;
  uint8_t hxc_l_green = (uint8_t)235;
  uint8_t hxc_l_blue = (uint8_t)255;
  int32_t hxc_l_tmp_conditional_result_n17 = 0;
  if (hxc_l_alpha_h95c819dc65aa < 0)
  {
    hxc_l_tmp_conditional_result_n17 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n18 = 0;
    if (hxc_l_alpha_h95c819dc65aa > 255)
    {
      hxc_l_tmp_conditional_result_n18 = 255;
    }
    else
    {
      hxc_l_tmp_conditional_result_n18 = hxc_l_alpha_h95c819dc65aa;
    }
    hxc_l_tmp_conditional_result_n17 = hxc_l_tmp_conditional_result_n18;
  }
  uint8_t hxc_l_alpha_hee96a468a4b7 = (uint8_t)hxc_l_tmp_conditional_result_n17;
  uint8_t hxc_l_tmp_load_result_n2 = hxc_l_red;
  uint8_t hxc_l_tmp_load_result_n3 = hxc_l_green;
  uint8_t hxc_l_tmp_load_result_n4 = hxc_l_blue;
  struct Color hxc_l_raw = (struct Color){ .r = hxc_l_tmp_load_result_n2, .g = hxc_l_tmp_load_result_n3, .b = hxc_l_tmp_load_result_n4, .a = hxc_l_alpha_hee96a468a4b7 };
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  uint8_t *hxc_l_tmp_imported_field_address_n9 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n10 = *hxc_l_tmp_imported_field_address_n9;
  uint8_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
  uint8_t *hxc_l_tmp_imported_field_address_n13 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n14 = *hxc_l_tmp_imported_field_address_n13;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n8, hxc_l_tmp_imported_field_load_result_n10, hxc_l_tmp_imported_field_load_result_n12, hxc_l_tmp_imported_field_load_result_n14);
  rlNormal3f((float)0.0, (float)0.0, (float)-1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x + hxc_l_offsetX, hxc_l_topWest + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_topEast + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitSouth(double hxc_l_x, double hxc_l_bottom, double hxc_l_topWest, double hxc_l_topEast, double hxc_l_z, int32_t hxc_l_alpha_he720d42c5db7, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  uint8_t hxc_l_red = (uint8_t)180;
  uint8_t hxc_l_green = (uint8_t)235;
  uint8_t hxc_l_blue = (uint8_t)255;
  int32_t hxc_l_tmp_conditional_result_n17 = 0;
  if (hxc_l_alpha_he720d42c5db7 < 0)
  {
    hxc_l_tmp_conditional_result_n17 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n18 = 0;
    if (hxc_l_alpha_he720d42c5db7 > 255)
    {
      hxc_l_tmp_conditional_result_n18 = 255;
    }
    else
    {
      hxc_l_tmp_conditional_result_n18 = hxc_l_alpha_he720d42c5db7;
    }
    hxc_l_tmp_conditional_result_n17 = hxc_l_tmp_conditional_result_n18;
  }
  uint8_t hxc_l_alpha_hc6db00844403 = (uint8_t)hxc_l_tmp_conditional_result_n17;
  uint8_t hxc_l_tmp_load_result_n2 = hxc_l_red;
  uint8_t hxc_l_tmp_load_result_n3 = hxc_l_green;
  uint8_t hxc_l_tmp_load_result_n4 = hxc_l_blue;
  struct Color hxc_l_raw = (struct Color){ .r = hxc_l_tmp_load_result_n2, .g = hxc_l_tmp_load_result_n3, .b = hxc_l_tmp_load_result_n4, .a = hxc_l_alpha_hc6db00844403 };
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  uint8_t *hxc_l_tmp_imported_field_address_n9 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n10 = *hxc_l_tmp_imported_field_address_n9;
  uint8_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
  uint8_t *hxc_l_tmp_imported_field_address_n13 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n14 = *hxc_l_tmp_imported_field_address_n13;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n8, hxc_l_tmp_imported_field_load_result_n10, hxc_l_tmp_imported_field_load_result_n12, hxc_l_tmp_imported_field_load_result_n14);
  rlNormal3f((float)0.0, (float)0.0, (float)1.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_topEast + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x + hxc_l_offsetX, hxc_l_topWest + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitTop(double hxc_l_x, double hxc_l_z, struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_corners, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  struct Color hxc_l_raw = (struct Color){ .r = 175, .g = 235, .b = 255, .a = 145 };
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
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x + hxc_l_offsetX, hxc_l_corners.hxc_northWest + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x + hxc_l_offsetX, hxc_l_corners.hxc_southWest + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_corners.hxc_southEast + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + 1.0 + hxc_l_offsetX, hxc_l_corners.hxc_northEast + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  return;
}

void hxc_caxecraft_app_WaterRenderer_emitWest(double hxc_l_x, double hxc_l_bottom, double hxc_l_topNorth, double hxc_l_topSouth, double hxc_l_z, int32_t hxc_l_alpha_h635eff5a640e, double hxc_l_u0, double hxc_l_u1, double hxc_l_v0, double hxc_l_v1, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  uint8_t hxc_l_red = (uint8_t)180;
  uint8_t hxc_l_green = (uint8_t)235;
  uint8_t hxc_l_blue = (uint8_t)255;
  int32_t hxc_l_tmp_conditional_result_n17 = 0;
  if (hxc_l_alpha_h635eff5a640e < 0)
  {
    hxc_l_tmp_conditional_result_n17 = 0;
  }
  else
  {
    int32_t hxc_l_tmp_conditional_result_n18 = 0;
    if (hxc_l_alpha_h635eff5a640e > 255)
    {
      hxc_l_tmp_conditional_result_n18 = 255;
    }
    else
    {
      hxc_l_tmp_conditional_result_n18 = hxc_l_alpha_h635eff5a640e;
    }
    hxc_l_tmp_conditional_result_n17 = hxc_l_tmp_conditional_result_n18;
  }
  uint8_t hxc_l_alpha_haced742d59ee = (uint8_t)hxc_l_tmp_conditional_result_n17;
  uint8_t hxc_l_tmp_load_result_n2 = hxc_l_red;
  uint8_t hxc_l_tmp_load_result_n3 = hxc_l_green;
  uint8_t hxc_l_tmp_load_result_n4 = hxc_l_blue;
  struct Color hxc_l_raw = (struct Color){ .r = hxc_l_tmp_load_result_n2, .g = hxc_l_tmp_load_result_n3, .b = hxc_l_tmp_load_result_n4, .a = hxc_l_alpha_haced742d59ee };
  uint8_t *hxc_l_tmp_imported_field_address_n7 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n8 = *hxc_l_tmp_imported_field_address_n7;
  uint8_t *hxc_l_tmp_imported_field_address_n9 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n10 = *hxc_l_tmp_imported_field_address_n9;
  uint8_t *hxc_l_tmp_imported_field_address_n11 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n12 = *hxc_l_tmp_imported_field_address_n11;
  uint8_t *hxc_l_tmp_imported_field_address_n13 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n14 = *hxc_l_tmp_imported_field_address_n13;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n8, hxc_l_tmp_imported_field_load_result_n10, hxc_l_tmp_imported_field_load_result_n12, hxc_l_tmp_imported_field_load_result_n14);
  rlNormal3f((float)-1.0, (float)0.0, (float)0.0);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v1, hxc_l_x + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v1, hxc_l_x + hxc_l_offsetX, hxc_l_bottom + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u1, hxc_l_v0, hxc_l_x + hxc_l_offsetX, hxc_l_topSouth + hxc_l_offsetY, hxc_l_z + 1.0 + hxc_l_offsetZ);
  hxc_caxecraft_app_WaterRenderer_vertex(hxc_l_u0, hxc_l_v0, hxc_l_x + hxc_l_offsetX, hxc_l_topNorth + hxc_l_offsetY, hxc_l_z + hxc_l_offsetZ);
  return;
}

struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_caxecraft_app_WaterRenderer_makeFace(int32_t hxc_l_kind, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z, struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_corners, struct hxc_caxecraft_domain_WaterCellState hxc_l_state, double hxc_l_eyeX, double hxc_l_eyeY, double hxc_l_eyeZ, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  double hxc_l_centerX = (double)hxc_l_x + 0.5;
  double hxc_l_centerY = (double)hxc_l_y + 0.5;
  double hxc_l_centerZ = (double)hxc_l_z + 0.5;
  switch (hxc_l_kind) {
    case 0:
      {
        hxc_l_centerY = (hxc_l_corners.hxc_northWest + hxc_l_corners.hxc_northEast + hxc_l_corners.hxc_southEast + hxc_l_corners.hxc_southWest) * 0.25;
        break;
      }
    case 1:
      {
        hxc_l_centerY = ((double)hxc_l_y + hxc_l_corners.hxc_northWest + (double)hxc_l_y + hxc_l_corners.hxc_northEast) * 0.25;
        hxc_l_centerZ = (double)hxc_l_z;
        break;
      }
    case 2:
      {
        hxc_l_centerY = ((double)hxc_l_y + hxc_l_corners.hxc_southWest + (double)hxc_l_y + hxc_l_corners.hxc_southEast) * 0.25;
        hxc_l_centerZ = (double)hxc_l_z + 1.0;
        break;
      }
    case 3:
      {
        hxc_l_centerX = (double)hxc_l_x + 1.0;
        hxc_l_centerY = ((double)hxc_l_y + hxc_l_corners.hxc_northEast + (double)hxc_l_y + hxc_l_corners.hxc_southEast) * 0.25;
        break;
      }
    case 4:
      {
        hxc_l_centerX = (double)hxc_l_x;
        hxc_l_centerY = ((double)hxc_l_y + hxc_l_corners.hxc_northWest + (double)hxc_l_y + hxc_l_corners.hxc_southWest) * 0.25;
        break;
      }
    default:
      {
        break;
      }
  }
  int32_t hxc_l_tmp_record_field_kind_n15 = hxc_l_kind;
  int32_t hxc_l_tmp_record_field_x_n16 = hxc_l_x;
  int32_t hxc_l_tmp_record_field_y_n17 = hxc_l_y;
  int32_t hxc_l_tmp_record_field_z_n18 = hxc_l_z;
  struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_record_field_corners_n19 = hxc_l_corners;
  int32_t hxc_l_tmp_conditional_result_n20 = 0;
  if (hxc_l_kind == 0)
  {
    hxc_l_tmp_conditional_result_n20 = 145;
  }
  else
  {
    int32_t hxc_l_tmp_call_result_n12 = hxc_caxecraft_app_WaterSurfaceGeometry_sideAlpha(hxc_l_state);
    hxc_l_tmp_conditional_result_n20 = hxc_l_tmp_call_result_n12;
  }
  int32_t hxc_l_tmp_conditional_load_result_n13 = hxc_l_tmp_conditional_result_n20;
  double hxc_l_tmp_load_result_n14 = hxc_l_centerX;
  double hxc_l_tmp_load_result_n15 = hxc_l_centerY;
  double hxc_l_tmp_call_result_n17 = hxc_caxecraft_app_WaterRenderOrder_faceDepthSquared(hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, hxc_l_tmp_load_result_n14 + hxc_l_offsetX, hxc_l_tmp_load_result_n15 + hxc_l_offsetY, hxc_l_centerZ + hxc_l_offsetZ);
  struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n18 = hxc_caxecraft_domain_World_coord(hxc_l_x, hxc_l_y, hxc_l_z);
  int32_t hxc_l_tmp_call_result_n19 = hxc_caxecraft_domain_World_indexOf(hxc_l_tmp_call_result_n18);
  struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_record_field_corners_load_result_n20 = hxc_l_tmp_record_field_corners_n19;
  int32_t hxc_l_tmp_record_field_kind_load_result_n21 = hxc_l_tmp_record_field_kind_n15;
  int32_t hxc_l_tmp_record_field_x_load_result_n22 = hxc_l_tmp_record_field_x_n16;
  int32_t hxc_l_tmp_record_field_y_load_result_n23 = hxc_l_tmp_record_field_y_n17;
  return (struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace){ .hxc_alpha = hxc_l_tmp_conditional_load_result_n13, .hxc_corners = hxc_l_tmp_record_field_corners_load_result_n20, .hxc_depth = hxc_l_tmp_call_result_n17, .hxc_kind = hxc_l_tmp_record_field_kind_load_result_n21, .hxc_stableKey = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_l_tmp_call_result_n19, 5), hxc_l_kind), .hxc_x = hxc_l_tmp_record_field_x_load_result_n22, .hxc_y = hxc_l_tmp_record_field_y_load_result_n23, .hxc_z = hxc_l_tmp_record_field_z_n18 };
}

void hxc_caxecraft_app_WaterRenderer_vertex(double hxc_l_u, double hxc_l_v, double hxc_l_x, double hxc_l_y, double hxc_l_z)
{
  rlTexCoord2f((float)hxc_l_u, (float)hxc_l_v);
  rlVertex3f((float)hxc_l_x, (float)hxc_l_y, (float)hxc_l_z);
  return;
}

struct hxc_caxecraft_app_WaterRenderCounters hxc_caxecraft_app_WaterRenderer_draw(struct hxc_caxecraft_app_WaterRenderer *hxc_l_self, const uint8_t *hxc_l_cells, size_t hxc_l_length, struct Texture hxc_l_texture, bool hxc_l_textureReady, int32_t hxc_l_presentationCell, double hxc_l_eyeX, double hxc_l_eyeY, double hxc_l_eyeZ)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n8 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n8;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_WaterRenderCounters hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_app_WaterRenderer_drawTranslated(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_texture, hxc_l_textureReady, hxc_l_presentationCell, hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, 0.0, 0.0, 0.0);
  return hxc_l_tmp_instance_call_result_n0;
}

struct hxc_caxecraft_app_WaterRenderCounters hxc_caxecraft_app_WaterRenderer_drawTranslated(struct hxc_caxecraft_app_WaterRenderer *hxc_l_self, const uint8_t *hxc_l_cells, size_t hxc_l_length, struct Texture hxc_l_texture, bool hxc_l_textureReady, int32_t hxc_l_presentationCell, double hxc_l_eyeX, double hxc_l_eyeY, double hxc_l_eyeZ, double hxc_l_offsetX, double hxc_l_offsetY, double hxc_l_offsetZ)
{
  struct hxc_array_ref *hxc_l_g1 = { 0 };
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n11 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n11;
  bool hxc_l_tmp_short_circuit_result_n12 = !hxc_l_textureReady;
  if (!!hxc_l_textureReady)
  {
    hxc_l_tmp_short_circuit_result_n12 = hxc_l_presentationCell < 0;
  }
  if (hxc_l_tmp_short_circuit_result_n12)
  {
    return (struct hxc_caxecraft_app_WaterRenderCounters){ .hxc_drawCalls = 0, .hxc_visible = 0 };
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_faces;
  if (hxc_l_tmp_class_field_load_result_n2 == NULL)
  {
    abort();
  }
  if (hxc_array_resize(&hxc_l_tmp_class_field_load_result_n2->value, (size_t)0, NULL) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_column = hxc_i32_modulo_zero_safe(hxc_l_presentationCell, 4);
  int32_t hxc_l_row = hxc_l_presentationCell / 4;
  double hxc_l_halfPixel = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_l_texture.width);
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_column;
  double hxc_l_u0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n4, (double)4) + hxc_l_halfPixel;
  int32_t hxc_l_tmp_load_result_n6 = hxc_l_column;
  double hxc_l_u1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n6, 1), (double)4) - hxc_l_halfPixel;
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_row;
  double hxc_l_v0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_load_result_n8, (double)4) + hxc_l_halfPixel;
  int32_t hxc_l_tmp_load_result_n10 = hxc_l_row;
  double hxc_l_v1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_load_result_n10, 1), (double)4) - hxc_l_halfPixel;
  int32_t hxc_l_visible = 0;
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
        if (!(hxc_l_x < 64))
        {
          break;
        }
        int32_t hxc_l_tmp_load_result_n15 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n16 = hxc_l_y;
        struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n18 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n15, hxc_l_tmp_load_result_n16, hxc_l_z);
        struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n19 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n18);
        struct hxc_caxecraft_domain_WaterCellState hxc_l_state = hxc_l_tmp_call_result_n19;
        bool hxc_l_tmp_call_result_n21 = hxc_caxecraft_app_WaterSurfaceGeometry_isWater(hxc_l_state);
        if (hxc_l_tmp_call_result_n21)
        {
          int32_t hxc_l_tmp_load_result_n22 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n23 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n25 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n22, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n23, 1), hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n26 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n25);
          bool hxc_l_tmp_call_result_n27 = hxc_caxecraft_app_WaterSurfaceGeometry_isOpenTop(hxc_l_tmp_call_result_n26);
          bool hxc_l_topOpen = hxc_l_tmp_call_result_n27;
          int32_t hxc_l_tmp_load_result_n28 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n29 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n31 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n28, hxc_l_tmp_load_result_n29, hxc_i32_subtract_wrapping(hxc_l_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n32 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n31);
          bool hxc_l_tmp_call_result_n33 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n32);
          bool hxc_l_northOpen = hxc_l_tmp_call_result_n33;
          int32_t hxc_l_tmp_load_result_n34 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n35 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n37 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n34, hxc_l_tmp_load_result_n35, hxc_i32_add_wrapping(hxc_l_z, 1));
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n38 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n37);
          bool hxc_l_tmp_call_result_n39 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n38);
          bool hxc_l_southOpen = hxc_l_tmp_call_result_n39;
          int32_t hxc_l_tmp_load_result_n40 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n41 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n43 = hxc_caxecraft_domain_World_coord(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n40, 1), hxc_l_tmp_load_result_n41, hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n44 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n43);
          bool hxc_l_tmp_call_result_n45 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n44);
          bool hxc_l_eastOpen = hxc_l_tmp_call_result_n45;
          int32_t hxc_l_tmp_load_result_n46 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n47 = hxc_l_y;
          struct hxc_caxecraft_scenario_VoxelPoint hxc_l_tmp_call_result_n49 = hxc_caxecraft_domain_World_coord(hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n46, 1), hxc_l_tmp_load_result_n47, hxc_l_z);
          struct hxc_caxecraft_domain_WaterCellState hxc_l_tmp_call_result_n50 = hxc_caxecraft_domain_WaterCellCodec_stateInView(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n49);
          bool hxc_l_tmp_call_result_n51 = hxc_caxecraft_app_WaterSurfaceGeometry_sideIsExposed(hxc_l_tmp_call_result_n50);
          bool hxc_l_westOpen = hxc_l_tmp_call_result_n51;
          bool hxc_l_tmp_load_result_n52 = hxc_l_topOpen;
          bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_load_result_n52;
          if (!hxc_l_tmp_load_result_n52)
          {
            hxc_l_tmp_short_circuit_result_n30 = hxc_l_northOpen;
          }
          bool hxc_l_tmp_short_circuit_load_result_n54 = hxc_l_tmp_short_circuit_result_n30;
          bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_short_circuit_load_result_n54;
          if (!hxc_l_tmp_short_circuit_load_result_n54)
          {
            hxc_l_tmp_short_circuit_result_n31 = hxc_l_southOpen;
          }
          bool hxc_l_tmp_short_circuit_load_result_n56 = hxc_l_tmp_short_circuit_result_n31;
          bool hxc_l_tmp_short_circuit_result_n32 = hxc_l_tmp_short_circuit_load_result_n56;
          if (!hxc_l_tmp_short_circuit_load_result_n56)
          {
            hxc_l_tmp_short_circuit_result_n32 = hxc_l_eastOpen;
          }
          bool hxc_l_tmp_short_circuit_load_result_n58 = hxc_l_tmp_short_circuit_result_n32;
          bool hxc_l_tmp_short_circuit_result_n33 = hxc_l_tmp_short_circuit_load_result_n58;
          if (!hxc_l_tmp_short_circuit_load_result_n58)
          {
            hxc_l_tmp_short_circuit_result_n33 = hxc_l_westOpen;
          }
          if (hxc_l_tmp_short_circuit_result_n33)
          {
            int32_t hxc_l_tmp_load_result_n61 = hxc_l_x;
            int32_t hxc_l_tmp_load_result_n62 = hxc_l_y;
            struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_call_result_n64 = hxc_caxecraft_app_WaterSurfaceGeometry_surfaceCorners(hxc_l_cells, hxc_l_length, hxc_l_tmp_load_result_n61, hxc_l_tmp_load_result_n62, hxc_l_z);
            struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_corners = hxc_l_tmp_call_result_n64;
            if (hxc_l_topOpen)
            {
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n66 = (*hxc_l_self).hxc_faces;
              int32_t hxc_l_tmp_load_result_n67 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n68 = hxc_l_y;
              int32_t hxc_l_tmp_load_result_n69 = hxc_l_z;
              struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_load_result_n70 = hxc_l_corners;
              struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_tmp_call_result_n72 = hxc_caxecraft_app_WaterRenderer_makeFace(0, hxc_l_tmp_load_result_n67, hxc_l_tmp_load_result_n68, hxc_l_tmp_load_result_n69, hxc_l_tmp_load_result_n70, hxc_l_state, hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
              int32_t hxc_l_tmp_array_push_result_n73;
              if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n66, &hxc_l_tmp_call_result_n72, &hxc_l_tmp_array_push_result_n73) != HXC_STATUS_OK)
              {
                abort();
              }
              (void)hxc_l_tmp_array_push_result_n73;
            }
            if (hxc_l_northOpen)
            {
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n75 = (*hxc_l_self).hxc_faces;
              int32_t hxc_l_tmp_load_result_n76 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n77 = hxc_l_y;
              int32_t hxc_l_tmp_load_result_n78 = hxc_l_z;
              struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_load_result_n79 = hxc_l_corners;
              struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_tmp_call_result_n81 = hxc_caxecraft_app_WaterRenderer_makeFace(1, hxc_l_tmp_load_result_n76, hxc_l_tmp_load_result_n77, hxc_l_tmp_load_result_n78, hxc_l_tmp_load_result_n79, hxc_l_state, hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
              int32_t hxc_l_tmp_array_push_result_n82;
              if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n75, &hxc_l_tmp_call_result_n81, &hxc_l_tmp_array_push_result_n82) != HXC_STATUS_OK)
              {
                abort();
              }
              (void)hxc_l_tmp_array_push_result_n82;
            }
            if (hxc_l_southOpen)
            {
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n84 = (*hxc_l_self).hxc_faces;
              int32_t hxc_l_tmp_load_result_n85 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n86 = hxc_l_y;
              int32_t hxc_l_tmp_load_result_n87 = hxc_l_z;
              struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_load_result_n88 = hxc_l_corners;
              struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_tmp_call_result_n90 = hxc_caxecraft_app_WaterRenderer_makeFace(2, hxc_l_tmp_load_result_n85, hxc_l_tmp_load_result_n86, hxc_l_tmp_load_result_n87, hxc_l_tmp_load_result_n88, hxc_l_state, hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
              int32_t hxc_l_tmp_array_push_result_n91;
              if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n84, &hxc_l_tmp_call_result_n90, &hxc_l_tmp_array_push_result_n91) != HXC_STATUS_OK)
              {
                abort();
              }
              (void)hxc_l_tmp_array_push_result_n91;
            }
            if (hxc_l_eastOpen)
            {
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n93 = (*hxc_l_self).hxc_faces;
              int32_t hxc_l_tmp_load_result_n94 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n95 = hxc_l_y;
              int32_t hxc_l_tmp_load_result_n96 = hxc_l_z;
              struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_load_result_n97 = hxc_l_corners;
              struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_tmp_call_result_n99 = hxc_caxecraft_app_WaterRenderer_makeFace(3, hxc_l_tmp_load_result_n94, hxc_l_tmp_load_result_n95, hxc_l_tmp_load_result_n96, hxc_l_tmp_load_result_n97, hxc_l_state, hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
              int32_t hxc_l_tmp_array_push_result_n100;
              if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n93, &hxc_l_tmp_call_result_n99, &hxc_l_tmp_array_push_result_n100) != HXC_STATUS_OK)
              {
                abort();
              }
              (void)hxc_l_tmp_array_push_result_n100;
            }
            if (hxc_l_westOpen)
            {
              struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n102 = (*hxc_l_self).hxc_faces;
              int32_t hxc_l_tmp_load_result_n103 = hxc_l_x;
              int32_t hxc_l_tmp_load_result_n104 = hxc_l_y;
              int32_t hxc_l_tmp_load_result_n105 = hxc_l_z;
              struct hxc_caxecraft_app_WaterSurfaceCorners hxc_l_tmp_load_result_n106 = hxc_l_corners;
              struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_tmp_call_result_n108 = hxc_caxecraft_app_WaterRenderer_makeFace(4, hxc_l_tmp_load_result_n103, hxc_l_tmp_load_result_n104, hxc_l_tmp_load_result_n105, hxc_l_tmp_load_result_n106, hxc_l_state, hxc_l_eyeX, hxc_l_eyeY, hxc_l_eyeZ, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
              int32_t hxc_l_tmp_array_push_result_n109;
              if (hxc_array_ref_push_copy(hxc_l_tmp_class_field_load_result_n102, &hxc_l_tmp_call_result_n108, &hxc_l_tmp_array_push_result_n109) != HXC_STATUS_OK)
              {
                abort();
              }
              (void)hxc_l_tmp_array_push_result_n109;
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
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n114 = (*hxc_l_self).hxc_faces;
  int32_t (*hxc_l_tmp_array_sort_comparator_n35)(struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace, struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace) = hxc_caxecraft_app_WaterRenderer_compareFaces;
  if (hxc_array_ref_sort(hxc_l_tmp_class_field_load_result_n114, hxc_array_90b40e8a_sort_compare, (void *)&hxc_l_tmp_array_sort_comparator_n35) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n116 = (*hxc_l_self).hxc_faces;
  int32_t hxc_l_tmp_array_length_result_n117;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n116, &hxc_l_tmp_array_length_result_n117) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n117 > 0)
  {
    rlDrawRenderBatchActive();
    rlDisableDepthMask();
    struct Texture hxc_l_raw = hxc_l_texture;
    uint32_t *hxc_l_tmp_imported_field_address_n118 = &hxc_l_raw.id;
    uint32_t hxc_l_tmp_imported_field_load_result_n119 = *hxc_l_tmp_imported_field_address_n118;
    rlSetTexture(hxc_l_tmp_imported_field_load_result_n119);
    rlBegin((int32_t)7);
    int32_t hxc_l_g = 0;
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n120 = (*hxc_l_self).hxc_faces;
    hxc_l_g1 = hxc_l_tmp_class_field_load_result_n120;
    if (hxc_array_ref_retain(hxc_l_g1) != HXC_STATUS_OK)
    {
      abort();
    }
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n121 = hxc_l_g;
      int32_t hxc_l_tmp_array_length_result_n123;
      if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n123) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n121 < hxc_l_tmp_array_length_result_n123))
      {
        break;
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n124 = hxc_l_g1;
      struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_tmp_array_get_result_n126;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n124, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n126) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_face = hxc_l_tmp_array_get_result_n126;
      hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
      struct hxc_caxecraft_app_WaterRenderer_WaterRenderFace hxc_l_tmp_load_result_n128 = hxc_l_face;
      double hxc_l_tmp_load_result_n129 = hxc_l_u0;
      double hxc_l_tmp_load_result_n130 = hxc_l_u1;
      double hxc_l_tmp_load_result_n131 = hxc_l_v0;
      hxc_caxecraft_app_WaterRenderer_emitFace(hxc_l_tmp_load_result_n128, hxc_l_tmp_load_result_n129, hxc_l_tmp_load_result_n130, hxc_l_tmp_load_result_n131, hxc_l_v1, hxc_l_offsetX, hxc_l_offsetY, hxc_l_offsetZ);
    }
    rlEnd();
    rlSetTexture((uint32_t)0);
    rlDrawRenderBatchActive();
    rlEnableDepthMask();
    if (hxc_array_ref_release(hxc_l_g1) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  int32_t hxc_l_drawCalls = 0;
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n133 = (*hxc_l_self).hxc_faces;
  int32_t hxc_l_tmp_array_length_result_n134;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n133, &hxc_l_tmp_array_length_result_n134) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n134 > 0)
  {
    hxc_l_drawCalls = 1;
  }
  int32_t hxc_l_tmp_load_result_n135 = hxc_l_visible;
  return (struct hxc_caxecraft_app_WaterRenderCounters){ .hxc_drawCalls = hxc_l_drawCalls, .hxc_visible = hxc_l_tmp_load_result_n135 };
}
