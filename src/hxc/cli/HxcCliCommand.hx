package hxc.cli;

/**
	The closed command vocabulary understood by the bootstrap CLI router.

	Later command owners attach behavior through `HxcCliExecutor`; keeping names
	here prevents each command from inventing a second parser or help registry.
**/
enum abstract HxcCliCommand(String) to String {
	var New = "new";
	var Build = "build";
	var Run = "run";
	var Test = "test";
	var Clean = "clean";
	var Doctor = "doctor";
	var Inspect = "inspect";
	var Bindgen = "bindgen";
	var Export = "export";
	var FmtGenerated = "fmt-generated";
	var Version = "version";
	var Help = "help";

	/** Parse one exact command name; aliases remain explicit router policy. */
	public static function parse(value:String):Null<HxcCliCommand> {
		return switch value {
			case "new": New;
			case "build": Build;
			case "run": Run;
			case "test": Test;
			case "clean": Clean;
			case "doctor": Doctor;
			case "inspect": Inspect;
			case "bindgen": Bindgen;
			case "export": Export;
			case "fmt-generated": FmtGenerated;
			case "version": Version;
			case "help": Help;
			case _: null;
		};
	}

	/** Return commands in the stable order used by help and completion. */
	public static function all():Array<HxcCliCommand> {
		return [
			New,
			Build,
			Run,
			Test,
			Clean,
			Doctor,
			Inspect,
			Bindgen,
			Export,
			FmtGenerated,
			Version,
			Help
		];
	}

	/** Give one short user-facing purpose without claiming unimplemented behavior. */
	public function summary():String {
		return switch this {
			case "new": "Create a project from a reviewed template.";
			case "build": "Generate C and build the selected artifact.";
			case "run": "Build and run an executable.";
			case "test": "Run selected project tests.";
			case "clean": "Remove outputs owned by hxc manifests.";
			case "doctor": "Check the selected development toolchain.";
			case "inspect": "Explain compiler and artifact decisions.";
			case "bindgen": "Capture Clang semantic binding facts.";
			case "export": "Build and verify a public C ABI package.";
			case "fmt-generated": "Format generated C without changing semantics.";
			case "version": "Print hxc and CLI protocol versions.";
			case "help": "Show command help.";
			case _: "";
		};
	}
}
