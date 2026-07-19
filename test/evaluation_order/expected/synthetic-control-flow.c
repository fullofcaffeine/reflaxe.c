#include <stdbool.h>
#include <stdint.h>

static void hxc_bounded_control_flow(bool condition, int32_t selector)
{
  while (1)
  {
    if (!condition)
    {
      break;
    }
    switch (selector) {
      case 1:
        {
          goto hxc_bounded_label_3;
        }
      default:
        {
          break;
        }
    }
  }
hxc_bounded_label_3:
    ;
  return;
}

static void hxc_legacy_control_flow(bool condition, int32_t selector)
{
hxc_legacy_label_0:
    ;
  switch (selector) {
    case 1:
      {
        goto hxc_legacy_label_1;
      }
    case 2:
      {
        goto hxc_legacy_label_2;
      }
    default:
      {
        goto hxc_legacy_label_3;
      }
  }
hxc_legacy_label_1:
    ;
  goto hxc_legacy_label_2;
hxc_legacy_label_2:
    ;
  if (condition)
  {
    goto hxc_legacy_label_1;
  }
  else
  {
    goto hxc_legacy_label_0;
  }
hxc_legacy_label_3:
    ;
  return;
}

int main(void)
{
  hxc_bounded_control_flow(false, 0);
  hxc_bounded_control_flow(true, 1);
  hxc_legacy_control_flow(false, 0);
  return 0;
}
