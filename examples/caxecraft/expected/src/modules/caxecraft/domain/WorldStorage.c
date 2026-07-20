#include "hxc/program.h"

int32_t hxc_caxecraft_domain_WorldStorage_readCode(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_index)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n2 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n0 = hxc_borrow[(size_t)hxc_index];
  return (int32_t)hxc_tmp_collection_index_load_result_n0;
}

void hxc_caxecraft_domain_WorldStorage_writeCode(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_index, int32_t hxc_code)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n3 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n3)
  {
    abort();
  }
  hxc_borrow[(size_t)hxc_index] = (uint8_t)hxc_code;
  return;
}
