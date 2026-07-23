#ifndef HXC_CAXECRAFT_DOMAIN_CHARACTER_PHYSICS_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_CHARACTER_PHYSICS_H_INCLUDED

#include "hxc/detail/program_types.h"

double hxc_caxecraft_domain_CharacterPhysics_abs(double hxc_value);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_body(double hxc_x, double hxc_y, double hxc_z);

bool hxc_caxecraft_domain_CharacterPhysics_canPlaceAt(struct hxc_caxecraft_domain_CharacterBody hxc_state, struct hxc_caxecraft_domain_BlockCoord hxc_coord);

int32_t hxc_caxecraft_domain_CharacterPhysics_floorToInt(double hxc_value);

struct hxc_caxecraft_domain_StepInput hxc_caxecraft_domain_CharacterPhysics_input(double hxc_moveX, double hxc_moveZ, bool hxc_jump);

struct hxc_caxecraft_domain_AxisMove hxc_caxecraft_domain_CharacterPhysics_moveAxis(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z, double hxc_delta, int32_t hxc_axis);

bool hxc_caxecraft_domain_CharacterPhysics_overlaps(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_recoverSpawn(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_state);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_resolveVelocity(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_state, double hxc_velocityX, double hxc_requestedVelocityY, double hxc_velocityZ);

struct hxc_caxecraft_domain_CharacterBody hxc_caxecraft_domain_CharacterPhysics_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_CharacterBody hxc_original, struct hxc_caxecraft_domain_StepInput hxc_command);

#endif /* HXC_CAXECRAFT_DOMAIN_CHARACTER_PHYSICS_H_INCLUDED */
