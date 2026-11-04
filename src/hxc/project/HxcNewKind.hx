package hxc.project;

/**
	The three reviewed project shapes that `hxc new` can create.

	A closed enum abstract keeps command input separate from template selection;
	unknown strings fail before any filesystem work begins.
**/
enum abstract HxcNewKind(String) to String {
	var App = "app";
	var Library = "library";
	var Embedded = "embedded";

	/** Convert one exact command-line spelling to a supported project kind. */
	public static function parse(value:String):Null<HxcNewKind> {
		return switch value {
			case "app": App;
			case "library": Library;
			case "embedded": Embedded;
			case _: null;
		};
	}
}
