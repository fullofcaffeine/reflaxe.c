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

struct hxc_caxecraft_domain_GameSession {
  struct hxc_caxecraft_domain_EntityStore hxc_entities;
  struct hxc_caxecraft_domain_PlayerAgent hxc_localPlayer;
  int32_t hxc_completedTicks;
  struct hxc_caxecraft_domain_WaterSimulation hxc_water;
  uint8_t hxc_worldStorage[16384];
  uint8_t hxc_pendingWaterStorage[16384];
  int32_t hxc_authoredItemStorage[256];
};

struct hxc_caxecraft_domain_GameTickInput {
  int32_t hxc_damagePolicy;
  struct hxc_caxecraft_domain_CharacterIntent hxc_intent;
  int32_t hxc_waterUpdateBudget;
};

struct hxc_caxecraft_domain_GameTickResult {
  struct hxc_caxecraft_domain_Character hxc_character;
  bool hxc_committed;
  int32_t hxc_drowningDamage;
  struct hxc_caxecraft_domain_Immersion hxc_immersion;
  int32_t hxc_tickIndex;
  struct hxc_caxecraft_domain_WaterTickResult hxc_water;
};

void hxc_compiler_constructor_caxecraft_domain_GameSession(struct hxc_caxecraft_domain_GameSession *hxc_self);

bool hxc_caxecraft_domain_GameSession_activateAuthoredItemDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_index);

bool hxc_caxecraft_domain_GameSession_bindLocalPlayer(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_Character hxc_character);

bool hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_BlockCoord hxc_origin, int32_t hxc_width, int32_t hxc_height, int32_t hxc_depth);

bool hxc_caxecraft_domain_GameSession_placeWaterSource(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_BlockCoord hxc_coord);

void hxc_caxecraft_domain_GameSession_resetEmptyWorld(struct hxc_caxecraft_domain_GameSession *hxc_self);

struct hxc_caxecraft_domain_GameTickResult hxc_caxecraft_domain_GameSession_tick(struct hxc_caxecraft_domain_GameSession *hxc_self, struct hxc_caxecraft_domain_GameTickInput hxc_input);

struct hxc_caxecraft_domain_GameView hxc_caxecraft_domain_GameSession_view(struct hxc_caxecraft_domain_GameSession *hxc_self);

int32_t hxc_caxecraft_domain_GameSession_writeTerrainRunDuringLoad(struct hxc_caxecraft_domain_GameSession *hxc_self, int32_t hxc_startIndex, int32_t hxc_storageCode, int32_t hxc_count);

#endif /* HXC_CAXECRAFT_DOMAIN_GAME_SESSION_H_INCLUDED */
