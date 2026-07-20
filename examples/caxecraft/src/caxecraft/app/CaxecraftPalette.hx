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

	public static inline function selection():Color
		return Color.rgba(255, 132, 47);

	public static inline function hudPanel():Color
		return Color.rgba(15, 27, 39, 220);

	public static inline function hudText():Color
		return Color.rgba(228, 239, 227);

	public static function block(kind:BlockKind):Color {
		return switch (kind) {
			case Air: Color.rgba(0, 0, 0, 0);
			case Grass: Color.rgba(83, 145, 92);
			case Dirt: Color.rgba(132, 91, 62);
			case Stone: Color.rgba(91, 107, 117);
			case Bedrock: Color.rgba(47, 54, 64);
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
		};
	}
}
#end
