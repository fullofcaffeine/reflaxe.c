#include "hxc/program.h"

void hxc_caxecraft_app_HudDigits_drawDigit(int32_t hxc_l_digit, int32_t hxc_l_x, int32_t hxc_l_y, struct Color hxc_l_color)
{
  int32_t hxc_l_mask = 0;
  switch (hxc_l_digit) {
    case 0:
      {
        hxc_l_mask = 63;
        break;
      }
    case 1:
      {
        hxc_l_mask = 6;
        break;
      }
    case 2:
      {
        hxc_l_mask = 91;
        break;
      }
    case 3:
      {
        hxc_l_mask = 79;
        break;
      }
    case 4:
      {
        hxc_l_mask = 102;
        break;
      }
    case 5:
      {
        hxc_l_mask = 109;
        break;
      }
    case 6:
      {
        hxc_l_mask = 125;
        break;
      }
    case 7:
      {
        hxc_l_mask = 7;
        break;
      }
    case 8:
      {
        hxc_l_mask = 127;
        break;
      }
    case 9:
      {
        hxc_l_mask = 111;
        break;
      }
    default:
      {
        hxc_l_mask = 0;
        break;
      }
  }
  if (hxc_i32_bit_and(hxc_l_mask, 1) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_add_wrapping(hxc_l_x, 2), (int32_t)hxc_l_y, (int32_t)4, (int32_t)2, hxc_l_color);
  }
  if (hxc_i32_bit_and(hxc_l_mask, 2) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_x, 8), 2), (int32_t)hxc_i32_add_wrapping(hxc_l_y, 2), (int32_t)2, (int32_t)4, hxc_l_color);
  }
  if (hxc_i32_bit_and(hxc_l_mask, 4) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_x, 8), 2), (int32_t)hxc_i32_add_wrapping(hxc_l_y, 6), (int32_t)2, (int32_t)4, hxc_l_color);
  }
  if (hxc_i32_bit_and(hxc_l_mask, 8) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_add_wrapping(hxc_l_x, 2), (int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_y, 12), 2), (int32_t)4, (int32_t)2, hxc_l_color);
  }
  if (hxc_i32_bit_and(hxc_l_mask, 16) != 0)
  {
    DrawRectangle((int32_t)hxc_l_x, (int32_t)hxc_i32_add_wrapping(hxc_l_y, 6), (int32_t)2, (int32_t)4, hxc_l_color);
  }
  if (hxc_i32_bit_and(hxc_l_mask, 32) != 0)
  {
    DrawRectangle((int32_t)hxc_l_x, (int32_t)hxc_i32_add_wrapping(hxc_l_y, 2), (int32_t)2, (int32_t)4, hxc_l_color);
  }
  if (hxc_i32_bit_and(hxc_l_mask, 64) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_add_wrapping(hxc_l_x, 2), (int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_l_y, 6), 1), (int32_t)4, (int32_t)2, hxc_l_color);
  }
  return;
}

void hxc_caxecraft_app_HudDigits_drawNumber(int32_t hxc_l_value, int32_t hxc_l_x, int32_t hxc_l_y, int32_t hxc_l_digits, struct Color hxc_l_color)
{
  int32_t hxc_l_remaining = hxc_l_value;
  int32_t hxc_l_index = hxc_i32_subtract_wrapping(hxc_l_digits, 1);
  while (1)
  {
    if (!(hxc_l_index >= 0))
    {
      break;
    }
    int32_t hxc_l_digit = hxc_i32_modulo_zero_safe(hxc_l_remaining, 10);
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_digit;
    hxc_caxecraft_app_HudDigits_drawDigit(hxc_l_tmp_load_result_n2, hxc_i32_add_wrapping(hxc_l_x, hxc_i32_multiply_wrapping(hxc_l_index, 11)), hxc_l_y, hxc_l_color);
    hxc_l_remaining = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_l_remaining, (double)10));
    hxc_l_index = hxc_i32_subtract_wrapping(hxc_l_index, 1);
  }
  return;
}
