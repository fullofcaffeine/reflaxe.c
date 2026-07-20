package caxecraft.scenario;

/** Stable identity of one object, rule, sequence, objective, or map record. */
abstract ScenarioId(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}
