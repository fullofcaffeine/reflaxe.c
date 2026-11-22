package;

/** Exercise the constructor diagnostic before exception runtime selection. */
class ExceptionConstructor {
	/** Construct the standard exception at a source location shared by both policy cases. */
	static function main():Void {
		throw new haxe.Exception("fixture");
	}
}
