/**
 * Keeps the arithmetic oracle in ordinary Haxe and free of target-specific code.
 *
 * The same source feeds Eval and haxe.c so wrapping integers, floating-point
 * edge cases, conversions, local updates, and parameter updates can be compared
 * at their observable function boundaries.
 */

/** Exercises the admitted primitive arithmetic operations as small typed functions. */
class ArithmeticFixture {
	static function iadd(left:Int, right:Int):Int
		return left + right;

	static function isub(left:Int, right:Int):Int
		return left - right;

	static function imul(left:Int, right:Int):Int
		return left * right;

	static function ineg(value:Int):Int
		return -value;

	static function idiv(left:Int, right:Int):Float
		return left / right;

	static function imod(left:Int, right:Int):Int
		return left % right;

	static function ishl(left:Int, right:Int):Int
		return left << right;

	static function ishr(left:Int, right:Int):Int
		return left >> right;

	static function iushr(left:Int, right:Int):Int
		return left >>> right;

	static function iand(left:Int, right:Int):Int
		return left & right;

	static function ior(left:Int, right:Int):Int
		return left | right;

	static function ixor(left:Int, right:Int):Int
		return left ^ right;

	static function inot(value:Int):Int
		return ~value;

	static function iless(left:Int, right:Int):Bool
		return left < right;

	static function fadd(left:Float, right:Float):Float
		return left + right;

	static function fsub(left:Float, right:Float):Float
		return left - right;

	static function fmul(left:Float, right:Float):Float
		return left * right;

	static function fneg(value:Float):Float
		return -value;

	static function fdiv(left:Float, right:Float):Float
		return left / right;

	static function fmod(left:Float, right:Float):Float
		return left % right;

	/** Use the ordinary Haxe standard-library surface for binary64 square roots. */
	static function fsqrt(value:Float):Float
		return Math.sqrt(value);

	static function fint(value:Float):Int
		return Std.int(value);

	/** Proves a positive constant Int divisor can stay integral through Std.int. */
	static function intQuotientByEight(value:Int):Int
		return Std.int(value / 8);

	/** Covers a positive divisor that binary64 cannot represent as a reciprocal. */
	static function intQuotientBySix(value:Int):Int
		return Std.int(value / 6);

	/** Exercises the narrow rounding margin near one with the largest divisor. */
	static function intQuotientByMaximum(value:Int):Int
		return Std.int(value / 2147483647);

	/** Covers the exact endpoint quotient for the smallest positive divisor. */
	static function intQuotientByOne(value:Int):Int
		return Std.int(value / 1);

	/** Proves that a side-effecting numerator executes exactly once. */
	static function intQuotientSideEffect(value:Int):Int {
		var observed = value;
		final quotient = Std.int(observed++ / 6);
		return quotient * 10 + observed - value;
	}

	/** Keeps a runtime divisor on the general Float division path. */
	static function intQuotientByVariable(value:Int, divisor:Int):Int
		return Std.int(value / divisor);

	/** Keeps division by zero on the IEEE and saturating-conversion path. */
	static function intQuotientByZero(value:Int):Int
		return Std.int(value / 0);

	/** Keeps a negative divisor on the general path, including INT_MIN / -1. */
	static function intQuotientByNegativeOne(value:Int):Int
		return Std.int(value / -1);

	/** Keeps a real Float operand on the general division and conversion path. */
	static function floatQuotientByEight(value:Float):Int
		return Std.int(value / 8);

	/** Keeps a cast-wrapped division outside the direct syntactic proof. */
	static function intQuotientThroughFloatCast(value:Int):Int
		return Std.int((cast(value / 8) : Float));

	static function fequal(left:Float, right:Float):Bool
		return left == right;

	static function uadd(left:UInt, right:UInt):UInt
		return left + right;

	static function umod(left:UInt, right:UInt):UInt
		return left % right;

	static function ushl(left:UInt, right:Int):UInt
		return left << right;

	static function ushr(left:UInt, right:Int):UInt
		return left >> right;

	#if !arithmetic_semantics_oracle
	static function literalToU8():c.UInt8
		return c.IntConvert.modulo(300);

	static function i32ToU8(value:Int):c.UInt8
		return c.IntConvert.modulo(value);

	static function u8ToI32(value:c.UInt8):Int
		return c.IntConvert.exact(value);

	static function i64ToU16(value:c.Int64):c.UInt16
		return c.IntConvert.modulo(value);

	static function u32ToU64(value:c.UInt32):c.UInt64
		return c.IntConvert.exact(value);

	static function u32ToU8(value:c.UInt32):c.UInt8
		return c.IntConvert.modulo(value);

	static function u8ToI16(value:c.UInt8):c.Int16
		return c.IntConvert.exact(value);

	static function i8ToI32(value:c.Int8):Int
		return c.IntConvert.exact(value);
	#end

	static function update(value:Int):Int {
		var current = value;
		var old = current++;
		var fresh = --current;
		current += old;
		current *= fresh;
		return current;
	}

	/**
	 * Proves that compound arithmetic may update a function parameter itself.
	 *
	 * The parameter starts as an immutable incoming HxcIR value. haxe.c gives
	 * this function one initialized automatic local so `+=` can preserve Haxe's
	 * wrapping `Int` semantics without assigning the C parameter directly.
	 */
	static function updateParameter(value:Int):Int {
		value += 3;
		return value;
	}

	static function main():Void {
		#if arithmetic_semantics_oracle
		var minimum = -2147483647 - 1;
		var unsignedMaximum:UInt = -1;
		var unsignedHalf:UInt = minimum;
		Sys.println([
			iadd(2147483647, 1),
			isub(minimum, 1),
			imul(2147483647, 2),
			ineg(minimum),
			idiv(minimum, -1),
			imod(minimum, -1),
			ishl(1, -1),
			ishr(minimum, -1),
			iushr(minimum, -1),
			iand(-1, 85),
			ior(80, 15),
			ixor(85, 15),
			inot(0),
			fmod(-7.0, 3.0),
			fsqrt(9.0),
			fsqrt(0.0),
			1.0 / fsqrt(-0.0) == Math.NEGATIVE_INFINITY ? 1 : 0,
			Math.isNaN(fsqrt(-1.0)) ? 1 : 0,
			fsqrt(Math.POSITIVE_INFINITY) == Math.POSITIVE_INFINITY ? 1 : 0,
			Math.isNaN(fsqrt(Math.NaN)) ? 1 : 0,
			fsqrt(3.0 * 3.0 + 4.0 * 4.0),
			fint(3.75),
			intQuotientByEight(2147483647),
			intQuotientByEight(minimum),
			intQuotientBySix(2147483647),
			intQuotientBySix(minimum),
			intQuotientByMaximum(2147483646),
			intQuotientByMaximum(minimum),
			intQuotientByOne(2147483647),
			intQuotientSideEffect(47),
			intQuotientByVariable(47, 8),
			intQuotientByZero(1),
			intQuotientByNegativeOne(minimum),
			floatQuotientByEight(47.0),
			intQuotientThroughFloatCast(47),
			uadd(unsignedMaximum, 1),
			umod(unsignedMaximum, unsignedHalf),
			ushl(1, -1),
			ushr(unsignedHalf, -1),
			update(3),
			updateParameter(3)
		].join(","));
		#else
		iadd(1, 2);
		isub(1, 2);
		imul(1, 2);
		ineg(1);
		idiv(1, 2);
		imod(1, 2);
		ishl(1, -1);
		ishr(-1, -1);
		iushr(-1, -1);
		iand(1, 2);
		ior(1, 2);
		ixor(1, 2);
		inot(1);
		iless(1, 2);
		fadd(1.0, 2.0);
		fsub(1.0, 2.0);
		fmul(1.0, 2.0);
		fneg(1.0);
		fdiv(1.0, 0.0);
		fmod(1.0, 0.0);
		fsqrt(25.0);
		fint(3.75);
		intQuotientByEight(47);
		intQuotientBySix(47);
		intQuotientByMaximum(47);
		intQuotientByOne(47);
		intQuotientSideEffect(47);
		intQuotientByVariable(47, 8);
		intQuotientByZero(47);
		intQuotientByNegativeOne(47);
		floatQuotientByEight(47.0);
		intQuotientThroughFloatCast(47);
		fequal(1.0, 2.0);
		uadd(1, 2);
		umod(1, 2);
		ushl(1, -1);
		ushr(1, -1);
		literalToU8();
		i32ToU8(-1);
		u8ToI32(c.IntConvert.modulo(255));
		i64ToU16(c.IntConvert.exact(-1));
		u32ToU64(c.IntConvert.modulo(-1));
		u32ToU8(c.IntConvert.modulo(-1));
		u8ToI16(c.IntConvert.modulo(255));
		update(3);
		updateParameter(3);
		#end
	}
}
