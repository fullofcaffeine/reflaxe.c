#ifndef HXC_CAXECRAFT_DOMAIN_GAME_SESSION_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_GAME_SESSION_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/domain/Character.h"
#include "hxc/modules/caxecraft/domain/CharacterIntent.h"
#include "hxc/modules/caxecraft/domain/EntityStore.h"
#include "hxc/modules/caxecraft/domain/Immersion.h"
#include "hxc/modules/caxecraft/domain/PlayerAgent.h"
#include "hxc/modules/caxecraft/domain/WaterSimulation.h"
#include "hxc/modules/caxecraft/domain/WaterTickResult.h"
#include "hxc/modules/caxecraft/gameplay/InventoryState.h"

struct hxc_caxecraft_domain_GameSession {
  struct hxc_caxecraft_domain_EntityStore hxc_entities;
  struct hxc_array_ref *hxc_actorControllers;
  struct hxc_array_ref *hxc_actorControllerEvents;
  struct hxc_caxecraft_domain_PlayerAgent hxc_localPlayer;
  int32_t hxc_completedTicks;
  struct hxc_caxecraft_domain_WaterSimulation hxc_water;
  uint8_t hxc_worldStorage[16384];
  int32_t hxc_authoredItemStorage[256];
};

struct hxc_caxecraft_domain_LocalCharacterCommandResult {
  struct hxc_caxecraft_domain_Character hxc_character;
  bool hxc_resolved;
};

struct hxc_caxecraft_domain_GameTickInput {
  int32_t hxc_damagePolicy;
  struct hxc_caxecraft_domain_CharacterIntent hxc_intent;
  int32_t hxc_waterUpdateBudget;
};

struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult {
  struct hxc_caxecraft_domain_Character hxc_character;
  bool hxc_collected;
  bool hxc_resolved;
};

struct hxc_caxecraft_domain_CharacterCommandStepResult {
  struct hxc_caxecraft_domain_Character hxc_character;
  int32_t hxc_drowningDamage;
  struct hxc_caxecraft_domain_Immersion hxc_immersion;
  bool hxc_resolved;
};

struct hxc_caxecraft_domain_LocalRecoveryResult {
  struct hxc_caxecraft_domain_Character hxc_character;
  int32_t hxc_decision;
  struct hxc_caxecraft_gameplay_InventoryState hxc_inventory;
  bool hxc_resolved;
};

struct hxc_caxecraft_domain_GameTickResult {
  struct hxc_caxecraft_domain_Character hxc_character;
  bool hxc_committed;
  int32_t hxc_drowningDamage;
  struct hxc_caxecraft_domain_Immersion hxc_immersion;
  int32_t hxc_tickIndex;
  struct hxc_caxecraft_domain_WaterTickResult hxc_water;
};

struct hxc_caxecraft_domain_CharacterDamageResult {
  struct hxc_caxecraft_domain_Character hxc_character;
  int32_t hxc_damageApplied;
  bool hxc_defeated;
  bool hxc_resolved;
};

void hxc_compiler_constructor_caxecraft_domain_GameSession(struct hxc_caxecraft_domain_GameSession *hxc_self);

bool hxc_caxecraft_domain_GameSession_activateAuthoredItemDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_index);

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_actorControllerEventSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_self);

struct hxc_array_ref *hxc_caxecraft_domain_GameSession_actorControllerStateSnapshots(struct hxc_caxecraft_domain_GameSession *hxc_self);

struct hxc_caxecraft_domain_ActorControllerTickResult hxc_caxecraft_domain_GameSession_actorControllerTick(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_ActorControllerTickStatus hxc_status, int32_t hxc_processed);

bool hxc_caxecraft_domain_GameSession_actorInteractionAvailable(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id);

bool hxc_caxecraft_domain_GameSession_authoredItemIsActive(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_index);

const int32_t *hxc_caxecraft_domain_GameSession_authoredItemsView(struct hxc_caxecraft_domain_GameSession *hxc_self, size_t *hxc_returned_span_length);

bool hxc_caxecraft_domain_GameSession_bindLocalPlayer(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_Character hxc_character);

void hxc_caxecraft_domain_GameSession_clearActorControllerEvents(struct hxc_caxecraft_domain_GameSession *hxc_self);

struct hxc_caxecraft_domain_AuthoredAquaticEquipmentResult hxc_caxecraft_domain_GameSession_collectAuthoredAquaticEquipment(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_index, struct hxc_caxecraft_domain_AquaticProfile hxc_replacement);

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_commitLocalCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_Character hxc_replacement);

struct hxc_caxecraft_domain_CharacterDamageResult hxc_caxecraft_domain_GameSession_damageCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id, int32_t hxc_amount);

struct hxc_caxecraft_gameplay_MiningResult hxc_caxecraft_domain_GameSession_mineTerrain(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory);

bool hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_origin, int32_t hxc_width, int32_t hxc_height, int32_t hxc_depth);

bool hxc_caxecraft_domain_GameSession_placeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord, enum hxc_caxecraft_domain_BlockKind hxc_kind);

bool hxc_caxecraft_domain_GameSession_placeWaterSource(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord);

struct hxc_caxecraft_domain_Character hxc_caxecraft_domain_GameSession_readCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id);

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_receiveLocalPlayerAttack(struct hxc_caxecraft_domain_GameSession *hxc_self);

bool hxc_caxecraft_domain_GameSession_removeTerrain(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_scenario_VoxelPoint hxc_coord);

bool hxc_caxecraft_domain_GameSession_replaceAuthoredActors(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_array_ref *hxc_characters, struct hxc_array_ref *hxc_controllers);

void hxc_caxecraft_domain_GameSession_resetEmptyWorld(struct hxc_caxecraft_domain_GameSession *hxc_self);

struct hxc_caxecraft_domain_LocalCharacterCommandResult hxc_caxecraft_domain_GameSession_reviveLocalPlayerAt(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_CharacterBody hxc_body);

struct hxc_caxecraft_domain_ActorControllerTickResult hxc_caxecraft_domain_GameSession_stepAuthoredActorControllers(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_tickNumber, int32_t hxc_damagePolicy);

struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_caxecraft_domain_GameSession_stepCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_id, struct hxc_caxecraft_domain_CharacterIntent hxc_intent, int32_t hxc_damagePolicy);

struct hxc_caxecraft_domain_CharacterCommandStepResult hxc_caxecraft_domain_GameSession_stepStoredCharacter(struct hxc_caxecraft_domain_GameSession *hxc_self, const uint8_t *hxc_readCells, size_t hxc_length, struct hxc_caxecraft_domain_Character hxc_original, struct hxc_caxecraft_domain_CharacterIntent hxc_intent, int32_t hxc_damagePolicy);

struct hxc_caxecraft_domain_GameTickResult hxc_caxecraft_domain_GameSession_tick(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_GameTickInput hxc_input);

struct hxc_caxecraft_domain_LocalRecoveryResult hxc_caxecraft_domain_GameSession_useSelectedRecovery(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_gameplay_InventoryState hxc_inventory);

struct hxc_caxecraft_domain_GameView hxc_caxecraft_domain_GameSession_view(struct hxc_caxecraft_domain_GameSession *hxc_self);

const uint8_t *hxc_caxecraft_domain_GameSession_worldView(struct hxc_caxecraft_domain_GameSession *hxc_self, size_t *hxc_returned_span_length);

int32_t hxc_caxecraft_domain_GameSession_writeTerrainRunDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_startIndex, int32_t hxc_storageCode, int32_t hxc_count);

#endif /* HXC_CAXECRAFT_DOMAIN_GAME_SESSION_H_INCLUDED */
