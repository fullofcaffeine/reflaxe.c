#include "hxc/program.h"

void hxc_caxecraft_app_Main_main(void)
{
  const void *hxc_gc_roots[3] = { NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 3, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_app_CaxecraftApp *hxc_tmp_managed_class_result_n0 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_app_CaxecraftApp_descriptor, (void **)&hxc_tmp_managed_class_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[0] = (const void *)hxc_tmp_managed_class_result_n0;
  hxc_compiler_constructor_caxecraft_app_CaxecraftApp(hxc_tmp_managed_class_result_n0);
  hxc_gc_roots[1] = (const void *)hxc_tmp_managed_class_result_n0;
  struct hxc_caxecraft_app_CaxecraftApp *hxc_app = hxc_tmp_managed_class_result_n0;
  struct hxc_caxecraft_app_CaxecraftApp *hxc_tmp_load_result_n1 = hxc_app;
  hxc_gc_roots[2] = (const void *)hxc_tmp_load_result_n1;
  if (hxc_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  hxc_caxecraft_app_CaxecraftApp_run(hxc_tmp_load_result_n1);
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}
