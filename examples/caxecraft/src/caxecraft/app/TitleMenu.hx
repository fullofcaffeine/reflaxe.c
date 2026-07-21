package caxecraft.app;

#if c
import caxecraft.localization.FirstPlayableCatalog;
import caxecraft.localization.FirstPlayableCatalog.ScenarioMessage;
import caxecraft.localization.UiCatalog;
import caxecraft.localization.UiCatalog.LocaleCursor;
import caxecraft.localization.UiCatalog.UiMessage;
import raylib.Color;
import raylib.Raylib;
import raylib.Texture2D;

/** Draws and hit-tests the first Caxecraft title screen. */
final class TitleMenu {
	static inline final BUTTON_WIDTH:Int = 360;
	static inline final BUTTON_HEIGHT:Int = 54;
	static inline final BUTTON_GAP:Int = 14;

	public static function selectionAt(mouseX:Float, mouseY:Float, width:Int, height:Int):Int {
		final left = Std.int(width / 2) - Std.int(BUTTON_WIDTH / 2);
		final firstTop = buttonTop(height);
		if (mouseX >= left && mouseX < left + BUTTON_WIDTH) {
			if (mouseY >= firstTop && mouseY < firstTop + BUTTON_HEIGHT)
				return 0;
			final secondTop = firstTop + BUTTON_HEIGHT + BUTTON_GAP;
			if (mouseY >= secondTop && mouseY < secondTop + BUTTON_HEIGHT)
				return 1;
		}
		return -1;
	}

	public static inline function draw(title:Texture2D, titleReady:Bool, wordmark:Texture2D, wordmarkReady:Bool, selected:GameMode, locale:LocaleCursor):Void {
		final width = Raylib.GetScreenWidth();
		final height = Raylib.GetScreenHeight();
		if (titleReady)
			CaxecraftTextures.drawCover(title, width, height, Color.rgba(255, 255, 255));
		else
			Raylib.ClearBackground(CaxecraftPalette.sky());

		// The dark veil preserves the panorama while giving both languages a
		// stable contrast ratio on bright sky, snow, or water regions.
		Raylib.DrawRectangle(0, 0, width, height, Color.rgba(4, 14, 23, 132));
		if (wordmarkReady)
			CaxecraftTextures.drawContained(wordmark, Std.int(width / 2), 28, 560, 132, Color.rgba(255, 255, 255));
		else
			UiCatalog.draw(locale, UiMessage.TitleFallback, Std.int(width / 2) - 92, 52, 34, Color.rgba(242, 249, 245));

		final firstTop = buttonTop(height);
		drawButton(firstTop, selected == GameMode.Creative, UiMessage.MenuCreative, locale, width);
		drawButton(firstTop + BUTTON_HEIGHT + BUTTON_GAP, selected == GameMode.Adventure, UiMessage.MenuAdventure, locale, width);
		UiCatalog.draw(locale, UiMessage.MenuInstructions, Std.int(width / 2) - 285, height - 58, 16, Color.rgba(229, 241, 235));
		if (selected == GameMode.Adventure)
			FirstPlayableCatalog.draw(locale, ScenarioMessage.AdventureTagline, Std.int(width / 2) - 230, firstTop - 36, 17, Color.rgba(255, 205, 91));
	}

	static inline function drawButton(top:Int, active:Bool, message:UiMessage, locale:LocaleCursor, width:Int):Void {
		final left = Std.int(width / 2) - Std.int(BUTTON_WIDTH / 2);
		if (active) {
			Raylib.DrawRectangle(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(16, 88, 102, 232));
			Raylib.DrawRectangleLines(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(255, 190, 55));
		} else {
			Raylib.DrawRectangle(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(6, 26, 36, 218));
			Raylib.DrawRectangleLines(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(92, 194, 188));
		}
		UiCatalog.draw(locale, message, left + 28, top + 16, 21, Color.rgba(242, 249, 245));
	}

	static inline function buttonTop(height:Int):Int
		return height - 225;
}
#end
