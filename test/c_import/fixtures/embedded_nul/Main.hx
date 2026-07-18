class Main {
	static function main():Void {
		CStringApi.consume("bad\x00value");
	}
}
