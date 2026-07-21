package caxecraft.localization;

#if c
import raylib.Color;
import raylib.Raylib;

enum abstract LocaleCursor(Int) {
	var Locale0 = 0;
	var Locale1 = 1;
}

enum abstract UiMessage(Int) {
	var Brand = 0;
	var CapturePrompt = 1;
	var Controls = 2;
	var DebugCells = 3;
	var DebugDraws = 4;
	var DebugFrame = 5;
	var DebugTick = 6;
	var DebugVisible = 7;
	var HealthFull = 8;
	var MenuAdventure = 9;
	var MenuCreative = 10;
	var MenuInstructions = 11;
	var NoBlockInReach = 12;
	var PauseHelp = 13;
	var PauseTitle = 14;
	var PlaceBlocked = 15;
	var TitleFallback = 16;
}

/**
 * C rendering adapter generated from `locales/ui.json`.
 *
 * The JSON catalog is the editable source of truth. Each branch keeps a
 * direct string literal at the raylib call so haxe.c can prove static C
 * lifetime. Gameplay and UI code choose only typed message IDs.
 */
final class UiCatalog {
	public static inline function defaultLocale():LocaleCursor
		return LocaleCursor.Locale0;

	public static function nextLocale(locale:LocaleCursor):LocaleCursor
		return switch (locale) {
			case Locale0: Locale1;
			case Locale1: Locale0;
			case _: Locale0;
		}

	public static function draw(locale:LocaleCursor, message:UiMessage, x:Int, y:Int, fontSize:Int, color:Color):Void {
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
	static function drawLocale0(message:UiMessage, x:Int, y:Int, fontSize:Int, color:Color):Void {
		switch (message) {
			case Brand:
				Raylib.DrawText("CAXECRAFT  //  C + HAXE", x, y, fontSize, color);
			case CapturePrompt:
				Raylib.DrawText("CLICK TO CAPTURE", x, y, fontSize, color);
			case Controls:
				Raylib.DrawText("WASD MOVE  1-8/WHEEL ITEMS  SPACE JUMP  E TALK  LMB MINE/STRIKE  RMB USE  ESC PAUSE  Q QUIT", x, y, fontSize, color);
			case DebugCells:
				Raylib.DrawText("CELLS", x, y, fontSize, color);
			case DebugDraws:
				Raylib.DrawText("DRAWS", x, y, fontSize, color);
			case DebugFrame:
				Raylib.DrawText("FRAME", x, y, fontSize, color);
			case DebugTick:
				Raylib.DrawText("TICK", x, y, fontSize, color);
			case DebugVisible:
				Raylib.DrawText("VISIBLE", x, y, fontSize, color);
			case HealthFull:
				Raylib.DrawText("HEALTH ALREADY FULL", x, y, fontSize, color);
			case MenuAdventure:
				Raylib.DrawText("ADVENTURE", x, y, fontSize, color);
			case MenuCreative:
				Raylib.DrawText("CREATIVE MODE", x, y, fontSize, color);
			case MenuInstructions:
				Raylib.DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", x, y, fontSize, color);
			case NoBlockInReach:
				Raylib.DrawText("NO BLOCK IN REACH", x, y, fontSize, color);
			case PauseHelp:
				Raylib.DrawText("CLICK TO CAPTURE  //  Q TO QUIT", x, y, fontSize, color);
			case PauseTitle:
				Raylib.DrawText("PAUSED", x, y, fontSize, color);
			case PlaceBlocked:
				Raylib.DrawText("PLACE BLOCKED: PLAYER OR WORLD EDGE", x, y, fontSize, color);
			case TitleFallback:
				Raylib.DrawText("CAXECRAFT", x, y, fontSize, color);
		}
	}

	/** Direct C literals for the validated `es-MX` catalog. */
	static function drawLocale1(message:UiMessage, x:Int, y:Int, fontSize:Int, color:Color):Void {
		switch (message) {
			case Brand:
				Raylib.DrawText("CAXECRAFT  //  C + HAXE", x, y, fontSize, color);
			case CapturePrompt:
				Raylib.DrawText("CLIC PARA CAPTURAR", x, y, fontSize, color);
			case Controls:
				Raylib.DrawText("WASD MOVER  1-8/RUEDA OBJETOS  ESPACIO SALTAR  E HABLAR  CLIC IZQ USAR  ESC PAUSA  Q SALIR", x, y, fontSize, color);
			case DebugCells:
				Raylib.DrawText("CELDAS", x, y, fontSize, color);
			case DebugDraws:
				Raylib.DrawText("DIBUJOS", x, y, fontSize, color);
			case DebugFrame:
				Raylib.DrawText("CUADRO", x, y, fontSize, color);
			case DebugTick:
				Raylib.DrawText("PASO", x, y, fontSize, color);
			case DebugVisible:
				Raylib.DrawText("VISIBLES", x, y, fontSize, color);
			case HealthFull:
				Raylib.DrawText("SALUD COMPLETA", x, y, fontSize, color);
			case MenuAdventure:
				Raylib.DrawText("AVENTURA", x, y, fontSize, color);
			case MenuCreative:
				Raylib.DrawText("MODO CREATIVO", x, y, fontSize, color);
			case MenuInstructions:
				Raylib.DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", x, y, fontSize, color);
			case NoBlockInReach:
				Raylib.DrawText("NINGUN BLOQUE AL ALCANCE", x, y, fontSize, color);
			case PauseHelp:
				Raylib.DrawText("CLIC PARA CAPTURAR  //  Q PARA SALIR", x, y, fontSize, color);
			case PauseTitle:
				Raylib.DrawText("PAUSA", x, y, fontSize, color);
			case PlaceBlocked:
				Raylib.DrawText("NO SE PUEDE COLOCAR: JUGADOR O BORDE", x, y, fontSize, color);
			case TitleFallback:
				Raylib.DrawText("CAXECRAFT", x, y, fontSize, color);
		}
	}
}
#end
