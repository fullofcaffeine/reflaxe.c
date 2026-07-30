#include "hxc/program.h"

int32_t hxc_caxecraft_app_TerrainAtlas_column(enum hxc_caxecraft_app_TerrainTile hxc_l_tile)
{
  switch (hxc_l_tile) {
    case hxc_caxecraft_app_TerrainTile_RichSoil:
    case hxc_caxecraft_app_TerrainTile_BarkSide:
      {
        break;
      }
    case hxc_caxecraft_app_TerrainTile_SlateStone:
    case hxc_caxecraft_app_TerrainTile_FoundationRock:
    case hxc_caxecraft_app_TerrainTile_LogTop:
      {
        return 3;
      }
    case hxc_caxecraft_app_TerrainTile_MeadowGrassTop:
    case hxc_caxecraft_app_TerrainTile_RiverSand:
    case hxc_caxecraft_app_TerrainTile_LeafyCanopy:
    case hxc_caxecraft_app_TerrainTile_SnowTop:
    case hxc_caxecraft_app_TerrainTile_AshTop:
      {
        return 0;
      }
    case hxc_caxecraft_app_TerrainTile_GrassSide:
    case hxc_caxecraft_app_TerrainTile_SnowSide:
    case hxc_caxecraft_app_TerrainTile_AshSide:
      {
        return 1;
      }
    default:
      {
        abort();
      }
  }
  return 2;
}

int32_t hxc_caxecraft_app_TerrainAtlas_row(enum hxc_caxecraft_app_TerrainTile hxc_l_tile)
{
  switch (hxc_l_tile) {
    case hxc_caxecraft_app_TerrainTile_FoundationRock:
      {
        break;
      }
    case hxc_caxecraft_app_TerrainTile_LeafyCanopy:
      {
        return 2;
      }
    case hxc_caxecraft_app_TerrainTile_MeadowGrassTop:
    case hxc_caxecraft_app_TerrainTile_GrassSide:
    case hxc_caxecraft_app_TerrainTile_RichSoil:
    case hxc_caxecraft_app_TerrainTile_SlateStone:
    case hxc_caxecraft_app_TerrainTile_SnowTop:
    case hxc_caxecraft_app_TerrainTile_SnowSide:
      {
        return 0;
      }
    case hxc_caxecraft_app_TerrainTile_RiverSand:
    case hxc_caxecraft_app_TerrainTile_BarkSide:
    case hxc_caxecraft_app_TerrainTile_LogTop:
    case hxc_caxecraft_app_TerrainTile_AshTop:
    case hxc_caxecraft_app_TerrainTile_AshSide:
      {
        return 1;
      }
    default:
      {
        abort();
      }
  }
  return 3;
}

enum hxc_caxecraft_app_TerrainSheet hxc_caxecraft_app_TerrainAtlas_sheet(enum hxc_caxecraft_domain_BlockKind hxc_l_kind)
{
  switch (hxc_l_kind) {
    case hxc_caxecraft_domain_BlockKind_Air:
    case hxc_caxecraft_domain_BlockKind_Grass:
    case hxc_caxecraft_domain_BlockKind_Dirt:
    case hxc_caxecraft_domain_BlockKind_Stone:
    case hxc_caxecraft_domain_BlockKind_Bedrock:
    case hxc_caxecraft_domain_BlockKind_Sand:
    case hxc_caxecraft_domain_BlockKind_Wood:
    case hxc_caxecraft_domain_BlockKind_Leaves:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Snow:
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        return hxc_caxecraft_app_TerrainSheet_Adventure;
      }
    default:
      {
        abort();
      }
  }
  return hxc_caxecraft_app_TerrainSheet_Base;
}

enum hxc_caxecraft_app_TerrainTile hxc_caxecraft_app_TerrainAtlas_tile(enum hxc_caxecraft_domain_BlockKind hxc_l_kind, enum hxc_caxecraft_app_VoxelFace hxc_l_face)
{
  switch (hxc_l_kind) {
    case hxc_caxecraft_domain_BlockKind_Air:
      {
        return hxc_caxecraft_app_TerrainTile_RichSoil;
      }
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        break;
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
    case hxc_caxecraft_domain_BlockKind_Sand:
      {
        return hxc_caxecraft_app_TerrainTile_RiverSand;
      }
    case hxc_caxecraft_domain_BlockKind_Wood:
      {
        switch (hxc_l_face) {
          case hxc_caxecraft_app_VoxelFace_Top:
          case hxc_caxecraft_app_VoxelFace_Bottom:
            {
              break;
            }
          case hxc_caxecraft_app_VoxelFace_North:
          case hxc_caxecraft_app_VoxelFace_South:
          case hxc_caxecraft_app_VoxelFace_East:
          case hxc_caxecraft_app_VoxelFace_West:
            {
              return hxc_caxecraft_app_TerrainTile_BarkSide;
            }
          default:
            {
              abort();
            }
        }
        return hxc_caxecraft_app_TerrainTile_LogTop;
      }
    case hxc_caxecraft_domain_BlockKind_Leaves:
      {
        return hxc_caxecraft_app_TerrainTile_LeafyCanopy;
      }
    case hxc_caxecraft_domain_BlockKind_Snow:
      {
        switch (hxc_l_face) {
          case hxc_caxecraft_app_VoxelFace_Top:
            {
              break;
            }
          case hxc_caxecraft_app_VoxelFace_Bottom:
          case hxc_caxecraft_app_VoxelFace_North:
          case hxc_caxecraft_app_VoxelFace_South:
          case hxc_caxecraft_app_VoxelFace_East:
          case hxc_caxecraft_app_VoxelFace_West:
            {
              return hxc_caxecraft_app_TerrainTile_SnowSide;
            }
          default:
            {
              abort();
            }
        }
        return hxc_caxecraft_app_TerrainTile_SnowTop;
      }
    case hxc_caxecraft_domain_BlockKind_Ash:
      {
        switch (hxc_l_face) {
          case hxc_caxecraft_app_VoxelFace_Top:
            {
              break;
            }
          case hxc_caxecraft_app_VoxelFace_Bottom:
          case hxc_caxecraft_app_VoxelFace_North:
          case hxc_caxecraft_app_VoxelFace_South:
          case hxc_caxecraft_app_VoxelFace_East:
          case hxc_caxecraft_app_VoxelFace_West:
            {
              return hxc_caxecraft_app_TerrainTile_AshSide;
            }
          default:
            {
              abort();
            }
        }
        return hxc_caxecraft_app_TerrainTile_AshTop;
      }
    default:
      {
        abort();
      }
  }
  switch (hxc_l_face) {
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
    default:
      {
        abort();
      }
  }
  return hxc_caxecraft_app_TerrainTile_MeadowGrassTop;
}
