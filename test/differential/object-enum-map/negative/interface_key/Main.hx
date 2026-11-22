import haxe.ds.ObjectMap;

/** Reject interface identity until every view has canonical object extraction. */
final class Main {
	static function main():Void {
		final map = new ObjectMap<IdentityView, Int>();
		final key:IdentityView = new IdentityObject();
		map.set(key, 1);
	}
}

/** An open identity view with no first-slice canonical allocation proof. */
interface IdentityView {}

/** One implementation that must not make the open interface silently safe. */
final class IdentityObject implements IdentityView {
	public function new() {}
}
