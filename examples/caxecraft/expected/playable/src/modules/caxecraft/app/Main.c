#include "hxc/program.h"

void hxc_caxecraft_app_Main_main(void)
{
  struct hxc_caxecraft_app_CaxecraftApp hxc_tmp_object_storage_n1 = { 0 };
  struct hxc_caxecraft_app_CaxecraftApp *hxc_tmp_class_object_address_n0 = &hxc_tmp_object_storage_n1;
  hxc_compiler_constructor_caxecraft_app_CaxecraftApp(hxc_tmp_class_object_address_n0);
  struct hxc_caxecraft_app_CaxecraftApp *hxc_app = hxc_tmp_class_object_address_n0;
  struct hxc_caxecraft_app_CaxecraftApp *hxc_tmp_load_result_n1 = hxc_app;
  if (hxc_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  hxc_caxecraft_app_CaxecraftApp_run(hxc_tmp_load_result_n1);
  return;
}
