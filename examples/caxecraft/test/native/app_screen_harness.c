#include "hxc/program.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Independent native observer for the Haxe-authored screen specification.
 * The screen rules remain in shared Haxe; this fixture only prints the result.
 */
int main(void)
{
	const int32_t check = hxc_caxecraft_qa_AppScreenProbe_selfCheck();
	(void)printf("%" PRId32 "\n", check);
	return check == INT32_C(0) ? 0 : 1;
}
