#ifndef HXC_CAXECRAFT_GAMEPLAY_GUIDE_STATE_H_INCLUDED
#define HXC_CAXECRAFT_GAMEPLAY_GUIDE_STATE_H_INCLUDED

#include "hxc/detail/program_types.h"
#include "hxc/modules/caxecraft/gameplay/GuidePhase.h"

struct hxc_caxecraft_gameplay_GuideState {
  enum hxc_caxecraft_gameplay_GuidePhase hxc_phase;
  double hxc_x;
  double hxc_y;
  double hxc_z;
};

#endif /* HXC_CAXECRAFT_GAMEPLAY_GUIDE_STATE_H_INCLUDED */
