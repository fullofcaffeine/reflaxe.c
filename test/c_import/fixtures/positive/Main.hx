/** One ordinary immutable Haxe record containing a header-owned C value. */
typedef PointResources = {
	final point:Point;
	final ready:Bool;
}

class Main {
	/**
	 * Model the shape used by generated localization catalogs: ordinary Haxe
	 * chooses one of a closed set of source literals, while the C boundary
	 * borrows the selected literal's static storage without allocating.
	 */
	static function localizedLabel(locale:Int, message:Int):c.CString {
		return switch (locale) {
			case 0: localizedMessage(message);
			case _: "other-locale";
		}
	}

	/** Prove the conditional-expression half of static text selection. */
	static function localizedMessage(message:Int):c.CString
		return message == 0 ? "c-import-é" : "other-message";

	static function neverReachUnusedImport():c.Int32 {
		return UnusedLib.value;
	}

	static function localPoint(y:Coord, useY:Bool):Point {
		return c.StructInit.make({
			x: PointLib.component(PointLib.make(PointLib.five, PointLib.seven), Axis.X),
			y: {
				var selected = y;
				if (!useY)
					selected = PointLib.one;
				selected;
			}
		});
	}

	/** Bundle an imported by-value struct without raw C or ABI metadata. */
	static function pointResources(point:Point):PointResources
		return {point: point, ready: true};

	static function main():Void {
		PointLib.pointlib_build_fact_probe();
		var left = PointLib.make(PointLib.one, PointLib.negativeThree);
		var resources = pointResources(localPoint(PointLib.seven, true));
		var right = resources.point;
		left.x = PointLib.one;
		var delta = left.x;
		left = PointLib.translate(left, delta, PointLib.five);
		var pointAlias = PointLib.aliasIdentity(left);
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
		var locale = 0;
		while (!resources.ready
			|| !PointLib.verify(left, right, dot, component, axis, localizedLabel(locale, 0))
			|| !PointLib.verifyFloat32(floatPoint, floatDot, widened, tie, subnormal, positiveInfinity, nan, negativeZero, finiteOverflow)
			|| !InlineFloat32Probe.run()) {}
	}
}
