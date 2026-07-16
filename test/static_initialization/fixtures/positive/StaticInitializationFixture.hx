class StaticInitializationFixture {
	static function outcome():Int {
		return MStaticInitTrace.classOrder + MStaticInitTrace.phaseValue + MStaticInitTrace.trace + ZStaticInitPrerequisite.value
			+ AStaticInitDependent.first + AStaticInitDependent.second + AStaticInitDependent.value;
	}

	static function main():Void {
		#if static_initialization_oracle
		trace([
			MStaticInitTrace.classOrder,
			MStaticInitTrace.phaseValue,
			MStaticInitTrace.trace,
			ZStaticInitPrerequisite.value,
			AStaticInitDependent.first,
			AStaticInitDependent.second,
			AStaticInitDependent.value,
			outcome()
		].join(","));
		#else
		outcome();
		#end
	}
}
