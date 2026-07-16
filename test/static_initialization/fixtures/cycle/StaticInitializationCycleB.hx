class StaticInitializationCycleB {
	public static var value:Int = StaticInitializationCycleA.value + 1;
}
