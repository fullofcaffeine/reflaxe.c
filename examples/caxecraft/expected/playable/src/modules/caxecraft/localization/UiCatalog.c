#include "hxc/program.h"

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

const char *hxc_caxecraft_localization_UiCatalog_text(int32_t hxc_locale, int32_t hxc_message)
{
  switch (hxc_locale) {
    case 0:
      {
        break;
      }
    case 1:
      {
        const char *hxc_tmp_call_result_n1 = hxc_caxecraft_localization_UiCatalog_textLocale1(hxc_message);
        return hxc_tmp_call_result_n1;
      }
    default:
      {
        const char *hxc_tmp_call_result_n2 = hxc_caxecraft_localization_UiCatalog_textLocale0(hxc_message);
        return hxc_tmp_call_result_n2;
      }
  }
  const char *hxc_tmp_call_result_n0 = hxc_caxecraft_localization_UiCatalog_textLocale0(hxc_message);
  return hxc_tmp_call_result_n0;
}

const char *hxc_caxecraft_localization_UiCatalog_textLocale0(int32_t hxc_message)
{
  switch (hxc_message) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return "CAXECRAFT  //  C + HAXE";
      }
    case 2:
      {
        return "CLICK TO CAPTURE";
      }
    case 3:
      {
        return "WASD MOVE  1-8/WHEEL ITEMS  SPACE JUMP  E TALK  LMB MINE/STRIKE  RMB USE  ESC PAUSE  Q QUIT";
      }
    case 4:
      {
        return "CELLS";
      }
    case 5:
      {
        return "DRAWS";
      }
    case 6:
      {
        return "FRAME";
      }
    case 7:
      {
        return "TICK";
      }
    case 8:
      {
        return "VISIBLE";
      }
    case 9:
      {
        return "HEALTH ALREADY FULL";
      }
    case 10:
      {
        return "ADVENTURE";
      }
    case 11:
      {
        return "CREATIVE MODE";
      }
    case 12:
      {
        return "ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL";
      }
    case 13:
      {
        return "NO BLOCK IN REACH";
      }
    case 14:
      {
        return "CLICK TO CAPTURE  //  Q TO QUIT";
      }
    case 15:
      {
        return "PAUSED";
      }
    case 16:
      {
        return "PLACE BLOCKED: PLAYER OR WORLD EDGE";
      }
    case 17:
      {
        return "CAXECRAFT";
      }
    default:
      {
        return "";
      }
  }
  return "AQUATIC GEAR EQUIPPED";
}

const char *hxc_caxecraft_localization_UiCatalog_textLocale1(int32_t hxc_message)
{
  switch (hxc_message) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return "CAXECRAFT  //  C + HAXE";
      }
    case 2:
      {
        return "CLIC PARA CAPTURAR";
      }
    case 3:
      {
        return "WASD MOVER  1-8/RUEDA OBJETOS  ESPACIO SALTAR  E HABLAR  CLIC IZQ USAR  ESC PAUSA  Q SALIR";
      }
    case 4:
      {
        return "CELDAS";
      }
    case 5:
      {
        return "DIBUJOS";
      }
    case 6:
      {
        return "CUADRO";
      }
    case 7:
      {
        return "PASO";
      }
    case 8:
      {
        return "VISIBLES";
      }
    case 9:
      {
        return "SALUD COMPLETA";
      }
    case 10:
      {
        return "AVENTURA";
      }
    case 11:
      {
        return "MODO CREATIVO";
      }
    case 12:
      {
        return "FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH";
      }
    case 13:
      {
        return "NINGUN BLOQUE AL ALCANCE";
      }
    case 14:
      {
        return "CLIC PARA CAPTURAR  //  Q PARA SALIR";
      }
    case 15:
      {
        return "PAUSA";
      }
    case 16:
      {
        return "NO SE PUEDE COLOCAR: JUGADOR O BORDE";
      }
    case 17:
      {
        return "CAXECRAFT";
      }
    default:
      {
        return "";
      }
  }
  return "EQUIPO ACUATICO ACTIVADO";
}
