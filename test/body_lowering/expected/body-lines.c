#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool hxc_BodyFixture_booleanValue(void);

static int32_t hxc_BodyFixture_chooseOrThrow(int32_t hxc_choice);

static int32_t hxc_BodyFixture_directInteger(void);

static void hxc_BodyFixture_explicitVoid(void);

static double hxc_BodyFixture_floatingValue(void);

static void hxc_BodyFixture_implicitVoid(void);

static int32_t hxc_BodyFixture_integerValue(void);

static int32_t hxc_BodyFixture_throwPayload(void);

static uint32_t hxc_BodyFixture_unsignedValue(void);

#line 20 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static bool hxc_BodyFixture_booleanValue(void)
{
#line 21 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  bool hxc_value = true;
#line 22 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_value;
}

#line 49 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_chooseOrThrow(int32_t hxc_choice)
{
#line 50 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_tmp_switch_result_n1 = 0;
#line 50 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  switch (hxc_choice) {
    case 0:
      {
#line 50 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        hxc_tmp_switch_result_n1 = 41;
        break;
      }
    case 1:
      {
#line 52 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        int32_t hxc_tmp_call_result_n0 = hxc_BodyFixture_throwPayload();
#line 52 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        (void)hxc_tmp_call_result_n0;
        abort();
      }
    default:
      {
#line 50 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        hxc_tmp_switch_result_n1 = 43;
        break;
      }
  }
#line 50 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_tmp_switch_result_n1;
}

#line 11 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_directInteger(void)
{
#line 12 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return 23;
}

#line 25 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static void hxc_BodyFixture_explicitVoid(void)
{
#line 26 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_first = 1;
#line 27 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_tmp_load_result_n0 = hxc_first;
  (void)hxc_tmp_load_result_n0;
#line 29 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_second = 2;
#line 30 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_tmp_load_result_n1 = hxc_second;
  (void)hxc_tmp_load_result_n1;
#line 32 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return;
}

#line 15 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static double hxc_BodyFixture_floatingValue(void)
{
#line 16 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  double hxc_value = 1.5;
#line 17 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_value;
}

#line 35 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static void hxc_BodyFixture_implicitVoid(void)
{
#line 36 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  bool hxc_value = false;
#line 37 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  bool hxc_tmp_load_result_n0 = hxc_value;
  (void)hxc_tmp_load_result_n0;
#line 35 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return;
}

#line 2 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_integerValue(void)
{
#line 3 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_value_h276946f7ce4a = 17;
#line 4 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_tmp_load_result_n0 = hxc_value_h276946f7ce4a;
  (void)hxc_tmp_load_result_n0;
#line 6 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_value_h95f4913cd08b = 42;
#line 7 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_value_h95f4913cd08b;
}

#line 45 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_throwPayload(void)
{
#line 46 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return 99;
}

#line 40 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static uint32_t hxc_BodyFixture_unsignedValue(void)
{
#line 41 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  uint32_t hxc_value = 29;
#line 42 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_value;
}
