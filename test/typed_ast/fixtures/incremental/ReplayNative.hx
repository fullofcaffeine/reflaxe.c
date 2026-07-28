/**
	Models one existing C-library boundary in the incremental-backend fixture.

	The test never links this generated project. It changes the external symbol
	metadata and checks that a warm compiler request cannot replay semantic
	functions prepared against the previous import plan.
**/
@:c.include("stdlib.h", c.IncludeKind.System)
extern class ReplayNative {
	/** Header-owned zero value used without an ordinary-Haxe-to-C cast. */
	@:c.constant
	@:c.name("EXIT_SUCCESS")
	public static var zero(default, never):c.Int32;

	/** Call C `abs` through a typed binary32-free integer boundary. */
	@:c.name("abs")
	public static function magnitude(value:c.Int32):c.Int32;
}
