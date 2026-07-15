package c;

/** Haxe 5 custom-target entry point. */
@:noCompletion
class Init {
	#if macro
	public static function init():Void {
		reflaxe.c.TargetPlatform.configure();
	}
	#else
	public static function init():Void {}
	#end
}
