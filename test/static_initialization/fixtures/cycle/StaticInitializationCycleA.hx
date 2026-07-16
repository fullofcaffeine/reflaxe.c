class StaticInitializationCycleA {
	public static var value:Int = StaticInitializationCycleB.value + 1;
}
