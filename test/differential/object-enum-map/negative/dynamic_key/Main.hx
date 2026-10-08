import haxe.ds.ObjectMap;

/** Reject Dynamic object keys instead of choosing structural or pointer accidents. */
final class Main {
	static function main():Void {
		final map = new ObjectMap<Dynamic, Int>();
		map.set({value: 1}, 1);
	}
}
