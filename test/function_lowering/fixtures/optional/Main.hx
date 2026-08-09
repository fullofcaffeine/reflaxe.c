private enum Choice {
	Fallback;
	Given(value:Int);
}

private typedef Point = {
	final x:Int;
}

private final class Marker {
	public var value:Int;

	public function new(value:Int) {
		this.value = value;
	}

	public function plus(value:Int = 5):Int {
		return this.value + value;
	}
}

private typedef PrimitiveRecord = {
	final flag:Null<Bool>;
	final count:Null<Int>;
	final code:Null<UInt>;
	final ratio:Null<Float>;
}

private enum PrimitivePacket {
	Flag(value:Null<Bool>);
	Count(value:Null<Int>);
	Code(value:Null<UInt>);
	Ratio(value:Null<Float>);
}

/** Exercises omission, explicit null, and present values without arity tests. */
class Main {
	static function optionalValue(?value:Choice):Int {
		return value == null ? -1 : switch value {
			case Fallback: 0;
			case Given(number): number;
		};
	}

	static function optionalPoint(?value:Point):Int {
		return value == null ? -1 : value.x;
	}

	static function optionalMarker(?value:Marker):Int {
		return value == null ? -1 : value.value;
	}

	/** Keep omitted, explicit null, false, and true distinct. */
	static function optionalBool(?value:Bool):Int {
		return value == null ? -1 : value ? 1 : 0;
	}

	/** Unwrap a checked nullable Int before ordinary arithmetic. */
	static function optionalInt(?value:Int):Int {
		return value == null ? -1 : value + 1;
	}

	/** Preserve unsigned zero separately from an omitted UInt. */
	static function optionalUInt(?value:UInt):Int {
		return value == null ? -1 : Std.int(value);
	}

	/** Preserve floating-point zero separately from an omitted Float. */
	static function optionalFloat(?value:Float):Int {
		return value == null ? -1 : Std.int(value);
	}

	/** Return a tagged nullable Bool without confusing false with absence. */
	static function maybeBool(present:Bool):Null<Bool> {
		return present ? false : null;
	}

	/** Return a tagged nullable Int without confusing zero with absence. */
	static function maybeInt(present:Bool):Null<Int> {
		return present ? 0 : null;
	}

	/** Return a tagged nullable UInt without confusing zero with absence. */
	static function maybeUInt(present:Bool):Null<UInt> {
		return present ? 0 : null;
	}

	/** Return a tagged nullable Float without confusing zero with absence. */
	static function maybeFloat(present:Bool):Null<Float> {
		return present ? 0.0 : null;
	}

	/**
		Match a copied optional scalar through a statement switch.

		The named `escape` local is deliberate: its load occurs before the switch
		branches, while the payload may be unwrapped only inside the present-value
		branch. HxcIR therefore has to carry the value through an explicit local
		instead of reusing a block-local result ID.
	**/
	static function optionalStatementSwitch(value:Null<Int>):Int {
		final escape = value;
		switch escape {
			case 34:
				return 1;
			case 92:
				return 2;
			case _:
				return 0;
		}
	}

	/** Keep the first conditional result alive while the second opens another join. */
	static function mismatchedOptionalFacts(left:Null<Int>, right:Null<Int>):Bool {
		return (left == 1) != (right == 0);
	}

	/** Read every primitive optional from one ordinary immutable record. */
	static function recordScore(value:PrimitiveRecord):Int {
		return (value.flag == null ? -8 : value.flag ? 8 : 0) + (value.count == null ? -4 : value.count) + (value.code == null ? -2 : Std.int(value.code))
			+ (value.ratio == null ? -1 : Std.int(value.ratio));
	}

	/** Match nullable primitive payloads without reading an absent payload. */
	static function packetScore(value:PrimitivePacket):Int {
		return switch value {
			case Flag(flag): flag == null ? -8 : flag ? 8 : 0;
			case Count(count): count == null ? -4 : count;
			case Code(code): code == null ? -2 : Std.int(code);
			case Ratio(ratio): ratio == null ? -1 : Std.int(ratio);
		};
	}

	/** Preserve tagged primitive values while storing and loading Array elements. */
	static function arrayScore(flags:Array<Null<Bool>>, counts:Array<Null<Int>>, codes:Array<Null<UInt>>, ratios:Array<Null<Float>>):Int {
		final flag = flags[0];
		final count = counts[0];
		final code = codes[0];
		final ratio = ratios[0];
		return (flag == null ? -8 : flag ? 8 : 0)
			+ (count == null ? -4 : count)
			+ (code == null ? -2 : Std.int(code))
			+ (ratio == null ? -1 : Std.int(ratio));
	}

	static function acceptText(value:String = "haxe"):Void {
		value;
	}

	static function main():Void {
		final marker = new Marker(13);
		final missingRecord:PrimitiveRecord = {
			flag: null,
			count: null,
			code: null,
			ratio: null
		};
		final zeroRecord:PrimitiveRecord = {
			flag: false,
			count: 0,
			code: 0,
			ratio: 0.0
		};
		final missingFlags:Array<Null<Bool>> = [null];
		final missingCounts:Array<Null<Int>> = [null];
		final missingCodes:Array<Null<UInt>> = [null];
		final missingRatios:Array<Null<Float>> = [null];
		final zeroFlags:Array<Null<Bool>> = [false];
		final zeroCounts:Array<Null<Int>> = [0];
		final zeroCodes:Array<Null<UInt>> = [0];
		final zeroRatios:Array<Null<Float>> = [0.0];
		while (optionalValue() != -1
			|| optionalValue(null) != -1
			|| optionalValue(Fallback) != 0
			|| optionalValue(Given(11)) != 11
			|| optionalPoint() != -1
			|| optionalPoint(null) != -1
			|| optionalPoint({
				x: 12
			}) != 12
			|| optionalMarker() != -1
			|| optionalMarker(null) != -1
			|| optionalMarker(marker) != 13
			|| optionalBool() != -1
			|| optionalBool(null) != -1
			|| optionalBool(false) != 0
			|| optionalBool(true) != 1
			|| optionalInt() != -1
			|| optionalInt(null) != -1
			|| optionalInt(0) != 1
			|| optionalInt(4) != 5
			|| optionalUInt() != -1
			|| optionalUInt(null) != -1
			|| optionalUInt(0) != 0
			|| optionalUInt(7) != 7
			|| optionalFloat() != -1
			|| optionalFloat(null) != -1
			|| optionalFloat(0.0) != 0
			|| optionalFloat(3.5) != 3
			|| maybeBool(false) != null
			|| optionalBool(maybeBool(true)) != 0
			|| maybeInt(false) != null
			|| optionalInt(maybeInt(true)) != 1
			|| maybeUInt(false) != null
			|| optionalUInt(maybeUInt(true)) != 0
			|| maybeFloat(false) != null
			|| optionalFloat(maybeFloat(true)) != 0
			|| optionalStatementSwitch(null) != 0
			|| optionalStatementSwitch(34) != 1
			|| optionalStatementSwitch(92) != 2
			|| optionalStatementSwitch(7) != 0
			|| mismatchedOptionalFacts(null, null)
			|| !mismatchedOptionalFacts(1, null)
			|| !mismatchedOptionalFacts(null, 0)
			|| mismatchedOptionalFacts(1, 0)
			|| recordScore(missingRecord) != -15
			|| recordScore(zeroRecord) != 0
			|| packetScore(Flag(null)) != -8
			|| packetScore(Flag(false)) != 0
			|| packetScore(Count(null)) != -4
			|| packetScore(Count(0)) != 0
			|| packetScore(Code(null)) != -2
			|| packetScore(Code(0)) != 0
			|| packetScore(Ratio(null)) != -1
			|| packetScore(Ratio(0.0)) != 0
			|| arrayScore(missingFlags, missingCounts, missingCodes, missingRatios) != -15
			|| arrayScore(zeroFlags, zeroCounts, zeroCodes, zeroRatios) != 0
			|| marker.plus() != 18
			|| marker.plus(7) != 20) {}
		acceptText();
		acceptText("C");
	}
}
