package caxecraft.scenario;

/** Language-neutral key resolved by the English or Spanish message catalog. */
abstract MessageId(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}
