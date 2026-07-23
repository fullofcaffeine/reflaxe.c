class Main {
	static function main():Void {
		CStringApi.consume(CStringApi.condition() ? "c-import" : Std.string(CStringApi.condition()));
	}
}
