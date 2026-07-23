#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_app_TerrainChunkCache(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  hxc_caxecraft_app_TerrainChunkCache_invalidateAll(hxc_self);
  return;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_chunk, int32_t hxc_count, enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_app_VoxelFace hxc_face, int32_t hxc_x, int32_t hxc_y, int32_t hxc_z, int32_t hxc_neighborX, int32_t hxc_neighborY, int32_t hxc_neighborZ)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n12 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n12;
  struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_coord(hxc_neighborX, hxc_neighborY, hxc_neighborZ);
  enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n1 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n0);
  bool hxc_tmp_call_result_n2 = hxc_caxecraft_domain_World_isSolid(hxc_tmp_call_result_n1);
  if (!hxc_tmp_call_result_n2)
  {
    if (!(hxc_count >= 3072))
    {
      int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_app_TerrainChunkLayout_facePartitionStart(hxc_chunk);
      int32_t hxc_index = hxc_i32_add_wrapping(hxc_tmp_call_result_n3, hxc_count);
      if (hxc_self == NULL)
      {
        abort();
      }
      int32_t hxc_tmp_load_result_n4 = hxc_index;
      if (hxc_tmp_load_result_n4 < 0 || (size_t)hxc_tmp_load_result_n4 >= 49152)
      {
        abort();
      }
      (*hxc_self).hxc_faceX[(size_t)hxc_tmp_load_result_n4] = (uint8_t)hxc_x;
      int32_t hxc_tmp_load_result_n5 = hxc_index;
      if (hxc_tmp_load_result_n5 < 0 || (size_t)hxc_tmp_load_result_n5 >= 49152)
      {
        abort();
      }
      (*hxc_self).hxc_faceY[(size_t)hxc_tmp_load_result_n5] = (uint8_t)hxc_y;
      int32_t hxc_tmp_load_result_n6 = hxc_index;
      if (hxc_tmp_load_result_n6 < 0 || (size_t)hxc_tmp_load_result_n6 >= 49152)
      {
        abort();
      }
      (*hxc_self).hxc_faceZ[(size_t)hxc_tmp_load_result_n6] = (uint8_t)hxc_z;
      int32_t hxc_tmp_load_result_n7 = hxc_index;
      if (hxc_tmp_load_result_n7 < 0 || (size_t)hxc_tmp_load_result_n7 >= 49152)
      {
        abort();
      }
      int32_t hxc_tmp_call_result_n8 = hxc_caxecraft_app_TerrainChunkLayout_packFace(hxc_kind, hxc_face);
      (*hxc_self).hxc_packedFaces[(size_t)hxc_tmp_load_result_n7] = (uint8_t)hxc_tmp_call_result_n8;
      return hxc_i32_add_wrapping(hxc_count, 1);
    }
    return -1;
  }
  return hxc_count;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_chunkFaceCount(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, int32_t hxc_chunk)
{
  bool hxc_tmp_short_circuit_result_n2 = hxc_chunk < 0;
  if (!(hxc_chunk < 0))
  {
    hxc_tmp_short_circuit_result_n2 = hxc_chunk >= 16;
  }
  if (!hxc_tmp_short_circuit_result_n2)
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    if (hxc_chunk < 0 || (size_t)hxc_chunk >= 16)
    {
      abort();
    }
    int32_t hxc_tmp_collection_index_load_result_n1 = (*hxc_self).hxc_faceCounts[(size_t)hxc_chunk];
    return hxc_tmp_collection_index_load_result_n1;
  }
  return 0;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_invalidate(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_app_TerrainChunkLayout_chunkFor(hxc_coord);
  int32_t hxc_owner = hxc_tmp_call_result_n0;
  if (!(hxc_owner < 0))
  {
    int32_t hxc_changed = 0;
    if (hxc_self == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_load_result_n2 = hxc_owner;
    if (hxc_tmp_load_result_n2 < 0 || (size_t)hxc_tmp_load_result_n2 >= 16)
    {
      abort();
    }
    uint8_t hxc_tmp_collection_index_load_result_n3 = (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n2];
    if ((int32_t)hxc_tmp_collection_index_load_result_n3 != 0)
    {
      hxc_changed = 0;
    }
    else
    {
      int32_t hxc_tmp_load_result_n4 = hxc_owner;
      if (hxc_tmp_load_result_n4 < 0 || (size_t)hxc_tmp_load_result_n4 >= 16)
      {
        abort();
      }
      (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n4] = (uint8_t)1;
      hxc_changed = 1;
    }
    int32_t hxc_chunkX = hxc_i32_modulo_zero_safe(hxc_owner, 4);
    int32_t hxc_chunkZ = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_owner, (double)4));
    int32_t hxc_localX = hxc_i32_modulo_zero_safe(hxc_coord.hxc_x, 8);
    int32_t hxc_localZ = hxc_i32_modulo_zero_safe(hxc_coord.hxc_z, 8);
    int32_t hxc_tmp_load_result_n9 = hxc_localX;
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_load_result_n9 == 0;
    if (hxc_tmp_load_result_n9 == 0)
    {
      hxc_tmp_short_circuit_result_n8 = hxc_chunkX > 0;
    }
    if (hxc_tmp_short_circuit_result_n8)
    {
      int32_t hxc_chunk_hbdc0f19f7d1e = hxc_i32_subtract_wrapping(hxc_owner, 1);
      int32_t hxc_changed1_hb5c7351c1ca8 = 0;
      int32_t hxc_tmp_load_result_n13 = hxc_chunk_hbdc0f19f7d1e;
      if (hxc_tmp_load_result_n13 < 0 || (size_t)hxc_tmp_load_result_n13 >= 16)
      {
        abort();
      }
      uint8_t hxc_tmp_collection_index_load_result_n14 = (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n13];
      if ((int32_t)hxc_tmp_collection_index_load_result_n14 != 0)
      {
        hxc_changed1_hb5c7351c1ca8 = 0;
      }
      else
      {
        int32_t hxc_tmp_load_result_n15 = hxc_chunk_hbdc0f19f7d1e;
        if (hxc_tmp_load_result_n15 < 0 || (size_t)hxc_tmp_load_result_n15 >= 16)
        {
          abort();
        }
        (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n15] = (uint8_t)1;
        hxc_changed1_hb5c7351c1ca8 = 1;
      }
      int32_t hxc_tmp_compound_load_result_n16 = hxc_changed;
      hxc_changed = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n16, hxc_changed1_hb5c7351c1ca8);
    }
    int32_t hxc_tmp_load_result_n18 = hxc_localX;
    bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_load_result_n18 == 7;
    if (hxc_tmp_load_result_n18 == 7)
    {
      hxc_tmp_short_circuit_result_n11 = hxc_i32_add_wrapping(hxc_chunkX, 1) < 4;
    }
    if (hxc_tmp_short_circuit_result_n11)
    {
      int32_t hxc_chunk_hdfbbd5c546d5 = hxc_i32_add_wrapping(hxc_owner, 1);
      int32_t hxc_changed1_hcaee589737bb = 0;
      int32_t hxc_tmp_load_result_n22 = hxc_chunk_hdfbbd5c546d5;
      if (hxc_tmp_load_result_n22 < 0 || (size_t)hxc_tmp_load_result_n22 >= 16)
      {
        abort();
      }
      uint8_t hxc_tmp_collection_index_load_result_n23 = (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n22];
      if ((int32_t)hxc_tmp_collection_index_load_result_n23 != 0)
      {
        hxc_changed1_hcaee589737bb = 0;
      }
      else
      {
        int32_t hxc_tmp_load_result_n24 = hxc_chunk_hdfbbd5c546d5;
        if (hxc_tmp_load_result_n24 < 0 || (size_t)hxc_tmp_load_result_n24 >= 16)
        {
          abort();
        }
        (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n24] = (uint8_t)1;
        hxc_changed1_hcaee589737bb = 1;
      }
      int32_t hxc_tmp_compound_load_result_n25 = hxc_changed;
      hxc_changed = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n25, hxc_changed1_hcaee589737bb);
    }
    int32_t hxc_tmp_load_result_n27 = hxc_localZ;
    bool hxc_tmp_short_circuit_result_n14 = hxc_tmp_load_result_n27 == 0;
    if (hxc_tmp_load_result_n27 == 0)
    {
      hxc_tmp_short_circuit_result_n14 = hxc_chunkZ > 0;
    }
    if (hxc_tmp_short_circuit_result_n14)
    {
      int32_t hxc_chunk_hfca213983f72 = hxc_i32_subtract_wrapping(hxc_owner, 4);
      int32_t hxc_changed1_he6a99094f331 = 0;
      int32_t hxc_tmp_load_result_n31 = hxc_chunk_hfca213983f72;
      if (hxc_tmp_load_result_n31 < 0 || (size_t)hxc_tmp_load_result_n31 >= 16)
      {
        abort();
      }
      uint8_t hxc_tmp_collection_index_load_result_n32 = (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n31];
      if ((int32_t)hxc_tmp_collection_index_load_result_n32 != 0)
      {
        hxc_changed1_he6a99094f331 = 0;
      }
      else
      {
        int32_t hxc_tmp_load_result_n33 = hxc_chunk_hfca213983f72;
        if (hxc_tmp_load_result_n33 < 0 || (size_t)hxc_tmp_load_result_n33 >= 16)
        {
          abort();
        }
        (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n33] = (uint8_t)1;
        hxc_changed1_he6a99094f331 = 1;
      }
      int32_t hxc_tmp_compound_load_result_n34 = hxc_changed;
      hxc_changed = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n34, hxc_changed1_he6a99094f331);
    }
    int32_t hxc_tmp_load_result_n36 = hxc_localZ;
    bool hxc_tmp_short_circuit_result_n17 = hxc_tmp_load_result_n36 == 7;
    if (hxc_tmp_load_result_n36 == 7)
    {
      hxc_tmp_short_circuit_result_n17 = hxc_i32_add_wrapping(hxc_chunkZ, 1) < 4;
    }
    if (hxc_tmp_short_circuit_result_n17)
    {
      int32_t hxc_chunk_h4d4772237513 = hxc_i32_add_wrapping(hxc_owner, 4);
      int32_t hxc_changed1_h04d5efdaa156 = 0;
      int32_t hxc_tmp_load_result_n40 = hxc_chunk_h4d4772237513;
      if (hxc_tmp_load_result_n40 < 0 || (size_t)hxc_tmp_load_result_n40 >= 16)
      {
        abort();
      }
      uint8_t hxc_tmp_collection_index_load_result_n41 = (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n40];
      if ((int32_t)hxc_tmp_collection_index_load_result_n41 != 0)
      {
        hxc_changed1_h04d5efdaa156 = 0;
      }
      else
      {
        int32_t hxc_tmp_load_result_n42 = hxc_chunk_h4d4772237513;
        if (hxc_tmp_load_result_n42 < 0 || (size_t)hxc_tmp_load_result_n42 >= 16)
        {
          abort();
        }
        (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n42] = (uint8_t)1;
        hxc_changed1_h04d5efdaa156 = 1;
      }
      int32_t hxc_tmp_compound_load_result_n43 = hxc_changed;
      hxc_changed = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n43, hxc_changed1_h04d5efdaa156);
    }
    return hxc_changed;
  }
  return 0;
}

void hxc_caxecraft_app_TerrainChunkCache_invalidateAll(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self)
{
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
    int32_t hxc_tmp_load_result_n1 = hxc_chunk;
    if (hxc_tmp_load_result_n1 < 0 || (size_t)hxc_tmp_load_result_n1 >= 16)
    {
      abort();
    }
    (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n1] = (uint8_t)1;
    hxc_chunk = hxc_i32_add_wrapping(hxc_chunk, 1);
  }
  return;
}

bool hxc_caxecraft_app_TerrainChunkCache_invalidateBrokenChunk(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, int32_t hxc_chunk)
{
  if (hxc_self == NULL)
  {
    abort();
  }
  if (hxc_chunk < 0 || (size_t)hxc_chunk >= 16)
  {
    abort();
  }
  (*hxc_self).hxc_faceCounts[(size_t)hxc_chunk] = 0;
  if (hxc_chunk < 0 || (size_t)hxc_chunk >= 16)
  {
    abort();
  }
  (*hxc_self).hxc_baseVisible[(size_t)hxc_chunk] = 0;
  if (hxc_chunk < 0 || (size_t)hxc_chunk >= 16)
  {
    abort();
  }
  (*hxc_self).hxc_adventureVisible[(size_t)hxc_chunk] = 0;
  return false;
}

struct hxc_caxecraft_app_TerrainChunkPreparation hxc_caxecraft_app_TerrainChunkCache_prepare(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, uint8_t *hxc_cells, size_t hxc_length)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  int32_t hxc_rebuilt = 0;
  bool hxc_valid = true;
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
    int32_t hxc_tmp_load_result_n1 = hxc_chunk;
    if (hxc_tmp_load_result_n1 < 0 || (size_t)hxc_tmp_load_result_n1 >= 16)
    {
      abort();
    }
    uint8_t hxc_tmp_collection_index_load_result_n2 = (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n1];
    if ((int32_t)hxc_tmp_collection_index_load_result_n2 != 0)
    {
      bool hxc_tmp_instance_call_result_n4 = hxc_caxecraft_app_TerrainChunkCache_rebuild(hxc_self, hxc_cells, hxc_length, hxc_chunk);
      if (!hxc_tmp_instance_call_result_n4)
      {
        hxc_valid = false;
      }
      int32_t hxc_tmp_load_result_n5 = hxc_chunk;
      if (hxc_tmp_load_result_n5 < 0 || (size_t)hxc_tmp_load_result_n5 >= 16)
      {
        abort();
      }
      (*hxc_self).hxc_dirty[(size_t)hxc_tmp_load_result_n5] = (uint8_t)0;
      hxc_rebuilt = hxc_i32_add_wrapping(hxc_rebuilt, 1);
    }
    hxc_chunk = hxc_i32_add_wrapping(hxc_chunk, 1);
  }
  int32_t hxc_tmp_load_result_n8 = hxc_rebuilt;
  if (hxc_self == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_instance_call_result_n9 = hxc_caxecraft_app_TerrainChunkCache_totalFaces(hxc_self);
  int32_t hxc_tmp_instance_call_result_n11 = hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(hxc_self, hxc_caxecraft_app_TerrainSheet_Base);
  int32_t hxc_tmp_instance_call_result_n13 = hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(hxc_self, hxc_caxecraft_app_TerrainSheet_Adventure);
  return (struct hxc_caxecraft_app_TerrainChunkPreparation){ .hxc_faces = hxc_tmp_instance_call_result_n9, .hxc_rebuiltChunks = hxc_tmp_load_result_n8, .hxc_valid = hxc_valid, .hxc_visibleBlocks = hxc_i32_add_wrapping(hxc_tmp_instance_call_result_n11, hxc_tmp_instance_call_result_n13) };
}

bool hxc_caxecraft_app_TerrainChunkCache_rebuild(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_chunk)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n3;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_app_TerrainChunkLayout_chunkOriginX(hxc_chunk);
  int32_t hxc_originX = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_app_TerrainChunkLayout_chunkOriginZ(hxc_chunk);
  int32_t hxc_originZ = hxc_tmp_call_result_n1;
  int32_t hxc_count = 0;
  int32_t hxc_visibleBase = 0;
  int32_t hxc_visibleAdventure = 0;
  bool hxc_valid = true;
  int32_t hxc_z = hxc_originZ;
  while (1)
  {
    int32_t hxc_tmp_load_result_n3 = hxc_z;
    if (!(hxc_tmp_load_result_n3 < hxc_i32_add_wrapping(hxc_originZ, 8)))
    {
      break;
    }
    int32_t hxc_y = 0;
    while (1)
    {
      if (!(hxc_y < 16))
      {
        break;
      }
      int32_t hxc_x = hxc_originX;
      while (1)
      {
        int32_t hxc_tmp_load_result_n7 = hxc_x;
        if (!(hxc_tmp_load_result_n7 < hxc_i32_add_wrapping(hxc_originX, 8)))
        {
          break;
        }
        int32_t hxc_tmp_load_result_n9 = hxc_x;
        int32_t hxc_tmp_load_result_n10 = hxc_y;
        struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n12 = hxc_caxecraft_domain_World_coord(hxc_tmp_load_result_n9, hxc_tmp_load_result_n10, hxc_z);
        enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n13 = hxc_caxecraft_domain_World_query(hxc_cells, hxc_length, hxc_tmp_call_result_n12);
        enum hxc_caxecraft_domain_BlockKind hxc_kind = hxc_tmp_call_result_n13;
        bool hxc_tmp_call_result_n15 = hxc_caxecraft_domain_World_isSolid(hxc_kind);
        if (hxc_tmp_call_result_n15)
        {
          bool hxc_blockVisible = false;
          if (hxc_self == NULL)
          {
            abort();
          }
          int32_t hxc_tmp_load_result_n16 = hxc_count;
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n17 = hxc_kind;
          int32_t hxc_tmp_load_result_n19 = hxc_x;
          int32_t hxc_tmp_load_result_n20 = hxc_y;
          int32_t hxc_tmp_load_result_n21 = hxc_z;
          int32_t hxc_tmp_load_result_n22 = hxc_x;
          int32_t hxc_tmp_load_result_n23 = hxc_y;
          int32_t hxc_tmp_instance_call_result_n25 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_self, hxc_cells, hxc_length, hxc_chunk, hxc_tmp_load_result_n16, hxc_tmp_load_result_n17, hxc_caxecraft_app_VoxelFace_Top, hxc_tmp_load_result_n19, hxc_tmp_load_result_n20, hxc_tmp_load_result_n21, hxc_tmp_load_result_n22, hxc_i32_add_wrapping(hxc_tmp_load_result_n23, 1), hxc_z);
          int32_t hxc_nextCount = hxc_tmp_instance_call_result_n25;
          if (hxc_nextCount < 0)
          {
            hxc_valid = false;
            hxc_nextCount = hxc_count;
          }
          int32_t hxc_tmp_load_result_n28 = hxc_nextCount;
          if (hxc_tmp_load_result_n28 > hxc_count)
          {
            hxc_blockVisible = true;
          }
          hxc_count = hxc_nextCount;
          int32_t hxc_tmp_load_result_n31 = hxc_count;
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n32 = hxc_kind;
          int32_t hxc_tmp_load_result_n34 = hxc_x;
          int32_t hxc_tmp_load_result_n35 = hxc_y;
          int32_t hxc_tmp_load_result_n36 = hxc_z;
          int32_t hxc_tmp_load_result_n37 = hxc_x;
          int32_t hxc_tmp_load_result_n38 = hxc_y;
          int32_t hxc_tmp_instance_call_result_n40 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_self, hxc_cells, hxc_length, hxc_chunk, hxc_tmp_load_result_n31, hxc_tmp_load_result_n32, hxc_caxecraft_app_VoxelFace_Bottom, hxc_tmp_load_result_n34, hxc_tmp_load_result_n35, hxc_tmp_load_result_n36, hxc_tmp_load_result_n37, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n38, 1), hxc_z);
          hxc_nextCount = hxc_tmp_instance_call_result_n40;
          if (hxc_nextCount < 0)
          {
            hxc_valid = false;
            hxc_nextCount = hxc_count;
          }
          int32_t hxc_tmp_load_result_n43 = hxc_nextCount;
          if (hxc_tmp_load_result_n43 > hxc_count)
          {
            hxc_blockVisible = true;
          }
          hxc_count = hxc_nextCount;
          int32_t hxc_tmp_load_result_n46 = hxc_count;
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n47 = hxc_kind;
          int32_t hxc_tmp_load_result_n49 = hxc_x;
          int32_t hxc_tmp_load_result_n50 = hxc_y;
          int32_t hxc_tmp_load_result_n51 = hxc_z;
          int32_t hxc_tmp_load_result_n52 = hxc_x;
          int32_t hxc_tmp_load_result_n53 = hxc_y;
          int32_t hxc_tmp_instance_call_result_n55 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_self, hxc_cells, hxc_length, hxc_chunk, hxc_tmp_load_result_n46, hxc_tmp_load_result_n47, hxc_caxecraft_app_VoxelFace_North, hxc_tmp_load_result_n49, hxc_tmp_load_result_n50, hxc_tmp_load_result_n51, hxc_tmp_load_result_n52, hxc_tmp_load_result_n53, hxc_i32_subtract_wrapping(hxc_z, 1));
          hxc_nextCount = hxc_tmp_instance_call_result_n55;
          if (hxc_nextCount < 0)
          {
            hxc_valid = false;
            hxc_nextCount = hxc_count;
          }
          int32_t hxc_tmp_load_result_n58 = hxc_nextCount;
          if (hxc_tmp_load_result_n58 > hxc_count)
          {
            hxc_blockVisible = true;
          }
          hxc_count = hxc_nextCount;
          int32_t hxc_tmp_load_result_n61 = hxc_count;
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n62 = hxc_kind;
          int32_t hxc_tmp_load_result_n64 = hxc_x;
          int32_t hxc_tmp_load_result_n65 = hxc_y;
          int32_t hxc_tmp_load_result_n66 = hxc_z;
          int32_t hxc_tmp_load_result_n67 = hxc_x;
          int32_t hxc_tmp_load_result_n68 = hxc_y;
          int32_t hxc_tmp_instance_call_result_n70 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_self, hxc_cells, hxc_length, hxc_chunk, hxc_tmp_load_result_n61, hxc_tmp_load_result_n62, hxc_caxecraft_app_VoxelFace_South, hxc_tmp_load_result_n64, hxc_tmp_load_result_n65, hxc_tmp_load_result_n66, hxc_tmp_load_result_n67, hxc_tmp_load_result_n68, hxc_i32_add_wrapping(hxc_z, 1));
          hxc_nextCount = hxc_tmp_instance_call_result_n70;
          if (hxc_nextCount < 0)
          {
            hxc_valid = false;
            hxc_nextCount = hxc_count;
          }
          int32_t hxc_tmp_load_result_n73 = hxc_nextCount;
          if (hxc_tmp_load_result_n73 > hxc_count)
          {
            hxc_blockVisible = true;
          }
          hxc_count = hxc_nextCount;
          int32_t hxc_tmp_load_result_n76 = hxc_count;
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n77 = hxc_kind;
          int32_t hxc_tmp_load_result_n79 = hxc_x;
          int32_t hxc_tmp_load_result_n80 = hxc_y;
          int32_t hxc_tmp_load_result_n81 = hxc_z;
          int32_t hxc_tmp_load_result_n82 = hxc_x;
          int32_t hxc_tmp_load_result_n83 = hxc_y;
          int32_t hxc_tmp_instance_call_result_n85 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_self, hxc_cells, hxc_length, hxc_chunk, hxc_tmp_load_result_n76, hxc_tmp_load_result_n77, hxc_caxecraft_app_VoxelFace_East, hxc_tmp_load_result_n79, hxc_tmp_load_result_n80, hxc_tmp_load_result_n81, hxc_i32_add_wrapping(hxc_tmp_load_result_n82, 1), hxc_tmp_load_result_n83, hxc_z);
          hxc_nextCount = hxc_tmp_instance_call_result_n85;
          if (hxc_nextCount < 0)
          {
            hxc_valid = false;
            hxc_nextCount = hxc_count;
          }
          int32_t hxc_tmp_load_result_n88 = hxc_nextCount;
          if (hxc_tmp_load_result_n88 > hxc_count)
          {
            hxc_blockVisible = true;
          }
          hxc_count = hxc_nextCount;
          int32_t hxc_tmp_load_result_n91 = hxc_count;
          enum hxc_caxecraft_domain_BlockKind hxc_tmp_load_result_n92 = hxc_kind;
          int32_t hxc_tmp_load_result_n94 = hxc_x;
          int32_t hxc_tmp_load_result_n95 = hxc_y;
          int32_t hxc_tmp_load_result_n96 = hxc_z;
          int32_t hxc_tmp_load_result_n97 = hxc_x;
          int32_t hxc_tmp_load_result_n98 = hxc_y;
          int32_t hxc_tmp_instance_call_result_n100 = hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(hxc_self, hxc_cells, hxc_length, hxc_chunk, hxc_tmp_load_result_n91, hxc_tmp_load_result_n92, hxc_caxecraft_app_VoxelFace_West, hxc_tmp_load_result_n94, hxc_tmp_load_result_n95, hxc_tmp_load_result_n96, hxc_i32_subtract_wrapping(hxc_tmp_load_result_n97, 1), hxc_tmp_load_result_n98, hxc_z);
          hxc_nextCount = hxc_tmp_instance_call_result_n100;
          if (hxc_nextCount < 0)
          {
            hxc_valid = false;
            hxc_nextCount = hxc_count;
          }
          int32_t hxc_tmp_load_result_n103 = hxc_nextCount;
          if (hxc_tmp_load_result_n103 > hxc_count)
          {
            hxc_blockVisible = true;
          }
          hxc_count = hxc_nextCount;
          if (hxc_blockVisible)
          {
            enum hxc_caxecraft_app_TerrainSheet hxc_tmp_call_result_n108 = hxc_caxecraft_app_TerrainAtlas_sheet(hxc_kind);
            switch (hxc_tmp_call_result_n108) {
              case hxc_caxecraft_app_TerrainSheet_Base:
                {
                  hxc_visibleBase = hxc_i32_add_wrapping(hxc_visibleBase, 1);
                  break;
                }
              case hxc_caxecraft_app_TerrainSheet_Adventure:
                {
                  hxc_visibleAdventure = hxc_i32_add_wrapping(hxc_visibleAdventure, 1);
                  break;
                }
            }
          }
        }
        hxc_x = hxc_i32_add_wrapping(hxc_x, 1);
      }
      hxc_y = hxc_i32_add_wrapping(hxc_y, 1);
    }
    hxc_z = hxc_i32_add_wrapping(hxc_z, 1);
  }
  if (!!hxc_valid)
  {
    if (hxc_self == NULL)
    {
      abort();
    }
    if (hxc_chunk < 0 || (size_t)hxc_chunk >= 16)
    {
      abort();
    }
    (*hxc_self).hxc_faceCounts[(size_t)hxc_chunk] = hxc_count;
    if (hxc_chunk < 0 || (size_t)hxc_chunk >= 16)
    {
      abort();
    }
    (*hxc_self).hxc_baseVisible[(size_t)hxc_chunk] = hxc_visibleBase;
    if (hxc_chunk < 0 || (size_t)hxc_chunk >= 16)
    {
      abort();
    }
    (*hxc_self).hxc_adventureVisible[(size_t)hxc_chunk] = hxc_visibleAdventure;
    return true;
  }
  if (hxc_self == NULL)
  {
    abort();
  }
  bool hxc_tmp_instance_call_result_n115 = hxc_caxecraft_app_TerrainChunkCache_invalidateBrokenChunk(hxc_self, hxc_chunk);
  return hxc_tmp_instance_call_result_n115;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_totalFaces(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self)
{
  int32_t hxc_total = 0;
  int32_t hxc_chunk = 0;
  while (1)
  {
    if (!(hxc_chunk < 16))
    {
      break;
    }
    int32_t hxc_tmp_compound_load_result_n1 = hxc_total;
    if (hxc_self == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_load_result_n2 = hxc_chunk;
    if (hxc_tmp_load_result_n2 < 0 || (size_t)hxc_tmp_load_result_n2 >= 16)
    {
      abort();
    }
    int32_t hxc_tmp_collection_index_load_result_n3 = (*hxc_self).hxc_faceCounts[(size_t)hxc_tmp_load_result_n2];
    hxc_total = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n1, hxc_tmp_collection_index_load_result_n3);
    hxc_chunk = hxc_i32_add_wrapping(hxc_chunk, 1);
  }
  return hxc_total;
}

int32_t hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, enum hxc_caxecraft_app_TerrainSheet hxc_sheet)
{
  int32_t hxc_total = 0;
  int32_t hxc_chunk = 0;
  while (1)
  {
    if (!(hxc_chunk < 16))
    {
      break;
    }
    switch (hxc_sheet) {
      case hxc_caxecraft_app_TerrainSheet_Base:
        {
          int32_t hxc_tmp_compound_load_result_n1 = hxc_total;
          if (hxc_self == NULL)
          {
            abort();
          }
          int32_t hxc_tmp_load_result_n2 = hxc_chunk;
          if (hxc_tmp_load_result_n2 < 0 || (size_t)hxc_tmp_load_result_n2 >= 16)
          {
            abort();
          }
          int32_t hxc_tmp_collection_index_load_result_n3 = (*hxc_self).hxc_baseVisible[(size_t)hxc_tmp_load_result_n2];
          hxc_total = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n1, hxc_tmp_collection_index_load_result_n3);
          break;
        }
      case hxc_caxecraft_app_TerrainSheet_Adventure:
        {
          int32_t hxc_tmp_compound_load_result_n4 = hxc_total;
          if (hxc_self == NULL)
          {
            abort();
          }
          int32_t hxc_tmp_load_result_n5 = hxc_chunk;
          if (hxc_tmp_load_result_n5 < 0 || (size_t)hxc_tmp_load_result_n5 >= 16)
          {
            abort();
          }
          int32_t hxc_tmp_collection_index_load_result_n6 = (*hxc_self).hxc_adventureVisible[(size_t)hxc_tmp_load_result_n5];
          hxc_total = hxc_i32_add_wrapping(hxc_tmp_compound_load_result_n4, hxc_tmp_collection_index_load_result_n6);
          break;
        }
    }
    hxc_chunk = hxc_i32_add_wrapping(hxc_chunk, 1);
  }
  return hxc_total;
}
