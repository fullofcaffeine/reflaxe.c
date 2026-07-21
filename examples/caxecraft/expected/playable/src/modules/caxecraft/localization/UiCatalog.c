#include "hxc/program.h"

void hxc_caxecraft_localization_UiCatalog_draw(int32_t hxc_locale, int32_t hxc_message, int32_t hxc_x, int32_t hxc_y, int32_t hxc_fontSize, struct Color hxc_color)
{
  switch (hxc_locale) {
    case 0:
      {
        hxc_caxecraft_localization_UiCatalog_drawLocale0(hxc_message, hxc_x, hxc_y, hxc_fontSize, hxc_color);
        break;
      }
    case 1:
      {
        hxc_caxecraft_localization_UiCatalog_drawLocale1(hxc_message, hxc_x, hxc_y, hxc_fontSize, hxc_color);
        break;
      }
    default:
      {
        hxc_caxecraft_localization_UiCatalog_drawLocale0(hxc_message, hxc_x, hxc_y, hxc_fontSize, hxc_color);
        break;
      }
  }
  return;
}

void hxc_caxecraft_localization_UiCatalog_drawLocale0(int32_t hxc_message, int32_t hxc_x, int32_t hxc_y, int32_t hxc_fontSize, struct Color hxc_color)
{
  switch (hxc_message) {
    case 0:
      {
        DrawText("CAXECRAFT  //  C + HAXE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 1:
      {
        DrawText("CLICK TO CAPTURE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 2:
      {
        DrawText("WASD MOVE  1-8/WHEEL ITEMS  SPACE JUMP  E TALK  LMB MINE/STRIKE  RMB USE  ESC PAUSE  Q QUIT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 3:
      {
        DrawText("CELLS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 4:
      {
        DrawText("DRAWS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 5:
      {
        DrawText("FRAME", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 6:
      {
        DrawText("TICK", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 7:
      {
        DrawText("VISIBLE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 8:
      {
        DrawText("HEALTH ALREADY FULL", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 9:
      {
        DrawText("ADVENTURE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 10:
      {
        DrawText("CREATIVE MODE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 11:
      {
        DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 12:
      {
        DrawText("NO BLOCK IN REACH", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 13:
      {
        DrawText("CLICK TO CAPTURE  //  Q TO QUIT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 14:
      {
        DrawText("PAUSED", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 15:
      {
        DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 16:
      {
        DrawText("CAXECRAFT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    default:
      {
        abort();
      }
  }
  return;
}

void hxc_caxecraft_localization_UiCatalog_drawLocale1(int32_t hxc_message, int32_t hxc_x, int32_t hxc_y, int32_t hxc_fontSize, struct Color hxc_color)
{
  switch (hxc_message) {
    case 0:
      {
        DrawText("CAXECRAFT  //  C + HAXE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 1:
      {
        DrawText("CLIC PARA CAPTURAR", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 2:
      {
        DrawText("WASD MOVER  1-8/RUEDA OBJETOS  ESPACIO SALTAR  E HABLAR  CLIC IZQ USAR  ESC PAUSA  Q SALIR", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 3:
      {
        DrawText("CELDAS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 4:
      {
        DrawText("DIBUJOS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 5:
      {
        DrawText("CUADRO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 6:
      {
        DrawText("PASO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 7:
      {
        DrawText("VISIBLES", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 8:
      {
        DrawText("SALUD COMPLETA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 9:
      {
        DrawText("AVENTURA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 10:
      {
        DrawText("MODO CREATIVO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 11:
      {
        DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 12:
      {
        DrawText("NINGUN BLOQUE AL ALCANCE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 13:
      {
        DrawText("CLIC PARA CAPTURAR  //  Q PARA SALIR", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 14:
      {
        DrawText("PAUSA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 15:
      {
        DrawText("NO SE PUEDE COLOCAR: JUGADOR O BORDE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 16:
      {
        DrawText("CAXECRAFT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    default:
      {
        abort();
      }
  }
  return;
}

int32_t hxc_caxecraft_localization_UiCatalog_nextLocale(int32_t hxc_locale)
{
  switch (hxc_locale) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 0;
      }
    default:
      {
        return 0;
      }
  }
  return 1;
}
