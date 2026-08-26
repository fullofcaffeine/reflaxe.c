import haxe.ds.Vector;

/**
	Isolates the pinned Vector surface from List dispatch during bring-up.

	This probe is diagnostic evidence for representation discovery. The complete
	`Main` fixture remains the acceptance oracle after both collection families
	are implemented.
**/
final class VectorProbe {
	/** Compare two values for target-side Vector sorting. */
	static function compare(left:Int, right:Int):Int
		return left - right;

	/** Exercise fixed construction, mutation, overlap, copy, and conversion. */
	static function main():Void {
		final values = new Vector<Int>(4);
		values.fill(3);
		values[1] = 1;
		values[2] = 2;
		Vector.blit(values, 0, values, 1, 3);
		final copy = values.copy();
		copy[0] = 4;
		final converted = Vector.fromArrayCopy(copy.toArray());
		#if !eval
		converted.sort(compare);
		#end
		if (values.length != 4 || values.join(":") != "3:3:1:2" || copy.join(":") != "4:3:1:2")
			throw "Vector contract failed";
	}
}
