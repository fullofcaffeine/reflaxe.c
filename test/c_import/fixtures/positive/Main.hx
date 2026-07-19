class Main {
	static function neverReachUnusedImport():c.Int32 {
		return UnusedLib.value;
	}

	static function main():Void {
		var left = PointLib.make(PointLib.one, PointLib.negativeThree);
		var right = PointLib.make(PointLib.five, PointLib.seven);
		left.x = PointLib.one;
		var delta = left.x;
		left = PointLib.translate(left, delta, PointLib.five);
		var dot = PointLib.dot(left, right);
		var axis = Axis.Y;
		var component = PointLib.component(left, axis);
		var floatPoint = PointLib.makeFloatPoint(PointLib.floatOnePointFive, PointLib.floatNegativeTwo);
		floatPoint.x = c.Float32.fromFloat(0.5);
		floatPoint = PointLib.scaleFloatPoint(floatPoint, c.Float32.fromFloat(2.0));
		var floatDot = PointLib.dotFloatPoint(floatPoint, floatPoint);
		var widened = floatDot.toFloat();
		var tie = c.Float32.fromFloat(1.000000059604644775390625);
		var subnormal = c.Float32.fromFloat(1.401298464324817e-45);
		var positiveInfinity = c.Float32.fromFloat(1.0 / 0.0);
		var nan = c.Float32.fromFloat(0.0 / 0.0);
		var negativeZero = c.Float32.fromFloat(-0.0);
		var finiteOverflow = c.Float32.fromFloat(1e300);
		while (!PointLib.verify(left, right, dot, component, axis, "c-import-é")
			|| !PointLib.verifyFloat32(floatPoint, floatDot, widened, tie, subnormal, positiveInfinity, nan, negativeZero, finiteOverflow)) {}
	}
}
