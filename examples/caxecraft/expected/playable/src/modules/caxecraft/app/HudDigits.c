#include "hxc/program.h"

void hxc_caxecraft_app_HudDigits_drawDigit(int32_t hxc_digit, int32_t hxc_x, int32_t hxc_y, struct Color hxc_color)
{
  int32_t hxc_mask = 0;
  switch (hxc_digit) {
    case 0:
      {
        hxc_mask = 63;
        break;
      }
    case 1:
      {
        hxc_mask = 6;
        break;
      }
    case 2:
      {
        hxc_mask = 91;
        break;
      }
    case 3:
      {
        hxc_mask = 79;
        break;
      }
    case 4:
      {
        hxc_mask = 102;
        break;
      }
    case 5:
      {
        hxc_mask = 109;
        break;
      }
    case 6:
      {
        hxc_mask = 125;
        break;
      }
    case 7:
      {
        hxc_mask = 7;
        break;
      }
    case 8:
      {
        hxc_mask = 127;
        break;
      }
    case 9:
      {
        hxc_mask = 111;
        break;
      }
    default:
      {
        hxc_mask = 0;
        break;
      }
  }
  if (hxc_i32_bit_and(hxc_mask, 1) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_add_wrapping(hxc_x, 2), (int32_t)hxc_y, (int32_t)4, (int32_t)2, hxc_color);
  }
  if (hxc_i32_bit_and(hxc_mask, 2) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_x, 8), 2), (int32_t)hxc_i32_add_wrapping(hxc_y, 2), (int32_t)2, (int32_t)4, hxc_color);
  }
  if (hxc_i32_bit_and(hxc_mask, 4) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_x, 8), 2), (int32_t)hxc_i32_add_wrapping(hxc_y, 6), (int32_t)2, (int32_t)4, hxc_color);
  }
  if (hxc_i32_bit_and(hxc_mask, 8) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_add_wrapping(hxc_x, 2), (int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_y, 12), 2), (int32_t)4, (int32_t)2, hxc_color);
  }
  if (hxc_i32_bit_and(hxc_mask, 16) != 0)
  {
    DrawRectangle((int32_t)hxc_x, (int32_t)hxc_i32_add_wrapping(hxc_y, 6), (int32_t)2, (int32_t)4, hxc_color);
  }
  if (hxc_i32_bit_and(hxc_mask, 32) != 0)
  {
    DrawRectangle((int32_t)hxc_x, (int32_t)hxc_i32_add_wrapping(hxc_y, 2), (int32_t)2, (int32_t)4, hxc_color);
  }
  if (hxc_i32_bit_and(hxc_mask, 64) != 0)
  {
    DrawRectangle((int32_t)hxc_i32_add_wrapping(hxc_x, 2), (int32_t)hxc_i32_subtract_wrapping(hxc_i32_add_wrapping(hxc_y, 6), 1), (int32_t)4, (int32_t)2, hxc_color);
  }
  return;
}

void hxc_caxecraft_app_HudDigits_drawNumber(int32_t hxc_value, int32_t hxc_x, int32_t hxc_y, int32_t hxc_digits, struct Color hxc_color)
{
  int32_t hxc_remaining = hxc_value;
  int32_t hxc_index = hxc_i32_subtract_wrapping(hxc_digits, 1);
  while (1)
  {
    if (!(hxc_index >= 0))
    {
      break;
    }
    int32_t hxc_digit = hxc_i32_modulo_zero_safe(hxc_remaining, 10);
    int32_t hxc_tmp_load_result_n2 = hxc_digit;
    hxc_caxecraft_app_HudDigits_drawDigit(hxc_tmp_load_result_n2, hxc_i32_add_wrapping(hxc_x, hxc_i32_multiply_wrapping(hxc_index, 11)), hxc_y, hxc_color);
    hxc_remaining = hxc_f64_to_i32_saturating(hxc_f64_divide_zero_safe((double)hxc_remaining, (double)10));
    hxc_index = hxc_i32_subtract_wrapping(hxc_index, 1);
  }
  return;
}
