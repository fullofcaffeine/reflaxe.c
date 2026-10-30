bool hxc_Main_valid(struct hxc_array_ref *hxc_l_cells)
{
  int32_t hxc_l_index = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_index;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_cells, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_Main_CellState hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_cells, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    switch (hxc_l_tmp_array_get_result_n3.hxc_tag) {
      case hxc_Main_CellState_Empty:
      case hxc_Main_CellState_Solid:
      case hxc_Main_CellState_Water:
        {
          break;
        }
      case hxc_Main_CellState_InvalidStorage:
        {
          return false;
        }
      default:
        {
          abort();
        }
    }
    hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
  }
  return true;
}
