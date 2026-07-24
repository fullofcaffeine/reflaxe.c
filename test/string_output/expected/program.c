#include "hxc/program.h"

void hxc_Main_main(void)
{
  hxc_string hxc_tmp_call_result_n0 = hxc_Main_selectLabel(true);
  bool hxc_tmp_short_circuit_result_n0 = hxc_tmp_call_result_n0.byte_length == (hxc_string){ (const uint8_t *)"ASCII", 5, true }.byte_length && (hxc_tmp_call_result_n0.byte_length == 0 || memcmp(hxc_tmp_call_result_n0.data, (hxc_string){ (const uint8_t *)"ASCII", 5, true }.data, hxc_tmp_call_result_n0.byte_length) == 0);
  if (hxc_tmp_call_result_n0.byte_length == (hxc_string){ (const uint8_t *)"ASCII", 5, true }.byte_length && (hxc_tmp_call_result_n0.byte_length == 0 || memcmp(hxc_tmp_call_result_n0.data, (hxc_string){ (const uint8_t *)"ASCII", 5, true }.data, hxc_tmp_call_result_n0.byte_length) == 0))
  {
    hxc_string hxc_tmp_call_result_n2 = hxc_Main_selectLabel(false);
    hxc_tmp_short_circuit_result_n0 = hxc_tmp_call_result_n2.byte_length == (hxc_string){ (const uint8_t *)"fallback", 8, true }.byte_length && (hxc_tmp_call_result_n2.byte_length == 0 || memcmp(hxc_tmp_call_result_n2.data, (hxc_string){ (const uint8_t *)"fallback", 8, true }.data, hxc_tmp_call_result_n2.byte_length) == 0);
  }
  if (hxc_tmp_short_circuit_result_n0)
  {
    if (hxc_io_println((hxc_string){ (const uint8_t *)"ASCII", 5, true }) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  else
  {
    if (hxc_io_println((hxc_string){ (const uint8_t *)"conditional string failure", 26, true }) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_io_println((hxc_string){ (const uint8_t *)"\303\251\360\237\231\202", 6, true }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_io_println((hxc_string){ (const uint8_t *)"embedded\000NUL", 12, true }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_io_println((hxc_string){ (const uint8_t *)"Main.hx:9: traced", 17, true }) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

hxc_string hxc_Main_selectLabel(bool hxc_primary)
{
  hxc_string hxc_tmp_conditional_result_n2;
  if (hxc_primary)
  {
    hxc_tmp_conditional_result_n2 = (hxc_string){ (const uint8_t *)"ASCII", 5, true };
  }
  else
  {
    hxc_tmp_conditional_result_n2 = (hxc_string){ (const uint8_t *)"fallback", 8, true };
  }
  hxc_string hxc_selected = hxc_tmp_conditional_result_n2;
  return hxc_selected;
}

int main(void)
{
  hxc_Main_main();
  return 0;
}
