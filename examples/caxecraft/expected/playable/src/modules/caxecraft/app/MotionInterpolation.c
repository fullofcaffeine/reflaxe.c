#include "hxc/program.h"

struct hxc_caxecraft_app_MotionHistory hxc_caxecraft_app_MotionInterpolation_advance(struct hxc_caxecraft_app_MotionHistory hxc_l_history, struct hxc_caxecraft_domain_CharacterBody hxc_l_body)
{
  return (struct hxc_caxecraft_app_MotionHistory){ .hxc_current = (struct hxc_caxecraft_app_RenderPosition){ .hxc_x = hxc_l_body.hxc_x, .hxc_y = hxc_l_body.hxc_y, .hxc_z = hxc_l_body.hxc_z }, .hxc_previous = hxc_l_history.hxc_current };
}

struct hxc_caxecraft_app_RenderPosition hxc_caxecraft_app_MotionInterpolation_sample(struct hxc_caxecraft_app_MotionHistory hxc_l_history, double hxc_l_remainderSeconds, double hxc_l_fixedSeconds)
{
  if (hxc_l_fixedSeconds <= 0.0)
  {
    return hxc_l_history.hxc_current;
  }
  double hxc_l_fraction = hxc_f64_divide_zero_safe(hxc_l_remainderSeconds, hxc_l_fixedSeconds);
  if (hxc_l_fraction < 0.0)
  {
    hxc_l_fraction = 0.0;
  }
  if (hxc_l_fraction > 1.0)
  {
    hxc_l_fraction = 1.0;
  }
  double hxc_l_start = hxc_l_history.hxc_previous.hxc_x;
  double hxc_l_start1 = hxc_l_history.hxc_previous.hxc_y;
  double hxc_l_start2 = hxc_l_history.hxc_previous.hxc_z;
  double hxc_l_tmp_load_result_n9 = hxc_l_start;
  double hxc_l_tmp_load_result_n12 = hxc_l_start;
  double hxc_l_tmp_load_result_n13 = hxc_l_fraction;
  double hxc_l_tmp_load_result_n14 = hxc_l_start1;
  double hxc_l_tmp_load_result_n17 = hxc_l_start1;
  double hxc_l_tmp_load_result_n18 = hxc_l_fraction;
  double hxc_l_tmp_load_result_n19 = hxc_l_start2;
  double hxc_l_tmp_load_result_n22 = hxc_l_start2;
  return (struct hxc_caxecraft_app_RenderPosition){ .hxc_x = hxc_l_tmp_load_result_n9 + (hxc_l_history.hxc_current.hxc_x - hxc_l_tmp_load_result_n12) * hxc_l_tmp_load_result_n13, .hxc_y = hxc_l_tmp_load_result_n14 + (hxc_l_history.hxc_current.hxc_y - hxc_l_tmp_load_result_n17) * hxc_l_tmp_load_result_n18, .hxc_z = hxc_l_tmp_load_result_n19 + (hxc_l_history.hxc_current.hxc_z - hxc_l_tmp_load_result_n22) * hxc_l_fraction };
}

struct hxc_caxecraft_app_MotionHistory hxc_caxecraft_app_MotionInterpolation_start(struct hxc_caxecraft_domain_CharacterBody hxc_l_body)
{
  struct hxc_caxecraft_app_RenderPosition hxc_l_position = (struct hxc_caxecraft_app_RenderPosition){ .hxc_x = hxc_l_body.hxc_x, .hxc_y = hxc_l_body.hxc_y, .hxc_z = hxc_l_body.hxc_z };
  struct hxc_caxecraft_app_RenderPosition hxc_l_tmp_load_result_n4 = hxc_l_position;
  return (struct hxc_caxecraft_app_MotionHistory){ .hxc_current = hxc_l_position, .hxc_previous = hxc_l_tmp_load_result_n4 };
}
