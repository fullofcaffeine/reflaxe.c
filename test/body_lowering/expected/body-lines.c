#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool hxc_BodyFixture_booleanValue(void);

static int32_t hxc_BodyFixture_chooseOrThrow(int32_t hxc_l_choice);

static int32_t hxc_BodyFixture_definitelyAssignedAfterReturn(bool hxc_l_choice);

static int32_t hxc_BodyFixture_definitelyAssignedBranches(bool hxc_l_choice);

static int32_t hxc_BodyFixture_definitelyAssignedInteger(void);

static int32_t hxc_BodyFixture_definitelyAssignedLoop(int32_t hxc_l_limit);

static int32_t hxc_BodyFixture_directInteger(void);

static void hxc_BodyFixture_explicitVoid(void);

static double hxc_BodyFixture_floatingValue(void);

static void hxc_BodyFixture_implicitVoid(void);

static int32_t hxc_BodyFixture_integerValue(void);

static int32_t hxc_BodyFixture_throwPayload(void);

static uint32_t hxc_BodyFixture_unsignedValue(void);

#line 56 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static bool hxc_BodyFixture_booleanValue(void)
{
#line 57 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  bool hxc_l_value = true;
#line 58 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value;
}

#line 85 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_chooseOrThrow(int32_t hxc_l_choice)
{
#line 86 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_tmp_switch_result_n1 = 0;
#line 86 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  switch (hxc_l_choice) {
    case 0:
      {
#line 86 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        hxc_l_tmp_switch_result_n1 = 41;
        break;
      }
    case 1:
      {
#line 88 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        int32_t hxc_l_tmp_call_result_n0 = hxc_BodyFixture_throwPayload();
#line 88 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        (void)hxc_l_tmp_call_result_n0;
        abort();
      }
    default:
      {
#line 86 "test/body_lowering/fixtures/positive/BodyFixture.hx"
        hxc_l_tmp_switch_result_n1 = 43;
        break;
      }
  }
#line 86 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_tmp_switch_result_n1;
}

#line 20 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_definitelyAssignedAfterReturn(bool hxc_l_choice)
{
#line 21 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_value = { 0 };
#line 22 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  if (!hxc_l_choice)
  {
#line 25 "test/body_lowering/fixtures/positive/BodyFixture.hx"
    return 17;
  }
#line 23 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  hxc_l_value = 13;
#line 26 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value;
}

#line 10 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_definitelyAssignedBranches(bool hxc_l_choice)
{
#line 11 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_value = { 0 };
#line 12 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  if (hxc_l_choice)
  {
#line 13 "test/body_lowering/fixtures/positive/BodyFixture.hx"
    hxc_l_value = 7;
  }
  else
  {
#line 15 "test/body_lowering/fixtures/positive/BodyFixture.hx"
    hxc_l_value = 11;
  }
#line 16 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value;
}

#line 3 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_definitelyAssignedInteger(void)
{
#line 4 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_value = { 0 };
#line 5 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  hxc_l_value = 31;
#line 6 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value;
}

#line 30 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_definitelyAssignedLoop(int32_t hxc_l_limit)
{
#line 31 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_value = { 0 };
#line 32 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  hxc_l_value = 0;
  while (1)
  {
#line 33 "test/body_lowering/fixtures/positive/BodyFixture.hx"
    if (!(hxc_l_value < hxc_l_limit))
    {
      break;
    }
#line 34 "test/body_lowering/fixtures/positive/BodyFixture.hx"
    hxc_l_value = hxc_l_limit;
  }
#line 35 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value;
}

#line 47 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_directInteger(void)
{
#line 48 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return 23;
}

#line 61 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static void hxc_BodyFixture_explicitVoid(void)
{
#line 62 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_first = 1;
#line 63 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_first;
  (void)hxc_l_tmp_load_result_n0;
#line 65 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_second = 2;
#line 66 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_second;
  (void)hxc_l_tmp_load_result_n1;
#line 68 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return;
}

#line 51 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static double hxc_BodyFixture_floatingValue(void)
{
#line 52 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  double hxc_l_value = 1.5;
#line 53 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value;
}

#line 71 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static void hxc_BodyFixture_implicitVoid(void)
{
#line 72 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  bool hxc_l_value = false;
#line 73 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  bool hxc_l_tmp_load_result_n0 = hxc_l_value;
  (void)hxc_l_tmp_load_result_n0;
#line 71 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return;
}

#line 38 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_integerValue(void)
{
#line 39 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_value_h276946f7ce4a = 17;
#line 40 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_tmp_load_result_n0 = hxc_l_value_h276946f7ce4a;
  (void)hxc_l_tmp_load_result_n0;
#line 42 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  int32_t hxc_l_value_h95f4913cd08b = 42;
#line 43 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value_h95f4913cd08b;
}

#line 81 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static int32_t hxc_BodyFixture_throwPayload(void)
{
#line 82 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return 99;
}

#line 76 "test/body_lowering/fixtures/positive/BodyFixture.hx"
static uint32_t hxc_BodyFixture_unsignedValue(void)
{
#line 77 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  uint32_t hxc_l_value = 29;
#line 78 "test/body_lowering/fixtures/positive/BodyFixture.hx"
  return hxc_l_value;
}
