#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

int32_t hxc_FunctionFixture_apply(int32_t hxc_value, int32_t (*hxc_operation)(int32_t));

double hxc_FunctionFixture_asFloat(double hxc_value);

int32_t hxc_FunctionFixture_chain(int32_t hxc_value);

int32_t (*hxc_FunctionFixture_choose(void))(int32_t);

double hxc_FunctionFixture_convert(int32_t hxc_value);

void hxc_FunctionFixture_discarded(int32_t hxc_value);

int32_t hxc_FunctionFixture_first(int32_t hxc_left, int32_t hxc_right);

int32_t hxc_FunctionFixture_indirect(int32_t hxc_value);

void hxc_FunctionFixture_main(void);

_Noreturn void hxc_FunctionFixture_mutualLeft(int32_t hxc_value);

_Noreturn void hxc_FunctionFixture_mutualRight(int32_t hxc_value);

int32_t hxc_FunctionFixture_ordered(int32_t hxc_value);

int32_t hxc_FunctionFixture_passthrough(int32_t hxc_value);

_Noreturn void hxc_FunctionFixture_recursive(int32_t hxc_left, int32_t hxc_right);

#endif /* HXC_PROGRAM_H_INCLUDED */
