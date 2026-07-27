#include "hxc/program.h"

enum hxc_caxecraft_domain_BlockKind hxc_caxecraft_domain_WorldRead_query(const uint8_t *hxc_view, size_t hxc_length, struct hxc_caxecraft_domain_BlockCoord hxc_coord)
{
  const uint8_t *hxc_borrow = hxc_view;
  size_t hxc_tmp_length_n2 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n2;
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_domain_World_indexOf(hxc_coord);
  int32_t hxc_index = hxc_tmp_call_result_n0;
  if (!(hxc_index < 0))
  {
    int32_t hxc_tmp_call_result_n4 = hxc_caxecraft_domain_WorldRead_storageCodeAt(hxc_view, hxc_length, hxc_index);
    enum hxc_caxecraft_domain_BlockKind hxc_tmp_call_result_n5 = hxc_caxecraft_domain_World_kindFromCode(hxc_tmp_call_result_n4);
    return hxc_tmp_call_result_n5;
  }
  return hxc_caxecraft_domain_BlockKind_Air;
}

int32_t hxc_caxecraft_domain_WorldRead_storageCodeAt(const uint8_t *hxc_view, size_t hxc_length, int32_t hxc_index)
{
  const uint8_t *hxc_borrow = hxc_view;
  size_t hxc_tmp_length_n2 = hxc_length;
  if (hxc_index < 0 || (size_t)hxc_index >= hxc_tmp_length_n2)
  {
    abort();
  }
  uint8_t hxc_tmp_collection_index_load_result_n0 = hxc_borrow[(size_t)hxc_index];
  return (int32_t)hxc_tmp_collection_index_load_result_n0;
}
