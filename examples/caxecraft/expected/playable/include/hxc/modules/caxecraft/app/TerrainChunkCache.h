#ifndef HXC_CAXECRAFT_APP_TERRAIN_CHUNK_CACHE_H_INCLUDED
#define HXC_CAXECRAFT_APP_TERRAIN_CHUNK_CACHE_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/app/TerrainAtlas.h"
#include "hxc/modules/caxecraft/domain/BlockKind.h"

struct hxc_caxecraft_app_TerrainChunkCache {
  uint8_t hxc_faceX[49152];
  uint8_t hxc_faceY[49152];
  uint8_t hxc_faceZ[49152];
  uint8_t hxc_packedFaces[49152];
  int32_t hxc_faceCounts[16];
  int32_t hxc_baseVisible[16];
  int32_t hxc_adventureVisible[16];
  uint8_t hxc_dirty[16];
};

struct hxc_caxecraft_app_TerrainChunkPreparation {
  int32_t hxc_faces;
  int32_t hxc_rebuiltChunks;
  bool hxc_valid;
  int32_t hxc_visibleBlocks;
};

void hxc_compiler_constructor_caxecraft_app_TerrainChunkCache(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self);

int32_t hxc_caxecraft_app_TerrainChunkCache_appendIfExposed(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_chunk, int32_t hxc_count, enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_app_VoxelFace hxc_face, int32_t hxc_x, int32_t hxc_y, int32_t hxc_z, int32_t hxc_neighborX, int32_t hxc_neighborY, int32_t hxc_neighborZ);

int32_t hxc_caxecraft_app_TerrainChunkCache_chunkFaceCount(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, int32_t hxc_chunk);

int32_t hxc_caxecraft_app_TerrainChunkCache_invalidate(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, struct hxc_caxecraft_domain_BlockCoord hxc_coord);

void hxc_caxecraft_app_TerrainChunkCache_invalidateAll(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self);

bool hxc_caxecraft_app_TerrainChunkCache_invalidateBrokenChunk(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, int32_t hxc_chunk);

struct hxc_caxecraft_app_TerrainChunkPreparation hxc_caxecraft_app_TerrainChunkCache_prepare(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, uint8_t *hxc_cells, size_t hxc_length);

bool hxc_caxecraft_app_TerrainChunkCache_rebuild(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_chunk);

int32_t hxc_caxecraft_app_TerrainChunkCache_totalFaces(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self);

int32_t hxc_caxecraft_app_TerrainChunkCache_visibleBlocks(struct hxc_caxecraft_app_TerrainChunkCache *hxc_self, enum hxc_caxecraft_app_TerrainSheet hxc_sheet);

#endif /* HXC_CAXECRAFT_APP_TERRAIN_CHUNK_CACHE_H_INCLUDED */
