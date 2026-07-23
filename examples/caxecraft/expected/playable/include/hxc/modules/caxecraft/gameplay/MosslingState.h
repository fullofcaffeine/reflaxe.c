#ifndef HXC_CAXECRAFT_GAMEPLAY_MOSSLING_STATE_H_INCLUDED
#define HXC_CAXECRAFT_GAMEPLAY_MOSSLING_STATE_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/gameplay/MosslingMode.h"

struct hxc_caxecraft_gameplay_MosslingState {
  int32_t hxc_health;
  double hxc_homeX;
  double hxc_homeZ;
  enum hxc_caxecraft_gameplay_MosslingMode hxc_mode;
  int32_t hxc_phaseTicks;
  double hxc_x;
  double hxc_y;
  double hxc_z;
};

#endif /* HXC_CAXECRAFT_GAMEPLAY_MOSSLING_STATE_H_INCLUDED */
