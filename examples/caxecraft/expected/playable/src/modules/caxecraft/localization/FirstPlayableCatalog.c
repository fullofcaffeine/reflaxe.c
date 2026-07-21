#include "hxc/program.h"

void hxc_caxecraft_localization_FirstPlayableCatalog_draw(int32_t hxc_locale, int32_t hxc_message, int32_t hxc_x, int32_t hxc_y, int32_t hxc_fontSize, struct Color hxc_color)
{
  switch (hxc_locale) {
    case 0:
      {
        hxc_caxecraft_localization_FirstPlayableCatalog_drawLocale0(hxc_message, hxc_x, hxc_y, hxc_fontSize, hxc_color);
        break;
      }
    case 1:
      {
        hxc_caxecraft_localization_FirstPlayableCatalog_drawLocale1(hxc_message, hxc_x, hxc_y, hxc_fontSize, hxc_color);
        break;
      }
    default:
      {
        hxc_caxecraft_localization_FirstPlayableCatalog_drawLocale0(hxc_message, hxc_x, hxc_y, hxc_fontSize, hxc_color);
        break;
      }
  }
  return;
}

void hxc_caxecraft_localization_FirstPlayableCatalog_drawLocale0(int32_t hxc_message, int32_t hxc_x, int32_t hxc_y, int32_t hxc_fontSize, struct Color hxc_color)
{
  switch (hxc_message) {
    case 0:
      {
        DrawText("ADVENTURE: PROLOGUE IN PROGRESS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 1:
      {
        DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 2:
      {
        DrawText("+1 BERRY", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 3:
      {
        DrawText("+2 BERRIES", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 4:
      {
        DrawText("BERRIES: +1 HEART", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 5:
      {
        DrawText("BERRIES FULL: USE ONE FIRST", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 6:
      {
        DrawText("BLOCK STACK FULL: USE ONE FIRST", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 7:
      {
        DrawText("COPPER STRIKE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 8:
      {
        DrawText("HAXIRIO HAS FALLEN", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 9:
      {
        DrawText("MOSSLING ALERT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 10:
      {
        DrawText("MOSSLING DROPPED BERRIES", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 11:
      {
        DrawText("MOSSLING WINDUP: DODGE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 12:
      {
        DrawText("NIA: BERRIES FOR THE ROAD, HAXIRIO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 13:
      {
        DrawText("E  TALK TO NIA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 14:
      {
        DrawText("NIA: THE GROVE LISTENS. E: A SMALL GIFT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 15:
      {
        DrawText("NO BERRIES LEFT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 16:
      {
        DrawText("E  RETURN TO THE MEADOW", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 17:
      {
        DrawText("Caxecraft: Evergrove Prologue", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 18:
      {
        DrawText("TELEGRAPHED HIT: DODGE THE NEXT", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    default:
      {
        abort();
      }
  }
  return;
}

void hxc_caxecraft_localization_FirstPlayableCatalog_drawLocale1(int32_t hxc_message, int32_t hxc_x, int32_t hxc_y, int32_t hxc_fontSize, struct Color hxc_color)
{
  switch (hxc_message) {
    case 0:
      {
        DrawText("AVENTURA: PROLOGO EN CONSTRUCCION", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 1:
      {
        DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 2:
      {
        DrawText("+1 BAYA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 3:
      {
        DrawText("+2 BAYAS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 4:
      {
        DrawText("BAYAS: +1 CORAZON", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 5:
      {
        DrawText("BAYAS LLENAS: USA UNA PRIMERO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 6:
      {
        DrawText("PILA DE BLOQUES LLENA: USA UNO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 7:
      {
        DrawText("GOLPE DE COBRE", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 8:
      {
        DrawText("HAXIRIO HA CAIDO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 9:
      {
        DrawText("MUSGUITO ALERTA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 10:
      {
        DrawText("EL MUSGUITO SOLTO BAYAS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 11:
      {
        DrawText("MUSGUITO CARGANDO: ESQUIVA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 12:
      {
        DrawText("NIA: BAYAS PARA EL CAMINO, HAXIRIO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 13:
      {
        DrawText("E  HABLAR CON NIA", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 14:
      {
        DrawText("NIA: EL BOSQUE TE ESCUCHA. E: REGALO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 15:
      {
        DrawText("NO QUEDAN BAYAS", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 16:
      {
        DrawText("E  VOLVER AL PRADO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 17:
      {
        DrawText("Caxecraft: Prologo de la Arboleda Eterna", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    case 18:
      {
        DrawText("ATAQUE AVISADO: ESQUIVA EL PROXIMO", (int32_t)hxc_x, (int32_t)hxc_y, (int32_t)hxc_fontSize, hxc_color);
        break;
      }
    default:
      {
        abort();
      }
  }
  return;
}
