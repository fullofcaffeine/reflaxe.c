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
 * Supplies the pinned object-identity map API for the C target.
 *
 * Storage and iteration remain compiler-lowered typed operations. Formatting
 * stays in ordinary Haxe so punctuation, iteration order, and each admitted
 * key or value spelling have one readable source owner instead of a parallel
 * runtime callback protocol.
 */
extern class ObjectMap<K:{}, V> implements haxe.Constraints.IMap<K, V> {
	/** Create an empty map whose object keys use identity equality. */
	function new():Void;

	/** Associate `key` with `value`, replacing the prior value when present. */
	function set(key:K, value:V):Void;

	/** Return the current value, or null when this identity is absent. */
	function get(key:K):Null<V>;

	/** Report whether this exact object identity is present. */
	function exists(key:K):Bool;

	/** Remove this exact object identity and report whether it was present. */
	function remove(key:K):Bool;

	/** Traverse a stable snapshot of current keys. */
	function keys():Iterator<K>;

	/** Traverse a stable snapshot of current values. */
	function iterator():Iterator<V>;

	/** Traverse stable key-and-value records over the current snapshot. */
	function keyValueIterator():KeyValueIterator<K, V>;

	/** Return a shallow map copy with independent storage. */
	function copy():ObjectMap<K, V>;

	/**
	 * Format entries with Eval-compatible brackets, arrows, and separators.
	 *
	 * The generic body specializes at each call site. Unsupported key or value
	 * formatting therefore fails at its exact `Std.string` use before C exists.
	 */
	inline function toString():String {
		var result = "[";
		var separator = "";
		for (entry in keyValueIterator()) {
			result = result + separator + Std.string(entry.key) + " => " + Std.string(entry.value);
			separator = ", ";
		}
		return result + "]";
	}

	/** Remove all entries while preserving this map identity. */
	function clear():Void;
}
