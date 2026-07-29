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
        return "ADVANCED TOOLS";
      }
    case 10:
      {
        return "BACK";
      }
    case 11:
      {
        return "WORLD VIEWPORT \342\200\224 LEFT CLICK TO APPLY THE SELECTED TOOL";
      }
    case 12:
      {
        return "THE DRAFT NEEDS REPAIR BEFORE TEST PLAY";
      }
    case 13:
      {
        return "NEW WORLD";
      }
    case 14:
      {
        return "READY \342\200\224 EDITS USE THE REAL CAXEMAP SESSION";
      }
    case 15:
      {
        return "REDO";
      }
    case 16:
      {
        return "STOP TEST";
      }
    case 17:
      {
        return "TEST PLAY";
      }
    case 18:
      {
        return "TEST PLAY USES A DISPOSABLE COPY";
      }
    case 19:
      {
        return "CAXECRAFT WORLD EDITOR";
      }
    case 20:
      {
        return "SELECT;PAINT;ERASE;FILL";
      }
    case 21:
      {
        return "UNDO";
      }
    case 22:
      {
        return "VALID CAXEMAP \342\200\224 READY TO TEST";
      }
    case 23:
      {
        return "VALIDATE";
      }
    case 24:
      {
        return "HEALTH ALREADY FULL";
      }
    case 25:
      {
        return "ADVENTURE";
      }
    case 26:
      {
        return "CREATIVE MODE";
      }
    case 27:
      {
        return "WORLD EDITOR";
      }
    case 28:
      {
        return "ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL";
      }
    case 29:
      {
        return "NO BLOCK IN REACH";
      }
    case 30:
      {
        return "CLICK TO CAPTURE  //  Q TO QUIT";
      }
    case 31:
      {
        return "PAUSED";
      }
    case 32:
      {
        return "PLACE BLOCKED: PLAYER OR WORLD EDGE";
      }
    case 33:
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
        return "HERRAMIENTAS AVANZADAS";
      }
    case 10:
      {
        return "VOLVER";
      }
    case 11:
      {
        return "VISTA DEL MUNDO \342\200\224 CLIC IZQUIERDO PARA USAR LA HERRAMIENTA";
      }
    case 12:
      {
        return "CORRIGE EL BORRADOR ANTES DE PROBARLO";
      }
    case 13:
      {
        return "MUNDO NUEVO";
      }
    case 14:
      {
        return "LISTO \342\200\224 LOS CAMBIOS USAN LA SESION CAXEMAP REAL";
      }
    case 15:
      {
        return "REHACER";
      }
    case 16:
      {
        return "DETENER PRUEBA";
      }
    case 17:
      {
        return "PROBAR";
      }
    case 18:
      {
        return "LA PRUEBA USA UNA COPIA DESECHABLE";
      }
    case 19:
      {
        return "EDITOR DE MUNDOS CAXECRAFT";
      }
    case 20:
      {
        return "SELECCIONAR;PINTAR;BORRAR;RELLENAR";
      }
    case 21:
      {
        return "DESHACER";
      }
    case 22:
      {
        return "CAXEMAP VALIDO \342\200\224 LISTO PARA PROBAR";
      }
    case 23:
      {
        return "VALIDAR";
      }
    case 24:
      {
        return "SALUD COMPLETA";
      }
    case 25:
      {
        return "AVENTURA";
      }
    case 26:
      {
        return "MODO CREATIVO";
      }
    case 27:
      {
        return "EDITOR DE MUNDOS";
      }
    case 28:
      {
        return "FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH";
      }
    case 29:
      {
        return "NINGUN BLOQUE AL ALCANCE";
      }
    case 30:
      {
        return "CLIC PARA CAPTURAR  //  Q PARA SALIR";
      }
    case 31:
      {
        return "PAUSA";
      }
    case 32:
      {
        return "NO SE PUEDE COLOCAR: JUGADOR O BORDE";
      }
    case 33:
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
