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
	static inline final MINIMUM_PANEL_HEIGHT:Int = 390;
	static inline final PANEL_VERTICAL_MARGIN:Int = 176;
	static inline final LEVEL_TOP_OFFSET:Int = 190;
	static inline final LEVEL_ROW_HEIGHT:Int = 34;
	static inline final BUTTON_TOP_INSET:Int = 68;
	static inline final BUTTON_WIDTH:Int = 250;
	static inline final BUTTON_HEIGHT:Int = 48;

	/**
	 * Return the level row or action button under one pointer position.
	 *
	 * The caller supplies the validated manifest's level count, so hit testing and
	 * drawing agree without keeping a second list inside the presentation layer.
	 */
	public static function selectionAt(mouseX:Float, mouseY:Float, width:Int, height:Int, levelCount:Int, selectedLevelIndex:Int):CampaignMenuHit {
		final layout = layoutFor(width, height, levelCount, selectedLevelIndex);
		final panelLeft = layout.panelLeft;
		final buttonTop = layout.buttonTop;
		if (mouseY >= buttonTop && mouseY < buttonTop + BUTTON_HEIGHT) {
			if (mouseX >= panelLeft + 42 && mouseX < panelLeft + 42 + BUTTON_WIDTH)
				return LaunchHit;
			if (mouseX >= panelLeft + PANEL_WIDTH - 42 - BUTTON_WIDTH && mouseX < panelLeft + PANEL_WIDTH - 42)
				return BackHit;
		}
		final levelTop = layout.levelTop;
		if (mouseX >= panelLeft + 32 && mouseX < panelLeft + PANEL_WIDTH - 32 && mouseY >= levelTop && mouseY < buttonTop) {
			final index = layout.firstLevelIndex + Std.int((mouseY - levelTop) / LEVEL_ROW_HEIGHT);
			if (index >= layout.firstLevelIndex && index < layout.firstLevelIndex + layout.visibleLevelCount)
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

		final layout = layoutFor(width, height, campaign.levelCount(), selectedLevelIndex);
		final panelLeft = layout.panelLeft;
		final panelTop = layout.panelTop;
		Raylib.DrawRectangle(panelLeft, panelTop, PANEL_WIDTH, layout.panelHeight, Color.rgba(6, 26, 36, 238));
		Raylib.DrawRectangleLines(panelLeft, panelTop, PANEL_WIDTH, layout.panelHeight, Color.rgba(92, 194, 188));
		drawUiText(catalog, locale, UiMessage.MenuAdventure, panelLeft + 32, panelTop + 24, 26, Color.rgba(255, 190, 55));
		Raylib.DrawTextString(scenarioTitle, panelLeft + 32, panelTop + 66, 23, Color.rgba(242, 249, 245));
		Raylib.DrawTextString(adventureTagline, panelLeft + 32, panelTop + 102, 16, Color.rgba(255, 205, 91));
		Raylib.DrawTextString(campaign.id.text(), panelLeft + 32, panelTop + 140, 17, Color.rgba(142, 210, 205));

		final levelTop = layout.levelTop;
		for (offset in 0...layout.visibleLevelCount) {
			final index = layout.firstLevelIndex + offset;
			final level = campaign.levelAt(index);
			final rowTop = levelTop + offset * LEVEL_ROW_HEIGHT;
			final active = index == selectedLevelIndex;
			if (active)
				Raylib.DrawRectangle(panelLeft + 32, rowTop - 5, PANEL_WIDTH - 64, 28, Color.rgba(16, 88, 102, 178));
			Raylib.DrawCircle(panelLeft + 47, rowTop + 9, c.Float32.fromFloat(active ? 7.0 : 5.0),
				active ? Color.rgba(255, 190, 55) : Color.rgba(92, 194, 188));
			Raylib.DrawTextString(level.id.text(), panelLeft + 66, rowTop, 18, Color.rgba(229, 241, 235));
			if (index + 1 < campaign.levelCount())
				Raylib.DrawLine(panelLeft + 47, rowTop + 17, panelLeft + 47, rowTop + 32, Color.rgba(92, 194, 188));
		}

		final buttonTop = layout.buttonTop;
		drawButton(panelLeft + 42, buttonTop, UiMessage.MenuAdventure, locale, catalog, true);
		drawButton(panelLeft + PANEL_WIDTH - 42 - BUTTON_WIDTH, buttonTop, UiMessage.EditorBack, locale, catalog, false);
	}

	/**
	 * Fit campaign rows between the story header and action buttons.
	 *
	 * Large campaigns use a moving row window. This keeps the selected level
	 * visible without letting runtime data draw over the controls.
	 */
	static function layoutFor(width:Int, height:Int, levelCount:Int, selectedLevelIndex:Int):CampaignMenuLayout {
		var panelHeight = 270 + levelCount * LEVEL_ROW_HEIGHT;
		if (panelHeight < MINIMUM_PANEL_HEIGHT)
			panelHeight = MINIMUM_PANEL_HEIGHT;
		var maximumPanelHeight = height - PANEL_VERTICAL_MARGIN;
		if (maximumPanelHeight < 300)
			maximumPanelHeight = 300;
		if (panelHeight > maximumPanelHeight)
			panelHeight = maximumPanelHeight;
		final panelTop = Std.int(height / 2) - Std.int(panelHeight / 2) + 40;
		var visibleCapacity = Std.int((panelHeight - 270) / LEVEL_ROW_HEIGHT);
		if (visibleCapacity < 1)
			visibleCapacity = 1;
		final visibleLevelCount = levelCount < visibleCapacity ? levelCount : visibleCapacity;
		var firstLevelIndex = selectedLevelIndex < visibleLevelCount ? 0 : selectedLevelIndex - visibleLevelCount + 1;
		if (firstLevelIndex + visibleLevelCount > levelCount)
			firstLevelIndex = levelCount - visibleLevelCount;
		return {
			panelLeft: Std.int(width / 2) - Std.int(PANEL_WIDTH / 2),
			panelTop: panelTop,
			panelHeight: panelHeight,
			levelTop: panelTop + LEVEL_TOP_OFFSET,
			buttonTop: panelTop + panelHeight - BUTTON_TOP_INSET,
			firstLevelIndex: firstLevelIndex,
			visibleLevelCount: visibleLevelCount
		};
	}

	static inline function drawButton(left:Int, top:Int, message:UiMessage, locale:LocaleCursor, catalog:RuntimeUiCatalog, active:Bool):Void {
		Raylib.DrawRectangle(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, active ? Color.rgba(16, 88, 102, 232) : Color.rgba(12, 42, 50, 230));
		Raylib.DrawRectangleLines(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, active ? Color.rgba(255, 190, 55) : Color.rgba(92, 194, 188));
		drawUiText(catalog, locale, message, left + 24, top + 14, 19, Color.rgba(242, 249, 245));
	}

	static inline function drawUiText(catalog:RuntimeUiCatalog, locale:LocaleCursor, message:UiMessage, x:Int, y:Int, fontSize:Int, color:Color):Void
		Raylib.DrawTextString(catalog.text(locale, message), x, y, fontSize, color);
}

/** Derived rectangles and row window for one campaign-menu frame. */
private typedef CampaignMenuLayout = {
	final panelLeft:Int;
	final panelTop:Int;
	final panelHeight:Int;
	final levelTop:Int;
	final buttonTop:Int;
	final firstLevelIndex:Int;
	final visibleLevelCount:Int;
}
#end
