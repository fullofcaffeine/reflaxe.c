/*
	Test-only translation-unit shim. The generated program owns an executable
	entry point; the differential harness owns another. Rename only the included
	generated entry point so both can be linked without editing generated output.
*/
int hxc_generated_main(void);

#define main hxc_generated_main
#include "../generated/src/program.c"
