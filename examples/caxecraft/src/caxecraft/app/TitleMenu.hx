package caxecraft.app;

#if c
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

	public static inline function draw(title:Texture2D, titleReady:Bool, wordmark:Texture2D, wordmarkReady:Bool, selected:GameMode,
			language:GameLanguage):Void {
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
			Raylib.DrawText("CAXECRAFT", Std.int(width / 2) - 92, 52, 34, Color.rgba(242, 249, 245));

		final firstTop = buttonTop(height);
		if (language == GameLanguage.Spanish) {
			drawButton(firstTop, selected == GameMode.Creative, "MODO CREATIVO", width);
			drawButton(firstTop + BUTTON_HEIGHT + BUTTON_GAP, selected == GameMode.Adventure, "AVENTURA", width);
			Raylib.DrawText("FLECHAS / RATON PARA ELEGIR   ENTER PARA JUGAR   L: ENGLISH", Std.int(width / 2) - 270, height - 58, 16,
				Color.rgba(229, 241, 235));
		} else {
			drawButton(firstTop, selected == GameMode.Creative, "CREATIVE MODE", width);
			drawButton(firstTop + BUTTON_HEIGHT + BUTTON_GAP, selected == GameMode.Adventure, "ADVENTURE", width);
			Raylib.DrawText("ARROWS / MOUSE TO CHOOSE   ENTER TO PLAY   L: ESPANOL", Std.int(width / 2) - 270, height - 58, 16, Color.rgba(229, 241, 235));
		}
		if (selected == GameMode.Adventure) {
			if (language == GameLanguage.Spanish)
				Raylib.DrawText("SALVA A CEESH. ENCUENTRA A IVVY. DETEN A BROWSER.", Std.int(width / 2) - 210, firstTop - 36, 17, Color.rgba(255, 205, 91));
			else
				Raylib.DrawText("SAVE CEESH. FIND IVVY. STOP BROWSER.", Std.int(width / 2) - 210, firstTop - 36, 17, Color.rgba(255, 205, 91));
		}
	}

	static inline function drawButton(top:Int, active:Bool, label:c.CString, width:Int):Void {
		final left = Std.int(width / 2) - Std.int(BUTTON_WIDTH / 2);
		if (active) {
			Raylib.DrawRectangle(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(16, 88, 102, 232));
			Raylib.DrawRectangleLines(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(255, 190, 55));
		} else {
			Raylib.DrawRectangle(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(6, 26, 36, 218));
			Raylib.DrawRectangleLines(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, Color.rgba(92, 194, 188));
		}
		Raylib.DrawText(label, left + 28, top + 16, 21, Color.rgba(242, 249, 245));
	}

	static inline function buttonTop(height:Int):Int
		return height - 225;
}
#end
