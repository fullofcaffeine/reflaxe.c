package raylib;

#if c
/** Raylib trace threshold; distinct from keyboard and mouse integer domains. */
enum abstract TraceLogLevel(Int) {
	var All = 0;
	var Trace = 1;
	var Debug = 2;
	var Info = 3;
	var Warning = 4;
	var Error = 5;
	var Fatal = 6;
	var None = 7;
}
#end
