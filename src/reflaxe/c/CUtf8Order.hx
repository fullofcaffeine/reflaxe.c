package reflaxe.c;

import haxe.io.Bytes;

/**
	Provides the one deterministic UTF-8 ordering used by compiler collections.

	Compiler maps and arrays must produce the same order on every machine. The
	Haxe Eval host stores strings as UTF-8 and compares their bytes
	lexicographically, so its native comparison already implements that contract
	without allocating temporary byte buffers. Other Haxe hosts use the explicit
	byte walk below. Keeping the choice here prevents individual compiler passes
	from accidentally reintroducing allocation-heavy comparators or choosing a
	different order.
**/
/**
	Compares two strings by their exact UTF-8 byte order.

	Sorting calls a comparator many times, so allocating two byte buffers per
	call made large programs need gigabytes of avoidable temporary memory under
	Eval. Eval's native String order already has the same result for its UTF-8
	strings; other hosts retain the explicit byte walk. A negative result places
	`left` first, zero means equality, and a positive result places `right` first.
**/
function compare(left:String, right:String):Int {
	#if (macro || eval)
	return left < right ? -1 : left > right ? 1 : 0;
	#else
	final leftBytes = Bytes.ofString(left);
	final rightBytes = Bytes.ofString(right);
	final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
	for (index in 0...length) {
		final difference = leftBytes.get(index) - rightBytes.get(index);
		if (difference != 0)
			return difference;
	}
	return leftBytes.length - rightBytes.length;
	#end
}
