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
        return "BACK";
      }
    case 10:
      {
        return "WORLD VIEWPORT \342\200\224 VISUAL BLOCK TOOLS ARE NEXT";
      }
    case 11:
      {
        return "THE DRAFT NEEDS REPAIR BEFORE TEST PLAY";
      }
    case 12:
      {
        return "NEW WORLD";
      }
    case 13:
      {
        return "READY \342\200\224 EDITS USE THE REAL CAXEMAP SESSION";
      }
    case 14:
      {
        return "REDO";
      }
    case 15:
      {
        return "STOP TEST";
      }
    case 16:
      {
        return "TEST PLAY";
      }
    case 17:
      {
        return "TEST PLAY USES A DISPOSABLE COPY";
      }
    case 18:
      {
        return "CAXECRAFT WORLD EDITOR";
      }
    case 19:
      {
        return "UNDO";
      }
    case 20:
      {
        return "VALID CAXEMAP \342\200\224 READY TO TEST";
      }
    case 21:
      {
        return "VALIDATE";
      }
    case 22:
      {
        return "HEALTH ALREADY FULL";
      }
    case 23:
      {
        return "ADVENTURE";
      }
    case 24:
      {
        return "CREATIVE MODE";
      }
    case 25:
      {
        return "WORLD EDITOR";
      }
    case 26:
      {
        return "ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL";
      }
    case 27:
      {
        return "NO BLOCK IN REACH";
      }
    case 28:
      {
        return "CLICK TO CAPTURE  //  Q TO QUIT";
      }
    case 29:
      {
        return "PAUSED";
      }
    case 30:
      {
        return "PLACE BLOCKED: PLAYER OR WORLD EDGE";
      }
    case 31:
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
        return "VOLVER";
      }
    case 10:
      {
        return "VISTA DEL MUNDO \342\200\224 HERRAMIENTAS DE BLOQUES MUY PRONTO";
      }
    case 11:
      {
        return "CORRIGE EL BORRADOR ANTES DE PROBARLO";
      }
    case 12:
      {
        return "MUNDO NUEVO";
      }
    case 13:
      {
        return "LISTO \342\200\224 LOS CAMBIOS USAN LA SESION CAXEMAP REAL";
      }
    case 14:
      {
        return "REHACER";
      }
    case 15:
      {
        return "DETENER PRUEBA";
      }
    case 16:
      {
        return "PROBAR";
      }
    case 17:
      {
        return "LA PRUEBA USA UNA COPIA DESECHABLE";
      }
    case 18:
      {
        return "EDITOR DE MUNDOS CAXECRAFT";
      }
    case 19:
      {
        return "DESHACER";
      }
    case 20:
      {
        return "CAXEMAP VALIDO \342\200\224 LISTO PARA PROBAR";
      }
    case 21:
      {
        return "VALIDAR";
      }
    case 22:
      {
        return "SALUD COMPLETA";
      }
    case 23:
      {
        return "AVENTURA";
      }
    case 24:
      {
        return "MODO CREATIVO";
      }
    case 25:
      {
        return "EDITOR DE MUNDOS";
      }
    case 26:
      {
        return "FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH";
      }
    case 27:
      {
        return "NINGUN BLOQUE AL ALCANCE";
      }
    case 28:
      {
        return "CLIC PARA CAPTURAR  //  Q PARA SALIR";
      }
    case 29:
      {
        return "PAUSA";
      }
    case 30:
      {
        return "NO SE PUEDE COLOCAR: JUGADOR O BORDE";
      }
    case 31:
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
