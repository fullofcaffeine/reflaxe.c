#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* Source-safe / * comments * / neutralize \077\077/ trigraphs\012and UTF-8 \303\251. */

struct hxc_pair {
  int32_t left;
  int32_t right;
};

static int32_t identity(int32_t value)
{
  return value;
}

#line 200 "haxe/source\"\\questions\?\?.hx"
_Static_assert(__LINE__ == 200, "declaration line mapping is immediate");

int main(void)
{
  int32_t a = 8;
  int32_t b = 3;
  int32_t c = 1;
  int32_t scratch = 0;
  struct hxc_pair pair = { .left = 2, .right = 4 };
  struct hxc_pair *pair_ptr = &pair;
  int32_t values[2] = { 1, 2 };
  /* statement comment closes * / and opens / * without leaking \077\077/ */
#line 400 "haxe/main\"\\questions\?\?.hx"
  int32_t mapped_line = __LINE__;
  if (mapped_line != 400)
  {
    return 10;
  }
  const char *escaped = "quote\" slash\\ question\? tab\t newline\n nul\000 \303\251\360\237\231\202";
  if (escaped[0] != 'q')
  {
    return 11;
  }
  if (identity(a) != 8)
  {
    return 12;
  }
  if (pair_ptr->right != 4)
  {
    return 13;
  }
  if (values[0] + values[1] != 3)
  {
    return 14;
  }
  if ((a > b ? a : b) != 8)
  {
    return 15;
  }
  if (_Generic(a, int32_t: 1, default: 0) != 1)
  {
    return 16;
  }
  if ((struct hxc_pair){ .left = 6, .right = 7 }.right != 7)
  {
    return 17;
  }
  if (sizeof(a + b) == 0)
  {
    return 18;
  }
  (void)1.25F;
  (void)'\'';
  (void)'\\';
  (void)'\?';
  (void)'\000';
  (void)true;
  (void)NULL;
  (void)_Alignof(int32_t);
  (void)sizeof(int32_t);
  (void)&scratch;
  (void)*&scratch;
  (void)+scratch;
  (void)-scratch;
  (void)!scratch;
  (void)~scratch;
  (void)++scratch;
  (void)--scratch;
  (void)scratch++;
  (void)scratch--;
  (void)sizeof(scratch + b);
  (void)(a * b);
  (void)(a / b);
  (void)(a % b);
  (void)(a + b);
  (void)(a - b);
  (void)(a << c);
  (void)(a >> c);
  (void)(a < b);
  (void)(a <= b);
  (void)(a > b);
  (void)(a >= b);
  (void)(a == b);
  (void)(a != b);
  (void)(a & b);
  (void)(a ^ b);
  (void)(a | b);
  (void)(a && b);
  (void)(a || b);
  (void)(scratch = a);
  (void)(scratch += b);
  (void)(scratch -= c);
  (void)(scratch *= b);
  (void)(scratch /= b);
  (void)(scratch %= b);
  (void)(scratch <<= c);
  (void)(scratch >>= c);
  (void)(scratch &= a);
  (void)(scratch ^= b);
  (void)(scratch |= c);
  (void)(scratch = a, b);
  (void)(a > b ? a : b);
  (void)identity(a);
  (void)values[0];
  (void)pair.left;
  (void)(int32_t)a;
  (void)(a + b);
  scratch = 0;
  if (a > b)
  {
    scratch = a;
  }
  else
  {
    return 20;
  }
  while (c < 2)
  {
    ++c;
  }
  do
  {
    --c;
  }
  while (c > 1);
  for (int32_t index = 0; index < 2; ++index)
  {
    if (index == 0)
    {
      continue;
    }
    scratch += values[index];
  }
  switch (c) {
    case 1:
      {
        int32_t local = pair.left;
        scratch += local;
        break;
      }
    default:
      {
        return 30;
      }
  }
  if (scratch != 12)
  {
    return 40;
  }
  goto hxc_done;
hxc_done:
    ;
  puts("c-expression-golden: OK");
  return 0;
}
