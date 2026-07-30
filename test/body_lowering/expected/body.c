#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool hxc_BodyFixture_booleanValue(void);

static int32_t hxc_BodyFixture_chooseOrThrow(int32_t hxc_l_choice);

static int32_t hxc_BodyFixture_directInteger(void);

static void hxc_BodyFixture_explicitVoid(void);

static double hxc_BodyFixture_floatingValue(void);

static void hxc_BodyFixture_implicitVoid(void);

static int32_t hxc_BodyFixture_integerValue(void);

static int32_t hxc_BodyFixture_throwPayload(void);

static uint32_t hxc_BodyFixture_unsignedValue(void);

static bool hxc_BodyFixture_booleanValue(void)
{
  bool hxc_l_value = true;
  return hxc_l_value;
}

static int32_t hxc_BodyFixture_chooseOrThrow(int32_t hxc_l_choice)
{
  int32_t hxc_l_tmp_switch_result_n1 = 0;
  switch (hxc_l_choice) {
    case 0:
      {
        hxc_l_tmp_switch_result_n1 = 41;
        break;
      }
    case 1:
      {
        int32_t hxc_l_tmp_call_result_n0 = hxc_BodyFixture_throwPayload();
        (void)hxc_l_tmp_call_result_n0;
        abort();
      }
    default:
      {
        hxc_l_tmp_switch_result_n1 = 43;
        break;
      }
  }
  return hxc_l_tmp_switch_result_n1;
}

static int32_t hxc_BodyFixture_directInteger(void)
{
  return 23;
}

static void hxc_BodyFixture_explicitVoid(void)
{
  int32_t hxc_l_first = 1;
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_first;
  (void)hxc_l_tmp_load_result_n0;
  int32_t hxc_l_second = 2;
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_second;
  (void)hxc_l_tmp_load_result_n1;
  return;
}

static double hxc_BodyFixture_floatingValue(void)
{
  double hxc_l_value = 1.5;
  return hxc_l_value;
}

static void hxc_BodyFixture_implicitVoid(void)
{
  bool hxc_l_value = false;
  bool hxc_l_tmp_load_result_n0 = hxc_l_value;
  (void)hxc_l_tmp_load_result_n0;
  return;
}

static int32_t hxc_BodyFixture_integerValue(void)
{
  int32_t hxc_l_value_h276946f7ce4a = 17;
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_value_h276946f7ce4a;
  (void)hxc_l_tmp_load_result_n0;
  int32_t hxc_l_value_h95f4913cd08b = 42;
  return hxc_l_value_h95f4913cd08b;
}

static int32_t hxc_BodyFixture_throwPayload(void)
{
  return 99;
}

static uint32_t hxc_BodyFixture_unsignedValue(void)
{
  uint32_t hxc_l_value = 29;
  return hxc_l_value;
}
