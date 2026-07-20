package caxecraft.scenario;

/** Author-facing alias used to pick or group scenario objects. */
abstract ScenarioTag(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}
