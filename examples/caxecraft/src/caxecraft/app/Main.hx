package caxecraft.app;

#if c
/**
 * Starts the native Caxecraft executable.
 *
 * This class is intentionally only the Haxe entry boundary. `CaxecraftApp`
 * owns the window, resources, outer frame loop, and shutdown, while `Main`
 * constructs exactly one application and hands control to it. Keeping startup
 * this small makes application ownership obvious in both Haxe and generated C.
 */
final class Main {
	/** Construct the application owner and run one complete native lifetime. */
	static function main():Void {
		final app = new CaxecraftApp();
		app.run();
	}
}
#end
