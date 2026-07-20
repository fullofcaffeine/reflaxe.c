package raylib;

#if c
/** Camera projection values. The Haxe type prevents mixing unrelated ints. */
enum abstract CameraProjection(Int) {
	var Perspective = 0;
	var Orthographic = 1;
}
#end
