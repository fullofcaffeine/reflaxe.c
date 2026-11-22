package hxc.cli;

/** A parsed command request passed to exactly one command owner. */
class HxcCliRequest {
	public final command:HxcCliCommand;
	public final arguments:Array<String>;
	public final json:Bool;

	public function new(command:HxcCliCommand, arguments:Array<String>, json:Bool) {
		this.command = command;
		this.arguments = arguments.copy();
		this.json = json;
	}
}
