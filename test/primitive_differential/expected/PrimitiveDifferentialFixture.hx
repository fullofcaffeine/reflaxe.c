class PrimitiveDifferentialFixture {
	static function iadd(left:Int, right:Int):Int
		return left + right;

	static function iand(left:Int, right:Int):Int
		return left & right;

	static function idivInt(left:Int, right:Int):Int
		return Std.int(left / right);

	static function ieq(left:Int, right:Int):Bool
		return left == right;

	static function ilt(left:Int, right:Int):Bool
		return left < right;

	static function imod(left:Int, right:Int):Int
		return left % right;

	static function imul(left:Int, right:Int):Int
		return left * right;

	static function inot(value:Int):Int
		return ~value;

	static function ior(left:Int, right:Int):Int
		return left | right;

	static function ishl(left:Int, right:Int):Int
		return left << right;

	static function ishr(left:Int, right:Int):Int
		return left >> right;

	static function isub(left:Int, right:Int):Int
		return left - right;

	static function iushr(left:Int, right:Int):Int
		return left >>> right;

	static function ixor(left:Int, right:Int):Int
		return left ^ right;

	static function faddInt(left:Float, right:Float):Int
		return Std.int(left + right);

	static function fdivInt(left:Float, right:Float):Int
		return Std.int(left / right);

	static function feq(left:Float, right:Float):Bool
		return left == right;

	static function flt(left:Float, right:Float):Bool
		return left < right;

	static function fmodInt(left:Float, right:Float):Int
		return Std.int(left % right);

	static function fmulInt(left:Float, right:Float):Int
		return Std.int(left * right);

	static function fsubInt(left:Float, right:Float):Int
		return Std.int(left - right);

	static function uadd(left:UInt, right:UInt):UInt
		return left + right;

	static function ult(left:UInt, right:UInt):Bool
		return left < right;

	static function umod(left:UInt, right:UInt):UInt
		return left % right;

	static function umul(left:UInt, right:UInt):UInt
		return left * right;

	static function ushl(left:UInt, right:Int):UInt
		return left << right;

	static function ushr(left:UInt, right:Int):UInt
		return left >> right;

	static function usub(left:UInt, right:UInt):UInt
		return left - right;

	static function fint(value:Float):Int
		return Std.int(value);

	static function main():Void {
		#if primitive_differential_oracle
		Sys.println("case-000:u32:" + usub((2 : UInt), (65536 : UInt)));
		Sys.println("case-001:u32:" + usub((1242540958 : UInt), (-1487789886 : UInt)));
		Sys.println("case-002:i32:" + iushr(-1529960360, -595944127));
		Sys.println("case-003:u32:" + ushr((-1 : UInt), -1));
		Sys.println("case-004:u32:" + ushl((2 : UInt), -1942861741));
		Sys.println("case-005:u32:" + umul((31 : UInt), (65536 : UInt)));
		Sys.println("case-006:i32:" + ishl(663349440, -65536));
		Sys.println("case-007:bool:" + flt(-1.0, 2411.0));
		Sys.println("case-008:i32:" + ishl(1, 2147483646));
		Sys.println("case-009:i32:" + inot(-65536));
		Sys.println("case-010:u32:" + umul(((-2147483647 - 1) : UInt), (-2 : UInt)));
		Sys.println("case-011:bool:" + ilt(2140014822, 65535));
		Sys.println("case-012:i32:" + ishr(2147483646, -17340960));
		Sys.println("case-013:bool:" + feq(-8192.0, 1.0));
		Sys.println("case-014:i32:" + fsubInt(-1563.25, -0.0));
		Sys.println("case-015:bool:" + flt(-8192.0, -5133.75));
		Sys.println("case-016:i32:" + imul(-65536, 1659658283));
		Sys.println("case-017:i32:" + isub(-192194838, 0));
		Sys.println("case-018:i32:" + iushr(2147483647, 864338147));
		Sys.println("case-019:i32:" + isub(65535, 1889045594));
		Sys.println("case-020:i32:" + idivInt(-65536, 65535));
		Sys.println("case-021:u32:" + umod((2 : UInt), (31 : UInt)));
		Sys.println("case-022:u32:" + umod((2147483647 : UInt), (-1965165540 : UInt)));
		Sys.println("case-023:i32:" + isub(-2146252034, 555746818));
		Sys.println("case-024:i32:" + fmodInt(-5015.25, 1.0));
		Sys.println("case-025:i32:" + idivInt(2, (-2147483647 - 1)));
		Sys.println("case-026:i32:" + ishr(31, 2));
		Sys.println("case-027:bool:" + flt(-0.0, 6775.0));
		Sys.println("case-028:bool:" + flt(0.0, 4015.0));
		Sys.println("case-029:i32:" + fmulInt(1.0, -130.5));
		Sys.println("case-030:i32:" + iand(-1, -65536));
		Sys.println("case-031:i32:" + fmulInt(-1.0, 1.0));
		Sys.println("case-032:u32:" + ushr((-1885620801 : UInt), -31));
		Sys.println("case-033:i32:" + ishl(-394090625, -478136379));
		Sys.println("case-034:bool:" + ilt(32, -1590363146));
		Sys.println("case-035:u32:" + ushl((-2 : UInt), -33295407));
		Sys.println("case-036:i32:" + imul(-1371727805, 42081295));
		Sys.println("case-037:i32:" + inot(33));
		Sys.println("case-038:bool:" + ult((-1296410921 : UInt), (2147483647 : UInt)));
		Sys.println("case-039:i32:" + iadd(32, 65535));
		Sys.println("case-040:bool:" + ilt(31, 0));
		Sys.println("case-041:u32:" + ushr((65536 : UInt), -2));
		Sys.println("case-042:i32:" + ior(-1546044362, 1723067671));
		Sys.println("case-043:i32:" + imod(-1772570994, -32));
		Sys.println("case-044:i32:" + fmodInt(-6818.0, -8052.75));
		Sys.println("case-045:i32:" + inot(-456293137));
		Sys.println("case-046:i32:" + fdivInt(1024.5, -2246.25));
		Sys.println("case-047:i32:" + ixor(-1687817877, -865181082));
		Sys.println("case-048:bool:" + ult((625950247 : UInt), (1730156747 : UInt)));
		Sys.println("case-049:bool:" + flt(1024.5, 1024.5));
		Sys.println("case-050:i32:" + faddInt(1.0, -4873.0));
		Sys.println("case-051:i32:" + idivInt(65535, 33));
		Sys.println("case-052:i32:" + ixor(660778134, 138556725));
		Sys.println("case-053:bool:" + flt(8191.75, 3211.5));
		Sys.println("case-054:bool:" + feq(5275.5, 1024.5));
		Sys.println("case-055:u32:" + ushl((-23526078 : UInt), 0));
		Sys.println("case-056:i32:" + ior(574871839, -2));
		Sys.println("case-057:i32:" + ixor(65535, 2147483646));
		Sys.println("case-058:u32:" + uadd((65535 : UInt), (32 : UInt)));
		Sys.println("case-059:i32:" + faddInt(0.25, 8191.75));
		Sys.println("case-060:i32:" + isub(1625540872, -62965307));
		Sys.println("case-061:bool:" + ilt(-2, 1547661062));
		Sys.println("case-062:i32:" + imul((-2147483647 - 1), -1816108123));
		Sys.println("case-063:i32:" + iadd(2147483646, 1));
		Sys.println("case-064:i32:" + idivInt(-33, (-2147483647 - 1)));
		Sys.println("case-065:bool:" + ieq(-2147483647, (-2147483647 - 1)));
		Sys.println("case-066:i32:" + fdivInt(-0.0, -8192.0));
		Sys.println("case-067:bool:" + ult((65536 : UInt), (31 : UInt)));
		Sys.println("case-068:i32:" + inot(-1243861418));
		Sys.println("case-069:bool:" + ieq(65535, (-2147483647 - 1)));
		Sys.println("case-070:i32:" + ior(0, -65536));
		Sys.println("case-071:u32:" + umul((1 : UInt), (33 : UInt)));
		Sys.println("case-072:u32:" + ushr((32 : UInt), 0));
		Sys.println("case-073:i32:" + iushr(2, -65536));
		Sys.println("case-074:i32:" + ixor(487681730, 2147483647));
		Sys.println("case-075:i32:" + faddInt(1024.5, -1024.25));
		Sys.println("case-076:i32:" + iand(65535, 374364252));
		Sys.println("case-077:u32:" + usub((0 : UInt), (-2 : UInt)));
		Sys.println("case-078:u32:" + umul((-1 : UInt), (2147483647 : UInt)));
		Sys.println("case-079:i32:" + idivInt(-685406939, -2147483647));
		Sys.println("case-080:i32:" + ishr(-31, -2));
		Sys.println("case-081:i32:" + ixor(-1574636976, -31));
		Sys.println("case-082:i32:" + fdivInt(1024.5, 1.0));
		Sys.println("case-083:bool:" + flt(8191.75, -1.0));
		Sys.println("case-084:bool:" + ieq(1, 932078650));
		Sys.println("case-085:i32:" + isub(1772983708, 65535));
		Sys.println("case-086:i32:" + fdivInt(0.0, 8191.75));
		Sys.println("case-087:bool:" + feq(1024.5, 0.0));
		Sys.println("case-088:bool:" + feq(-8192.0, -2758.0));
		Sys.println("case-089:u32:" + umod((65535 : UInt), (237502992 : UInt)));
		Sys.println("case-090:i32:" + fdivInt(0.0, -8192.0));
		Sys.println("case-091:i32:" + iand(0, -2));
		Sys.println("case-092:i32:" + imul(-31, 1421103656));
		Sys.println("case-093:i32:" + idivInt(-2, 1035513225));
		Sys.println("case-094:u32:" + usub((-80221250 : UInt), (-1 : UInt)));
		Sys.println("case-095:bool:" + ilt(-65536, 2));
		Sys.println("case-096:i32:" + iadd(-65536, 1636456628));
		Sys.println("case-097:i32:" + fmodInt(-1.0, 1024.5));
		Sys.println("case-098:i32:" + ior(-33, -65536));
		Sys.println("case-099:i32:" + fsubInt(-0.0, 1024.5));
		Sys.println("case-100:u32:" + usub((31 : UInt), (32 : UInt)));
		Sys.println("case-101:i32:" + iushr(65535, 31));
		Sys.println("case-102:i32:" + fmulInt(0.25, -6485.5));
		Sys.println("case-103:i32:" + fmodInt(-1887.0, -1024.25));
		Sys.println("case-104:i32:" + imod(-65536, -32));
		Sys.println("case-105:i32:" + fsubInt(1024.5, -8192.0));
		Sys.println("case-106:i32:" + iand(-353266926, -31));
		Sys.println("case-107:i32:" + fdivInt(-1.0, 757.25));
		Sys.println("case-108:u32:" + uadd((65536 : UInt), (-345080564 : UInt)));
		Sys.println("case-109:i32:" + imod(1864848646, (-2147483647 - 1)));
		Sys.println("case-110:i32:" + isub(32, 2147483646));
		Sys.println("case-111:i32:" + imod(-33, -290659865));
		Sys.println("case-112:i32:" + faddInt(1024.5, 1.0));
		Sys.println("case-113:i32:" + ior(389429777, (-2147483647 - 1)));
		Sys.println("case-114:i32:" + fsubInt(-0.0, -6050.0));
		Sys.println("case-115:u32:" + umul((0 : UInt), (2147483647 : UInt)));
		Sys.println("case-116:i32:" + imul(32, 1375878956));
		Sys.println("case-117:i32:" + iand(32, -2147483647));
		Sys.println("case-118:u32:" + uadd((-377354062 : UInt), (0 : UInt)));
		Sys.println("case-119:i32:" + ishl(33, -607981955));
		Sys.println("case-120:i32:" + ishr(-126297699, -31));
		Sys.println("case-121:i32:" + iand(-33, 0));
		Sys.println("case-122:i32:" + ixor(-31, -2147483647));
		Sys.println("case-123:bool:" + ult((-2 : UInt), (-28441671 : UInt)));
		Sys.println("case-124:u32:" + ushr((0 : UInt), 0));
		Sys.println("case-125:i32:" + ishl(-1221893229, -65536));
		Sys.println("case-126:i32:" + idivInt(-1159360382, 2147483647));
		Sys.println("case-127:u32:" + ushr((-1 : UInt), -32));
		Sys.println("div-int-modulo-zero:i32:" + imod(7, 0));
		Sys.println("div-std-int-positive-infinity:i32:" + fint(Math.POSITIVE_INFINITY));
		Sys.println("div-std-int-positive-overflow:i32:" + fint(2147483648.0));
		#else
		usub((2 : UInt), (65536 : UInt));
		usub((1242540958 : UInt), (-1487789886 : UInt));
		iushr(-1529960360, -595944127);
		ushr((-1 : UInt), -1);
		ushl((2 : UInt), -1942861741);
		umul((31 : UInt), (65536 : UInt));
		ishl(663349440, -65536);
		flt(-1.0, 2411.0);
		ishl(1, 2147483646);
		inot(-65536);
		umul(((-2147483647 - 1) : UInt), (-2 : UInt));
		ilt(2140014822, 65535);
		ishr(2147483646, -17340960);
		feq(-8192.0, 1.0);
		fsubInt(-1563.25, -0.0);
		flt(-8192.0, -5133.75);
		imul(-65536, 1659658283);
		isub(-192194838, 0);
		iushr(2147483647, 864338147);
		isub(65535, 1889045594);
		idivInt(-65536, 65535);
		umod((2 : UInt), (31 : UInt));
		umod((2147483647 : UInt), (-1965165540 : UInt));
		isub(-2146252034, 555746818);
		fmodInt(-5015.25, 1.0);
		idivInt(2, (-2147483647 - 1));
		ishr(31, 2);
		flt(-0.0, 6775.0);
		flt(0.0, 4015.0);
		fmulInt(1.0, -130.5);
		iand(-1, -65536);
		fmulInt(-1.0, 1.0);
		ushr((-1885620801 : UInt), -31);
		ishl(-394090625, -478136379);
		ilt(32, -1590363146);
		ushl((-2 : UInt), -33295407);
		imul(-1371727805, 42081295);
		inot(33);
		ult((-1296410921 : UInt), (2147483647 : UInt));
		iadd(32, 65535);
		ilt(31, 0);
		ushr((65536 : UInt), -2);
		ior(-1546044362, 1723067671);
		imod(-1772570994, -32);
		fmodInt(-6818.0, -8052.75);
		inot(-456293137);
		fdivInt(1024.5, -2246.25);
		ixor(-1687817877, -865181082);
		ult((625950247 : UInt), (1730156747 : UInt));
		flt(1024.5, 1024.5);
		faddInt(1.0, -4873.0);
		idivInt(65535, 33);
		ixor(660778134, 138556725);
		flt(8191.75, 3211.5);
		feq(5275.5, 1024.5);
		ushl((-23526078 : UInt), 0);
		ior(574871839, -2);
		ixor(65535, 2147483646);
		uadd((65535 : UInt), (32 : UInt));
		faddInt(0.25, 8191.75);
		isub(1625540872, -62965307);
		ilt(-2, 1547661062);
		imul((-2147483647 - 1), -1816108123);
		iadd(2147483646, 1);
		idivInt(-33, (-2147483647 - 1));
		ieq(-2147483647, (-2147483647 - 1));
		fdivInt(-0.0, -8192.0);
		ult((65536 : UInt), (31 : UInt));
		inot(-1243861418);
		ieq(65535, (-2147483647 - 1));
		ior(0, -65536);
		umul((1 : UInt), (33 : UInt));
		ushr((32 : UInt), 0);
		iushr(2, -65536);
		ixor(487681730, 2147483647);
		faddInt(1024.5, -1024.25);
		iand(65535, 374364252);
		usub((0 : UInt), (-2 : UInt));
		umul((-1 : UInt), (2147483647 : UInt));
		idivInt(-685406939, -2147483647);
		ishr(-31, -2);
		ixor(-1574636976, -31);
		fdivInt(1024.5, 1.0);
		flt(8191.75, -1.0);
		ieq(1, 932078650);
		isub(1772983708, 65535);
		fdivInt(0.0, 8191.75);
		feq(1024.5, 0.0);
		feq(-8192.0, -2758.0);
		umod((65535 : UInt), (237502992 : UInt));
		fdivInt(0.0, -8192.0);
		iand(0, -2);
		imul(-31, 1421103656);
		idivInt(-2, 1035513225);
		usub((-80221250 : UInt), (-1 : UInt));
		ilt(-65536, 2);
		iadd(-65536, 1636456628);
		fmodInt(-1.0, 1024.5);
		ior(-33, -65536);
		fsubInt(-0.0, 1024.5);
		usub((31 : UInt), (32 : UInt));
		iushr(65535, 31);
		fmulInt(0.25, -6485.5);
		fmodInt(-1887.0, -1024.25);
		imod(-65536, -32);
		fsubInt(1024.5, -8192.0);
		iand(-353266926, -31);
		fdivInt(-1.0, 757.25);
		uadd((65536 : UInt), (-345080564 : UInt));
		imod(1864848646, (-2147483647 - 1));
		isub(32, 2147483646);
		imod(-33, -290659865);
		faddInt(1024.5, 1.0);
		ior(389429777, (-2147483647 - 1));
		fsubInt(-0.0, -6050.0);
		umul((0 : UInt), (2147483647 : UInt));
		imul(32, 1375878956);
		iand(32, -2147483647);
		uadd((-377354062 : UInt), (0 : UInt));
		ishl(33, -607981955);
		ishr(-126297699, -31);
		iand(-33, 0);
		ixor(-31, -2147483647);
		ult((-2 : UInt), (-28441671 : UInt));
		ushr((0 : UInt), 0);
		ishl(-1221893229, -65536);
		idivInt(-1159360382, 2147483647);
		ushr((-1 : UInt), -32);
		imod(7, 0);
		fint(0.0);
		fint(2147483648.0);
		#end
	}
}
