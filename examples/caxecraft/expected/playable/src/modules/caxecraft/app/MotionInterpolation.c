#include "hxc/program.h"

struct hxc_caxecraft_app_MotionHistory hxc_caxecraft_app_MotionInterpolation_advance(struct hxc_caxecraft_app_MotionHistory hxc_history, struct hxc_caxecraft_domain_CharacterBody hxc_body)
{
  return (struct hxc_caxecraft_app_MotionHistory){ .hxc_current = (struct hxc_caxecraft_app_RenderPosition){ .hxc_x = hxc_body.hxc_x, .hxc_y = hxc_body.hxc_y, .hxc_z = hxc_body.hxc_z }, .hxc_previous = hxc_history.hxc_current };
}

struct hxc_caxecraft_app_RenderPosition hxc_caxecraft_app_MotionInterpolation_sample(struct hxc_caxecraft_app_MotionHistory hxc_history, double hxc_remainderSeconds, double hxc_fixedSeconds)
{
  if (!(hxc_fixedSeconds <= 0.0))
  {
    double hxc_fraction = hxc_f64_divide_zero_safe(hxc_remainderSeconds, hxc_fixedSeconds);
    if (hxc_fraction < 0.0)
    {
      hxc_fraction = 0.0;
    }
    if (hxc_fraction > 1.0)
    {
      hxc_fraction = 1.0;
    }
    double hxc_start = hxc_history.hxc_previous.hxc_x;
    double hxc_start1 = hxc_history.hxc_previous.hxc_y;
    double hxc_start2 = hxc_history.hxc_previous.hxc_z;
    double hxc_tmp_load_result_n9 = hxc_start;
    double hxc_tmp_load_result_n12 = hxc_start;
    double hxc_tmp_load_result_n13 = hxc_fraction;
    double hxc_tmp_load_result_n14 = hxc_start1;
    double hxc_tmp_load_result_n17 = hxc_start1;
    double hxc_tmp_load_result_n18 = hxc_fraction;
    double hxc_tmp_load_result_n19 = hxc_start2;
    double hxc_tmp_load_result_n22 = hxc_start2;
    return (struct hxc_caxecraft_app_RenderPosition){ .hxc_x = hxc_tmp_load_result_n9 + (hxc_history.hxc_current.hxc_x - hxc_tmp_load_result_n12) * hxc_tmp_load_result_n13, .hxc_y = hxc_tmp_load_result_n14 + (hxc_history.hxc_current.hxc_y - hxc_tmp_load_result_n17) * hxc_tmp_load_result_n18, .hxc_z = hxc_tmp_load_result_n19 + (hxc_history.hxc_current.hxc_z - hxc_tmp_load_result_n22) * hxc_fraction };
  }
  return hxc_history.hxc_current;
}

struct hxc_caxecraft_app_MotionHistory hxc_caxecraft_app_MotionInterpolation_start(struct hxc_caxecraft_domain_CharacterBody hxc_body)
{
  struct hxc_caxecraft_app_RenderPosition hxc_position = (struct hxc_caxecraft_app_RenderPosition){ .hxc_x = hxc_body.hxc_x, .hxc_y = hxc_body.hxc_y, .hxc_z = hxc_body.hxc_z };
  struct hxc_caxecraft_app_RenderPosition hxc_tmp_load_result_n4 = hxc_position;
  return (struct hxc_caxecraft_app_MotionHistory){ .hxc_current = hxc_position, .hxc_previous = hxc_tmp_load_result_n4 };
}
