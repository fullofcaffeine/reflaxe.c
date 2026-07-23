package caxecraft.localization;

import caxecraft.localization.UiCatalog.LocaleCursor;
import caxecraft.localization.LocalizationText;

enum abstract ScenarioMessage(Int) {
	var AdventureProgress = 0;
	var AdventureTagline = 1;
	var BerryPickupOne = 2;
	var BerryPickupTwo = 3;
	var BerryRecovery = 4;
	var BerryStackFull = 5;
	var BlockStackFull = 6;
	var CopperStrike = 7;
	var HaxirioFallen = 8;
	var MosslingAlert = 9;
	var MosslingDroppedBerries = 10;
	var MosslingWindup = 11;
	var NiaGift = 12;
	var NiaTalk = 13;
	var NiaWelcome = 14;
	var NoBerries = 15;
	var ReturnToMeadow = 16;
	var ScenarioTitle = 17;
	var TelegraphedHit = 18;
}

/**
 * Typed text catalog generated from `scenarios/first-playable/map.caxemap`.
 *
 * The embedded CaxeMap catalog is the editable source of truth. Callers choose a
 * typed message ID and receive text; rendering, layout, and input remain
 * outside localization. The C carrier borrows only generated literals
 * with static lifetime and therefore allocates nothing.
 */
final class FirstPlayableCatalog {
	/** Reject an unknown raw message code before constructing a typed ID. */
	public static inline function isValidMessageStorageCode(code:Int):Bool
		return code >= 0 && code < 19;

	public static function text(locale:LocaleCursor, message:ScenarioMessage):LocalizationText {
		return switch (locale) {
			case Locale0: textLocale0(message);
			case Locale1: textLocale1(message);
			case _: textLocale0(message);
		}
	}

	/** Select one validated `en` literal without allocation. */
	static function textLocale0(message:ScenarioMessage):LocalizationText {
		return switch (message) {
			case AdventureProgress: "ADVENTURE: PROLOGUE IN PROGRESS";
			case AdventureTagline: "SAVE CEESH. FIND IVVY. STOP BROWSER.";
			case BerryPickupOne: "+1 BERRY";
			case BerryPickupTwo: "+2 BERRIES";
			case BerryRecovery: "BERRIES: +1 HEART";
			case BerryStackFull: "BERRIES FULL: USE ONE FIRST";
			case BlockStackFull: "BLOCK STACK FULL: USE ONE FIRST";
			case CopperStrike: "COPPER STRIKE";
			case HaxirioFallen: "HAXIRIO HAS FALLEN";
			case MosslingAlert: "MOSSLING ALERT";
			case MosslingDroppedBerries: "MOSSLING DROPPED BERRIES";
			case MosslingWindup: "MOSSLING WINDUP: DODGE";
			case NiaGift: "NIA: BERRIES FOR THE ROAD, HAXIRIO";
			case NiaTalk: "E  TALK TO NIA";
			case NiaWelcome: "NIA: THE GROVE LISTENS. E: A SMALL GIFT";
			case NoBerries: "NO BERRIES LEFT";
			case ReturnToMeadow: "E  RETURN TO THE MEADOW";
			case ScenarioTitle: "Caxecraft: Evergrove Prologue";
			case TelegraphedHit: "TELEGRAPHED HIT: DODGE THE NEXT";
			case _: "";
		}
	}

	/** Select one validated `es-mx` literal without allocation. */
	static function textLocale1(message:ScenarioMessage):LocalizationText {
		return switch (message) {
			case AdventureProgress: "AVENTURA: PROLOGO EN CONSTRUCCION";
			case AdventureTagline: "SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.";
			case BerryPickupOne: "+1 BAYA";
			case BerryPickupTwo: "+2 BAYAS";
			case BerryRecovery: "BAYAS: +1 CORAZON";
			case BerryStackFull: "BAYAS LLENAS: USA UNA PRIMERO";
			case BlockStackFull: "PILA DE BLOQUES LLENA: USA UNO";
			case CopperStrike: "GOLPE DE COBRE";
			case HaxirioFallen: "HAXIRIO HA CAIDO";
			case MosslingAlert: "MUSGUITO ALERTA";
			case MosslingDroppedBerries: "EL MUSGUITO SOLTO BAYAS";
			case MosslingWindup: "MUSGUITO CARGANDO: ESQUIVA";
			case NiaGift: "NIA: BAYAS PARA EL CAMINO, HAXIRIO";
			case NiaTalk: "E  HABLAR CON NIA";
			case NiaWelcome: "NIA: EL BOSQUE TE ESCUCHA. E: REGALO";
			case NoBerries: "NO QUEDAN BAYAS";
			case ReturnToMeadow: "E  VOLVER AL PRADO";
			case ScenarioTitle: "Caxecraft: Prologo de la Arboleda Eterna";
			case TelegraphedHit: "ATAQUE AVISADO: ESQUIVA EL PROXIMO";
			case _: "";
		}
	}
}
