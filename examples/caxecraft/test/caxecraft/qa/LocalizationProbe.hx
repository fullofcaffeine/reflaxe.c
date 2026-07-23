package caxecraft.qa;

import caxecraft.localization.FirstPlayableCatalog;
import caxecraft.localization.FirstPlayableCatalog.ScenarioMessage;
import caxecraft.localization.UiCatalog;
import caxecraft.localization.UiCatalog.UiMessage;

/**
 * Runs the generated localization API on Haxe Eval without Raylib.
 *
 * This is the fast semantic oracle for locale traversal, built-in English and
 * Spanish text, and validation of raw integer codes before a file loader may
 * construct typed IDs. The native Caxecraft lane separately proves that the
 * same lookup becomes a static `const char *` borrow at the Raylib boundary.
 */
final class LocalizationProbe {
	static function main():Void {
		final english = UiCatalog.defaultLocale();
		final spanish = UiCatalog.nextLocale(english);
		require(UiCatalog.text(english, UiMessage.MenuAdventure) == "ADVENTURE", "English UI lookup");
		require(UiCatalog.text(spanish, UiMessage.MenuAdventure) == "AVENTURA", "Spanish UI lookup");
		require(FirstPlayableCatalog.text(english, ScenarioMessage.NiaTalk) == "E  TALK TO NIA", "English campaign lookup");
		require(FirstPlayableCatalog.text(spanish, ScenarioMessage.NiaTalk) == "E  HABLAR CON NIA", "Spanish campaign lookup");
		require(UiCatalog.nextLocale(spanish) == english, "locale cycle");

		require(UiCatalog.isValidLocaleStorageCode(0), "known locale storage code");
		require(!UiCatalog.isValidLocaleStorageCode(99), "unknown locale storage code");
		require(UiCatalog.isValidMessageStorageCode(0), "known UI message storage code");
		require(!UiCatalog.isValidMessageStorageCode(99), "unknown UI message storage code");
		require(FirstPlayableCatalog.isValidMessageStorageCode(0), "known campaign message storage code");
		require(!FirstPlayableCatalog.isValidMessageStorageCode(99), "unknown campaign message storage code");

		Sys.println("caxecraft-localization: typed Eval lookup passed");
	}

	static inline function require(condition:Bool, label:String):Void {
		if (!condition)
			throw label;
	}
}
