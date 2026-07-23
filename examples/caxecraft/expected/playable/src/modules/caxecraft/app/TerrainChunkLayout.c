#include "hxc/program.h"

int32_t hxc_caxecraft_app_TerrainChunkLayout_chunkFor(struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  bool hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_contains(hxc_coord);
  if (!!hxc_tmp_call_result_n0)
  {
    return hxc_i32_add_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_coord.hxc_x, (double)8)), hxc_i32_multiply_wrapping(4, hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_coord.hxc_z, (double)8))));
  }
  return -1;
}

int32_t hxc_caxecraft_app_TerrainChunkLayout_chunkOriginX(int32_t hxc_chunk)
{
  bool hxc_tmp_short_circuit_result_n1 = hxc_chunk < 0;
  if (!(hxc_chunk < 0))
  {
    hxc_tmp_short_circuit_result_n1 = hxc_chunk >= 16;
  }
  if (!hxc_tmp_short_circuit_result_n1)
  {
    return hxc_i32_multiply_wrapping(hxc_i32_modulo_zero_safe(hxc_chunk, 4), 8);
  }
  return -1;
}

int32_t hxc_caxecraft_app_TerrainChunkLayout_chunkOriginZ(int32_t hxc_chunk)
{
  bool hxc_tmp_short_circuit_result_n1 = hxc_chunk < 0;
  if (!(hxc_chunk < 0))
  {
    hxc_tmp_short_circuit_result_n1 = hxc_chunk >= 16;
  }
  if (!hxc_tmp_short_circuit_result_n1)
  {
    return hxc_i32_multiply_wrapping(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_chunk, (double)4)), 8);
  }
  return -1;
}

int32_t hxc_caxecraft_app_TerrainChunkLayout_faceCode(enum hxc_caxecraft_app_VoxelFace hxc_face)
{
  switch (hxc_face) {
    case hxc_caxecraft_app_VoxelFace_Top:
      {
        break;
      }
    case hxc_caxecraft_app_VoxelFace_Bottom:
      {
        return 1;
      }
    case hxc_caxecraft_app_VoxelFace_North:
      {
        return 2;
      }
    case hxc_caxecraft_app_VoxelFace_South:
      {
        return 3;
      }
    case hxc_caxecraft_app_VoxelFace_East:
      {
        return 4;
      }
    case hxc_caxecraft_app_VoxelFace_West:
      {
        return 5;
      }
  }
  return 0;
}

int32_t hxc_caxecraft_app_TerrainChunkLayout_facePartitionStart(int32_t hxc_chunk)
{
  return hxc_i32_multiply_wrapping(hxc_chunk, 3072);
}

int32_t hxc_caxecraft_app_TerrainChunkLayout_packFace(enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_app_VoxelFace hxc_face)
{
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_kindCode(hxc_kind);
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_app_TerrainChunkLayout_faceCode(hxc_face);
  return hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_tmp_call_result_n0, 6), hxc_tmp_call_result_n1);
}

enum hxc_caxecraft_app_VoxelFace hxc_caxecraft_app_TerrainChunkLayout_unpackFace(int32_t hxc_packed)
{
  switch (hxc_i32_modulo_zero_safe(hxc_packed, 6)) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return hxc_caxecraft_app_VoxelFace_Bottom;
      }
    case 2:
      {
        return hxc_caxecraft_app_VoxelFace_North;
      }
    case 3:
      {
        return hxc_caxecraft_app_VoxelFace_South;
      }
    case 4:
      {
        return hxc_caxecraft_app_VoxelFace_East;
      }
    default:
      {
        return hxc_caxecraft_app_VoxelFace_West;
      }
  }
  return hxc_caxecraft_app_VoxelFace_Top;
}

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_app_TerrainChunkLayout_unpackKind(int32_t hxc_packed)
{
  enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_kindFromCode(hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_packed, (double)6)));
  return hxc_tmp_call_result_n0;
}
