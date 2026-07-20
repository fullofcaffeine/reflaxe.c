package caxecraft.scenario;

/** Logical registry key for a block, item, entity, prefab, state, or effect. */
abstract ContentId(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}
