/** Attempts to use a non-extern class as a header-owned field-hidden struct. */
class Main {
	static function main():Void {
		final value:HiddenPoint = PointLib.hiddenPointIdentity(PointLib.hiddenPointMake());
		if (PointLib.hiddenPointVerify(value)) {}
	}
}
