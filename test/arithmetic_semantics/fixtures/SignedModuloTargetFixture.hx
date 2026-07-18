class SignedModuloTargetFixture {
	static function invalid(value:Int):Int
		return c.IntConvert.modulo(value);

	static function main():Void {
		invalid(1);
	}
}
