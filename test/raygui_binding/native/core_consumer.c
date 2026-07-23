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
  puts("raygui-c-consumer: OK");
  return 0;
}
