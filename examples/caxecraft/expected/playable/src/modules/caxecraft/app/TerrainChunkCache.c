#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_TerrainChunkCache(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_l_self);
  return;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self, const uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_chunk, int32_t hxc_l_count, enum hxc_caxecraft_domain_BlockKind hxc_l_kind, enum hxc_caxecraft_app_VoxelFace hxc_l_face, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z, int32_t hxc_l_neighborX, int32_t hxc_l_neighborY, int32_t hxc_l_neighborZ)
{
  int32_t hxc_l_index = { 0 };
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n12 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n12;
  struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(hxc_l_neighborX, hxc_l_neighborY, hxc_l_neighborZ);
  enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n1 = hxc_caxecraft_domain_WorldRead_query(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n0);
  bool hxc_l_tmp_call_result_n2 = hxc_caxecraft_domain_World_isSolid(hxc_l_tmp_call_result_n1);
  if (hxc_l_tmp_call_result_n2)
  {
    return hxc_l_count;
  }
  if (!(hxc_l_count >= 3072))
  {
    int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_app_TerrainChunkLayout_facePartitionStart(hxc_l_chunk);
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_tmp_call_result_n3, hxc_l_count);
    if (hxc_l_self == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_index;
    if (hxc_l_tmp_load_result_n4 < 0 || (size_t)hxc_l_tmp_load_result_n4 >= 49152)
    {
      abort();
    }
    (*hxc_l_self).hxc_faceX[(size_t)hxc_l_tmp_load_result_n4] = (uint8_t)hxc_l_x;
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_index;
    if (hxc_l_tmp_load_result_n5 < 0 || (size_t)hxc_l_tmp_load_result_n5 >= 49152)
    {
      abort();
    }
    (*hxc_l_self).hxc_faceY[(size_t)hxc_l_tmp_load_result_n5] = (uint8_t)hxc_l_y;
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_index;
    if (hxc_l_tmp_load_result_n6 < 0 || (size_t)hxc_l_tmp_load_result_n6 >= 49152)
    {
      abort();
    }
    (*hxc_l_self).hxc_faceZ[(size_t)hxc_l_tmp_load_result_n6] = (uint8_t)hxc_l_z;
    int32_t hxc_l_tmp_load_result_n7 = hxc_l_index;
    if (hxc_l_tmp_load_result_n7 < 0 || (size_t)hxc_l_tmp_load_result_n7 >= 49152)
    {
      abort();
    }
    int32_t hxc_l_tmp_call_result_n8 = hxc_caxecraft_app_TerrainChunkLayout_packFace(hxc_l_kind, hxc_l_face);
    (*hxc_l_self).hxc_packedFaces[(size_t)hxc_l_tmp_load_result_n7] = (uint8_t)hxc_l_tmp_call_result_n8;
    return hxc_i32_add_wrapping(hxc_l_count, 1);
  }
  return -1;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_chunkFaceCount(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self, int32_t hxc_l_chunk)
{
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_chunk < 0;
  if (!(hxc_l_chunk < 0))
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_chunk >= 16;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    if (hxc_l_chunk < 0 || (size_t)hxc_l_chunk >= 16)
    {
      abort();
    }
    int32_t hxc_l_tmp_collection_index_load_result_n1 = (*hxc_l_self).hxc_faceCounts[(size_t)hxc_l_chunk];
    return hxc_l_tmp_collection_index_load_result_n1;
  }
  return 0;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_invalidate(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_TerrainChunkLayout_chunkFor(hxc_l_coord);
  int32_t hxc_l_owner = hxc_l_tmp_call_result_n0;
  if (hxc_l_owner < 0)
  {
    return 0;
  }
  int32_t hxc_l_changed = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_owner;
  if (hxc_l_tmp_load_result_n2 < 0 || (size_t)hxc_l_tmp_load_result_n2 >= 16)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n3 = (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n2];
  if ((int32_t)hxc_l_tmp_collection_index_load_result_n3 != 0)
  {
    hxc_l_changed = 0;
  }
  else
  {
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_owner;
    if (hxc_l_tmp_load_result_n4 < 0 || (size_t)hxc_l_tmp_load_result_n4 >= 16)
    {
      abort();
    }
    (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n4] = (uint8_t)1;
    hxc_l_changed = 1;
  }
  int32_t hxc_l_chunkX = hxc_i32_modulo_zero_safe(hxc_l_owner, 4);
  int32_t hxc_l_chunkZ = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_owner, (double)4));
  int32_t hxc_l_localX = hxc_i32_modulo_zero_safe(hxc_l_coord.hxc_x, 8);
  int32_t hxc_l_localZ = hxc_i32_modulo_zero_safe(hxc_l_coord.hxc_z, 8);
  int32_t hxc_l_tmp_load_result_n9 = hxc_l_localX;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_load_result_n9 == 0;
  if (hxc_l_tmp_load_result_n9 == 0)
  {
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_chunkX > 0;
  }
  if (hxc_l_tmp_short_circuit_result_n8)
  {
    int32_t hxc_l_chunk_hbdc0f19f7d1e = hxc_i32_subtract_wrapping(hxc_l_owner, 1);
    int32_t hxc_l_changed1_hb5c7351c1ca8 = 0;
    int32_t hxc_l_tmp_load_result_n13 = hxc_l_chunk_hbdc0f19f7d1e;
    if (hxc_l_tmp_load_result_n13 < 0 || (size_t)hxc_l_tmp_load_result_n13 >= 16)
    {
      abort();
    }
    uint8_t hxc_l_tmp_collection_index_load_result_n14 = (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n13];
    if ((int32_t)hxc_l_tmp_collection_index_load_result_n14 != 0)
    {
      hxc_l_changed1_hb5c7351c1ca8 = 0;
    }
    else
    {
      int32_t hxc_l_tmp_load_result_n15 = hxc_l_chunk_hbdc0f19f7d1e;
      if (hxc_l_tmp_load_result_n15 < 0 || (size_t)hxc_l_tmp_load_result_n15 >= 16)
      {
        abort();
      }
      (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n15] = (uint8_t)1;
      hxc_l_changed1_hb5c7351c1ca8 = 1;
    }
    int32_t hxc_l_tmp_compound_load_result_n16 = hxc_l_changed;
    hxc_l_changed = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n16, hxc_l_changed1_hb5c7351c1ca8);
  }
  int32_t hxc_l_tmp_load_result_n18 = hxc_l_localX;
  bool hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_load_result_n18 == 7;
  if (hxc_l_tmp_load_result_n18 == 7)
  {
    hxc_l_tmp_short_circuit_result_n11 = hxc_i32_add_wrapping(hxc_l_chunkX, 1) < 4;
  }
  if (hxc_l_tmp_short_circuit_result_n11)
  {
    int32_t hxc_l_chunk_hdfbbd5c546d5 = hxc_i32_add_wrapping(hxc_l_owner, 1);
    int32_t hxc_l_changed1_hcaee589737bb = 0;
    int32_t hxc_l_tmp_load_result_n22 = hxc_l_chunk_hdfbbd5c546d5;
    if (hxc_l_tmp_load_result_n22 < 0 || (size_t)hxc_l_tmp_load_result_n22 >= 16)
    {
      abort();
    }
    uint8_t hxc_l_tmp_collection_index_load_result_n23 = (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n22];
    if ((int32_t)hxc_l_tmp_collection_index_load_result_n23 != 0)
    {
      hxc_l_changed1_hcaee589737bb = 0;
    }
    else
    {
      int32_t hxc_l_tmp_load_result_n24 = hxc_l_chunk_hdfbbd5c546d5;
      if (hxc_l_tmp_load_result_n24 < 0 || (size_t)hxc_l_tmp_load_result_n24 >= 16)
      {
        abort();
      }
      (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n24] = (uint8_t)1;
      hxc_l_changed1_hcaee589737bb = 1;
    }
    int32_t hxc_l_tmp_compound_load_result_n25 = hxc_l_changed;
    hxc_l_changed = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n25, hxc_l_changed1_hcaee589737bb);
  }
  int32_t hxc_l_tmp_load_result_n27 = hxc_l_localZ;
  bool hxc_l_tmp_short_circuit_result_n14 = hxc_l_tmp_load_result_n27 == 0;
  if (hxc_l_tmp_load_result_n27 == 0)
  {
    hxc_l_tmp_short_circuit_result_n14 = hxc_l_chunkZ > 0;
  }
  if (hxc_l_tmp_short_circuit_result_n14)
  {
    int32_t hxc_l_chunk_hfca213983f72 = hxc_i32_subtract_wrapping(hxc_l_owner, 4);
    int32_t hxc_l_changed1_he6a99094f331 = 0;
    int32_t hxc_l_tmp_load_result_n31 = hxc_l_chunk_hfca213983f72;
    if (hxc_l_tmp_load_result_n31 < 0 || (size_t)hxc_l_tmp_load_result_n31 >= 16)
    {
      abort();
    }
    uint8_t hxc_l_tmp_collection_index_load_result_n32 = (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n31];
    if ((int32_t)hxc_l_tmp_collection_index_load_result_n32 != 0)
    {
      hxc_l_changed1_he6a99094f331 = 0;
    }
    else
    {
      int32_t hxc_l_tmp_load_result_n33 = hxc_l_chunk_hfca213983f72;
      if (hxc_l_tmp_load_result_n33 < 0 || (size_t)hxc_l_tmp_load_result_n33 >= 16)
      {
        abort();
      }
      (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n33] = (uint8_t)1;
      hxc_l_changed1_he6a99094f331 = 1;
    }
    int32_t hxc_l_tmp_compound_load_result_n34 = hxc_l_changed;
    hxc_l_changed = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n34, hxc_l_changed1_he6a99094f331);
  }
  int32_t hxc_l_tmp_load_result_n36 = hxc_l_localZ;
  bool hxc_l_tmp_short_circuit_result_n17 = hxc_l_tmp_load_result_n36 == 7;
  if (hxc_l_tmp_load_result_n36 == 7)
  {
    hxc_l_tmp_short_circuit_result_n17 = hxc_i32_add_wrapping(hxc_l_chunkZ, 1) < 4;
  }
  if (hxc_l_tmp_short_circuit_result_n17)
  {
    int32_t hxc_l_chunk_h4d4772237513 = hxc_i32_add_wrapping(hxc_l_owner, 4);
    int32_t hxc_l_changed1_h04d5efdaa156 = 0;
    int32_t hxc_l_tmp_load_result_n40 = hxc_l_chunk_h4d4772237513;
    if (hxc_l_tmp_load_result_n40 < 0 || (size_t)hxc_l_tmp_load_result_n40 >= 16)
    {
      abort();
    }
    uint8_t hxc_l_tmp_collection_index_load_result_n41 = (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n40];
    if ((int32_t)hxc_l_tmp_collection_index_load_result_n41 != 0)
    {
      hxc_l_changed1_h04d5efdaa156 = 0;
    }
    else
    {
      int32_t hxc_l_tmp_load_result_n42 = hxc_l_chunk_h4d4772237513;
      if (hxc_l_tmp_load_result_n42 < 0 || (size_t)hxc_l_tmp_load_result_n42 >= 16)
      {
        abort();
      }
      (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n42] = (uint8_t)1;
      hxc_l_changed1_h04d5efdaa156 = 1;
    }
    int32_t hxc_l_tmp_compound_load_result_n43 = hxc_l_changed;
    hxc_l_changed = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n43, hxc_l_changed1_h04d5efdaa156);
  }
  return hxc_l_changed;
}

void hxc_caxecraft_app_TerrainChunkCache_invalidateAll(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self)
{
  int32_t hxc_l_chunk = 0;
  while (1)
  {
    if (!(hxc_l_chunk < 16))
    {
      break;
    }
    if (hxc_l_self == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_chunk;
    if (hxc_l_tmp_load_result_n1 < 0 || (size_t)hxc_l_tmp_load_result_n1 >= 16)
    {
      abort();
    }
    (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n1] = (uint8_t)1;
    hxc_l_chunk = hxc_i32_add_wrapping(hxc_l_chunk, 1);
  }
  return;
}

bool hxc_caxecraft_app_TerrainChunkCache_invalidateBrokenChunk(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self, int32_t hxc_l_chunk)
{
  if (hxc_l_self == NULL)
  {
    abort();
  }
  if (hxc_l_chunk < 0 || (size_t)hxc_l_chunk >= 16)
  {
    abort();
  }
  (*hxc_l_self).hxc_faceCounts[(size_t)hxc_l_chunk] = 0;
  if (hxc_l_chunk < 0 || (size_t)hxc_l_chunk >= 16)
  {
    abort();
  }
  (*hxc_l_self).hxc_baseVisible[(size_t)hxc_l_chunk] = 0;
  if (hxc_l_chunk < 0 || (size_t)hxc_l_chunk >= 16)
  {
    abort();
  }
  (*hxc_l_self).hxc_adventureVisible[(size_t)hxc_l_chunk] = 0;
  return false;
}

struct hxc_caxecraft_app_TerrainChunkPreparation hxc_caxecraft_app_TerrainChunkCache_prepare(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self, const uint8_t *hxc_l_cells, size_t hxc_l_length)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  int32_t hxc_l_rebuilt = 0;
  bool hxc_l_valid = true;
  int32_t hxc_l_chunk = 0;
  while (1)
  {
    if (!(hxc_l_chunk < 16))
    {
      break;
    }
    if (hxc_l_self == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_chunk;
    if (hxc_l_tmp_load_result_n1 < 0 || (size_t)hxc_l_tmp_load_result_n1 >= 16)
    {
      abort();
    }
    uint8_t hxc_l_tmp_collection_index_load_result_n2 = (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n1];
    if ((int32_t)hxc_l_tmp_collection_index_load_result_n2 != 0)
    {
      bool hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_app_TerrainChunkCache_rebuild(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_chunk);
      if (!hxc_l_tmp_instance_call_result_n4)
      {
        hxc_l_valid = false;
      }
      int32_t hxc_l_tmp_load_result_n5 = hxc_l_chunk;
      if (hxc_l_tmp_load_result_n5 < 0 || (size_t)hxc_l_tmp_load_result_n5 >= 16)
      {
        abort();
      }
      (*hxc_l_self).hxc_dirty[(size_t)hxc_l_tmp_load_result_n5] = (uint8_t)0;
      hxc_l_rebuilt = hxc_i32_add_wrapping(hxc_l_rebuilt, 1);
    }
    hxc_l_chunk = hxc_i32_add_wrapping(hxc_l_chunk, 1);
  }
  int32_t hxc_l_tmp_load_result_n8 = hxc_l_rebuilt;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_instance_call_result_n9 = hxc_caxecraft_app_TerrainChunkCache_totalFaces(hxc_l_self);
  int32_t hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(hxc_l_self, hxc_caxecraft_app_TerrainSheet_Base);
  int32_t hxc_l_tmp_instance_call_result_n13 = hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(hxc_l_self, hxc_caxecraft_app_TerrainSheet_Adventure);
  return (struct hxc_caxecraft_app_TerrainChunkPreparation){ .hxc_faces = hxc_l_tmp_instance_call_result_n9, .hxc_rebuiltChunks = hxc_l_tmp_load_result_n8, .hxc_valid = hxc_l_valid, .hxc_visibleBlocks = hxc_i32_add_wrapping(hxc_l_tmp_instance_call_result_n11, hxc_l_tmp_instance_call_result_n13) };
}

bool hxc_caxecraft_app_TerrainChunkCache_rebuild(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self, const uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_chunk)
{
  const uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n3;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_app_TerrainChunkLayout_chunkOriginX(hxc_l_chunk);
  int32_t hxc_l_originX = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_app_TerrainChunkLayout_chunkOriginZ(hxc_l_chunk);
  int32_t hxc_l_originZ = hxc_l_tmp_call_result_n1;
  int32_t hxc_l_count = 0;
  int32_t hxc_l_visibleBase = 0;
  int32_t hxc_l_visibleAdventure = 0;
  bool hxc_l_valid = true;
  int32_t hxc_l_z = hxc_l_originZ;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_z;
    if (!(hxc_l_tmp_load_result_n3 < hxc_i32_add_wrapping(hxc_l_originZ, 8)))
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
      int32_t hxc_l_x = hxc_l_originX;
      while (1)
      {
        int32_t hxc_l_tmp_load_result_n7 = hxc_l_x;
        if (!(hxc_l_tmp_load_result_n7 < hxc_i32_add_wrapping(hxc_l_originX, 8)))
        {
          break;
        }
        int32_t hxc_l_tmp_load_result_n9 = hxc_l_x;
        int32_t hxc_l_tmp_load_result_n10 = hxc_l_y;
        struct hxc_caxecraft_domain_BlockCoord hxc_l_tmp_call_result_n12 = hxc_caxecraft_domain_World_coord(hxc_l_tmp_load_result_n9, hxc_l_tmp_load_result_n10, hxc_l_z);
        enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n13 = hxc_caxecraft_domain_WorldRead_query(hxc_l_cells, hxc_l_length, hxc_l_tmp_call_result_n12);
        enum hxc_caxecraft_domain_BlockKind hxc_l_kind = hxc_l_tmp_call_result_n13;
        bool hxc_l_tmp_call_result_n15 = hxc_caxecraft_domain_World_isSolid(hxc_l_kind);
        if (hxc_l_tmp_call_result_n15)
        {
          bool hxc_l_blockVisible = false;
          if (hxc_l_self == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n16 = hxc_l_count;
          enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n17 = hxc_l_kind;
          int32_t hxc_l_tmp_load_result_n19 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n20 = hxc_l_y;
          int32_t hxc_l_tmp_load_result_n21 = hxc_l_z;
          int32_t hxc_l_tmp_load_result_n22 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n23 = hxc_l_y;
          int32_t hxc_l_tmp_instance_call_result_n25 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_chunk, hxc_l_tmp_load_result_n16, hxc_l_tmp_load_result_n17, hxc_caxecraft_app_VoxelFace_Top, hxc_l_tmp_load_result_n19, hxc_l_tmp_load_result_n20, hxc_l_tmp_load_result_n21, hxc_l_tmp_load_result_n22, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n23, 1), hxc_l_z);
          int32_t hxc_l_nextCount = hxc_l_tmp_instance_call_result_n25;
          if (hxc_l_nextCount < 0)
          {
            hxc_l_valid = false;
            hxc_l_nextCount = hxc_l_count;
          }
          int32_t hxc_l_tmp_load_result_n28 = hxc_l_nextCount;
          if (hxc_l_tmp_load_result_n28 > hxc_l_count)
          {
            hxc_l_blockVisible = true;
          }
          hxc_l_count = hxc_l_nextCount;
          int32_t hxc_l_tmp_load_result_n31 = hxc_l_count;
          enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n32 = hxc_l_kind;
          int32_t hxc_l_tmp_load_result_n34 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n35 = hxc_l_y;
          int32_t hxc_l_tmp_load_result_n36 = hxc_l_z;
          int32_t hxc_l_tmp_load_result_n37 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n38 = hxc_l_y;
          int32_t hxc_l_tmp_instance_call_result_n40 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_chunk, hxc_l_tmp_load_result_n31, hxc_l_tmp_load_result_n32, hxc_caxecraft_app_VoxelFace_Bottom, hxc_l_tmp_load_result_n34, hxc_l_tmp_load_result_n35, hxc_l_tmp_load_result_n36, hxc_l_tmp_load_result_n37, hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n38, 1), hxc_l_z);
          hxc_l_nextCount = hxc_l_tmp_instance_call_result_n40;
          if (hxc_l_nextCount < 0)
          {
            hxc_l_valid = false;
            hxc_l_nextCount = hxc_l_count;
          }
          int32_t hxc_l_tmp_load_result_n43 = hxc_l_nextCount;
          if (hxc_l_tmp_load_result_n43 > hxc_l_count)
          {
            hxc_l_blockVisible = true;
          }
          hxc_l_count = hxc_l_nextCount;
          int32_t hxc_l_tmp_load_result_n46 = hxc_l_count;
          enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n47 = hxc_l_kind;
          int32_t hxc_l_tmp_load_result_n49 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n50 = hxc_l_y;
          int32_t hxc_l_tmp_load_result_n51 = hxc_l_z;
          int32_t hxc_l_tmp_load_result_n52 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n53 = hxc_l_y;
          int32_t hxc_l_tmp_instance_call_result_n55 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_chunk, hxc_l_tmp_load_result_n46, hxc_l_tmp_load_result_n47, hxc_caxecraft_app_VoxelFace_North, hxc_l_tmp_load_result_n49, hxc_l_tmp_load_result_n50, hxc_l_tmp_load_result_n51, hxc_l_tmp_load_result_n52, hxc_l_tmp_load_result_n53, hxc_i32_subtract_wrapping(hxc_l_z, 1));
          hxc_l_nextCount = hxc_l_tmp_instance_call_result_n55;
          if (hxc_l_nextCount < 0)
          {
            hxc_l_valid = false;
            hxc_l_nextCount = hxc_l_count;
          }
          int32_t hxc_l_tmp_load_result_n58 = hxc_l_nextCount;
          if (hxc_l_tmp_load_result_n58 > hxc_l_count)
          {
            hxc_l_blockVisible = true;
          }
          hxc_l_count = hxc_l_nextCount;
          int32_t hxc_l_tmp_load_result_n61 = hxc_l_count;
          enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n62 = hxc_l_kind;
          int32_t hxc_l_tmp_load_result_n64 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n65 = hxc_l_y;
          int32_t hxc_l_tmp_load_result_n66 = hxc_l_z;
          int32_t hxc_l_tmp_load_result_n67 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n68 = hxc_l_y;
          int32_t hxc_l_tmp_instance_call_result_n70 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_chunk, hxc_l_tmp_load_result_n61, hxc_l_tmp_load_result_n62, hxc_caxecraft_app_VoxelFace_South, hxc_l_tmp_load_result_n64, hxc_l_tmp_load_result_n65, hxc_l_tmp_load_result_n66, hxc_l_tmp_load_result_n67, hxc_l_tmp_load_result_n68, hxc_i32_add_wrapping(hxc_l_z, 1));
          hxc_l_nextCount = hxc_l_tmp_instance_call_result_n70;
          if (hxc_l_nextCount < 0)
          {
            hxc_l_valid = false;
            hxc_l_nextCount = hxc_l_count;
          }
          int32_t hxc_l_tmp_load_result_n73 = hxc_l_nextCount;
          if (hxc_l_tmp_load_result_n73 > hxc_l_count)
          {
            hxc_l_blockVisible = true;
          }
          hxc_l_count = hxc_l_nextCount;
          int32_t hxc_l_tmp_load_result_n76 = hxc_l_count;
          enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n77 = hxc_l_kind;
          int32_t hxc_l_tmp_load_result_n79 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n80 = hxc_l_y;
          int32_t hxc_l_tmp_load_result_n81 = hxc_l_z;
          int32_t hxc_l_tmp_load_result_n82 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n83 = hxc_l_y;
          int32_t hxc_l_tmp_instance_call_result_n85 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_chunk, hxc_l_tmp_load_result_n76, hxc_l_tmp_load_result_n77, hxc_caxecraft_app_VoxelFace_East, hxc_l_tmp_load_result_n79, hxc_l_tmp_load_result_n80, hxc_l_tmp_load_result_n81, hxc_i32_add_wrapping(hxc_l_tmp_load_result_n82, 1), hxc_l_tmp_load_result_n83, hxc_l_z);
          hxc_l_nextCount = hxc_l_tmp_instance_call_result_n85;
          if (hxc_l_nextCount < 0)
          {
            hxc_l_valid = false;
            hxc_l_nextCount = hxc_l_count;
          }
          int32_t hxc_l_tmp_load_result_n88 = hxc_l_nextCount;
          if (hxc_l_tmp_load_result_n88 > hxc_l_count)
          {
            hxc_l_blockVisible = true;
          }
          hxc_l_count = hxc_l_nextCount;
          int32_t hxc_l_tmp_load_result_n91 = hxc_l_count;
          enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_load_result_n92 = hxc_l_kind;
          int32_t hxc_l_tmp_load_result_n94 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n95 = hxc_l_y;
          int32_t hxc_l_tmp_load_result_n96 = hxc_l_z;
          int32_t hxc_l_tmp_load_result_n97 = hxc_l_x;
          int32_t hxc_l_tmp_load_result_n98 = hxc_l_y;
          int32_t hxc_l_tmp_instance_call_result_n100 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_l_self, hxc_l_cells, hxc_l_length, hxc_l_chunk, hxc_l_tmp_load_result_n91, hxc_l_tmp_load_result_n92, hxc_caxecraft_app_VoxelFace_West, hxc_l_tmp_load_result_n94, hxc_l_tmp_load_result_n95, hxc_l_tmp_load_result_n96, hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n97, 1), hxc_l_tmp_load_result_n98, hxc_l_z);
          hxc_l_nextCount = hxc_l_tmp_instance_call_result_n100;
          if (hxc_l_nextCount < 0)
          {
            hxc_l_valid = false;
            hxc_l_nextCount = hxc_l_count;
          }
          int32_t hxc_l_tmp_load_result_n103 = hxc_l_nextCount;
          if (hxc_l_tmp_load_result_n103 > hxc_l_count)
          {
            hxc_l_blockVisible = true;
          }
          hxc_l_count = hxc_l_nextCount;
          if (hxc_l_blockVisible)
          {
            enum hxc_caxecraft_app_TerrainSheet hxc_l_tmp_call_result_n108 = hxc_caxecraft_app_TerrainAtlas_sheet(hxc_l_kind);
            switch (hxc_l_tmp_call_result_n108) {
              case hxc_caxecraft_app_TerrainSheet_Base:
                {
                  hxc_l_visibleBase = hxc_i32_add_wrapping(hxc_l_visibleBase, 1);
                  break;
                }
              case hxc_caxecraft_app_TerrainSheet_Adventure:
                {
                  hxc_l_visibleAdventure = hxc_i32_add_wrapping(hxc_l_visibleAdventure, 1);
                  break;
                }
              default:
                {
                  abort();
                }
            }
          }
        }
        hxc_l_x = hxc_i32_add_wrapping(hxc_l_x, 1);
      }
      hxc_l_y = hxc_i32_add_wrapping(hxc_l_y, 1);
    }
    hxc_l_z = hxc_i32_add_wrapping(hxc_l_z, 1);
  }
  if (!!hxc_l_valid)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    if (hxc_l_chunk < 0 || (size_t)hxc_l_chunk >= 16)
    {
      abort();
    }
    (*hxc_l_self).hxc_faceCounts[(size_t)hxc_l_chunk] = hxc_l_count;
    if (hxc_l_chunk < 0 || (size_t)hxc_l_chunk >= 16)
    {
      abort();
    }
    (*hxc_l_self).hxc_baseVisible[(size_t)hxc_l_chunk] = hxc_l_visibleBase;
    if (hxc_l_chunk < 0 || (size_t)hxc_l_chunk >= 16)
    {
      abort();
    }
    (*hxc_l_self).hxc_adventureVisible[(size_t)hxc_l_chunk] = hxc_l_visibleAdventure;
    return true;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n115 = hxc_caxecraft_app_TerrainChunkCache_invalidateBrokenChunk(hxc_l_self, hxc_l_chunk);
  return hxc_l_tmp_instance_call_result_n115;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_totalFaces(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self)
{
  int32_t hxc_l_total = 0;
  int32_t hxc_l_chunk = 0;
  while (1)
  {
    if (!(hxc_l_chunk < 16))
    {
      break;
    }
    int32_t hxc_l_tmp_compound_load_result_n1 = hxc_l_total;
    if (hxc_l_self == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_chunk;
    if (hxc_l_tmp_load_result_n2 < 0 || (size_t)hxc_l_tmp_load_result_n2 >= 16)
    {
      abort();
    }
    int32_t hxc_l_tmp_collection_index_load_result_n3 = (*hxc_l_self).hxc_faceCounts[(size_t)hxc_l_tmp_load_result_n2];
    hxc_l_total = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n1, hxc_l_tmp_collection_index_load_result_n3);
    hxc_l_chunk = hxc_i32_add_wrapping(hxc_l_chunk, 1);
  }
  return hxc_l_total;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(struct hxc_caxecraft_app_TerrainChunkCache *hxc_l_self, enum hxc_caxecraft_app_TerrainSheet hxc_l_sheet)
{
  int32_t hxc_l_total = 0;
  int32_t hxc_l_chunk = 0;
  while (1)
  {
    if (!(hxc_l_chunk < 16))
    {
      break;
    }
    switch (hxc_l_sheet) {
      case hxc_caxecraft_app_TerrainSheet_Base:
        {
          int32_t hxc_l_tmp_compound_load_result_n1 = hxc_l_total;
          if (hxc_l_self == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n2 = hxc_l_chunk;
          if (hxc_l_tmp_load_result_n2 < 0 || (size_t)hxc_l_tmp_load_result_n2 >= 16)
          {
            abort();
          }
          int32_t hxc_l_tmp_collection_index_load_result_n3 = (*hxc_l_self).hxc_baseVisible[(size_t)hxc_l_tmp_load_result_n2];
          hxc_l_total = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n1, hxc_l_tmp_collection_index_load_result_n3);
          break;
        }
      case hxc_caxecraft_app_TerrainSheet_Adventure:
        {
          int32_t hxc_l_tmp_compound_load_result_n4 = hxc_l_total;
          if (hxc_l_self == NULL)
          {
            abort();
          }
          int32_t hxc_l_tmp_load_result_n5 = hxc_l_chunk;
          if (hxc_l_tmp_load_result_n5 < 0 || (size_t)hxc_l_tmp_load_result_n5 >= 16)
          {
            abort();
          }
          int32_t hxc_l_tmp_collection_index_load_result_n6 = (*hxc_l_self).hxc_adventureVisible[(size_t)hxc_l_tmp_load_result_n5];
          hxc_l_total = hxc_i32_add_wrapping(hxc_l_tmp_compound_load_result_n4, hxc_l_tmp_collection_index_load_result_n6);
          break;
        }
      default:
        {
          abort();
        }
    }
    hxc_l_chunk = hxc_i32_add_wrapping(hxc_l_chunk, 1);
  }
  return hxc_l_total;
}
