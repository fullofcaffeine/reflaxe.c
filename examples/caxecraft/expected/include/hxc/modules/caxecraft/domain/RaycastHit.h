#ifndef HXC_CAXECRAFT_DOMAIN_RAYCAST_HIT_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_RAYCAST_HIT_H_INCLUDED

#include "hxc/detail/program_types.h"

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

#endif /* HXC_CAXECRAFT_DOMAIN_RAYCAST_HIT_H_INCLUDED */
