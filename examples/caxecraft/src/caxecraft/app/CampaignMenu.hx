package caxecraft.app;

#if c
import caxecraft.content.CampaignManifest.CampaignManifest;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.localization.UiTypes.UiMessage;
import raylib.Color;
import raylib.Raylib;
import raylib.Texture2D;

/** The one campaign-screen control under a pointer for this frame. */
enum CampaignMenuHit {
	NoCampaignMenuHit;
	LevelHit(index:Int);
	LaunchHit;
	BackHit;
}

/**
 * Presents the campaign that the active runtime package actually supplied.
 *
 * The first slice has one selectable campaign, so this screen draws one card
 * from the validated manifest rather than inventing a hard-coded catalog. Its
 * identity, entry level, and route therefore change with staged data without a
 * game recompile. A later package-discovery owner can provide several cards
 * while preserving this manifest-backed presentation boundary.
 */
final class CampaignMenu {
	static inline final PANEL_WIDTH:Int = 680;
	static inline final PANEL_HEIGHT:Int = 390;
	static inline final BUTTON_WIDTH:Int = 250;
	static inline final BUTTON_HEIGHT:Int = 48;

	/**
	 * Return the level row or action button under one pointer position.
	 *
	 * The caller supplies the validated manifest's level count, so hit testing and
	 * drawing agree without keeping a second list inside the presentation layer.
	 */
	public static function selectionAt(mouseX:Float, mouseY:Float, width:Int, height:Int, levelCount:Int):CampaignMenuHit {
		final panelLeft = Std.int(width / 2) - Std.int(PANEL_WIDTH / 2);
		final panelTop = Std.int(height / 2) - 120;
		final buttonTop = panelTop + PANEL_HEIGHT - 68;
		if (mouseY >= buttonTop && mouseY < buttonTop + BUTTON_HEIGHT) {
			if (mouseX >= panelLeft + 42 && mouseX < panelLeft + 42 + BUTTON_WIDTH)
				return LaunchHit;
			if (mouseX >= panelLeft + PANEL_WIDTH - 42 - BUTTON_WIDTH && mouseX < panelLeft + PANEL_WIDTH - 42)
				return BackHit;
		}
		final levelTop = panelTop + 190;
		if (mouseX >= panelLeft + 32 && mouseX < panelLeft + PANEL_WIDTH - 32 && mouseY >= levelTop && mouseY < buttonTop) {
			final index = Std.int((mouseY - levelTop) / 34);
			if (index >= 0 && index < levelCount)
				return LevelHit(index);
		}
		return NoCampaignMenuHit;
	}

	/** Draw one validated campaign card with runtime-loaded title and summary text. */
	public static function draw(title:Texture2D, titleReady:Bool, wordmark:Texture2D, wordmarkReady:Bool, campaign:CampaignManifest, locale:LocaleCursor,
			catalog:RuntimeUiCatalog, selectedLevelIndex:Int, scenarioTitle:String, adventureTagline:String):Void {
		final width = Raylib.GetScreenWidth();
		final height = Raylib.GetScreenHeight();
		if (titleReady)
			CaxecraftTextures.drawCover(title, width, height, Color.rgba(255, 255, 255));
		else
			Raylib.ClearBackground(CaxecraftPalette.sky());
		Raylib.DrawRectangle(0, 0, width, height, Color.rgba(4, 14, 23, 166));
		if (wordmarkReady)
			CaxecraftTextures.drawContained(wordmark, Std.int(width / 2), 24, 480, 112, Color.rgba(255, 255, 255));
		else
			drawUiText(catalog, locale, UiMessage.TitleFallback, Std.int(width / 2) - 92, 46, 34, Color.rgba(242, 249, 245));

		final panelLeft = Std.int(width / 2) - Std.int(PANEL_WIDTH / 2);
		final panelTop = Std.int(height / 2) - 120;
		Raylib.DrawRectangle(panelLeft, panelTop, PANEL_WIDTH, PANEL_HEIGHT, Color.rgba(6, 26, 36, 238));
		Raylib.DrawRectangleLines(panelLeft, panelTop, PANEL_WIDTH, PANEL_HEIGHT, Color.rgba(92, 194, 188));
		drawUiText(catalog, locale, UiMessage.MenuAdventure, panelLeft + 32, panelTop + 24, 26, Color.rgba(255, 190, 55));
		Raylib.DrawTextString(scenarioTitle, panelLeft + 32, panelTop + 66, 23, Color.rgba(242, 249, 245));
		Raylib.DrawTextString(adventureTagline, panelLeft + 32, panelTop + 102, 16, Color.rgba(255, 205, 91));
		Raylib.DrawTextString(campaign.id.text(), panelLeft + 32, panelTop + 140, 17, Color.rgba(142, 210, 205));

		final levelTop = panelTop + 190;
		for (index in 0...campaign.levelCount()) {
			final level = campaign.levelAt(index);
			final rowTop = levelTop + index * 34;
			final active = index == selectedLevelIndex;
			if (active)
				Raylib.DrawRectangle(panelLeft + 32, rowTop - 5, PANEL_WIDTH - 64, 28, Color.rgba(16, 88, 102, 178));
			Raylib.DrawCircle(panelLeft + 47, rowTop + 9, c.Float32.fromFloat(active ? 7.0 : 5.0),
				active ? Color.rgba(255, 190, 55) : Color.rgba(92, 194, 188));
			Raylib.DrawTextString(level.id.text(), panelLeft + 66, rowTop, 18, Color.rgba(229, 241, 235));
			if (index + 1 < campaign.levelCount())
				Raylib.DrawLine(panelLeft + 47, rowTop + 17, panelLeft + 47, rowTop + 32, Color.rgba(92, 194, 188));
		}

		final buttonTop = panelTop + PANEL_HEIGHT - 68;
		drawButton(panelLeft + 42, buttonTop, UiMessage.MenuAdventure, locale, catalog, true);
		drawButton(panelLeft + PANEL_WIDTH - 42 - BUTTON_WIDTH, buttonTop, UiMessage.EditorBack, locale, catalog, false);
	}

	static inline function drawButton(left:Int, top:Int, message:UiMessage, locale:LocaleCursor, catalog:RuntimeUiCatalog, active:Bool):Void {
		Raylib.DrawRectangle(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, active ? Color.rgba(16, 88, 102, 232) : Color.rgba(12, 42, 50, 230));
		Raylib.DrawRectangleLines(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, active ? Color.rgba(255, 190, 55) : Color.rgba(92, 194, 188));
		drawUiText(catalog, locale, message, left + 24, top + 14, 19, Color.rgba(242, 249, 245));
	}

	static inline function drawUiText(catalog:RuntimeUiCatalog, locale:LocaleCursor, message:UiMessage, x:Int, y:Int, fontSize:Int, color:Color):Void
		Raylib.DrawTextString(catalog.text(locale, message), x, y, fontSize, color);
}
#end
