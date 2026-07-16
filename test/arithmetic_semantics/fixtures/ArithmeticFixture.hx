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

	static function fint(value:Float):Int
		return Std.int(value);

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

	static function update(value:Int):Int {
		var current = value;
		var old = current++;
		var fresh = --current;
		current += old;
		current *= fresh;
		return current;
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
			fint(3.75),
			uadd(unsignedMaximum, 1),
			umod(unsignedMaximum, unsignedHalf),
			ushl(1, -1),
			ushr(unsignedHalf, -1),
			update(3)
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
		fint(3.75);
		fequal(1.0, 2.0);
		uadd(1, 2);
		umod(1, 2);
		ushl(1, -1);
		ushr(1, -1);
		update(3);
		#end
	}
}
