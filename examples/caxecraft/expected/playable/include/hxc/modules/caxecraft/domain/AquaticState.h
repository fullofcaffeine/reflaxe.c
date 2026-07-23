#ifndef HXC_CAXECRAFT_DOMAIN_AQUATIC_STATE_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_AQUATIC_STATE_H_INCLUDED

#include "hxc/detail/program_types.h"

struct hxc_caxecraft_domain_AquaticState {
  int32_t hxc_breathTicks;
  int32_t hxc_drowningTicks;
  bool hxc_headSubmerged;
  int32_t hxc_medium;
  double hxc_submersion;
};

#endif /* HXC_CAXECRAFT_DOMAIN_AQUATIC_STATE_H_INCLUDED */
