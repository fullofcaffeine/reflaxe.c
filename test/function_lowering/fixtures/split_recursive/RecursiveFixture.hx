class RecursiveFixture {
	static function recursive(left:Int, right:Int):Void {
		recursive(right, left);
	}

	static function main():Void {
		recursive(1, 2);
	}
}
