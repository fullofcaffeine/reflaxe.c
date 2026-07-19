package caxecraft.domain;

#if c
// The inactive import is removed before the non-C oracle is typed.
import c.IntConvert;
#end

/**
	The narrow storage-operation adapter between semantic block codes and the
	compile-time carrier selected by `WorldCells`.

	No method allocates in the C build. Reads widen an exact `uint8_t` to Haxe
	`Int`; writes apply an explicit modulo conversion after `World` has validated
	the material code. The non-C branch uses `Array<Int>` directly so the pinned
	Eval target can execute the same algorithms as a semantic oracle.

	`#if c` is resolved by Haxe at compile time, so these methods do not branch at
	runtime. Representation-specific conversion stays here instead of leaking
	through terrain, picking, or physics. See `docs/caxecraft-domain.md` for the
	target-seam rules and long-term adapter plan.
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
