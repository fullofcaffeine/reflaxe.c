#ifndef HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED
#define HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/app/TerrainRenderer.h"
#include "hxc/modules/caxecraft/domain/GameSession.h"

struct hxc_caxecraft_app_CaxecraftApp {
  struct hxc_caxecraft_domain_GameSession hxc_session;
  struct hxc_caxecraft_app_TerrainRenderer hxc_terrainRenderer;
};

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_self);

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_camera, struct Texture hxc_entityTexture, bool hxc_entityTextureReady, struct hxc_caxecraft_gameplay_GuideState hxc_guide, struct hxc_caxecraft_gameplay_MosslingState hxc_mossling, struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop);

void hxc_caxecraft_app_CaxecraftApp_drawBreath(int32_t hxc_breathTicks, int32_t hxc_maximumBreathTicks, int32_t hxc_width, int32_t hxc_height);

void hxc_caxecraft_app_CaxecraftApp_drawHealth(struct hxc_caxecraft_domain_VitalsState hxc_vitals, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, int32_t hxc_width);

void hxc_caxecraft_app_CaxecraftApp_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, int32_t hxc_width, int32_t hxc_height);

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_view, struct hxc_caxecraft_app_HudResources hxc_resources);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_app_CaxecraftApp_spawnPlayer(uint8_t *hxc_cells, size_t hxc_length);

void hxc_caxecraft_app_CaxecraftApp_run(struct hxc_caxecraft_app_CaxecraftApp *hxc_self);

#endif /* HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED */
