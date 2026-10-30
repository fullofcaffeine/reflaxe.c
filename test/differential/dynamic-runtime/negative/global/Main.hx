/** Proves that Dynamic globals remain closed until global roots own them. */
class Main {
	/** A global Dynamic carrier has no admitted root publication contract. */
	static var stored:Dynamic = 7;

	/** Make the unsupported global reachable from the program entry point. */
	static function main():Void {
		final value:Int = stored;
		Sys.println(value);
	}
}
