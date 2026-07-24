/**
	Keeps a runtime-created nullable String operation outside the admitted slice.

	Null representation support must not accidentally claim that every String
	operation or owned String producer is implemented.
**/
class Main {
	static function unsupported(value:String):String {
		return value.toUpperCase();
	}

	static function main():Void {
		unsupported(null);
	}
}
