class ExactModuloRequiredFixture {
	static function narrow(value:Int):c.UInt8
		return c.IntConvert.exact(value);

	static function main():Void {
		narrow(-1);
	}
}
