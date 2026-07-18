class SignedNarrowingConversionFixture {
	static function narrow(value:Int):c.Int8
		return c.IntConvert.exact(value);

	static function main():Void {
		narrow(128);
	}
}
