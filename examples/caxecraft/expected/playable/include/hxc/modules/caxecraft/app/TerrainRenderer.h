#ifndef HXC_CAXECRAFT_APP_TERRAIN_RENDERER_H_INCLUDED
#define HXC_CAXECRAFT_APP_TERRAIN_RENDERER_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/app/TerrainChunkCache.h"

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

void hxc_compiler_constructor_caxecraft_app_TerrainRenderer(struct hxc_caxecraft_app_TerrainRenderer *hxc_l_self);

void hxc_caxecraft_app_TerrainRenderer_emitFace(enum hxc_caxecraft_domain_BlockKind hxc_l_kind, enum hxc_caxecraft_app_VoxelFace hxc_l_face, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z, double hxc_l_halfPixel, int32_t hxc_l_distance);

struct Color hxc_caxecraft_app_TerrainRenderer_faceTint(enum hxc_caxecraft_app_VoxelFace hxc_l_face, int32_t hxc_l_distance);

int32_t hxc_caxecraft_app_TerrainRenderer_squareDistance(int32_t hxc_l_x, int32_t hxc_l_z, int32_t hxc_l_playerX, int32_t hxc_l_playerZ);

struct hxc_caxecraft_app_TerrainRenderCounters hxc_caxecraft_app_TerrainRenderer_draw(struct hxc_caxecraft_app_TerrainRenderer *hxc_l_self, const uint8_t *hxc_l_cells, size_t hxc_l_length, struct Texture hxc_l_baseTexture, bool hxc_l_baseReady, struct Texture hxc_l_adventureTexture, bool hxc_l_adventureReady, double hxc_l_playerX, double hxc_l_playerZ);

struct hxc_caxecraft_app_TerrainRenderer_TerrainSheetCounters hxc_caxecraft_app_TerrainRenderer_drawSheet(struct hxc_caxecraft_app_TerrainRenderer *hxc_l_self, struct Texture hxc_l_texture, bool hxc_l_textureReady, enum hxc_caxecraft_app_TerrainSheet hxc_l_sheet, int32_t hxc_l_playerCellX, int32_t hxc_l_playerCellZ);

#endif /* HXC_CAXECRAFT_APP_TERRAIN_RENDERER_H_INCLUDED */
