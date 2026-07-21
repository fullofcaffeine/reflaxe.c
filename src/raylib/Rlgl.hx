package raylib;

#if c
private typedef RawColor = raylib.raw.Color;
private typedef RawTexture2D = raylib.raw.Texture2D;
private typedef RawRlgl = raylib.raw.Rlgl;

/**
 * Small zero-allocation facade for one textured `RL_QUADS` render batch.
 *
 * rlgl is raylib's low-level immediate drawing API. `BeginQuads` binds one
 * already-owned texture; `Normal`, `Color`, `TexCoord`, and `Vertex` then add
 * attributes to raylib's existing render batch. `EndQuads` closes the region
 * and restores the default texture. No method allocates, retains a value, or
 * selects hxrt. Calls must stay balanced on raylib's main render thread.
 *
 * This is intentionally not a general OpenGL or rlgl wrapper. The exact raw
 * seven-function slice is generated and verified from pinned `rlgl.h`; expand
 * it only when another reviewed rendering need earns the additional state.
 */
final class Rlgl {
	/** Bind `texture` and begin one quad region. The caller keeps ownership. */
	public static inline function BeginQuads(texture:Texture2D):Void {
		final raw:RawTexture2D = texture;
		RawRlgl.rlSetTexture(raw.id);
		RawRlgl.rlBegin(c.IntConvert.exact(7));
	}

	/** Finish the quad region and restore raylib's default texture. */
	public static inline function EndQuads():Void {
		RawRlgl.rlEnd();
		RawRlgl.rlSetTexture(c.IntConvert.modulo(0));
	}

	public static inline function Normal(x:Float, y:Float, z:Float):Void
		RawRlgl.rlNormal3f(c.Float32.fromFloat(x), c.Float32.fromFloat(y), c.Float32.fromFloat(z));

	/** Copy four already range-checked native color channels into the batch. */
	public static inline function Color(color:Color):Void {
		final raw:RawColor = color;
		RawRlgl.rlColor4ub(raw.r, raw.g, raw.b, raw.a);
	}

	public static inline function TexCoord(u:Float, v:Float):Void
		RawRlgl.rlTexCoord2f(c.Float32.fromFloat(u), c.Float32.fromFloat(v));

	public static inline function Vertex(x:Float, y:Float, z:Float):Void
		RawRlgl.rlVertex3f(c.Float32.fromFloat(x), c.Float32.fromFloat(y), c.Float32.fromFloat(z));
}
#end
