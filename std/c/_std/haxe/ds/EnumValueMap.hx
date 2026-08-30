/*
 * Copyright (C)2005-2019 Haxe Foundation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

package haxe.ds;

/**
 * Supplies the pinned enum-value map API for the C target.
 *
 * Storage, key comparison, and iteration remain compiler-lowered typed
 * operations. Formatting stays in ordinary Haxe so it reuses those operations
 * and the exact admitted `Std.string` rules instead of adding a second policy.
 */
class EnumValueMap<K:EnumValue, V> extends haxe.ds.BalancedTree<K, V> implements haxe.Constraints.IMap<K, V> {
	/**
	 * Format entries with Eval-compatible brackets, arrows, and separators.
	 *
	 * The generic body specializes at each call site. Unsupported enum payloads
	 * or value spellings therefore fail at their exact `Std.string` use.
	 */
	override inline function toString():String {
		var result = "[";
		var separator = "";
		for (entry in keyValueIterator()) {
			result = result + separator + Std.string(entry.key) + " => " + Std.string(entry.value);
			separator = ", ";
		}
		return result + "]";
	}

	/** Return a shallow map copy with independent compiler-owned storage. */
	override function copy():EnumValueMap<K, V> {
		final copied = new EnumValueMap<K, V>();
		copied.root = root;
		return copied;
	}
}
