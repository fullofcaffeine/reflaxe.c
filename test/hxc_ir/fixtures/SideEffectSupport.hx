class SideEffectSupport {
	public static final calls:Array<String> = [];

	public static function nextIndex():Int {
		calls.push("nextIndex");
		return 0;
	}

	public static function produce():Int {
		calls.push("produce");
		return 7;
	}
}
