import haxe.ds.ObjectMap;

/** Reject default class-name formatting when a subtype can change behavior. */
final class Main {
	static function main():Void {
		final map = new ObjectMap<OpenKey, Int>();
		map.set(new OpenKey(), 1);
		map.toString();
	}
}

/** An intentionally open key whose runtime subtype is not statically exact. */
class OpenKey {
	public function new() {}
}
