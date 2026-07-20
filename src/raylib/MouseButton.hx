package raylib;

#if c
/** Mouse-button domain used by the safe semantic input methods. */
enum abstract MouseButton(Int) {
	var Left = 0;
	var Right = 1;
	var Middle = 2;
	var Side = 3;
	var Extra = 4;
	var Forward = 5;
	var Back = 6;
}
#end
