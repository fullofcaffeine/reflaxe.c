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

import haxe.iterators.ArrayKeyValueIterator;

/**
 * Supplies the pinned ordinary Array API for the C target.
 *
 * Small collection algorithms stay in typed Haxe and inline into their caller,
 * where haxe.c can specialize every element without boxing. Storage operations
 * remain declarations: the compiler lowers the admitted calls through typed
 * HxcIR, while unsupported element or operation shapes still fail before C is
 * written. This split keeps one Array owner instead of adding a parallel C-only
 * collection implementation.
 */
@:coreApi
extern class Array<T> {
	/** The number of live elements in this shared Array. */
	var length(default, null):Int;

	/** Create one empty Array with this call site's exact element type. */
	function new():Void;

	/** Return a shallow copy followed by every element of `a` in source order. */
	inline function concat(a:Array<T>):Array<T> {
		final result = copy();
		for (index in 0...a.length)
			result.push(a[index]);
		return result;
	}

	function join(sep:String):String;
	function pop():Null<T>;
	function push(x:T):Int;

	/** Reverse this shared Array in place with exact typed element moves. */
	inline function reverse():Void {
		var leftIndex = 0;
		var rightIndex = length - 1;
		while (leftIndex < rightIndex) {
			final left = this[leftIndex];
			final right = this[rightIndex];
			this[leftIndex] = right;
			this[rightIndex] = left;
			leftIndex++;
			rightIndex--;
		}
	}

	function shift():Null<T>;

	/** Return the pinned, normalized half-open range without changing this Array. */
	inline function slice(pos:Int, ?end:Int):Array<T> {
		var start = pos;
		if (start < 0) {
			start = length + start;
			if (start < 0)
				start = 0;
		}
		var stop:Int = end == null || end > length ? length : end;
		if (stop < 0) {
			stop = length + stop;
			if (stop < 0)
				stop = 0;
		}
		final result:Array<T> = [];
		if (start <= length && stop > start) {
			for (index in start...stop)
				result.push(this[index]);
		}
		return result;
	}

	function sort(f:T->T->Int):Void;
	function splice(pos:Int, len:Int):Array<T>;
	function toString():String;

	/** Insert at the front through the same normalized operation as `insert`. */
	inline function unshift(x:T):Void {
		insert(0, x);
	}

	function insert(pos:Int, x:T):Void;

	/** Remove the first equal value and preserve the order of the suffix. */
	inline function remove(x:T):Bool {
		final index = indexOf(x);
		final found = index >= 0;
		if (found)
			splice(index, 1);
		return found;
	}

	/** Report whether standard equality finds the supplied value. */
	@:pure inline function contains(x:T):Bool {
		return indexOf(x) >= 0;
	}

	/** Search forward from the pinned normalized optional start index. */
	inline function indexOf(x:T, ?fromIndex:Int):Int {
		var index:Int = fromIndex == null ? 0 : fromIndex;
		if (index < 0) {
			index = length + index;
			if (index < 0)
				index = 0;
		}
		var found = -1;
		while (index < length && found < 0) {
			if (this[index] == x)
				found = index;
			index++;
		}
		return found;
	}

	/** Search backward from the pinned normalized optional start index. */
	inline function lastIndexOf(x:T, ?fromIndex:Int):Int {
		var index:Int = fromIndex == null || fromIndex >= length ? length - 1 : fromIndex;
		if (index < 0) {
			index = length + index;
		}
		var found = -1;
		while (index >= 0 && found < 0) {
			if (this[index] == x)
				found = index;
			index--;
		}
		return found;
	}

	function copy():Array<T>;

	/** Create one live value cursor over this shared Array identity. */
	@:runtime inline function iterator():haxe.iterators.ArrayIterator<T> {
		return new haxe.iterators.ArrayIterator(this);
	}

	/** Create one live index-and-value cursor over this shared Array identity. */
	@:pure @:runtime inline function keyValueIterator():ArrayKeyValueIterator<T> {
		return new ArrayKeyValueIterator(this);
	}

	/** Apply `f` once per current element and preserve source order. */
	@:runtime inline function map<S>(f:T->S):Array<S> {
		return [for (value in this) f(value)];
	}

	/** Keep each current element for which `f` returns true, in source order. */
	@:runtime inline function filter(f:T->Bool):Array<T> {
		return [for (value in this) if (f(value)) value];
	}

	function resize(len:Int):Void;
}
