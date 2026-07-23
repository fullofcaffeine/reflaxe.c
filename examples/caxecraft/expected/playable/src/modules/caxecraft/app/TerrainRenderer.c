#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_TerrainRenderer(struct hxc_caxecraft_app_TerrainRenderer *hxc_self)
{
  (*hxc_self).hxc_totalRebuiltChunks = 0;
  struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_cache;
  hxc_compiler_constructor_caxecraft_app_TerrainChunkCache(hxc_tmp_owned_class_field_address_n0);
  return;
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
        rlTexCoord2f((float)hxc_tmp_load_result_n18, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_z);
        double hxc_tmp_load_result_n20 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n20, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n22 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n22, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n24 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n24, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_z);
        break;
      }
    case hxc_caxecraft_app_VoxelFace_Bottom:
      {
        rlNormal3f((float)0.0, (float)-1.0, (float)0.0);
        double hxc_tmp_load_result_n26 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n26, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_y, (float)(double)hxc_z);
        double hxc_tmp_load_result_n28 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n28, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_y, (float)(double)hxc_z);
        double hxc_tmp_load_result_n30 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n30, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_y, (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n32 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n32, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_y, (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_North:
      {
        rlNormal3f((float)0.0, (float)0.0, (float)-1.0);
        double hxc_tmp_load_result_n34 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n34, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_y, (float)(double)hxc_z);
        double hxc_tmp_load_result_n36 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n36, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_z);
        double hxc_tmp_load_result_n38 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n38, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_z);
        double hxc_tmp_load_result_n40 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n40, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_y, (float)(double)hxc_z);
        break;
      }
    case hxc_caxecraft_app_VoxelFace_South:
      {
        rlNormal3f((float)0.0, (float)0.0, (float)1.0);
        double hxc_tmp_load_result_n42 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n42, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_y, (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n44 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n44, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_y, (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n46 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n46, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n48 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n48, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_East:
      {
        rlNormal3f((float)1.0, (float)0.0, (float)0.0);
        double hxc_tmp_load_result_n50 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n50, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_y, (float)(double)hxc_z);
        double hxc_tmp_load_result_n52 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n52, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_z);
        double hxc_tmp_load_result_n54 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n54, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n56 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n56, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_x, 1), (float)(double)hxc_y, (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_West:
      {
        rlNormal3f((float)-1.0, (float)0.0, (float)0.0);
        double hxc_tmp_load_result_n58 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n58, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_y, (float)(double)hxc_z);
        double hxc_tmp_load_result_n60 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n60, (float)hxc_v1);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_y, (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n62 = hxc_u1;
        rlTexCoord2f((float)hxc_tmp_load_result_n62, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_z, 1));
        double hxc_tmp_load_result_n64 = hxc_u0;
        rlTexCoord2f((float)hxc_tmp_load_result_n64, (float)hxc_v0);
        rlVertex3f((float)(double)hxc_x, (float)(double)hxc_i32_add_wrapping(hxc_y, 1), (float)(double)hxc_z);
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

int32_t hxc_caxecraft_app_TerrainRenderer_squareDistance(int32_t hxc_x, int32_t hxc_z, int32_t hxc_playerX, int32_t hxc_playerZ)
{
  int32_t hxc_dx = hxc_i32_subtract_wrapping(hxc_x, hxc_playerX);
  if (hxc_dx < 0)
  {
    hxc_dx = hxc_i32_negate_wrapping(hxc_dx);
  }
  int32_t hxc_dz = hxc_i32_subtract_wrapping(hxc_z, hxc_playerZ);
  if (hxc_dz < 0)
  {
    hxc_dz = hxc_i32_negate_wrapping(hxc_dz);
  }
  int32_t hxc_tmp_load_result_n4 = hxc_dx;
  if (!(hxc_tmp_load_result_n4 > hxc_dz))
  {
    return hxc_dz;
  }
  return hxc_dx;
}

struct hxc_caxecraft_app_TerrainRenderCounters hxc_caxecraft_app_TerrainRenderer_draw(struct hxc_caxecraft_app_TerrainRenderer *hxc_self, uint8_t *hxc_cells, size_t hxc_length, struct Texture hxc_baseTexture, bool hxc_baseReady, struct Texture hxc_adventureTexture, bool hxc_adventureReady, double hxc_playerX, double hxc_playerZ)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n8 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n8;
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n0 = &(*hxc_self).hxc_cache;
  if (hxc_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainChunkPreparation hxc_tmp_instance_call_result_n1 = hxc_caxecraft_app_TerrainChunkCache_prepare(hxc_tmp_owned_class_field_address_n0, hxc_cells, hxc_length);
  struct hxc_caxecraft_app_TerrainChunkPreparation hxc_preparation = hxc_tmp_instance_call_result_n1;
  int32_t hxc_tmp_compound_load_result_n2 = (*hxc_self).hxc_totalRebuiltChunks;
  (*hxc_self).hxc_totalRebuiltChunks = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n2, hxc_preparation.hxc_rebuiltChunks);
  if (!!hxc_preparation.hxc_valid)
  {
    int32_t hxc_playerCellX = hxc_f64_to_i32_saturating(hxc_playerX);
    int32_t hxc_playerCellZ = hxc_f64_to_i32_saturating(hxc_playerZ);
    int32_t hxc_tmp_load_result_n9 = hxc_playerCellX;
    struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_tmp_instance_call_result_n11 = hxc_caxecraft_app_TerrainRenderer_drawSheet(hxc_self, hxc_baseTexture, hxc_baseReady, hxc_caxecraft_app_TerrainSheet_Base, hxc_tmp_load_result_n9, hxc_playerCellZ);
    struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_base = hxc_tmp_instance_call_result_n11;
    int32_t hxc_tmp_load_result_n13 = hxc_playerCellX;
    struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_tmp_instance_call_result_n15 = hxc_caxecraft_app_TerrainRenderer_drawSheet(hxc_self, hxc_adventureTexture, hxc_adventureReady, hxc_caxecraft_app_TerrainSheet_Adventure, hxc_tmp_load_result_n13, hxc_playerCellZ);
    struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_adventure = hxc_tmp_instance_call_result_n15;
    int32_t hxc_tmp_record_field_load_result_n16 = hxc_base.hxc_visible;
    int32_t hxc_tmp_record_field_load_result_n17 = hxc_adventure.hxc_visible;
    int32_t hxc_tmp_record_field_load_result_n18 = hxc_base.hxc_faces;
    int32_t hxc_tmp_record_field_load_result_n19 = hxc_adventure.hxc_faces;
    int32_t hxc_tmp_record_field_load_result_n20 = hxc_base.hxc_drawCalls;
    int32_t hxc_tmp_record_field_load_result_n21 = hxc_adventure.hxc_drawCalls;
    int32_t hxc_tmp_record_field_load_result_n22 = hxc_preparation.hxc_rebuiltChunks;
    int32_t hxc_tmp_class_field_load_result_n23 = (*hxc_self).hxc_totalRebuiltChunks;
    return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_cacheValid = true, .hxc_drawCalls = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n20, hxc_tmp_record_field_load_result_n21), .hxc_faces = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n18, hxc_tmp_record_field_load_result_n19), .hxc_rebuiltChunks = hxc_tmp_record_field_load_result_n22, .hxc_totalRebuiltChunks = hxc_tmp_class_field_load_result_n23, .hxc_visible = hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n16, hxc_tmp_record_field_load_result_n17) };
  }
  int32_t hxc_tmp_record_field_load_result_n5 = hxc_preparation.hxc_rebuiltChunks;
  int32_t hxc_tmp_class_field_load_result_n6 = (*hxc_self).hxc_totalRebuiltChunks;
  return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_cacheValid = false, .hxc_drawCalls = 0, .hxc_faces = 0, .hxc_rebuiltChunks = hxc_tmp_record_field_load_result_n5, .hxc_totalRebuiltChunks = hxc_tmp_class_field_load_result_n6, .hxc_visible = 0 };
}

struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_caxecraft_app_TerrainRenderer_drawSheet(struct hxc_caxecraft_app_TerrainRenderer *hxc_self, struct Texture hxc_texture, bool hxc_textureReady, enum hxc_caxecraft_app_TerrainSheet hxc_sheet, int32_t hxc_playerCellX, int32_t hxc_playerCellZ)
{
  if (!hxc_textureReady)
  {
    return (struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters){ .hxc_drawCalls = 0, .hxc_faces = 0, .hxc_visible = 0 };
  }
  double hxc_halfPixel = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_texture.width);
  int32_t hxc_faces = 0;
  struct Texture hxc_raw = hxc_texture;
  uint32_t *hxc_tmp_imported_field_address_n2 = &hxc_raw.id;
  uint32_t hxc_tmp_imported_field_load_result_n3 = *hxc_tmp_imported_field_address_n2;
  rlSetTexture(hxc_tmp_imported_field_load_result_n3);
  rlBegin((int32_t)7);
  int32_t hxc_chunk = 0;
  while (1)
  {
    if (!(hxc_chunk < 16))
    {
      break;
    }
    if (hxc_self == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n5 = &(*hxc_self).hxc_cache;
    struct hxc_caxecraft_app_TerrainChunkCache *hxc_this = hxc_tmp_owned_class_field_address_n5;
    (void)hxc_this;
    int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_app_TerrainChunkLayout_facePartitionStart(hxc_chunk);
    int32_t hxc_start = hxc_tmp_call_result_n7;
    struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n8 = &(*hxc_self).hxc_cache;
    if (hxc_tmp_owned_class_field_address_n8 == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_instance_call_result_n10 = hxc_caxecraft_app_TerrainChunkCache_chunkFaceCount(hxc_tmp_owned_class_field_address_n8, hxc_chunk);
    int32_t hxc_count = hxc_tmp_instance_call_result_n10;
    int32_t hxc_offset = 0;
    while (1)
    {
      int32_t hxc_tmp_load_result_n11 = hxc_offset;
      if (!(hxc_tmp_load_result_n11 < hxc_count))
      {
        break;
      }
      int32_t hxc_tmp_load_result_n13 = hxc_start;
      int32_t hxc_index = hxc_i32_add_wrapping(hxc_tmp_load_result_n13, hxc_offset);
      struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n15 = &(*hxc_self).hxc_cache;
      int32_t hxc_tmp_load_result_n16 = hxc_index;
      if (hxc_tmp_load_result_n16 < 0 || (size_t)hxc_tmp_load_result_n16 >= 49152)
      {
        abort();
      }
      uint8_t hxc_tmp_collection_index_load_result_n17 = (*hxc_tmp_owned_class_field_address_n15).hxc_packedFaces[(size_t)hxc_tmp_load_result_n16];
      int32_t hxc_packed = (int32_t)hxc_tmp_collection_index_load_result_n17;
      enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n19 = hxc_caxecraft_app_TerrainChunkLayout_unpackKind(hxc_packed);
      enum hxc_caxecraft_domain_BlockKind hxc_kind = hxc_tmp_call_result_n19;
      enum hxc_caxecraft_app_TerrainSheet hxc_tmp_call_result_n21 = hxc_caxecraft_app_TerrainAtlas_sheet(hxc_kind);
      if (hxc_tmp_call_result_n21 == hxc_sheet)
      {
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n22 = &(*hxc_self).hxc_cache;
        int32_t hxc_tmp_load_result_n23 = hxc_index;
        if (hxc_tmp_load_result_n23 < 0 || (size_t)hxc_tmp_load_result_n23 >= 49152)
        {
          abort();
        }
        uint8_t hxc_tmp_collection_index_load_result_n24 = (*hxc_tmp_owned_class_field_address_n22).hxc_faceX[(size_t)hxc_tmp_load_result_n23];
        int32_t hxc_x = (int32_t)hxc_tmp_collection_index_load_result_n24;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n25 = &(*hxc_self).hxc_cache;
        int32_t hxc_tmp_load_result_n26 = hxc_index;
        if (hxc_tmp_load_result_n26 < 0 || (size_t)hxc_tmp_load_result_n26 >= 49152)
        {
          abort();
        }
        uint8_t hxc_tmp_collection_index_load_result_n27 = (*hxc_tmp_owned_class_field_address_n25).hxc_faceY[(size_t)hxc_tmp_load_result_n26];
        int32_t hxc_y = (int32_t)hxc_tmp_collection_index_load_result_n27;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n28 = &(*hxc_self).hxc_cache;
        int32_t hxc_tmp_load_result_n29 = hxc_index;
        if (hxc_tmp_load_result_n29 < 0 || (size_t)hxc_tmp_load_result_n29 >= 49152)
        {
          abort();
        }
        uint8_t hxc_tmp_collection_index_load_result_n30 = (*hxc_tmp_owned_class_field_address_n28).hxc_faceZ[(size_t)hxc_tmp_load_result_n29];
        int32_t hxc_z = (int32_t)hxc_tmp_collection_index_load_result_n30;
        enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n31 = hxc_kind;
        enum hxc_caxecraft_app_VoxelFace hxc_tmp_call_result_n33 = hxc_caxecraft_app_TerrainChunkLayout_unpackFace(hxc_packed);
        int32_t hxc_tmp_load_result_n34 = hxc_x;
        int32_t hxc_tmp_load_result_n35 = hxc_y;
        int32_t hxc_tmp_load_result_n36 = hxc_z;
        double hxc_tmp_load_result_n37 = hxc_halfPixel;
        int32_t hxc_tmp_load_result_n38 = hxc_x;
        int32_t hxc_tmp_call_result_n40 = hxc_caxecraft_app_TerrainRenderer_squareDistance(hxc_tmp_load_result_n38, hxc_z, hxc_playerCellX, hxc_playerCellZ);
        hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_tmp_load_result_n31, hxc_tmp_call_result_n33, hxc_tmp_load_result_n34, hxc_tmp_load_result_n35, hxc_tmp_load_result_n36, hxc_tmp_load_result_n37, hxc_tmp_call_result_n40);
        hxc_faces = hxc_i32_add_wrapping(hxc_faces, 1);
      }
      hxc_offset = hxc_i32_add_wrapping(hxc_offset, 1);
    }
    hxc_chunk = hxc_i32_add_wrapping(hxc_chunk, 1);
  }
  rlEnd();
  rlSetTexture((uint32_t)0);
  if (hxc_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainChunkCache *hxc_tmp_owned_class_field_address_n44 = &(*hxc_self).hxc_cache;
  if (hxc_tmp_owned_class_field_address_n44 == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n45 = hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(hxc_tmp_owned_class_field_address_n44, hxc_sheet);
  int32_t hxc_tmp_record_field_visible_n20 = hxc_tmp_instance_call_result_n45;
  int32_t hxc_tmp_record_field_faces_n21 = hxc_faces;
  int32_t hxc_tmp_load_result_n47 = hxc_faces;
  int32_t hxc_tmp_conditional_result_n22 = 0;
  if (hxc_tmp_load_result_n47 > 0)
  {
    hxc_tmp_conditional_result_n22 = 1;
  }
  else
  {
    hxc_tmp_conditional_result_n22 = 0;
  }
  int32_t hxc_tmp_conditional_load_result_n48 = hxc_tmp_conditional_result_n22;
  int32_t hxc_tmp_record_field_faces_load_result_n49 = hxc_tmp_record_field_faces_n21;
  return (struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters){ .hxc_drawCalls = hxc_tmp_conditional_load_result_n48, .hxc_faces = hxc_tmp_record_field_faces_load_result_n49, .hxc_visible = hxc_tmp_record_field_visible_n20 };
}
