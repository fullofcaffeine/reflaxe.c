package raylib;

#if c
/**
 * Raylib's named color macros with their original recognizable names.
 *
 * Each access is a header-owned by-value constant. It allocates nothing,
 * borrows nothing, cannot fail, and emits the macro name directly.
 */
@:c.include("raylib.h", c.IncludeKind.System)
extern class Colors {
	@:c.constant public static var LIGHTGRAY(default, never):Color;
	@:c.constant public static var GRAY(default, never):Color;
	@:c.constant public static var DARKGRAY(default, never):Color;
	@:c.constant public static var YELLOW(default, never):Color;
	@:c.constant public static var GOLD(default, never):Color;
	@:c.constant public static var ORANGE(default, never):Color;
	@:c.constant public static var PINK(default, never):Color;
	@:c.constant public static var RED(default, never):Color;
	@:c.constant public static var MAROON(default, never):Color;
	@:c.constant public static var GREEN(default, never):Color;
	@:c.constant public static var LIME(default, never):Color;
	@:c.constant public static var DARKGREEN(default, never):Color;
	@:c.constant public static var SKYBLUE(default, never):Color;
	@:c.constant public static var BLUE(default, never):Color;
	@:c.constant public static var DARKBLUE(default, never):Color;
	@:c.constant public static var PURPLE(default, never):Color;
	@:c.constant public static var VIOLET(default, never):Color;
	@:c.constant public static var DARKPURPLE(default, never):Color;
	@:c.constant public static var BEIGE(default, never):Color;
	@:c.constant public static var BROWN(default, never):Color;
	@:c.constant public static var DARKBROWN(default, never):Color;
	@:c.constant public static var WHITE(default, never):Color;
	@:c.constant public static var BLACK(default, never):Color;
	@:c.constant public static var BLANK(default, never):Color;
	@:c.constant public static var MAGENTA(default, never):Color;
	@:c.constant public static var RAYWHITE(default, never):Color;
}
#end
