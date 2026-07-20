#include "hxc/program.h"
#include "method_symbols.h"

#include <stdint.h>
#include <stdio.h>

int hxc_layout_generated_main(void);

#if defined(main)
#undef main
#endif

int main(void)
{
	(void)hxc_layout_generated_main();
	(void)printf("%d\n", (int)HXC_LAYOUT_TRACE());
	return 0;
}
