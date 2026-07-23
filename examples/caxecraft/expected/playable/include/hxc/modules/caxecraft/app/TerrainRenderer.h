#ifndef HXC_CAXECRAFT_APP_TERRAIN_RENDERER_H_INCLUDED
#define HXC_CAXECRAFT_APP_TERRAIN_RENDERER_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/app/TerrainAtlas.h"
#include "hxc/modules/caxecraft/app/TerrainChunkCache.h"
#include "hxc/modules/caxecraft/domain/BlockKind.h"

struct hxc_caxecraft_app_TerrainRenderer {
  struct hxc_caxecraft_app_TerrainChunkCache hxc_cache;
  int32_t hxc_totalRebuiltChunks;
};

struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters {
  int32_t hxc_drawCalls;
  int32_t hxc_faces;
  int32_t hxc_visible;
};

struct hxc_caxecraft_app_TerrainRenderCounters {
  bool hxc_cacheValid;
  int32_t hxc_drawCalls;
  int32_t hxc_faces;
  int32_t hxc_rebuiltChunks;
  int32_t hxc_totalRebuiltChunks;
  int32_t hxc_visible;
};

void hxc_compiler_constructor_caxecraft_app_TerrainRenderer(struct hxc_caxecraft_app_TerrainRenderer *hxc_self);

void hxc_caxecraft_app_TerrainRenderer_emitFace(enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_app_VoxelFace hxc_face, int32_t hxc_x, int32_t hxc_y, int32_t hxc_z, double hxc_halfPixel, int32_t hxc_distance);

struct Color hxc_caxecraft_app_TerrainRenderer_faceTint(enum hxc_caxecraft_app_VoxelFace hxc_face, int32_t hxc_distance);

int32_t hxc_caxecraft_app_TerrainRenderer_squareDistance(int32_t hxc_x, int32_t hxc_z, int32_t hxc_playerX, int32_t hxc_playerZ);

struct hxc_caxecraft_app_TerrainRenderCounters hxc_caxecraft_app_TerrainRenderer_draw(struct hxc_caxecraft_app_TerrainRenderer *hxc_self, uint8_t *hxc_cells, size_t hxc_length, struct Texture hxc_baseTexture, bool hxc_baseReady, struct Texture hxc_adventureTexture, bool hxc_adventureReady, double hxc_playerX, double hxc_playerZ);

struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_caxecraft_app_TerrainRenderer_drawSheet(struct hxc_caxecraft_app_TerrainRenderer *hxc_self, struct Texture hxc_texture, bool hxc_textureReady, enum hxc_caxecraft_app_TerrainSheet hxc_sheet, int32_t hxc_playerCellX, int32_t hxc_playerCellZ);

#endif /* HXC_CAXECRAFT_APP_TERRAIN_RENDERER_H_INCLUDED */
