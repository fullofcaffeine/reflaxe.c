#include "hxc/program.h"

void hxc_caxecraft_app_Main_main(void)
{
  struct hxc_caxecraft_app_CaxecraftApp hxc_l_tmp_object_storage_n1 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp *hxc_l_tmp_class_object_address_n0 = &hxc_l_tmp_object_storage_n1;
  hxc_compiler_constructor_caxecraft_app_CaxecraftApp(hxc_l_tmp_class_object_address_n0);
  struct hxc_caxecraft_app_CaxecraftApp *hxc_l_app = hxc_l_tmp_class_object_address_n0;
  struct hxc_caxecraft_app_CaxecraftApp *hxc_l_tmp_load_result_n1 = hxc_l_app;
  if (hxc_l_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  hxc_caxecraft_app_CaxecraftApp_run(hxc_l_tmp_load_result_n1);
  if (hxc_array_ref_release(hxc_l_tmp_object_storage_n1.hxc_waterRenderer.hxc_faces) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
