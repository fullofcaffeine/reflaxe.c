#ifndef HXC_CAXECRAFT_DOMAIN_VOXEL_RAYCAST_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_VOXEL_RAYCAST_H_INCLUDED

#include "hxc/detail/program_types.h"

double hxc_caxecraft_domain_VoxelRaycast_abs(double hxc_value);

int32_t hxc_caxecraft_domain_VoxelRaycast_floorToInt(double hxc_value);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_hit(int32_t hxc_cellX, int32_t hxc_cellY, int32_t hxc_cellZ, int32_t hxc_previousX, int32_t hxc_previousY, int32_t hxc_previousZ, int32_t hxc_normalX, int32_t hxc_normalY, int32_t hxc_normalZ, double hxc_distance, int32_t hxc_visited);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_miss(int32_t hxc_previousX, int32_t hxc_previousY, int32_t hxc_previousZ, double hxc_distance, int32_t hxc_visited);

struct hxc_caxecraft_domain_RaycastHit hxc_caxecraft_domain_VoxelRaycast_trace(uint8_t *hxc_cells, size_t hxc_length, double hxc_originX, double hxc_originY, double hxc_originZ, double hxc_directionX, double hxc_directionY, double hxc_directionZ, double hxc_maximumDistance);

#endif /* HXC_CAXECRAFT_DOMAIN_VOXEL_RAYCAST_H_INCLUDED */
