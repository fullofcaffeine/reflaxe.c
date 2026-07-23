package;

/** A nominal application ID that keeps ordinary String ergonomics in Haxe. */
abstract MessageId(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}

/** A representative private enum carrying one immutable String-backed ID. */
enum Message {
	Empty;
	Text(value:MessageId);
}

class Main {
	static function identity(value:MessageId):MessageId
		return value;

	static function label(value:Message):MessageId {
		return switch value {
			case Empty: new MessageId("");
			case Text(id): identity(id);
		};
	}

	static function main():Void {
		final original = Text(new MessageId("café🙂"));
		final copied = original;
		while (!(label(copied).text() == "café🙂" && label(Empty).text() == "" && label(copied).text() != "other")) {}
	}
}
