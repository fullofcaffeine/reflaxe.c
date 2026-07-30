#ifndef HXC_PACKAGES_CAXECRAFT_DOMAIN_PACKAGE_H_INCLUDED
#define HXC_PACKAGES_CAXECRAFT_DOMAIN_PACKAGE_H_INCLUDED

#include "hxc/detail/program_types.h"

struct hxc_caxecraft_domain_BlockCoord {
  int32_t hxc_x;
  int32_t hxc_y;
  int32_t hxc_z;
};

struct hxc_caxecraft_domain_RaycastHit {
  int32_t hxc_cellX;
  int32_t hxc_cellY;
  int32_t hxc_cellZ;
  double hxc_distance;
  bool hxc_hit;
  int32_t hxc_normalX;
  int32_t hxc_normalY;
  int32_t hxc_normalZ;
  int32_t hxc_previousX;
  int32_t hxc_previousY;
  int32_t hxc_previousZ;
  int32_t hxc_visited;
};

struct hxc_caxecraft_domain_StepInput {
  bool hxc_jump;
  double hxc_moveX;
  double hxc_moveZ;
};

struct hxc_caxecraft_domain_AxisMove {
  bool hxc_blocked;
  double hxc_x;
  double hxc_y;
  double hxc_z;
};

struct hxc_caxecraft_domain_CharacterBody {
  bool hxc_grounded;
  double hxc_velocityX;
  double hxc_velocityY;
  double hxc_velocityZ;
  double hxc_x;
  double hxc_y;
  double hxc_z;
};

int32_t hxc_caxecraft_domain_CaxecraftTrace_collisionTrace(void);

int32_t hxc_caxecraft_domain_CaxecraftTrace_editTrace(void);

void hxc_caxecraft_domain_CaxecraftTrace_makeFloor(uint8_t *hxc_l_cells, size_t hxc_l_length);

int32_t hxc_caxecraft_domain_CaxecraftTrace_mix(int32_t hxc_l_hash, int32_t hxc_l_value);

int32_t hxc_caxecraft_domain_CaxecraftTrace_propertyTrace(int32_t hxc_l_seed);

int32_t hxc_caxecraft_domain_CaxecraftTrace_rayTrace(void);

int32_t hxc_caxecraft_domain_CaxecraftTrace_runTrace(void);

int32_t hxc_caxecraft_domain_CaxecraftTrace_terrainTrace(void);

double hxc_caxecraft_domain_VoxelRaycast_abs(double hxc_l_value);

int32_t hxc_caxecraft_domain_VoxelRaycast_floorToInt(double hxc_l_value);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_hit(int32_t hxc_l_cellX, int32_t hxc_l_cellY, int32_t hxc_l_cellZ, int32_t hxc_l_previousX, int32_t hxc_l_previousY, int32_t hxc_l_previousZ, int32_t hxc_l_normalX, int32_t hxc_l_normalY, int32_t hxc_l_normalZ, double hxc_l_distance, int32_t hxc_l_visited);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_miss(int32_t hxc_l_previousX, int32_t hxc_l_previousY, int32_t hxc_l_previousZ, double hxc_l_distance, int32_t hxc_l_visited);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_trace(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_originX, double hxc_l_originY, double hxc_l_originZ, double hxc_l_directionX, double hxc_l_directionY, double hxc_l_directionZ, double hxc_l_maximumDistance);

bool hxc_caxecraft_domain_World_contains(struct hxc_caxecraft_domain_BlockCoord hxc_l_coord);

struct hxc_caxecraft_domain_BlockCoord hxc_caxecraft_domain_World_coord(int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_z);

void hxc_caxecraft_domain_World_generate(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_seed);

int32_t hxc_caxecraft_domain_World_indexOf(struct hxc_caxecraft_domain_BlockCoord hxc_l_coord);

bool hxc_caxecraft_domain_World_isPlaceable(enum hxc_caxecraft_domain_BlockKind hxc_l_kind);

bool hxc_caxecraft_domain_World_isSolid(enum hxc_caxecraft_domain_BlockKind hxc_l_kind);

int32_t hxc_caxecraft_domain_World_kindCode(enum hxc_caxecraft_domain_BlockKind hxc_l_kind);

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_kindFromCode(int32_t hxc_l_code);

bool hxc_caxecraft_domain_World_place(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind);

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_World_query(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord);

bool hxc_caxecraft_domain_World_remove(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord);

bool hxc_caxecraft_domain_World_replace(uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord, enum hxc_caxecraft_domain_BlockKind hxc_l_kind);

int32_t hxc_caxecraft_domain_World_stateHash(uint8_t *hxc_l_cells, size_t hxc_l_length);

int32_t hxc_caxecraft_domain_World_terrainNoise(int32_t hxc_l_seed, int32_t hxc_l_x, int32_t hxc_l_z);

int32_t hxc_caxecraft_domain_WorldStorage_readCode(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_index);

void hxc_caxecraft_domain_WorldStorage_writeCode(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_index, int32_t hxc_l_code);

double hxc_caxecraft_domain_CharacterPhysics_abs(double hxc_l_value);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_body(double hxc_l_x, double hxc_l_y, double hxc_l_z);

bool hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(struct hxc_caxecraft_domain_CharacterBody hxc_l_state, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord);

int32_t hxc_caxecraft_domain_CharacterPhysics_floorToInt(double hxc_l_value);

struct hxc_caxecraft_domain_StepInput hxc_caxecraft_domain_CharacterPhysics_input(double hxc_l_moveX, double hxc_l_moveZ, bool hxc_l_jump);

struct hxc_caxecraft_domain_AxisMove hxc_caxecraft_domain_CharacterPhysics_moveAxis(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_x, double hxc_l_y, double hxc_l_z, double hxc_l_delta, int32_t hxc_l_axis);

bool hxc_caxecraft_domain_CharacterPhysics_overlaps(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_x, double hxc_l_y, double hxc_l_z);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_CharacterBody hxc_l_state);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_resolveVelocity(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_CharacterBody hxc_l_state, double hxc_l_velocityX, double hxc_l_requestedVelocityY, double hxc_l_velocityZ);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_step(const uint8_t *hxc_l_cells, size_t hxc_l_length, struct hxc_caxecraft_domain_CharacterBody hxc_l_original, struct hxc_caxecraft_domain_StepInput hxc_l_command);

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_WorldRead_query(const uint8_t *hxc_l_view, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord);

int32_t hxc_caxecraft_domain_WorldRead_storageCodeAt(const uint8_t *hxc_l_view, size_t hxc_l_length, int32_t hxc_l_index);

#endif /* HXC_PACKAGES_CAXECRAFT_DOMAIN_PACKAGE_H_INCLUDED */
