#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_TerrainRenderer(struct hxc_caxecraft_app_TerrainRenderer *hxc_l_self)
{
  (*hxc_l_self).hxc_totalRebuiltChunks = 0;
  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_cache;
  hxc_compiler_constructor_caxecraft_app_TerrainChunkCache(hxc_l_tmp_owned_class_field_address_n0);
  return;
}

void hxc_caxecraft_app_TerrainRenderer_emitFace(enum hxc_caxecraft_domain_BlockKind hxc_l_kind, enum hxc_caxecraft_app_VoxelFace hxc_l_face, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z, double hxc_l_halfPixel, int32_t hxc_l_distance)
{
  enum hxc_caxecraft_app_TerrainTile hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_TerrainAtlas_tile(hxc_l_kind, hxc_l_face);
  enum hxc_caxecraft_app_TerrainTile hxc_l_tile = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_app_TerrainAtlas_column(hxc_l_tile);
  double hxc_l_u0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_call_result_n2, (double)4) + hxc_l_halfPixel;
  int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_app_TerrainAtlas_column(hxc_l_tile);
  double hxc_l_u1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_call_result_n4, 1), (double)4) - hxc_l_halfPixel;
  int32_t hxc_l_tmp_call_result_n6 = hxc_caxecraft_app_TerrainAtlas_row(hxc_l_tile);
  double hxc_l_v0 = hxc_f64_divide_zero_safe((double)hxc_l_tmp_call_result_n6, (double)4) + hxc_l_halfPixel;
  int32_t hxc_l_tmp_call_result_n8 = hxc_caxecraft_app_TerrainAtlas_row(hxc_l_tile);
  double hxc_l_v1 = hxc_f64_divide_zero_safe((double)hxc_i32_add_wrapping(hxc_l_tmp_call_result_n8, 1), (double)4) - hxc_l_halfPixel;
  struct Color hxc_l_tmp_call_result_n9 = hxc_caxecraft_app_TerrainRenderer_faceTint(hxc_l_face, hxc_l_distance);
  struct Color hxc_l_raw = hxc_l_tmp_call_result_n9;
  uint8_t *hxc_l_tmp_imported_field_address_n10 = &hxc_l_raw.r;
  uint8_t hxc_l_tmp_imported_field_load_result_n11 = *hxc_l_tmp_imported_field_address_n10;
  uint8_t *hxc_l_tmp_imported_field_address_n12 = &hxc_l_raw.g;
  uint8_t hxc_l_tmp_imported_field_load_result_n13 = *hxc_l_tmp_imported_field_address_n12;
  uint8_t *hxc_l_tmp_imported_field_address_n14 = &hxc_l_raw.b;
  uint8_t hxc_l_tmp_imported_field_load_result_n15 = *hxc_l_tmp_imported_field_address_n14;
  uint8_t *hxc_l_tmp_imported_field_address_n16 = &hxc_l_raw.a;
  uint8_t hxc_l_tmp_imported_field_load_result_n17 = *hxc_l_tmp_imported_field_address_n16;
  rlColor4ub(hxc_l_tmp_imported_field_load_result_n11, hxc_l_tmp_imported_field_load_result_n13, hxc_l_tmp_imported_field_load_result_n15, hxc_l_tmp_imported_field_load_result_n17);
  switch (hxc_l_face) {
    case hxc_caxecraft_app_VoxelFace_Top:
      {
        rlNormal3f((float)0.0, (float)1.0, (float)0.0);
        double hxc_l_tmp_load_result_n18 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n18, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n20 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n20, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n22 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n22, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n24 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n24, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_l_z);
        break;
      }
    case hxc_caxecraft_app_VoxelFace_Bottom:
      {
        rlNormal3f((float)0.0, (float)-1.0, (float)0.0);
        double hxc_l_tmp_load_result_n26 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n26, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_l_y, (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n28 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n28, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_l_y, (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n30 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n30, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_l_y, (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n32 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n32, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_l_y, (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_North:
      {
        rlNormal3f((float)0.0, (float)0.0, (float)-1.0);
        double hxc_l_tmp_load_result_n34 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n34, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_l_y, (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n36 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n36, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n38 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n38, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n40 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n40, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_l_y, (float)(double)hxc_l_z);
        break;
      }
    case hxc_caxecraft_app_VoxelFace_South:
      {
        rlNormal3f((float)0.0, (float)0.0, (float)1.0);
        double hxc_l_tmp_load_result_n42 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n42, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_l_y, (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n44 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n44, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_l_y, (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n46 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n46, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n48 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n48, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_East:
      {
        rlNormal3f((float)1.0, (float)0.0, (float)0.0);
        double hxc_l_tmp_load_result_n50 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n50, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_l_y, (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n52 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n52, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n54 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n54, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n56 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n56, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_i32_add_wrapping(hxc_l_x, 1), (float)(double)hxc_l_y, (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        break;
      }
    case hxc_caxecraft_app_VoxelFace_West:
      {
        rlNormal3f((float)-1.0, (float)0.0, (float)0.0);
        double hxc_l_tmp_load_result_n58 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n58, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_l_y, (float)(double)hxc_l_z);
        double hxc_l_tmp_load_result_n60 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n60, (float)hxc_l_v1);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_l_y, (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n62 = hxc_l_u1;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n62, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_i32_add_wrapping(hxc_l_z, 1));
        double hxc_l_tmp_load_result_n64 = hxc_l_u0;
        rlTexCoord2f((float)hxc_l_tmp_load_result_n64, (float)hxc_l_v0);
        rlVertex3f((float)(double)hxc_l_x, (float)(double)hxc_i32_add_wrapping(hxc_l_y, 1), (float)(double)hxc_l_z);
        break;
      }
    default:
      {
        abort();
      }
  }
  return;
}

struct Color hxc_caxecraft_app_TerrainRenderer_faceTint(enum hxc_caxecraft_app_VoxelFace hxc_l_face, int32_t hxc_l_distance)
{
  if (!(hxc_l_distance >= 22))
  {
    if (!(hxc_l_distance >= 12))
    {
      switch (hxc_l_face) {
        case hxc_caxecraft_app_VoxelFace_Top:
          {
            break;
          }
        case hxc_caxecraft_app_VoxelFace_Bottom:
          {
            return (struct Color){ .r = 178, .g = 191, .b = 188, .a = 255 };
          }
        case hxc_caxecraft_app_VoxelFace_North:
        case hxc_caxecraft_app_VoxelFace_East:
          {
            return (struct Color){ .r = 235, .g = 241, .b = 230, .a = 255 };
          }
        case hxc_caxecraft_app_VoxelFace_South:
        case hxc_caxecraft_app_VoxelFace_West:
          {
            return (struct Color){ .r = 211, .g = 225, .b = 220, .a = 255 };
          }
        default:
          {
            abort();
          }
      }
      return (struct Color){ .r = 255, .g = 255, .b = 255, .a = 255 };
    }
    switch (hxc_l_face) {
      case hxc_caxecraft_app_VoxelFace_Top:
        {
          break;
        }
      case hxc_caxecraft_app_VoxelFace_Bottom:
        {
          return (struct Color){ .r = 173, .g = 190, .b = 188, .a = 255 };
        }
      case hxc_caxecraft_app_VoxelFace_North:
      case hxc_caxecraft_app_VoxelFace_East:
        {
          return (struct Color){ .r = 219, .g = 231, .b = 222, .a = 255 };
        }
      case hxc_caxecraft_app_VoxelFace_South:
      case hxc_caxecraft_app_VoxelFace_West:
        {
          return (struct Color){ .r = 199, .g = 216, .b = 211, .a = 255 };
        }
      default:
        {
          abort();
        }
    }
    return (struct Color){ .r = 240, .g = 246, .b = 239, .a = 255 };
  }
  switch (hxc_l_face) {
    case hxc_caxecraft_app_VoxelFace_Top:
      {
        break;
      }
    case hxc_caxecraft_app_VoxelFace_Bottom:
      {
        return (struct Color){ .r = 158, .g = 182, .b = 185, .a = 255 };
      }
    case hxc_caxecraft_app_VoxelFace_North:
    case hxc_caxecraft_app_VoxelFace_East:
      {
        return (struct Color){ .r = 197, .g = 218, .b = 212, .a = 255 };
      }
    case hxc_caxecraft_app_VoxelFace_South:
    case hxc_caxecraft_app_VoxelFace_West:
      {
        return (struct Color){ .r = 181, .g = 205, .b = 204, .a = 255 };
      }
    default:
      {
        abort();
      }
  }
  return (struct Color){ .r = 220, .g = 235, .b = 226, .a = 255 };
}

int32_t hxc_caxecraft_app_TerrainRenderer_squareDistance(int32_t hxc_l_x, int32_t hxc_l_z, int32_t hxc_l_playerX, int32_t hxc_l_playerZ)
{
  int32_t hxc_l_dx = hxc_i32_subtract_wrapping(hxc_l_x, hxc_l_playerX);
  if (hxc_l_dx < 0)
  {
    hxc_l_dx = hxc_i32_negate_wrapping(hxc_l_dx);
  }
  int32_t hxc_l_dz = hxc_i32_subtract_wrapping(hxc_l_z, hxc_l_playerZ);
  if (hxc_l_dz < 0)
  {
    hxc_l_dz = hxc_i32_negate_wrapping(hxc_l_dz);
  }
  int32_t hxc_l_tmp_load_result_n4 = hxc_l_dx;
  if (!(hxc_l_tmp_load_result_n4 > hxc_l_dz))
  {
    return hxc_l_dz;
  }
  return hxc_l_dx;
}

struct hxc_caxecraft_app_TerrainRenderCounters hxc_caxecraft_app_TerrainRenderer_draw(struct hxc_caxecraft_app_TerrainRenderer *hxc_l_self, const uint8_t *hxc_l_cells, size_t hxc_l_length, struct Texture hxc_l_baseTexture, bool hxc_l_baseReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureReady, double hxc_l_playerX, double hxc_l_playerZ)
{
  int32_t hxc_l_playerCellX = { 0 };
  int32_t hxc_l_playerCellZ = { 0 };
  struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_l_base = { 0 };
  struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_l_adventure = { 0 };
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n8 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n8;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n0 = &(*hxc_l_self).hxc_cache;
  if (hxc_l_tmp_owned_class_field_address_n0 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainChunkPreparation hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_app_TerrainChunkCache_prepare(hxc_l_tmp_owned_class_field_address_n0, hxc_l_cells, hxc_l_length);
  struct hxc_caxecraft_app_TerrainChunkPreparation hxc_l_preparation = hxc_l_tmp_instance_call_result_n1;
  int32_t hxc_l_tmp_compound_load_result_n2 = (*hxc_l_self).hxc_totalRebuiltChunks;
  (*hxc_l_self).hxc_totalRebuiltChunks = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n2, hxc_l_preparation.hxc_rebuiltChunks);
  if (!!hxc_l_preparation.hxc_valid)
  {
    hxc_l_playerCellX = hxc_f64_to_i32_saturating(hxc_l_playerX);
    hxc_l_playerCellZ = hxc_f64_to_i32_saturating(hxc_l_playerZ);
    int32_t hxc_l_tmp_load_result_n9 = hxc_l_playerCellX;
    struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_app_TerrainRenderer_drawSheet(hxc_l_self, hxc_l_baseTexture, hxc_l_baseReady, hxc_caxecraft_app_TerrainSheet_Base, hxc_l_tmp_load_result_n9, hxc_l_playerCellZ);
    hxc_l_base = hxc_l_tmp_instance_call_result_n11;
    int32_t hxc_l_tmp_load_result_n13 = hxc_l_playerCellX;
    struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_l_tmp_instance_call_result_n15 = hxc_caxecraft_app_TerrainRenderer_drawSheet(hxc_l_self, hxc_l_adventureTexture, hxc_l_adventureReady, hxc_caxecraft_app_TerrainSheet_Adventure, hxc_l_tmp_load_result_n13, hxc_l_playerCellZ);
    hxc_l_adventure = hxc_l_tmp_instance_call_result_n15;
    int32_t hxc_l_tmp_record_field_load_result_n16 = hxc_l_base.hxc_visible;
    int32_t hxc_l_tmp_record_field_load_result_n17 = hxc_l_adventure.hxc_visible;
    int32_t hxc_l_tmp_record_field_load_result_n18 = hxc_l_base.hxc_faces;
    int32_t hxc_l_tmp_record_field_load_result_n19 = hxc_l_adventure.hxc_faces;
    int32_t hxc_l_tmp_record_field_load_result_n20 = hxc_l_base.hxc_drawCalls;
    int32_t hxc_l_tmp_record_field_load_result_n21 = hxc_l_adventure.hxc_drawCalls;
    int32_t hxc_l_tmp_record_field_load_result_n22 = hxc_l_preparation.hxc_rebuiltChunks;
    int32_t hxc_l_tmp_class_field_load_result_n23 = (*hxc_l_self).hxc_totalRebuiltChunks;
    return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_cacheValid = true, .hxc_drawCalls = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n20, hxc_l_tmp_record_field_load_result_n21), .hxc_faces = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n18, hxc_l_tmp_record_field_load_result_n19), .hxc_rebuiltChunks = hxc_l_tmp_record_field_load_result_n22, .hxc_totalRebuiltChunks = hxc_l_tmp_class_field_load_result_n23, .hxc_visible = hxc_i32_add_wrapping(hxc_l_tmp_record_field_load_result_n16, hxc_l_tmp_record_field_load_result_n17) };
  }
  int32_t hxc_l_tmp_record_field_load_result_n5 = hxc_l_preparation.hxc_rebuiltChunks;
  int32_t hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_totalRebuiltChunks;
  return (struct hxc_caxecraft_app_TerrainRenderCounters){ .hxc_cacheValid = false, .hxc_drawCalls = 0, .hxc_faces = 0, .hxc_rebuiltChunks = hxc_l_tmp_record_field_load_result_n5, .hxc_totalRebuiltChunks = hxc_l_tmp_class_field_load_result_n6, .hxc_visible = 0 };
}

struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_caxecraft_app_TerrainRenderer_drawSheet(struct hxc_caxecraft_app_TerrainRenderer *hxc_l_self, struct Texture hxc_l_texture, bool hxc_l_textureReady, enum hxc_caxecraft_app_TerrainSheet hxc_l_sheet, int32_t hxc_l_playerCellX, int32_t hxc_l_playerCellZ)
{
  if (!hxc_l_textureReady)
  {
    return (struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters){ .hxc_drawCalls = 0, .hxc_faces = 0, .hxc_visible = 0 };
  }
  double hxc_l_halfPixel = hxc_f64_divide_zero_safe(0.5, (double)(int32_t)hxc_l_texture.width);
  int32_t hxc_l_faces = 0;
  struct Texture hxc_l_raw = hxc_l_texture;
  uint32_t *hxc_l_tmp_imported_field_address_n2 = &hxc_l_raw.id;
  uint32_t hxc_l_tmp_imported_field_load_result_n3 = *hxc_l_tmp_imported_field_address_n2;
  rlSetTexture(hxc_l_tmp_imported_field_load_result_n3);
  rlBegin((int32_t)7);
  int32_t hxc_l_chunk = 0;
  while (1)
  {
    if (!(hxc_l_chunk < 32))
    {
      break;
    }
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n5 = &(*hxc_l_self).hxc_cache;
    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_this_hca496068c073 = hxc_l_tmp_owned_class_field_address_n5;
    (void)hxc_l_this_hca496068c073;
    int32_t hxc_l_tmp_call_result_n7 = hxc_caxecraft_app_TerrainChunkLayout_facePartitionStart(hxc_l_chunk);
    int32_t hxc_l_start = hxc_l_tmp_call_result_n7;
    struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n8 = &(*hxc_l_self).hxc_cache;
    if (hxc_l_tmp_owned_class_field_address_n8 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_app_TerrainChunkCache_chunkFaceCount(hxc_l_tmp_owned_class_field_address_n8, hxc_l_chunk);
    int32_t hxc_l_count = hxc_l_tmp_instance_call_result_n10;
    int32_t hxc_l_offset = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n11 = hxc_l_offset;
      if (!(hxc_l_tmp_load_result_n11 < hxc_l_count))
      {
        break;
      }
      int32_t hxc_l_tmp_load_result_n13 = hxc_l_start;
      int32_t hxc_l_index = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n13, hxc_l_offset);
      struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n15 = &(*hxc_l_self).hxc_cache;
      struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_this_hbc1ec84e75b5 = hxc_l_tmp_owned_class_field_address_n15;
      int32_t hxc_l_tmp_load_result_n16 = hxc_l_index;
      int32_t hxc_l_tmp_conditional_result_n17 = 0;
      if (hxc_l_tmp_load_result_n16 < 49152)
      {
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n17 = hxc_l_this_hbc1ec84e75b5;
        if (hxc_l_tmp_load_result_n17 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n18 = hxc_l_index;
        if (hxc_l_tmp_load_result_n18 < 0 || (size_t)hxc_l_tmp_load_result_n18 >= 49152)
        {
          abort();
        }
        uint8_t hxc_l_tmp_collection_index_load_result_n19 = (*hxc_l_tmp_load_result_n17).hxc_packedFaces0[(size_t)hxc_l_tmp_load_result_n18];
        hxc_l_tmp_conditional_result_n17 = (int32_t)hxc_l_tmp_collection_index_load_result_n19;
      }
      else
      {
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n20 = hxc_l_this_hbc1ec84e75b5;
        if (hxc_l_tmp_load_result_n20 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n21 = hxc_l_index;
        if (hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n21, 49152) < 0 || (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n21, 49152) >= 49152)
        {
          abort();
        }
        uint8_t hxc_l_tmp_collection_index_load_result_n22 = (*hxc_l_tmp_load_result_n20).hxc_packedFaces1[(size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n21, 49152)];
        hxc_l_tmp_conditional_result_n17 = (int32_t)hxc_l_tmp_collection_index_load_result_n22;
      }
      int32_t hxc_l_packed = hxc_l_tmp_conditional_result_n17;
      enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n25 = hxc_caxecraft_app_TerrainChunkLayout_unpackKind(hxc_l_packed);
      enum hxc_caxecraft_domain_BlockKind hxc_l_kind = hxc_l_tmp_call_result_n25;
      enum hxc_caxecraft_app_TerrainSheet hxc_l_tmp_call_result_n27 = hxc_caxecraft_app_TerrainAtlas_sheet(hxc_l_kind);
      if (hxc_l_tmp_call_result_n27 == hxc_l_sheet)
      {
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n28 = &(*hxc_l_self).hxc_cache;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_this_hb68518ee14f6 = hxc_l_tmp_owned_class_field_address_n28;
        int32_t hxc_l_tmp_load_result_n29 = hxc_l_index;
        int32_t hxc_l_tmp_conditional_result_n21 = 0;
        if (hxc_l_tmp_load_result_n29 < 49152)
        {
          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n30 = hxc_l_this_hb68518ee14f6;
          if (hxc_l_tmp_load_result_n30 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n31 = hxc_l_index;
          if (hxc_l_tmp_load_result_n31 < 0 || (size_t)hxc_l_tmp_load_result_n31 >= 49152)
          {
            abort();
          }
          uint8_t hxc_l_tmp_collection_index_load_result_n32 = (*hxc_l_tmp_load_result_n30).hxc_faceX0[(size_t)hxc_l_tmp_load_result_n31];
          hxc_l_tmp_conditional_result_n21 = (int32_t)hxc_l_tmp_collection_index_load_result_n32;
        }
        else
        {
          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n33 = hxc_l_this_hb68518ee14f6;
          if (hxc_l_tmp_load_result_n33 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n34 = hxc_l_index;
          if (hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n34, 49152) < 0 || (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n34, 49152) >= 49152)
          {
            abort();
          }
          uint8_t hxc_l_tmp_collection_index_load_result_n35 = (*hxc_l_tmp_load_result_n33).hxc_faceX1[(size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n34, 49152)];
          hxc_l_tmp_conditional_result_n21 = (int32_t)hxc_l_tmp_collection_index_load_result_n35;
        }
        int32_t hxc_l_x = hxc_l_tmp_conditional_result_n21;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n37 = &(*hxc_l_self).hxc_cache;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_this_hb33e315d43da = hxc_l_tmp_owned_class_field_address_n37;
        int32_t hxc_l_tmp_load_result_n38 = hxc_l_index;
        int32_t hxc_l_tmp_conditional_result_n24 = 0;
        if (hxc_l_tmp_load_result_n38 < 49152)
        {
          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n39 = hxc_l_this_hb33e315d43da;
          if (hxc_l_tmp_load_result_n39 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n40 = hxc_l_index;
          if (hxc_l_tmp_load_result_n40 < 0 || (size_t)hxc_l_tmp_load_result_n40 >= 49152)
          {
            abort();
          }
          uint8_t hxc_l_tmp_collection_index_load_result_n41 = (*hxc_l_tmp_load_result_n39).hxc_faceY0[(size_t)hxc_l_tmp_load_result_n40];
          hxc_l_tmp_conditional_result_n24 = (int32_t)hxc_l_tmp_collection_index_load_result_n41;
        }
        else
        {
          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n42 = hxc_l_this_hb33e315d43da;
          if (hxc_l_tmp_load_result_n42 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n43 = hxc_l_index;
          if (hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n43, 49152) < 0 || (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n43, 49152) >= 49152)
          {
            abort();
          }
          uint8_t hxc_l_tmp_collection_index_load_result_n44 = (*hxc_l_tmp_load_result_n42).hxc_faceY1[(size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n43, 49152)];
          hxc_l_tmp_conditional_result_n24 = (int32_t)hxc_l_tmp_collection_index_load_result_n44;
        }
        int32_t hxc_l_y = hxc_l_tmp_conditional_result_n24;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n46 = &(*hxc_l_self).hxc_cache;
        struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_this_hcf707273f095 = hxc_l_tmp_owned_class_field_address_n46;
        int32_t hxc_l_tmp_load_result_n47 = hxc_l_index;
        int32_t hxc_l_tmp_conditional_result_n27 = 0;
        if (hxc_l_tmp_load_result_n47 < 49152)
        {
          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n48 = hxc_l_this_hcf707273f095;
          if (hxc_l_tmp_load_result_n48 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n49 = hxc_l_index;
          if (hxc_l_tmp_load_result_n49 < 0 || (size_t)hxc_l_tmp_load_result_n49 >= 49152)
          {
            abort();
          }
          uint8_t hxc_l_tmp_collection_index_load_result_n50 = (*hxc_l_tmp_load_result_n48).hxc_faceZ0[(size_t)hxc_l_tmp_load_result_n49];
          hxc_l_tmp_conditional_result_n27 = (int32_t)hxc_l_tmp_collection_index_load_result_n50;
        }
        else
        {
          struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_load_result_n51 = hxc_l_this_hcf707273f095;
          if (hxc_l_tmp_load_result_n51 == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n52 = hxc_l_index;
          if (hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n52, 49152) < 0 || (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n52, 49152) >= 49152)
          {
            abort();
          }
          uint8_t hxc_l_tmp_collection_index_load_result_n53 = (*hxc_l_tmp_load_result_n51).hxc_faceZ1[(size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n52, 49152)];
          hxc_l_tmp_conditional_result_n27 = (int32_t)hxc_l_tmp_collection_index_load_result_n53;
        }
        int32_t hxc_l_z = hxc_l_tmp_conditional_result_n27;
        enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n55 = hxc_l_kind;
        enum hxc_caxecraft_app_VoxelFace hxc_l_tmp_call_result_n57 = hxc_caxecraft_app_TerrainChunkLayout_unpackFace(hxc_l_packed);
        int32_t hxc_l_tmp_load_result_n58 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n59 = hxc_l_y;
        int32_t hxc_l_tmp_load_result_n60 = hxc_l_z;
        double hxc_l_tmp_load_result_n61 = hxc_l_halfPixel;
        int32_t hxc_l_tmp_load_result_n62 = hxc_l_x;
        int32_t hxc_l_tmp_call_result_n64 = hxc_caxecraft_app_TerrainRenderer_squareDistance(hxc_l_tmp_load_result_n62, hxc_l_z, hxc_l_playerCellX, hxc_l_playerCellZ);
        hxc_caxecraft_app_TerrainRenderer_emitFace(hxc_l_tmp_load_result_n55, hxc_l_tmp_call_result_n57, hxc_l_tmp_load_result_n58, hxc_l_tmp_load_result_n59, hxc_l_tmp_load_result_n60, hxc_l_tmp_load_result_n61, hxc_l_tmp_call_result_n64);
        hxc_l_faces = hxc_i32_add_wrapping(hxc_l_faces, 1);
      }
      hxc_l_offset = hxc_i32_add_wrapping(hxc_l_offset, 1);
    }
    hxc_l_chunk = hxc_i32_add_wrapping(hxc_l_chunk, 1);
  }
  rlEnd();
  rlSetTexture((uint32_t)0);
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_tmp_owned_class_field_address_n68 = &(*hxc_l_self).hxc_cache;
  if (hxc_l_tmp_owned_class_field_address_n68 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_instance_call_result_n69 = hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(hxc_l_tmp_owned_class_field_address_n68, hxc_l_sheet);
  int32_t hxc_l_tmp_record_field_visible_n28 = hxc_l_tmp_instance_call_result_n69;
  int32_t hxc_l_tmp_record_field_faces_n29 = hxc_l_faces;
  int32_t hxc_l_tmp_load_result_n71 = hxc_l_faces;
  int32_t hxc_l_tmp_conditional_result_n30 = 0;
  if (hxc_l_tmp_load_result_n71 > 0)
  {
    hxc_l_tmp_conditional_result_n30 = 1;
  }
  else
  {
    hxc_l_tmp_conditional_result_n30 = 0;
  }
  int32_t hxc_l_tmp_conditional_load_result_n72 = hxc_l_tmp_conditional_result_n30;
  int32_t hxc_l_tmp_record_field_faces_load_result_n73 = hxc_l_tmp_record_field_faces_n29;
  return (struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters){ .hxc_drawCalls = hxc_l_tmp_conditional_load_result_n72, .hxc_faces = hxc_l_tmp_record_field_faces_load_result_n73, .hxc_visible = hxc_l_tmp_record_field_visible_n28 };
}
