#ifndef HXC_CAXECRAFT_DOMAIN_VOXEL_RAYCAST_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_VOXEL_RAYCAST_H_INCLUDED

#include "hxc/detail/program_types.h"

double hxc_caxecraft_domain_VoxelRaycast_abs(double hxc_l_value);

int32_t hxc_caxecraft_domain_VoxelRaycast_floorToInt(double hxc_l_value);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_hit(int32_t hxc_l_cellX, int32_t hxc_l_cellY, int32_t hxc_l_cellZ, int32_t hxc_l_previousX, int32_t hxc_l_previousY, int32_t hxc_l_previousZ, int32_t hxc_l_normalX, int32_t hxc_l_normalY, int32_t hxc_l_normalZ, double hxc_l_distance, int32_t hxc_l_visited);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_miss(int32_t hxc_l_previousX, int32_t hxc_l_previousY, int32_t hxc_l_previousZ, double hxc_l_distance, int32_t hxc_l_visited);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_trace(const uint8_t *hxc_l_cells, size_t hxc_l_length, double hxc_l_originX, double hxc_l_originY, double hxc_l_originZ, double hxc_l_directionX, double hxc_l_directionY, double hxc_l_directionZ, double hxc_l_maximumDistance);

#endif /* HXC_CAXECRAFT_DOMAIN_VOXEL_RAYCAST_H_INCLUDED */
