#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <hxrt/io.h>
#include <hxrt/string_literal.h>
#include <stdlib.h>

typedef char hxc_runtime_abi_major_must_match[HXC_RUNTIME_ABI_MAJOR == 0U ? 1 : -1];

void hxc_Main_main(void);

#endif /* HXC_PROGRAM_H_INCLUDED */
