#ifndef HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED
#define HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/app/ConversationFlow.h"
#include "hxc/modules/caxecraft/app/DistantHorizon.h"
#include "hxc/modules/caxecraft/app/MotionInterpolation.h"
#include "hxc/modules/caxecraft/app/TerrainRenderer.h"
#include "hxc/modules/caxecraft/app/WaterRenderer.h"
#include "hxc/modules/caxecraft/domain/Character.h"
#include "hxc/modules/caxecraft/gameplay/BerryDropState.h"
#include "hxc/modules/caxecraft/gameplay/InventoryState.h"
#include "hxc/modules/caxecraft/gameplay/SwordCombatState.h"
#include "hxc/modules/caxecraft/input/NavigationInput.h"

struct hxc_caxecraft_app_CaxecraftApp {
  struct hxc_caxecraft_app_TerrainRenderer hxc_terrainRenderer;
  struct hxc_caxecraft_app_TerrainRenderer hxc_portalTerrainRenderer;
  struct hxc_caxecraft_app_DistantHorizonRenderer hxc_distantHorizonRenderer;
  struct hxc_caxecraft_app_WaterRenderer hxc_waterRenderer;
  struct hxc_caxecraft_app_WaterRenderer hxc_portalWaterRenderer;
  struct hxc_caxecraft_input_NavigationRepeater hxc_editorNavigation;
};

struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation {
  enum hxc_caxecraft_domain_ActorControllerPhase hxc_phase;
  bool hxc_valid;
};

struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot {
  double hxc_accumulator;
  hxc_string hxc_activeDialogue;
  int32_t hxc_aquaticEquipmentCode;
  int32_t hxc_aquaticEquipmentFrames;
  enum hxc_caxecraft_app_InteractionPrompt hxc_availableInteractionPrompt;
  struct hxc_caxecraft_gameplay_BerryDropState hxc_berryDrop;
  enum hxc_caxecraft_domain_PlayerCameraMode hxc_cameraMode;
  double hxc_cameraWaterBlend;
  struct hxc_caxecraft_content_CampaignLevel *hxc_campaignLevel;
  struct hxc_optional_caxecraft_app_ConversationState hxc_conversation;
  hxc_string hxc_currentObjectiveId;
  struct hxc_caxecraft_domain_Character hxc_enemyActor;
  int32_t hxc_enemyAttackFrames;
  int32_t hxc_enemyDefeatedFrames;
  struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_enemyPhase;
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory;
  int32_t hxc_inventoryFullFrames;
  int32_t hxc_inventoryFullReason;
  bool hxc_jumpQueued;
  hxc_string hxc_latestJournalId;
  hxc_string hxc_levelLabel;
  bool hxc_loadingFramePresented;
  double hxc_lookX;
  double hxc_lookY;
  double hxc_lookZ;
  struct hxc_caxecraft_app_MotionHistory hxc_motionHistory;
  hxc_string hxc_pendingCampaignLabel;
  struct hxc_caxecraft_content_CampaignLevel *hxc_pendingCampaignLevel;
  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_pendingCampaignPortal;
  int32_t hxc_pickupAmount;
  int32_t hxc_pickupFrames;
  int32_t hxc_placementBlockedFrames;
  struct hxc_caxecraft_app_PreparedCampaignPortal *hxc_preparedPortal;
  int32_t hxc_recoveryFeedback;
  int32_t hxc_recoveryFeedbackFrames;
  int32_t hxc_selectedCampaignLevelIndex;
  int32_t hxc_strikeHitFrames;
  struct hxc_caxecraft_gameplay_SwordCombatState hxc_swordCombat;
  bool hxc_swordQueued;
};

struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun {
  int32_t hxc_generationId;
  struct hxc_caxecraft_app_EditorTestPlayRuntime *hxc_runtime;
  struct hxc_caxecraft_app_CaxecraftApp_EditorTestPlayShellSnapshot hxc_snapshot;
};

enum hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_tag {
  hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_NoAvailableInteraction = 0,
  hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction = 1,
  hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction = 2
};

struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction_payload {
  int32_t hxc_id;
};

struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction_payload {
  hxc_string hxc_id;
};

union hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_payload {
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_DialogueInteraction_payload hxc_DialogueInteraction;
  struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_StatefulObjectInteraction_payload hxc_StatefulObjectInteraction;
};

struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget {
  enum hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_tag hxc_tag;
  union hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget_payload hxc_payload;
};

struct hxc_optional_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun {
  bool hxc_has_value;
  struct hxc_caxecraft_app_CaxecraftApp_ActiveEditorTestPlayRun hxc_value;
};

void hxc_compiler_constructor_caxecraft_app_CaxecraftApp(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self);

hxc_string hxc_caxecraft_app_CaxecraftApp_campaignLevelLoadFailure(struct hxc_caxecraft_content_CampaignLevelLoadError hxc_l_error);

hxc_string hxc_caxecraft_app_CaxecraftApp_conversationSpeakerLabel(hxc_string hxc_l_id);

struct hxc_array_ref *hxc_caxecraft_app_CaxecraftApp_dialogueActorSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level);

bool hxc_caxecraft_app_CaxecraftApp_dialogueActorsAreValid(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct hxc_array_ref *hxc_l_states);

void hxc_caxecraft_app_CaxecraftApp_drawActionNotice(struct hxc_caxecraft_app_HudView hxc_l_view_h96a452c9f6a1, struct hxc_caxecraft_app_HudLayout hxc_l_layout, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_locale, struct Color hxc_l_color);

bool hxc_caxecraft_app_CaxecraftApp_drawActorPresentation(struct Camera3D hxc_l_camera, hxc_string hxc_l_asset, int32_t hxc_l_cellIndex, struct Vector3 hxc_l_position, double hxc_l_width, double hxc_l_height, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures);

void hxc_caxecraft_app_CaxecraftApp_drawActors(struct Camera3D hxc_l_camera, struct Texture hxc_l_entityTexture, bool hxc_l_entityTextureReady, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, struct hxc_array_ref *hxc_l_dialogueActors, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level, struct hxc_caxecraft_domain_Character hxc_l_enemy, hxc_string hxc_l_enemyAsset, int32_t hxc_l_enemyCell, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_enemyPhase, struct hxc_caxecraft_gameplay_BerryDropState hxc_l_berryDrop);

void hxc_caxecraft_app_CaxecraftApp_drawBreath(int32_t hxc_l_breathTicks, int32_t hxc_l_maximumBreathTicks, struct hxc_caxecraft_app_HudLayout hxc_l_layout, int32_t hxc_l_width);

void hxc_caxecraft_app_CaxecraftApp_drawCampaignLoading(hxc_string hxc_l_destinationLabel, int32_t hxc_l_locale, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_catalog);

void hxc_caxecraft_app_CaxecraftApp_drawCloud(double hxc_l_eyeX, double hxc_l_eyeY, double hxc_l_eyeZ, double hxc_l_simulationSeconds, double hxc_l_baseX, double hxc_l_baseZ, double hxc_l_speed, int32_t hxc_l_variant);

void hxc_caxecraft_app_CaxecraftApp_drawConversation(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudResources hxc_l_resources, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_locale, int32_t hxc_l_width_hf2ade07c703f, int32_t hxc_l_height_h54d8cf3f7625, struct Color hxc_l_color);

void hxc_caxecraft_app_CaxecraftApp_drawCrosshair(struct hxc_caxecraft_app_HudLayout hxc_l_layout, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, int32_t hxc_l_centerX, int32_t hxc_l_centerY, bool hxc_l_targetAvailable, struct Color hxc_l_color);

void hxc_caxecraft_app_CaxecraftApp_drawDefeat(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_width_h0218579225f3, int32_t hxc_l_height_h8715ce602c8d, struct Color hxc_l_color);

void hxc_caxecraft_app_CaxecraftApp_drawDiagnostics(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudLayout hxc_l_layout, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_locale, struct Color hxc_l_color);

void hxc_caxecraft_app_CaxecraftApp_drawEnemyAlert(struct hxc_caxecraft_app_HudView hxc_l_view_h9930f80d93d3, struct hxc_caxecraft_app_HudLayout hxc_l_layout, int32_t hxc_l_locale, struct Color hxc_l_color);

void hxc_caxecraft_app_CaxecraftApp_drawHealth(struct hxc_caxecraft_domain_VitalsState hxc_l_vitals, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct hxc_caxecraft_app_HudLayout hxc_l_layout);

void hxc_caxecraft_app_CaxecraftApp_drawHotbar(struct hxc_caxecraft_gameplay_InventoryState hxc_l_inventory, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct Texture hxc_l_itemTexture, bool hxc_l_itemTextureReady, struct hxc_caxecraft_app_HudLayout hxc_l_layout);

void hxc_caxecraft_app_CaxecraftApp_drawHud(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_app_HudResources hxc_l_resources, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_contentRegistry, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog);

void hxc_caxecraft_app_CaxecraftApp_drawNoticePanel(struct hxc_caxecraft_app_HudRectangle hxc_l_panel, hxc_string hxc_l_value, int32_t hxc_l_fontSize, struct Color hxc_l_color, struct Color hxc_l_border, int32_t hxc_l_maximumCharacters);

void hxc_caxecraft_app_CaxecraftApp_drawObjective(struct hxc_caxecraft_app_HudLayout hxc_l_layout, hxc_string hxc_l_levelLabel, hxc_string hxc_l_objectiveTitle, struct Texture hxc_l_hudTexture, bool hxc_l_hudTextureReady, struct Color hxc_l_color);

void hxc_caxecraft_app_CaxecraftApp_drawPause(struct hxc_caxecraft_app_HudView hxc_l_view, struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_uiCatalog, int32_t hxc_l_width_hf32a215f4e06, int32_t hxc_l_height_h274b1254a435, struct Color hxc_l_color);

void hxc_caxecraft_app_CaxecraftApp_drawPlayerAvatar(struct Camera3D hxc_l_camera, struct hxc_caxecraft_app_RuntimeTextureAtlasCatalog *hxc_l_runtimeTextures, double hxc_l_x, double hxc_l_y, double hxc_l_z);

void hxc_caxecraft_app_CaxecraftApp_drawWorldSunAndClouds(bool hxc_l_hasSun, int32_t hxc_l_sunX, int32_t hxc_l_sunY, int32_t hxc_l_sunZ, int32_t hxc_l_sunRadiusMilli, int32_t hxc_l_cloudCount, int32_t hxc_l_cloudSpeedMilli, int32_t hxc_l_cloudSeed, double hxc_l_eyeX, double hxc_l_eyeY, double hxc_l_eyeZ, double hxc_l_simulationSeconds);

void hxc_caxecraft_app_CaxecraftApp_drawWrappedText(hxc_string hxc_l_value, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_fontSize, int32_t hxc_l_maximumCharacters, int32_t hxc_l_lineHeight, int32_t hxc_l_maximumLines, struct Color hxc_l_color);

struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_caxecraft_app_CaxecraftApp_nearestAvailableInteraction(struct hxc_caxecraft_domain_GameSession *hxc_l_session, struct hxc_caxecraft_app_PlayableLevelView *hxc_l_level);

struct hxc_caxecraft_app_CaxecraftApp_ActorPhaseObservation hxc_caxecraft_app_CaxecraftApp_observeActorPhase(struct hxc_array_ref *hxc_l_states, int32_t hxc_l_id, enum hxc_caxecraft_domain_ActorControllerPhase hxc_l_fallback);

enum hxc_caxecraft_app_InteractionPrompt hxc_caxecraft_app_CaxecraftApp_promptForAvailableInteraction(struct hxc_caxecraft_app_CaxecraftApp_AvailableInteractionTarget hxc_l_target);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_app_CaxecraftApp_spawnPlayer(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_scenario_ScenarioTransform hxc_l_transform);

void hxc_caxecraft_app_CaxecraftApp_run(struct hxc_caxecraft_app_CaxecraftApp *hxc_l_self);

#endif /* HXC_CAXECRAFT_APP_CAXECRAFT_APP_H_INCLUDED */
