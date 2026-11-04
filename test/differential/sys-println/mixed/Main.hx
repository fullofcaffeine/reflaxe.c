/** Protects the boundary between a String selection and a heterogeneous value. */
class Main {
	/** Neither branch alone proves the formatting contract of this selection. */
	static function print(selected:Bool):Void {
		Sys.println(selected ? "text" : 42);
	}

	/** Keep the rejected call reachable through ordinary Haxe. */
	static function main():Void {
		print(true);
	}
}
