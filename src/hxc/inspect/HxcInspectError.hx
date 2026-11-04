package hxc.inspect;

import hxc.cli.HxcCliExitCategory;

/** A bounded inspection failure that maps to one stable CLI diagnostic. */
class HxcInspectError extends haxe.Exception {
	public final category:HxcCliExitCategory;
	public final code:String;
	public final remediation:String;

	public function new(category:HxcCliExitCategory, code:String, message:String, remediation:String) {
		super(message);
		this.category = category;
		this.code = code;
		this.remediation = remediation;
	}
}
