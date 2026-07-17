#include "hxc/program.h"

#include <stddef.h>

size_t hxc_test_option_size(void);
size_t hxc_test_option_alignment(void);
size_t hxc_test_option_tag_offset(void);
size_t hxc_test_option_payload_offset(void);
size_t hxc_test_chain_size(void);
size_t hxc_test_chain_alignment(void);
size_t hxc_test_chain_tag_offset(void);
size_t hxc_test_chain_payload_offset(void);

size_t hxc_test_option_size(void)
{
  return sizeof(struct HXC_OPTION_INT_TAG);
}

size_t hxc_test_option_alignment(void)
{
  return _Alignof(struct HXC_OPTION_INT_TAG);
}

size_t hxc_test_option_tag_offset(void)
{
  return offsetof(struct HXC_OPTION_INT_TAG, HXC_OPTION_INT_TAG_MEMBER);
}

size_t hxc_test_option_payload_offset(void)
{
  return offsetof(struct HXC_OPTION_INT_TAG, HXC_OPTION_INT_PAYLOAD_MEMBER);
}

size_t hxc_test_chain_size(void)
{
  return sizeof(struct HXC_CHAIN_TAG);
}

size_t hxc_test_chain_alignment(void)
{
  return _Alignof(struct HXC_CHAIN_TAG);
}

size_t hxc_test_chain_tag_offset(void)
{
  return offsetof(struct HXC_CHAIN_TAG, HXC_CHAIN_TAG_MEMBER);
}

size_t hxc_test_chain_payload_offset(void)
{
  return offsetof(struct HXC_CHAIN_TAG, HXC_CHAIN_PAYLOAD_MEMBER);
}
