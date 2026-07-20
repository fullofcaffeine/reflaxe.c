package raylib;

#if c
private typedef RawCamera3D = raylib.raw.Camera3D;

/** A direct, allocation-free camera value with typed projection mode. */
abstract Camera3D(RawCamera3D) from RawCamera3D to RawCamera3D {
	public static inline function make(position:Vector3, target:Vector3, up:Vector3, fovy:c.Float32, projection:CameraProjection):Camera3D
		return new Camera3D(c.StructInit.make({
			position: position,
			target: target,
			up: up,
			fovy: fovy,
			projection: projection
		}));

	private inline function new(value:RawCamera3D)
		this = value;
}
#end
