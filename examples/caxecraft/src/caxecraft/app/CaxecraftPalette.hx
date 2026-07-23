package caxecraft.app;

#if c
import caxecraft.domain.BlockKind;
import raylib.Color;

/** Original slate, meadow, earth, and Haxe-orange palette for the first slice. */
final class CaxecraftPalette {
	public static inline function sky():Color
		return Color.rgba(126, 190, 201);

	public static inline function fog():Color
		return Color.rgba(207, 226, 216);

	public static inline function sunGlow():Color
		return Color.rgba(255, 226, 136);

	public static inline function sunCore():Color
		return Color.rgba(255, 246, 205);

	public static inline function selection():Color
		return Color.rgba(255, 132, 47);

	public static inline function hudPanel():Color
		return Color.rgba(15, 27, 39, 220);

	public static inline function hudText():Color
		return Color.rgba(228, 239, 227);

	/** Camera tint while the player's eyes cross the simulated waterline. */
	public static inline function underwaterOverlay(alpha:Int):Color
		return Color.rgbaClamped(31, 115, 154, alpha);

	/** Filled fixed-tick breath bubble. */
	public static inline function breathFull():Color
		return Color.rgba(164, 235, 246);

	/** Empty breath bubble, kept visible against both water and the HUD. */
	public static inline function breathEmpty():Color
		return Color.rgba(49, 82, 103, 210);

	/** Neutral tint keeps reviewed source texture colors unchanged. */
	public static inline function textureTint():Color
		return Color.rgba(255, 255, 255);

	public static inline function niaCoat():Color
		return Color.rgba(42, 150, 160);

	public static inline function niaSkin():Color
		return Color.rgba(205, 139, 88);

	public static inline function niaHair():Color
		return Color.rgba(62, 40, 47);

	public static inline function mosslingBody():Color
		return Color.rgba(74, 119, 67);

	public static inline function mosslingCrown():Color
		return Color.rgba(157, 190, 82);

	public static inline function berry():Color
		return Color.rgba(174, 78, 136);

	public static inline function damage():Color
		return Color.rgba(218, 65, 72);

	/** Unique positive-feedback green used by deterministic recovery checks. */
	public static inline function recovery():Color
		return Color.rgba(94, 212, 136);

	/** Unique warning orange used by deterministic full-inventory checks. */
	public static inline function inventoryFull():Color
		return Color.rgba(238, 113, 78);

	public static function block(kind:BlockKind):Color {
		return switch (kind) {
			case Air: Color.rgba(0, 0, 0, 0);
			case Grass: Color.rgba(83, 145, 92);
			case Dirt: Color.rgba(132, 91, 62);
			case Stone: Color.rgba(91, 107, 117);
			case Bedrock: Color.rgba(47, 54, 64);
			case Sand: Color.rgba(196, 174, 113);
			case Wood: Color.rgba(109, 76, 52);
			case Leaves: Color.rgba(54, 112, 73);
			case Snow: Color.rgba(222, 235, 238);
			case Ash: Color.rgba(73, 70, 76);
		};
	}

	/** Darker per-material edge colors keep adjacent unit cubes readable. */
	public static function blockOutline(kind:BlockKind):Color {
		return switch (kind) {
			case Air: Color.rgba(0, 0, 0, 0);
			case Grass: Color.rgba(48, 91, 66);
			case Dirt: Color.rgba(82, 55, 45);
			case Stone: Color.rgba(57, 69, 78);
			case Bedrock: Color.rgba(25, 31, 39);
			case Sand: Color.rgba(131, 111, 70);
			case Wood: Color.rgba(65, 43, 34);
			case Leaves: Color.rgba(31, 69, 48);
			case Snow: Color.rgba(151, 174, 183);
			case Ash: Color.rgba(39, 36, 43);
		};
	}
}
#end
