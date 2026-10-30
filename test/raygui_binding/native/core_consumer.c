#include <stdio.h>

#include "raygui.h"

/*
 * Independent C consumer for the selected raygui ABI.
 *
 * This file is handwritten on purpose: it checks that the generated Haxe
 * declarations agree with a real C header and library rather than asking
 * haxe.c to validate its own output. It uses state/style calls that need no
 * desktop window, so the same binary runs with Raylib's headless build.
 */
int main(void) {
  int (*list_view)(Rectangle, const char *, int *, int *) = &GuiListView;
  int (*text_box)(Rectangle, char *, int, bool) = &GuiTextBox;
  if ((list_view == NULL) || (text_box == NULL)) {
    return 3;
  }
  GuiLoadStyleDefault();
  GuiSetState(STATE_FOCUSED);
  if (GuiGetState() != STATE_FOCUSED) {
    return 1;
  }
  GuiSetStyle(DEFAULT, TEXT_SIZE, 19);
  if (GuiGetStyle(DEFAULT, TEXT_SIZE) != 19) {
    return 2;
  }
  GuiDisable();
  GuiEnable();
  /* A version line must not fall through and overwrite property zero. A final
   * unterminated line must be processed once, and an empty file must return. */
  FILE *style = fopen("style-reader.rgs", "wb");
  if (style == NULL) return 4;
  GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x11223344);
  if (fputs("# raygui style\nv 500\np 0 16 0x17", style) == EOF) return 5;
  if (fclose(style) != 0) return 6;
  GuiLoadStyle("style-reader.rgs");
  if ((GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL) != 0x11223344) ||
      (GuiGetStyle(DEFAULT, TEXT_SIZE) != 23)) return 7;
  style = fopen("style-reader.rgs", "wb");
  if ((style == NULL) || (fclose(style) != 0)) return 8;
  GuiLoadStyle("style-reader.rgs");
  if (GuiGetStyle(DEFAULT, TEXT_SIZE) != 23) return 9;
  if (remove("style-reader.rgs") != 0) return 10;
  puts("raygui-c-consumer: OK");
  return 0;
}
