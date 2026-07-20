#ifndef HXC_PACKAGES_CAXECRAFT_QA_PACKAGE_H_INCLUDED
#define HXC_PACKAGES_CAXECRAFT_QA_PACKAGE_H_INCLUDED

#include "hxc/detail/program_types.h"

void hxc_caxecraft_qa_DomainProbe_clear(uint8_t *hxc_cells, size_t hxc_length);

void hxc_caxecraft_qa_DomainProbe_main(void);

bool hxc_caxecraft_qa_DomainProbe_near(double hxc_left, double hxc_right);

int32_t hxc_caxecraft_qa_DomainProbe_selfCheck(void);

#endif /* HXC_PACKAGES_CAXECRAFT_QA_PACKAGE_H_INCLUDED */
