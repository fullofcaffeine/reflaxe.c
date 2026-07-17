#include "hxc/program.h"

#include <cstddef>
#include <cstdint>

extern "C" {
std::size_t hxc_test_pair_size(void);
std::size_t hxc_test_pair_alignment(void);
std::size_t hxc_test_pair_a_offset(void);
std::size_t hxc_test_pair_z_offset(void);
std::size_t hxc_test_envelope_size(void);
std::size_t hxc_test_envelope_alignment(void);
std::size_t hxc_test_envelope_enabled_offset(void);
std::size_t hxc_test_envelope_point_offset(void);
}

int main()
{
  HXC_PAIR_TAG first{};
  first.HXC_PAIR_A = INT32_C(3);
  first.HXC_PAIR_Z = INT32_C(4);
  HXC_PAIR_TAG copied = first;
  HXC_ENVELOPE_TAG envelope{};
  envelope.HXC_ENVELOPE_ENABLED = true;
  envelope.HXC_ENVELOPE_POINT = copied;

  const bool layout_matches =
    hxc_test_pair_size() == sizeof(HXC_PAIR_TAG) &&
    hxc_test_pair_alignment() == alignof(HXC_PAIR_TAG) &&
    hxc_test_pair_a_offset() == offsetof(HXC_PAIR_TAG, HXC_PAIR_A) &&
    hxc_test_pair_z_offset() == offsetof(HXC_PAIR_TAG, HXC_PAIR_Z) &&
    hxc_test_envelope_size() == sizeof(HXC_ENVELOPE_TAG) &&
    hxc_test_envelope_alignment() == alignof(HXC_ENVELOPE_TAG) &&
    hxc_test_envelope_enabled_offset() ==
      offsetof(HXC_ENVELOPE_TAG, HXC_ENVELOPE_ENABLED) &&
    hxc_test_envelope_point_offset() ==
      offsetof(HXC_ENVELOPE_TAG, HXC_ENVELOPE_POINT);
  const bool values_match = envelope.HXC_ENVELOPE_ENABLED &&
    envelope.HXC_ENVELOPE_POINT.HXC_PAIR_A == INT32_C(3) &&
    envelope.HXC_ENVELOPE_POINT.HXC_PAIR_Z == INT32_C(4);
  return layout_matches && values_match ? 0 : 1;
}
