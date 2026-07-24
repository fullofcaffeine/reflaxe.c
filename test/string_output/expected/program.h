#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <hxrt/io.h>
#include <hxrt/string_literal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef char hxc_runtime_abi_major_must_match[HXC_RUNTIME_ABI_MAJOR == 0U ? 1 : -1];

void hxc_Main_main(void);

hxc_string hxc_Main_selectLabel(bool hxc_primary);

#endif /* HXC_PROGRAM_H_INCLUDED */
