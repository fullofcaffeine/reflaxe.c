/**
 * Proves that a constructor may receive one closed anonymous record by value.
 *
 * The record is immutable value data, while `ConfiguredSpawn` is a nonescaping
 * class with observable identity. haxe.c should copy the record into the
 * constructor call and keep the class itself in automatic C storage.
 */
typedef SpawnPoint = {
	final x:Int;
	final z:Int;
}

/** A stack-owned class configured from one direct value record. */
final class ConfiguredSpawn {
	public var code:Int;

	/** Read the by-value record during construction without retaining it. */
	public function new(point:SpawnPoint) {
		code = point.x * 100 + point.z;
	}
}

/** Executes the record-parameter constructor contract without runtime output. */
final class Main {
	/** Keep running only if generated behavior differs from the Eval oracle. */
	static function main():Void {
		final point:SpawnPoint = {x: 12, z: 34};
		final spawn = new ConfiguredSpawn(point);
		while (spawn.code != 1234) {}
	}
}
