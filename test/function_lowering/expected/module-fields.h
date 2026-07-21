#ifndef HXC_MODULE_FUNCTIONS_H_INCLUDED
#define HXC_MODULE_FUNCTIONS_H_INCLUDED

#include "hxc/detail/program_types.h"

extern int32_t hxc_ModuleFunctions_base;

int32_t hxc_ModuleFunctions_answer(void);

int32_t hxc_ModuleFunctions_doubled(int32_t hxc_value);

void hxc_ModuleFunctions_main(void);

void hxc_ModuleFunctions_static_field_base(void);

#endif /* HXC_MODULE_FUNCTIONS_H_INCLUDED */
