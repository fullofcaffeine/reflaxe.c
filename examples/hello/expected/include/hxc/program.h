#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <hxrt/io.h>
#include <hxrt/string_literal.h>
#include <stdlib.h>

_Static_assert(HXC_RUNTIME_ABI_MAJOR == 0U, "incompatible hxrt ABI major: generated code requires 0");

void hxc_Main_main(void);

#endif /* HXC_PROGRAM_H_INCLUDED */
