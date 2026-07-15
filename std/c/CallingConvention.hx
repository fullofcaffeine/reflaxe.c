package c;

/** Calling convention contract. Non-C values require a proven target capability. */
enum abstract CallingConvention(String) to String {
	var C = "c";
	var Cdecl = "cdecl";
	var Stdcall = "stdcall";
	var Fastcall = "fastcall";
	var Vectorcall = "vectorcall";
}
