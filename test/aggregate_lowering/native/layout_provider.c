#include "hxc/program.h"

#include <stddef.h>

size_t hxc_test_pair_size(void);
size_t hxc_test_pair_alignment(void);
size_t hxc_test_pair_a_offset(void);
size_t hxc_test_pair_z_offset(void);
size_t hxc_test_envelope_size(void);
size_t hxc_test_envelope_alignment(void);
size_t hxc_test_envelope_enabled_offset(void);
size_t hxc_test_envelope_point_offset(void);

size_t hxc_test_pair_size(void)
{
  return sizeof(struct HXC_PAIR_TAG);
}

size_t hxc_test_pair_alignment(void)
{
  return _Alignof(struct HXC_PAIR_TAG);
}

size_t hxc_test_pair_a_offset(void)
{
  return offsetof(struct HXC_PAIR_TAG, HXC_PAIR_A);
}

size_t hxc_test_pair_z_offset(void)
{
  return offsetof(struct HXC_PAIR_TAG, HXC_PAIR_Z);
}

size_t hxc_test_envelope_size(void)
{
  return sizeof(struct HXC_ENVELOPE_TAG);
}

size_t hxc_test_envelope_alignment(void)
{
  return _Alignof(struct HXC_ENVELOPE_TAG);
}

size_t hxc_test_envelope_enabled_offset(void)
{
  return offsetof(struct HXC_ENVELOPE_TAG, HXC_ENVELOPE_ENABLED);
}

size_t hxc_test_envelope_point_offset(void)
{
  return offsetof(struct HXC_ENVELOPE_TAG, HXC_ENVELOPE_POINT);
}
