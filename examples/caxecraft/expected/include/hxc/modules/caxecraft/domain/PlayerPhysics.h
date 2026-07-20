#ifndef HXC_CAXECRAFT_DOMAIN_PLAYER_PHYSICS_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_PLAYER_PHYSICS_H_INCLUDED

#include "hxc/detail/program_types.h"

double hxc_caxecraft_domain_PlayerPhysics_abs(double hxc_value);

bool hxc_caxecraft_domain_PlayerPhysics_canPlaceAt(struct hxc_caxecraft_domain_PlayerState hxc_state, struct hxc_caxecraft_domain_BlockCoord hxc_coord);

int32_t hxc_caxecraft_domain_PlayerPhysics_floorToInt(double hxc_value);

struct hxc_caxecraft_domain_StepInput hxc_caxecraft_domain_PlayerPhysics_input(double hxc_moveX, double hxc_moveZ, bool hxc_jump);

struct hxc_caxecraft_domain_AxisMove hxc_caxecraft_domain_PlayerPhysics_moveAxis(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z, double hxc_delta, int32_t hxc_axis);

bool hxc_caxecraft_domain_PlayerPhysics_overlaps(uint8_t *hxc_cells, size_t hxc_length, double hxc_x, double hxc_y, double hxc_z);

struct hxc_caxecraft_domain_PlayerState hxc_caxecraft_domain_PlayerPhysics_player(double hxc_x, double hxc_y, double hxc_z);

struct hxc_caxecraft_domain_PlayerState hxc_caxecraft_domain_PlayerPhysics_recoverSpawn(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_PlayerState hxc_state);

struct hxc_caxecraft_domain_PlayerState hxc_caxecraft_domain_PlayerPhysics_step(uint8_t *hxc_cells, size_t hxc_length, struct hxc_caxecraft_domain_PlayerState hxc_original, struct hxc_caxecraft_domain_StepInput hxc_command);

#endif /* HXC_CAXECRAFT_DOMAIN_PLAYER_PHYSICS_H_INCLUDED */
