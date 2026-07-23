#ifndef HXC_CAXECRAFT_APP_MOTION_INTERPOLATION_H_INCLUDED
#define HXC_CAXECRAFT_APP_MOTION_INTERPOLATION_H_INCLUDED

#include "hxc/detail/program_types.h"

struct hxc_caxecraft_app_RenderPosition {
  double hxc_x;
  double hxc_y;
  double hxc_z;
};

struct hxc_caxecraft_app_MotionHistory {
  struct hxc_caxecraft_app_RenderPosition hxc_current;
  struct hxc_caxecraft_app_RenderPosition hxc_previous;
};

struct hxc_caxecraft_app_MotionHistory hxc_caxecraft_app_MotionInterpolation_advance(struct hxc_caxecraft_app_MotionHistory hxc_history, struct hxc_caxecraft_domain_CharacterBody hxc_body);

struct hxc_caxecraft_app_RenderPosition hxc_caxecraft_app_MotionInterpolation_sample(struct hxc_caxecraft_app_MotionHistory hxc_history, double hxc_remainderSeconds, double hxc_fixedSeconds);

struct hxc_caxecraft_app_MotionHistory hxc_caxecraft_app_MotionInterpolation_start(struct hxc_caxecraft_domain_CharacterBody hxc_body);

#endif /* HXC_CAXECRAFT_APP_MOTION_INTERPOLATION_H_INCLUDED */
