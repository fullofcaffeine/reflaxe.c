package hxc.bindgen;

import hxc.cli.HxcCliExitCategory;

/** One bounded bindgen failure with an optional exact Clang stderr stream. */
class HxcBindgenError extends haxe.Exception {
	/** Stable CLI category selected by the failing boundary. */
	public final category:HxcCliExitCategory;

	/** Stable CLI diagnostic identifier. */
	public final code:String;

	/** Action the caller can take without inspecting implementation details. */
	public final remediation:String;

	/** Exact Clang stderr when a child process produced the failure. */
	public final childStderr:String;

	/** Preserve one bounded failure and its optional child diagnostics. */
	public function new(category:HxcCliExitCategory, code:String, message:String, remediation:String, childStderr:String = "") {
		super(message);
		this.category = category;
		this.code = code;
		this.remediation = remediation;
		this.childStderr = childStderr;
	}
}
