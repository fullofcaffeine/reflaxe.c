#include "hxc/program.h"

#include <cstddef>
#include <cstdint>

extern "C" {
std::size_t hxc_test_option_size(void);
std::size_t hxc_test_option_alignment(void);
std::size_t hxc_test_option_tag_offset(void);
std::size_t hxc_test_option_payload_offset(void);
std::size_t hxc_test_chain_size(void);
std::size_t hxc_test_chain_alignment(void);
std::size_t hxc_test_chain_tag_offset(void);
std::size_t hxc_test_chain_payload_offset(void);
}

int main()
{
  HXC_OPTION_INT_TAG some{};
  some.HXC_OPTION_INT_TAG_MEMBER = HXC_OPTION_INT_SOME;
  some.HXC_OPTION_INT_PAYLOAD_MEMBER.HXC_OPTION_INT_SOME_UNION
    .HXC_OPTION_INT_SOME_VALUE = INT32_C(7);

  HXC_CHAIN_TAG tail{};
  tail.HXC_CHAIN_TAG_MEMBER = HXC_CHAIN_END;
  tail.HXC_CHAIN_PAYLOAD_MEMBER.HXC_CHAIN_END_UNION.HXC_CHAIN_END_VALUE =
    INT32_C(2);
  HXC_CHAIN_TAG head{};
  head.HXC_CHAIN_TAG_MEMBER = HXC_CHAIN_LINK;
  head.HXC_CHAIN_PAYLOAD_MEMBER.HXC_CHAIN_LINK_UNION.HXC_CHAIN_LINK_VALUE =
    INT32_C(1);
  head.HXC_CHAIN_PAYLOAD_MEMBER.HXC_CHAIN_LINK_UNION.HXC_CHAIN_LINK_NEXT =
    &tail;

  const bool layout_matches =
    hxc_test_option_size() == sizeof(HXC_OPTION_INT_TAG) &&
    hxc_test_option_alignment() == alignof(HXC_OPTION_INT_TAG) &&
    hxc_test_option_tag_offset() ==
      offsetof(HXC_OPTION_INT_TAG, HXC_OPTION_INT_TAG_MEMBER) &&
    hxc_test_option_payload_offset() ==
      offsetof(HXC_OPTION_INT_TAG, HXC_OPTION_INT_PAYLOAD_MEMBER) &&
    hxc_test_chain_size() == sizeof(HXC_CHAIN_TAG) &&
    hxc_test_chain_alignment() == alignof(HXC_CHAIN_TAG) &&
    hxc_test_chain_tag_offset() ==
      offsetof(HXC_CHAIN_TAG, HXC_CHAIN_TAG_MEMBER) &&
    hxc_test_chain_payload_offset() ==
      offsetof(HXC_CHAIN_TAG, HXC_CHAIN_PAYLOAD_MEMBER);
  const bool values_match =
    some.HXC_OPTION_INT_TAG_MEMBER == HXC_OPTION_INT_SOME &&
    some.HXC_OPTION_INT_PAYLOAD_MEMBER.HXC_OPTION_INT_SOME_UNION
      .HXC_OPTION_INT_SOME_VALUE == INT32_C(7) &&
    head.HXC_CHAIN_PAYLOAD_MEMBER.HXC_CHAIN_LINK_UNION.HXC_CHAIN_LINK_NEXT ==
      &tail &&
    head.HXC_CHAIN_PAYLOAD_MEMBER.HXC_CHAIN_LINK_UNION.HXC_CHAIN_LINK_NEXT
      ->HXC_CHAIN_PAYLOAD_MEMBER.HXC_CHAIN_END_UNION.HXC_CHAIN_END_VALUE ==
      INT32_C(2);
  return layout_matches && values_match ? 0 : 1;
}
