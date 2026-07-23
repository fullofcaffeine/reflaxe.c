enum PayloadValue {
	Empty;
	Number(value:Int);
}

class Main {
	static function main():Void {
		final left = Number(1);
		final right = Number(1);
		while (left == right) {}
	}
}
