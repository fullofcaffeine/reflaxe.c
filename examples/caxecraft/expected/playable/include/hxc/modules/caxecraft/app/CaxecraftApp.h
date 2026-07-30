#ifndef HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED
#define HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/app/TerrainRenderer.h"
#include "hxc/modules/caxecraft/domain/GameSession.h"
#include "hxc/modules/caxecraft/input/NavigationInput.h"

struct hxc_caxecraft_app_CaxecraftApp {
  struct hxc_caxecraft_domain_GameSession hxc_session;
  struct hxc_caxecraft_app_TerrainRenderer hxc_terrainRenderer;
  struct hxc_caxecraft_app_CaxecraftEditorScreen *hxc_editorScreen;
  struct hxc_caxecraft_input_NavigationRepeater hxc_editorNavigation;
};

struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation {
  enum hxc_caxecraft_domain_ActorControllerPhase hxc_phase;
  bool hxc_valid;
};

struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection {
  int32_t hxc_dialogueActorId;
  int32_t hxc_enemyActorId;
  bool hxc_valid;
};

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_self);

enum hxc_caxecraft_gameplay_GuidePhase hxc_caxecraft_app_CaxecraftApp_advanceGuidePhase(enum hxc_caxecraft_gameplay_GuidePhase hxc_phase);

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_camera, struct Texture hxc_entityTexture, bool hxc_entityTextureReady, struct hxc_caxecraft_domain_Character hxc_guide, struct hxc_caxecraft_domain_Character hxc_enemy, enum hxc_caxecraft_domain_ActorControllerPhase hxc_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop);

void hxc_caxecraft_app_CaxecraftApp_drawBreath(int32_t hxc_breathTicks, int32_t hxc_maximumBreathTicks, int32_t hxc_width, int32_t hxc_height);

void hxc_caxecraft_app_CaxecraftApp_drawHealth(struct hxc_caxecraft_domain_VitalsState hxc_vitals, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, int32_t hxc_width);

void hxc_caxecraft_app_CaxecraftApp_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_inventory, struct Texture hxc_hudTexture, bool hxc_hudTextureReady, struct Texture hxc_itemTexture, bool hxc_itemTextureReady, int32_t hxc_width, int32_t hxc_height);

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_view, struct hxc_caxecraft_app_HudResources hxc_resources);

struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_caxecraft_app_CaxecraftApp_observeActorPhase(struct hxc_array_ref *hxc_states, int32_t hxc_id, enum hxc_caxecraft_domain_ActorControllerPhase hxc_fallback);

struct hxc_caxecraft_app_CaxecraftApp_PlayableActorSelection hxc_caxecraft_app_CaxecraftApp_selectPlayableActors(struct hxc_array_ref *hxc_bindings);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_app_CaxecraftApp_spawnPlayer(const uint8_t *hxc_cells, size_t hxc_length);

void hxc_caxecraft_app_CaxecraftApp_run(struct hxc_caxecraft_app_CaxecraftApp *hxc_self);

#endif /* HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED */
