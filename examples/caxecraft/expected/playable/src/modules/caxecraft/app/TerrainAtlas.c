#include "hxc/program.h"

int32_t hxc_caxecraft_app_TerrainAtlas_column(enum hxc_caxecraft_app_TerrainTile hxc_tile)
{
  switch (hxc_tile) {
    case hxc_caxecraft_app_TerrainTile_MeadowGrassTop:
      {
        break;
      }
    case hxc_caxecraft_app_TerrainTile_GrassSide:
      {
        return 1;
      }
    case hxc_caxecraft_app_TerrainTile_RichSoil:
      {
        return 2;
      }
    case hxc_caxecraft_app_TerrainTile_SlateStone:
    case hxc_caxecraft_app_TerrainTile_FoundationRock:
      {
        return 3;
      }
  }
  return 0;
}

int32_t hxc_caxecraft_app_TerrainAtlas_row(enum hxc_caxecraft_app_TerrainTile hxc_tile)
{
  switch (hxc_tile) {
    case hxc_caxecraft_app_TerrainTile_MeadowGrassTop:
    case hxc_caxecraft_app_TerrainTile_GrassSide:
    case hxc_caxecraft_app_TerrainTile_RichSoil:
    case hxc_caxecraft_app_TerrainTile_SlateStone:
      {
        break;
      }
    case hxc_caxecraft_app_TerrainTile_FoundationRock:
      {
        return 3;
      }
  }
  return 0;
}

enum hxc_caxecraft_app_TerrainTile hxc_caxecraft_app_TerrainAtlas_tile(enum hxc_caxecraft_domain_BlockKind hxc_kind, enum hxc_caxecraft_app_VoxelFace hxc_face)
{
  switch (hxc_kind) {
    case hxc_caxecraft_domain_BlockKind_Air:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        switch (hxc_face) {
          case hxc_caxecraft_app_VoxelFace_Top:
            {
              break;
            }
          case hxc_caxecraft_app_VoxelFace_Bottom:
            {
              return hxc_caxecraft_app_TerrainTile_RichSoil;
            }
          case hxc_caxecraft_app_VoxelFace_North:
          case hxc_caxecraft_app_VoxelFace_South:
          case hxc_caxecraft_app_VoxelFace_East:
          case hxc_caxecraft_app_VoxelFace_West:
            {
              return hxc_caxecraft_app_TerrainTile_GrassSide;
            }
        }
        return hxc_caxecraft_app_TerrainTile_MeadowGrassTop;
      }
    case hxc_caxecraft_domain_BlockKind_Dirt:
      {
        return hxc_caxecraft_app_TerrainTile_RichSoil;
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        return hxc_caxecraft_app_TerrainTile_SlateStone;
      }
    case hxc_caxecraft_domain_BlockKind_Bedrock:
      {
        return hxc_caxecraft_app_TerrainTile_FoundationRock;
      }
  }
  return hxc_caxecraft_app_TerrainTile_RichSoil;
}
