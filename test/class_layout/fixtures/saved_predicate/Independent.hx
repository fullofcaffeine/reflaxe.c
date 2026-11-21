/** An independent true Boolean says nothing about the presence of the reading. */
final class Independent {
	/** Keep the Boolean parameter independent of the nullable receiver. */
	static function read(selected:Null<Reading>, visible:Bool):Int {
		return visible ? selected.value : -1;
	}

	/** Taking the true branch with an absent reading must reach the checked field access. */
	static function main():Void {
		Sys.println(read(null, true));
	}
}
