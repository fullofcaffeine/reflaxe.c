#include "hxc/program.h"

#include <stdint.h>
#include <stdio.h>

int hxc_layout_generated_main(void);

#if defined(main)
#undef main
#endif

int main(void)
{
	(void)hxc_layout_generated_main();
	(void)printf("%d\n", (int)hxc_method_layout_Main_trace());
	return 0;
}
