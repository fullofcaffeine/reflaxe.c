package caxecraft.domain;

#if c
import c.IntConvert;
#end

/**
	Reads the target-selected world-view carrier as an ordinary Haxe `Int`.

	This adapter owns only exact byte widening. Queries, surface discovery, and
	hashing remain in the conditional-free `WorldRead` gameplay module.
**/
inline function readCode(view:WorldView, index:Int):Int {
	#if c
	return IntConvert.exact(view[index]);
	#else
	return view[index];
	#end
}
