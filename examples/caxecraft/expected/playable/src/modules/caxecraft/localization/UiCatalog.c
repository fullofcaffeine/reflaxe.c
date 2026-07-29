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
        return "3D WORLD \342\200\224 LMB TOOL \302\267 RMB LOOK \302\267 WASD/QE FLY \302\267 WHEEL \302\267 F FOCUS";
      }
    case 12:
      {
        return "THE DRAFT NEEDS REPAIR BEFORE TEST PLAY";
      }
    case 13:
      {
        return "WORLD NAME";
      }
    case 14:
      {
        return "NEW WORLD";
      }
    case 15:
      {
        return "READY \342\200\224 EDITS USE THE REAL CAXEMAP SESSION";
      }
    case 16:
      {
        return "REDO";
      }
    case 17:
      {
        return "STOP TEST";
      }
    case 18:
      {
        return "TEST PLAY";
      }
    case 19:
      {
        return "TEST PLAY USES A DISPOSABLE COPY";
      }
    case 20:
      {
        return "CAXECRAFT WORLD EDITOR";
      }
    case 21:
      {
        return "SELECT;PAINT;ERASE;FILL";
      }
    case 22:
      {
        return "UNDO";
      }
    case 23:
      {
        return "VALID CAXEMAP \342\200\224 READY TO TEST";
      }
    case 24:
      {
        return "VALIDATE";
      }
    case 25:
      {
        return "HEALTH ALREADY FULL";
      }
    case 26:
      {
        return "ADVENTURE";
      }
    case 27:
      {
        return "CREATIVE MODE";
      }
    case 28:
      {
        return "WORLD EDITOR";
      }
    case 29:
      {
        return "ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL";
      }
    case 30:
      {
        return "NO BLOCK IN REACH";
      }
    case 31:
      {
        return "CLICK TO CAPTURE  //  Q TO QUIT";
      }
    case 32:
      {
        return "PAUSED";
      }
    case 33:
      {
        return "PLACE BLOCKED: PLAYER OR WORLD EDGE";
      }
    case 34:
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
        return "MUNDO 3D \342\200\224 CLIC IZQ USAR \302\267 CLIC DER MIRAR \302\267 WASD/QE VOLAR \302\267 RUEDA \302\267 F ENFOCAR";
      }
    case 12:
      {
        return "CORRIGE EL BORRADOR ANTES DE PROBARLO";
      }
    case 13:
      {
        return "NOMBRE DEL MUNDO";
      }
    case 14:
      {
        return "MUNDO NUEVO";
      }
    case 15:
      {
        return "LISTO \342\200\224 LOS CAMBIOS USAN LA SESION CAXEMAP REAL";
      }
    case 16:
      {
        return "REHACER";
      }
    case 17:
      {
        return "DETENER PRUEBA";
      }
    case 18:
      {
        return "PROBAR";
      }
    case 19:
      {
        return "LA PRUEBA USA UNA COPIA DESECHABLE";
      }
    case 20:
      {
        return "EDITOR DE MUNDOS CAXECRAFT";
      }
    case 21:
      {
        return "SELECCIONAR;PINTAR;BORRAR;RELLENAR";
      }
    case 22:
      {
        return "DESHACER";
      }
    case 23:
      {
        return "CAXEMAP VALIDO \342\200\224 LISTO PARA PROBAR";
      }
    case 24:
      {
        return "VALIDAR";
      }
    case 25:
      {
        return "SALUD COMPLETA";
      }
    case 26:
      {
        return "AVENTURA";
      }
    case 27:
      {
        return "MODO CREATIVO";
      }
    case 28:
      {
        return "EDITOR DE MUNDOS";
      }
    case 29:
      {
        return "FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH";
      }
    case 30:
      {
        return "NINGUN BLOQUE AL ALCANCE";
      }
    case 31:
      {
        return "CLIC PARA CAPTURAR  //  Q PARA SALIR";
      }
    case 32:
      {
        return "PAUSA";
      }
    case 33:
      {
        return "NO SE PUEDE COLOCAR: JUGADOR O BORDE";
      }
    case 34:
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
