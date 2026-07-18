class UnsignedToSignedConversionFixture {
	static function reinterpret(value:c.UInt32):Int
		return c.IntConvert.exact(value);

	static function main():Void {
		reinterpret(c.IntConvert.modulo(-1));
	}
}
