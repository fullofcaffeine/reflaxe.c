#include "hxc/program.h"

int32_t hxc_caxecraft_domain_WorldStorage_readCode(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_index)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_borrow[(size_t)hxc_l_index];
  return (int32_t)hxc_l_tmp_collection_index_load_result_n0;
}

void hxc_caxecraft_domain_WorldStorage_writeCode(uint8_t *hxc_l_cells, size_t hxc_l_length, int32_t hxc_l_index, int32_t hxc_l_code)
{
  uint8_t *hxc_l_borrow = hxc_l_cells;
  size_t hxc_l_tmp_length_n3 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n3)
  {
    abort();
  }
  hxc_l_borrow[(size_t)hxc_l_index] = (uint8_t)hxc_l_code;
  return;
}
