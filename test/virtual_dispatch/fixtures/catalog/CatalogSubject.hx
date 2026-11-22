/** Ordinary typed declarations used to add dispatch inputs in a controlled order. */
class CatalogSubject {
	/** Type the fixture without executing target-specific compiler checks at runtime. */
	static function main():Void {}

	public static function baseCall(value:CatalogBase):Int
		return value.value();

	public static function leftCall(value:CatalogLeft):Int
		return value.value();

	public static function rightCall(value:CatalogRight):Int
		return value.value();
}

/** The inherited implementation stays reachable alongside later overrides. */
class CatalogBase {
	public function new() {}

	public function value():Int
		return 1;
}

/** A class without an override selects its base declaration. */
class CatalogInherited extends CatalogBase {}

/** The override is shared by ordinary and interface calls. */
class CatalogLeaf extends CatalogBase {
	override public function value():Int
		return 2;
}

/** A later override expands an existing slot without replacing earlier targets. */
class CatalogLater extends CatalogBase {
	override public function value():Int
		return 3;
}

/** A same-named method does not establish an inheritance relationship. */
class CatalogUnrelated {
	public function new() {}

	public function value():Int
		return 99;
}

/** Distinct interfaces may declare the same method name. */
interface CatalogLeft {
	public function value():Int;
}

/** Slot identity includes the interface, not only the method name. */
interface CatalogRight {
	public function value():Int;
}

/** Inherited methods satisfy both interface views. */
class CatalogBoth extends CatalogLeaf implements CatalogLeft implements CatalogRight {}
