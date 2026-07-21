package caxecraft.localization;

#if c
import caxecraft.localization.UiCatalog.LocaleCursor;
import raylib.Color;
import raylib.Raylib;

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
	var TelegraphedHit = 17;
}

/**
 * C rendering adapter generated from `scenarios/first-playable/messages.json`.
 *
 * The JSON catalog is the editable source of truth. Each branch keeps a
 * direct string literal at the raylib call so haxe.c can prove static C
 * lifetime. Gameplay and UI code choose only typed message IDs.
 */
final class FirstPlayableCatalog {
	public static function draw(locale:LocaleCursor, message:ScenarioMessage, x:Int, y:Int, fontSize:Int, color:Color):Void {
		switch (locale) {
			case Locale0:
				drawLocale0(message, x, y, fontSize, color);
			case Locale1:
				drawLocale1(message, x, y, fontSize, color);
			case _:
				drawLocale0(message, x, y, fontSize, color);
		}
	}

	/** Direct C literals for the validated `en` catalog. */
	static function drawLocale0(message:ScenarioMessage, x:Int, y:Int, fontSize:Int, color:Color):Void {
		switch (message) {
			case AdventureProgress:
				Raylib.DrawText("ADVENTURE: PROLOGUE IN PROGRESS", x, y, fontSize, color);
			case AdventureTagline:
				Raylib.DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", x, y, fontSize, color);
			case BerryPickupOne:
				Raylib.DrawText("+1 BERRY", x, y, fontSize, color);
			case BerryPickupTwo:
				Raylib.DrawText("+2 BERRIES", x, y, fontSize, color);
			case BerryRecovery:
				Raylib.DrawText("BERRIES: +1 HEART", x, y, fontSize, color);
			case BerryStackFull:
				Raylib.DrawText("BERRIES FULL: USE ONE FIRST", x, y, fontSize, color);
			case BlockStackFull:
				Raylib.DrawText("BLOCK STACK FULL: USE ONE FIRST", x, y, fontSize, color);
			case CopperStrike:
				Raylib.DrawText("COPPER STRIKE", x, y, fontSize, color);
			case HaxirioFallen:
				Raylib.DrawText("HAXIRIO HAS FALLEN", x, y, fontSize, color);
			case MosslingAlert:
				Raylib.DrawText("MOSSLING ALERT", x, y, fontSize, color);
			case MosslingDroppedBerries:
				Raylib.DrawText("MOSSLING DROPPED BERRIES", x, y, fontSize, color);
			case MosslingWindup:
				Raylib.DrawText("MOSSLING WINDUP: DODGE", x, y, fontSize, color);
			case NiaGift:
				Raylib.DrawText("NIA: BERRIES FOR THE ROAD, HAXIRIO", x, y, fontSize, color);
			case NiaTalk:
				Raylib.DrawText("E  TALK TO NIA", x, y, fontSize, color);
			case NiaWelcome:
				Raylib.DrawText("NIA: THE GROVE LISTENS. E: A SMALL GIFT", x, y, fontSize, color);
			case NoBerries:
				Raylib.DrawText("NO BERRIES LEFT", x, y, fontSize, color);
			case ReturnToMeadow:
				Raylib.DrawText("E  RETURN TO THE MEADOW", x, y, fontSize, color);
			case TelegraphedHit:
				Raylib.DrawText("TELEGRAPHED HIT: DODGE THE NEXT", x, y, fontSize, color);
		}
	}

	/** Direct C literals for the validated `es-MX` catalog. */
	static function drawLocale1(message:ScenarioMessage, x:Int, y:Int, fontSize:Int, color:Color):Void {
		switch (message) {
			case AdventureProgress:
				Raylib.DrawText("AVENTURA: PROLOGO EN CONSTRUCCION", x, y, fontSize, color);
			case AdventureTagline:
				Raylib.DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", x, y, fontSize, color);
			case BerryPickupOne:
				Raylib.DrawText("+1 BAYA", x, y, fontSize, color);
			case BerryPickupTwo:
				Raylib.DrawText("+2 BAYAS", x, y, fontSize, color);
			case BerryRecovery:
				Raylib.DrawText("BAYAS: +1 CORAZON", x, y, fontSize, color);
			case BerryStackFull:
				Raylib.DrawText("BAYAS LLENAS: USA UNA PRIMERO", x, y, fontSize, color);
			case BlockStackFull:
				Raylib.DrawText("PILA DE BLOQUES LLENA: USA UNO", x, y, fontSize, color);
			case CopperStrike:
				Raylib.DrawText("GOLPE DE COBRE", x, y, fontSize, color);
			case HaxirioFallen:
				Raylib.DrawText("HAXIRIO HA CAIDO", x, y, fontSize, color);
			case MosslingAlert:
				Raylib.DrawText("MUSGUITO ALERTA", x, y, fontSize, color);
			case MosslingDroppedBerries:
				Raylib.DrawText("EL MUSGUITO SOLTO BAYAS", x, y, fontSize, color);
			case MosslingWindup:
				Raylib.DrawText("MUSGUITO CARGANDO: ESQUIVA", x, y, fontSize, color);
			case NiaGift:
				Raylib.DrawText("NIA: BAYAS PARA EL CAMINO, HAXIRIO", x, y, fontSize, color);
			case NiaTalk:
				Raylib.DrawText("E  HABLAR CON NIA", x, y, fontSize, color);
			case NiaWelcome:
				Raylib.DrawText("NIA: EL BOSQUE TE ESCUCHA. E: REGALO", x, y, fontSize, color);
			case NoBerries:
				Raylib.DrawText("NO QUEDAN BAYAS", x, y, fontSize, color);
			case ReturnToMeadow:
				Raylib.DrawText("E  VOLVER AL PRADO", x, y, fontSize, color);
			case TelegraphedHit:
				Raylib.DrawText("ATAQUE AVISADO: ESQUIVA EL PROXIMO", x, y, fontSize, color);
		}
	}
}
#end
