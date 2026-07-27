package caxecraft.localization;

import caxecraft.localization.LocalizationText;

enum abstract LocaleCursor(Int) {
	var Locale0 = 0;
	var Locale1 = 1;
}

enum abstract UiMessage(Int) {
	var AquaticGearEquipped = 0;
	var Brand = 1;
	var CapturePrompt = 2;
	var Controls = 3;
	var DebugCells = 4;
	var DebugDraws = 5;
	var DebugFrame = 6;
	var DebugTick = 7;
	var DebugVisible = 8;
	var EditorAdvanced = 9;
	var EditorBack = 10;
	var EditorCanvasHelp = 11;
	var EditorInvalid = 12;
	var EditorNewWorld = 13;
	var EditorReady = 14;
	var EditorRedo = 15;
	var EditorStopTest = 16;
	var EditorTest = 17;
	var EditorTesting = 18;
	var EditorTitle = 19;
	var EditorToolList = 20;
	var EditorUndo = 21;
	var EditorValid = 22;
	var EditorValidate = 23;
	var HealthFull = 24;
	var MenuAdventure = 25;
	var MenuCreative = 26;
	var MenuEditor = 27;
	var MenuInstructions = 28;
	var NoBlockInReach = 29;
	var PauseHelp = 30;
	var PauseTitle = 31;
	var PlaceBlocked = 32;
	var TitleFallback = 33;
}

/**
 * Typed text catalog generated from `locales/ui.json`.
 *
 * The JSON catalog is the editable source of truth. Callers choose a
 * typed message ID and receive text; rendering, layout, and input remain
 * outside localization. The C carrier borrows only generated literals
 * with static lifetime and therefore allocates nothing.
 */
final class UiCatalog {
	public static inline function defaultLocale():LocaleCursor
		return LocaleCursor.Locale0;

	/** Reject an unknown raw locale code before constructing a typed cursor. */
	public static inline function isValidLocaleStorageCode(code:Int):Bool
		return code >= 0 && code < 2;

	public static function nextLocale(locale:LocaleCursor):LocaleCursor
		return switch (locale) {
			case Locale0: Locale1;
			case Locale1: Locale0;
			case _: Locale0;
		}

	/** Reject an unknown raw message code before constructing a typed ID. */
	public static inline function isValidMessageStorageCode(code:Int):Bool
		return code >= 0 && code < 34;

	public static function text(locale:LocaleCursor, message:UiMessage):LocalizationText {
		return switch (locale) {
			case Locale0: textLocale0(message);
			case Locale1: textLocale1(message);
			case _: textLocale0(message);
		}
	}

	/** Select one validated `en` literal without allocation. */
	static function textLocale0(message:UiMessage):LocalizationText {
		return switch (message) {
			case AquaticGearEquipped: "AQUATIC GEAR EQUIPPED";
			case Brand: "CAXECRAFT  //  C + HAXE";
			case CapturePrompt: "CLICK TO CAPTURE";
			case Controls: "WASD MOVE  1-8/WHEEL ITEMS  SPACE JUMP  E TALK  LMB MINE/STRIKE  RMB USE  ESC PAUSE  Q QUIT";
			case DebugCells: "CELLS";
			case DebugDraws: "DRAWS";
			case DebugFrame: "FRAME";
			case DebugTick: "TICK";
			case DebugVisible: "VISIBLE";
			case EditorAdvanced: "ADVANCED TOOLS";
			case EditorBack: "BACK";
			case EditorCanvasHelp: "WORLD VIEWPORT — VISUAL BLOCK TOOLS ARE NEXT";
			case EditorInvalid: "THE DRAFT NEEDS REPAIR BEFORE TEST PLAY";
			case EditorNewWorld: "NEW WORLD";
			case EditorReady: "READY — EDITS USE THE REAL CAXEMAP SESSION";
			case EditorRedo: "REDO";
			case EditorStopTest: "STOP TEST";
			case EditorTest: "TEST PLAY";
			case EditorTesting: "TEST PLAY USES A DISPOSABLE COPY";
			case EditorTitle: "CAXECRAFT WORLD EDITOR";
			case EditorToolList: "SELECT;PAINT;ERASE;FILL";
			case EditorUndo: "UNDO";
			case EditorValid: "VALID CAXEMAP — READY TO TEST";
			case EditorValidate: "VALIDATE";
			case HealthFull: "HEALTH ALREADY FULL";
			case MenuAdventure: "ADVENTURE";
			case MenuCreative: "CREATIVE MODE";
			case MenuEditor: "WORLD EDITOR";
			case MenuInstructions: "ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL";
			case NoBlockInReach: "NO BLOCK IN REACH";
			case PauseHelp: "CLICK TO CAPTURE  //  Q TO QUIT";
			case PauseTitle: "PAUSED";
			case PlaceBlocked: "PLACE BLOCKED: PLAYER OR WORLD EDGE";
			case TitleFallback: "CAXECRAFT";
			case _: "";
		}
	}

	/** Select one validated `es-MX` literal without allocation. */
	static function textLocale1(message:UiMessage):LocalizationText {
		return switch (message) {
			case AquaticGearEquipped: "EQUIPO ACUATICO ACTIVADO";
			case Brand: "CAXECRAFT  //  C + HAXE";
			case CapturePrompt: "CLIC PARA CAPTURAR";
			case Controls: "WASD MOVER  1-8/RUEDA OBJETOS  ESPACIO SALTAR  E HABLAR  CLIC IZQ USAR  ESC PAUSA  Q SALIR";
			case DebugCells: "CELDAS";
			case DebugDraws: "DIBUJOS";
			case DebugFrame: "CUADRO";
			case DebugTick: "PASO";
			case DebugVisible: "VISIBLES";
			case EditorAdvanced: "HERRAMIENTAS AVANZADAS";
			case EditorBack: "VOLVER";
			case EditorCanvasHelp: "VISTA DEL MUNDO — HERRAMIENTAS DE BLOQUES MUY PRONTO";
			case EditorInvalid: "CORRIGE EL BORRADOR ANTES DE PROBARLO";
			case EditorNewWorld: "MUNDO NUEVO";
			case EditorReady: "LISTO — LOS CAMBIOS USAN LA SESION CAXEMAP REAL";
			case EditorRedo: "REHACER";
			case EditorStopTest: "DETENER PRUEBA";
			case EditorTest: "PROBAR";
			case EditorTesting: "LA PRUEBA USA UNA COPIA DESECHABLE";
			case EditorTitle: "EDITOR DE MUNDOS CAXECRAFT";
			case EditorToolList: "SELECCIONAR;PINTAR;BORRAR;RELLENAR";
			case EditorUndo: "DESHACER";
			case EditorValid: "CAXEMAP VALIDO — LISTO PARA PROBAR";
			case EditorValidate: "VALIDAR";
			case HealthFull: "SALUD COMPLETA";
			case MenuAdventure: "AVENTURA";
			case MenuCreative: "MODO CREATIVO";
			case MenuEditor: "EDITOR DE MUNDOS";
			case MenuInstructions: "FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH";
			case NoBlockInReach: "NINGUN BLOQUE AL ALCANCE";
			case PauseHelp: "CLIC PARA CAPTURAR  //  Q PARA SALIR";
			case PauseTitle: "PAUSA";
			case PlaceBlocked: "NO SE PUEDE COLOCAR: JUGADOR O BORDE";
			case TitleFallback: "CAXECRAFT";
			case _: "";
		}
	}
}
