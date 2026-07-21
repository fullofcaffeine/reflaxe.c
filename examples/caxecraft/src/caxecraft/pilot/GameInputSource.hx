package caxecraft.pilot;

/**
 * Supplies one semantic input value for a zero-based rendered frame number.
 *
 * The production Raylib adapter and deterministic pilots share this boundary.
 * Implementations must not retain or mutate the returned value after sampling.
 */
interface GameInputSource {
	public function sample(frameNumber:Int):GameInputFrame;
}
