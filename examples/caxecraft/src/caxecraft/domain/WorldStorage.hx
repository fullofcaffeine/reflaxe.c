package caxecraft.domain;

#if c
import c.IntConvert;
#end

/**
	The narrow storage adapter between semantic block codes and target storage.

	No method allocates in the C build. Reads widen an exact `uint8_t` to Haxe
	`Int`; writes apply an explicit modulo conversion after `World` has validated
	the material code.
**/
final class WorldStorage {
	public static function readCode(cells:WorldCells, index:Int):Int {
		#if c
		return IntConvert.exact(cells[index]);
		#else
		return cells[index];
		#end
	}

	public static function writeCode(cells:WorldCells, index:Int, code:Int):Void {
		#if c
		cells[index] = IntConvert.modulo(code);
		#else
		cells[index] = code;
		#end
	}
}
