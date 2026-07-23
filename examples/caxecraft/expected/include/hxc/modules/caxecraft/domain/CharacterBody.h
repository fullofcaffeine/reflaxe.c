#ifndef HXC_CAXECRAFT_DOMAIN_CHARACTER_BODY_H_INCLUDED
#define HXC_CAXECRAFT_DOMAIN_CHARACTER_BODY_H_INCLUDED

#include "hxc/detail/program_types.h"

struct hxc_caxecraft_domain_CharacterBody {
  bool hxc_grounded;
  double hxc_velocityX;
  double hxc_velocityY;
  double hxc_velocityZ;
  double hxc_x;
  double hxc_y;
  double hxc_z;
};

#endif /* HXC_CAXECRAFT_DOMAIN_CHARACTER_BODY_H_INCLUDED */
