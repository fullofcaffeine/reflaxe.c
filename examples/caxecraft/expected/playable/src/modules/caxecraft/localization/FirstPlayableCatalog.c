#include "hxc/program.h"

const char *hxc_caxecraft_localization_FirstPlayableCatalog_text(int32_t hxc_locale, int32_t hxc_message)
{
  switch (hxc_locale) {
    case 0:
      {
        break;
      }
    case 1:
      {
        const char *hxc_tmp_call_result_n1 = hxc_caxecraft_localization_FirstPlayableCatalog_textLocale1(hxc_message);
        return hxc_tmp_call_result_n1;
      }
    default:
      {
        const char *hxc_tmp_call_result_n2 = hxc_caxecraft_localization_FirstPlayableCatalog_textLocale0(hxc_message);
        return hxc_tmp_call_result_n2;
      }
  }
  const char *hxc_tmp_call_result_n0 = hxc_caxecraft_localization_FirstPlayableCatalog_textLocale0(hxc_message);
  return hxc_tmp_call_result_n0;
}

const char *hxc_caxecraft_localization_FirstPlayableCatalog_textLocale0(int32_t hxc_message)
{
  switch (hxc_message) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return "SAVE CEESH. FIND IVVY. STOP BROWSER.";
      }
    case 2:
      {
        return "+1 BERRY";
      }
    case 3:
      {
        return "+2 BERRIES";
      }
    case 4:
      {
        return "BERRIES: +1 HEART";
      }
    case 5:
      {
        return "BERRIES FULL: USE ONE FIRST";
      }
    case 6:
      {
        return "BLOCK STACK FULL: USE ONE FIRST";
      }
    case 7:
      {
        return "COPPER STRIKE";
      }
    case 8:
      {
        return "HAXIRIO HAS FALLEN";
      }
    case 9:
      {
        return "MOSSLING ALERT";
      }
    case 10:
      {
        return "MOSSLING DROPPED BERRIES";
      }
    case 11:
      {
        return "MOSSLING WINDUP: DODGE";
      }
    case 12:
      {
        return "NIA: BERRIES FOR THE ROAD, HAXIRIO";
      }
    case 13:
      {
        return "E  TALK TO NIA";
      }
    case 14:
      {
        return "NIA: THE GROVE LISTENS. E: A SMALL GIFT";
      }
    case 15:
      {
        return "NO BERRIES LEFT";
      }
    case 16:
      {
        return "E  RETURN TO THE MEADOW";
      }
    case 17:
      {
        return "Caxecraft: Evergrove Prologue";
      }
    case 18:
      {
        return "TELEGRAPHED HIT: DODGE THE NEXT";
      }
    default:
      {
        return "";
      }
  }
  return "ADVENTURE: PROLOGUE IN PROGRESS";
}

const char *hxc_caxecraft_localization_FirstPlayableCatalog_textLocale1(int32_t hxc_message)
{
  switch (hxc_message) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return "SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.";
      }
    case 2:
      {
        return "+1 BAYA";
      }
    case 3:
      {
        return "+2 BAYAS";
      }
    case 4:
      {
        return "BAYAS: +1 CORAZON";
      }
    case 5:
      {
        return "BAYAS LLENAS: USA UNA PRIMERO";
      }
    case 6:
      {
        return "PILA DE BLOQUES LLENA: USA UNO";
      }
    case 7:
      {
        return "GOLPE DE COBRE";
      }
    case 8:
      {
        return "HAXIRIO HA CAIDO";
      }
    case 9:
      {
        return "MUSGUITO ALERTA";
      }
    case 10:
      {
        return "EL MUSGUITO SOLTO BAYAS";
      }
    case 11:
      {
        return "MUSGUITO CARGANDO: ESQUIVA";
      }
    case 12:
      {
        return "NIA: BAYAS PARA EL CAMINO, HAXIRIO";
      }
    case 13:
      {
        return "E  HABLAR CON NIA";
      }
    case 14:
      {
        return "NIA: EL BOSQUE TE ESCUCHA. E: REGALO";
      }
    case 15:
      {
        return "NO QUEDAN BAYAS";
      }
    case 16:
      {
        return "E  VOLVER AL PRADO";
      }
    case 17:
      {
        return "Caxecraft: Prologo de la Arboleda Eterna";
      }
    case 18:
      {
        return "ATAQUE AVISADO: ESQUIVA EL PROXIMO";
      }
    default:
      {
        return "";
      }
  }
  return "AVENTURA: PROLOGO EN CONSTRUCCION";
}
