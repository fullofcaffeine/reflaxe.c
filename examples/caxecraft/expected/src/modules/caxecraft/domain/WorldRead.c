#include "hxc/program.h"

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_WorldRead_query(const uint8_t *hxc_l_view, size_t hxc_l_length, struct hxc_caxecraft_domain_BlockCoord hxc_l_coord)
{
  const uint8_t *hxc_l_borrow = hxc_l_view;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  (void)hxc_l_borrow;
  (void)hxc_l_tmp_length_n2;
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_domain_World_indexOf(hxc_l_coord);
  int32_t hxc_l_index = hxc_l_tmp_call_result_n0;
  if (!(hxc_l_index < 0))
  {
    int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_domain_WorldRead_storageCodeAt(hxc_l_view, hxc_l_length, hxc_l_index);
    enum hxc_caxecraft_domain_BlockKind hxc_l_tmp_call_result_n5 = hxc_caxecraft_domain_World_kindFromCode(hxc_l_tmp_call_result_n4);
    return hxc_l_tmp_call_result_n5;
  }
  return hxc_caxecraft_domain_BlockKind_Air;
}

int32_t hxc_caxecraft_domain_WorldRead_storageCodeAt(const uint8_t *hxc_l_view, size_t hxc_l_length, int32_t hxc_l_index)
{
  const uint8_t *hxc_l_borrow = hxc_l_view;
  size_t hxc_l_tmp_length_n2 = hxc_l_length;
  if (hxc_l_index < 0 || (size_t)hxc_l_index >= hxc_l_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_l_tmp_collection_index_load_result_n0 = hxc_l_borrow[(size_t)hxc_l_index];
  return (int32_t)hxc_l_tmp_collection_index_load_result_n0;
}
