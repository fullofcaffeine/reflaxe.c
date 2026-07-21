#include "hxc/program.h"

struct Color hxc_caxecraft_app_CaxecraftPalette_block(enum hxc_caxecraft_domain_BlockKind hxc_kind)
{
  switch (hxc_kind) {
    case hxc_caxecraft_domain_BlockKind_Air:
      {
        break;
      }
    case hxc_caxecraft_domain_BlockKind_Grass:
      {
        return (struct Color){ .r = 83, .g = 145, .b = 92, .a = 255 };
      }
    case hxc_caxecraft_domain_BlockKind_Dirt:
      {
        return (struct Color){ .r = 132, .g = 91, .b = 62, .a = 255 };
      }
    case hxc_caxecraft_domain_BlockKind_Stone:
      {
        return (struct Color){ .r = 91, .g = 107, .b = 117, .a = 255 };
      }
    case hxc_caxecraft_domain_BlockKind_Bedrock:
      {
        return (struct Color){ .r = 47, .g = 54, .b = 64, .a = 255 };
      }
  }
  return (struct Color){ .r = 0, .g = 0, .b = 0, .a = 0 };
}
