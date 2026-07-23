/**
	A project-relative path whose Haxe name prevents accidental value mixing.

	The abstract is a zero-wrapper source type over String. Generic
	specialization must retain `LogicalPath` in its semantic key while aggregate
	lowering stores the already-admitted String carrier.
**/
abstract LogicalPath(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}

/** A closed record that carries the nominal abstract through one generic call. */
typedef AssetRecord = {
	final assetPack:LogicalPath;
}

/** Runs the smallest complete nominal-abstract record specialization program. */
class Main {
	/** Return the exact closed Haxe type instead of erasing it to a shared box. */
	static function identity<T>(value:T):T {
		return value;
	}

	/** Keep the returned record and its nominal projection live in generated C. */
	static function consume(value:AssetRecord):Void {
		value.assetPack.text();
	}

	static function main():Void {
		final asset:AssetRecord = {
			assetPack: new LogicalPath("packs/caxecraft/base")
		};
		consume(identity(asset));
	}
}
