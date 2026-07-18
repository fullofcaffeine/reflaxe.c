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
		while (!PointLib.verify(left, right, dot, component, axis, "c-import-é")) {}
	}
}
