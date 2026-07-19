import raylib.raw.Raylib;

class Main {
	static function main():Void {
		Raylib.InitWindow(NulConstants.size, NulConstants.size, "invalid\x00title");
	}
}
