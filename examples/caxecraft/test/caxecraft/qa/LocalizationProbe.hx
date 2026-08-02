package caxecraft.qa;

import caxecraft.localization.FirstPlayableCatalog;
import caxecraft.localization.FirstPlayableCatalog.ScenarioMessage;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.RuntimeUiCatalog.RuntimeUiCatalogResult;
import caxecraft.localization.UiTypes.UiMessage;
import sys.io.File;

/**
 * Runs runtime UI loading and the campaign fallback on Haxe Eval without Raylib.
 *
 * This is the fast semantic oracle for reading editable UI JSON, traversing its
 * locales, and retaining the still-generated campaign lookup. The native
 * Caxecraft lane separately proves that runtime-owned UI Strings are borrowed
 * only for the synchronous Raylib call.
 */
final class LocalizationProbe {
	static function main():Void {
		final catalog = switch RuntimeUiCatalog.decode(File.getBytes("locales/ui.json")) {
			case RuntimeUiCatalogReady(value): value;
			case RuntimeUiCatalogRejected(_): throw "shipped UI catalog was rejected";
		};
		final english = catalog.defaultLocale();
		final spanish = catalog.nextLocale(english);
		require(catalog.text(english, UiMessage.MenuAdventure) == "ADVENTURE", "English UI lookup");
		require(catalog.text(spanish, UiMessage.MenuAdventure) == "AVENTURA", "Spanish UI lookup");
		require(FirstPlayableCatalog.text(english, ScenarioMessage.NiaTalk) == "E  TALK TO NIA", "English campaign lookup");
		require(FirstPlayableCatalog.text(spanish, ScenarioMessage.NiaTalk) == "E  HABLAR CON NIA", "Spanish campaign lookup");
		require(catalog.nextLocale(spanish) == english, "locale cycle");

		require(catalog.localeCount() == 2, "runtime locale count");
		require(catalog.messageCount() == 35, "runtime message count");
		require(FirstPlayableCatalog.isValidMessageStorageCode(0), "known campaign message storage code");
		require(!FirstPlayableCatalog.isValidMessageStorageCode(99), "unknown campaign message storage code");

		Sys.println("caxecraft-localization: typed Eval lookup passed");
	}

	static inline function require(condition:Bool, label:String):Void {
		if (!condition)
			throw label;
	}
}
