class StaticInitializationCycleFixture {
	static function main():Void {
		StaticInitializationCycleA.value;
	}
}
