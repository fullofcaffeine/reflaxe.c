package caxecraft.scenario;

/** Project-relative forward-slash path; never an absolute host path. */
abstract LogicalPath(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}
