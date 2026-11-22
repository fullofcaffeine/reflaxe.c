package hxc.cli;

/** Stable process-exit categories shared by human and JSON CLI modes. */
enum abstract HxcCliExitCategory(String) to String {
	var Success = "success";
	var Usage = "usage";
	var Unavailable = "unavailable";
	var Command = "command";
	var Internal = "internal";

	/** Map router-owned categories to stable process codes. */
	public function code():Int {
		return switch this {
			case "success": 0;
			case "usage": 64;
			case "unavailable": 69;
			case "command": 1;
			case "internal": 70;
			case _: 70;
		};
	}
}
