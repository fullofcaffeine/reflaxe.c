/**
	Provides an executable semantic oracle for the first String-keyed Map slice.

	The same ordinary Haxe source runs under Eval and through haxe.c, allowing the
	runner to compare behavior without teaching the fixture about generated C.
**/

/**
	One direct record stored by value in the managed-record StringMap fixture.

	The record deliberately owns an Array. Copying a slot must therefore retain
	the shared Array, replacing or removing a slot must release the old owner, and
	a successful lookup must return a new record owner. A plain byte copy would
	appear to work until cleanup, then either leak or release the Array twice.
**/
private typedef StoredRecord = {
	final score:Int;
	final flags:Array<Bool>;
}

/**
	A payload-free enum stored directly in a StringMap slot.

	haxe.c keeps this nominal Haxe type in HxcIR and lowers it to one native C
	`enum`. The map can therefore copy the value bytes without callbacks while
	still preventing a different enum with the same numeric tags from being
	mistaken for `StoredKind`.
**/
private enum StoredKind {
	Draft;
	Ready;
	Complete;
}

/**
	A key transported through a managed enum payload instead of a literal.

	The payload forces the generated program to pass a runtime-owned String view
	to `Map.exists`. StringMap must hash that ordinary Haxe value exactly like a
	literal-backed String while leaving ownership with the enum.
**/
private enum LookupKey {
	Lookup(value:String);
}

/**
	A domain identity that keeps its String carrier nominal in Haxe.

	The map must not erase this type merely because its runtime representation is
	the same immutable UTF-8 view used by `String`.
**/
private abstract StoredName(String) {
	public inline function new(value:String)
		this = value;

	/** Return the visible text without giving up nominal storage at the map edge. */
	public inline function text():String
		return this;
}

/** A distinct String identity proves specializations do not collapse by carrier. */
private abstract StoredTag(String) {
	public inline function new(value:String)
		this = value;

	/** Return the text while preserving `StoredTag` at storage boundaries. */
	public inline function text():String
		return this;
}

/** Own one nominal managed-String map through an ordinary final class field. */
private final class NameTable {
	final values:Map<String, StoredName> = [];

	/** Construct one empty shared table. */
	public function new() {}

	/** Insert or failure-atomically replace one typed identity. */
	public function set(key:String, value:StoredName):Void
		values.set(key, value);

	/** Return an independently owned view when the key exists. */
	public function get(key:String):Null<StoredName>
		return values.get(key);

	/** Remove one owned slot without changing the caller's source value. */
	public function remove(key:String):Bool
		return values.remove(key);

	/** Release every owned slot while preserving the shared table object. */
	public function clear():Void
		values.clear();
}

/**
	Owns a fieldless-enum map through an ordinary Haxe class field.

	This mirrors the Caxecraft validation table that exposed the compiler gap.
	The class owns the shared Map object; each slot contains one direct enum
	value rather than a boxed object or an erased integer.
**/
private final class KindTable {
	final values:Map<String, StoredKind> = [];

	/** Construct one table; Haxe lowers the field initializer into this body. */
	public function new() {}

	/** Insert or replace one exact `StoredKind` value. */
	public function set(key:String, value:StoredKind):Void
		values.set(key, value);

	/** Distinguish an absent key from every valid enum constructor. */
	public function get(key:String):Null<StoredKind>
		return values.get(key);

	/**
		Return the stored enum or a caller-supplied value when the key is absent.

		This is ordinary Haxe null refinement: after the null branch, haxe.c
		unwraps the direct optional and returns the exact `StoredKind` value.
	**/
	public function getOr(key:String, fallback:StoredKind):StoredKind {
		final value = values.get(key);
		return value == null ? fallback : value;
	}

	/** Remove one enum slot and report whether it existed. */
	public function remove(key:String):Bool
		return values.remove(key);
}

/**
	Owns a `Map<String, StoredRecord>` as an ordinary final Haxe class field.

	This small class proves both halves of the boundary Caxecraft needs: the
	constructor transfers a fresh map into its field, and later methods borrow
	that same shared table without exposing compiler-specific ownership code.
**/
private final class RecordTable {
	final values:Map<String, StoredRecord> = [];

	/** Construct one table; Haxe lowers the field initializer into this body. */
	public function new() {}

	/** Insert or replace one direct record using normal Map semantics. */
	public function set(key:String, value:StoredRecord):Void
		values.set(key, value);

	/** Return a copied record owner when the key is present. */
	public function get(key:String):Null<StoredRecord>
		return values.get(key);

	/** Remove one live slot and report whether it existed. */
	public function remove(key:String):Bool
		return values.remove(key);
}

/** Supplies one real instance-call boundary for fresh StringMap arguments. */
private final class MapBorrower {
	/** Construct a stateless borrower; class identity exists only for call coverage. */
	public function new() {}

	/** Read one expected entry without retaining the caller-owned map. */
	public function contains(values:Map<String, Bool>, key:String, expected:Bool):Bool {
		final value = values.get(key);
		return value != null && value == expected;
	}
}

/**
	Exercises the first ordinary-Haxe `Map<String, Bool>` compiler slice.

	The program keeps every operation observable through control flow so haxe.c
	cannot replace the map with compile-time constants. It deliberately tests
	shared identity, because assigning a Haxe Map copies its reference rather
	than cloning its entries. Missing and stored `false` values are checked
	separately through `exists` and nullable `get`.
**/
final class Main {
	/**
		Return one integer lookup without confusing a stored zero with absence.

		`Map.get` returns `Null<Int>`, so the explicit null branch owns the
		difference. The present branch then unwraps the exact signed 32-bit value
		that haxe.c stored directly in the StringMap slot.
	**/
	static function intValue(values:Map<String, Int>, key:String, fallback:Int):Int {
		final value = values.get(key);
		return value == null ? fallback : value;
	}

	/** Look up a String borrowed from a managed enum payload. */
	static function hasLookup(values:Map<String, Int>, key:LookupKey):Bool
		return switch key {
			case Lookup(value): values.exists(value);
		};

	/**
		Exercise direct integer slots through aliases and all core mutations.

		The zero entry is important: `exists("zero")` must remain true even though
		`get("zero")` returns the scalar value zero, while a missing key follows the
		separate nullable path.
	**/
	static function integerTrace():Bool {
		final values:Map<String, Int> = [];
		final alias = values;
		values.set("zero", 0);
		values.set("depth", 3);
		if (!alias.exists("zero") || !hasLookup(alias, Lookup("zero")) || hasLookup(alias, Lookup("missing")) || intValue(alias, "zero", -1) != 0
			|| intValue(alias, "missing", -1) != -1)
			return false;

		alias.set("depth", 7);
		if (intValue(values, "depth", -1) != 7 || !values.remove("zero") || values.remove("zero"))
			return false;

		values.clear();
		return !alias.exists("depth") && alias.get("depth") == null;
	}

	/**
		Exercise direct enum insertion, replacement, lookup, absence, and removal.

		The class-field path proves construction and cleanup of the shared table.
		The values themselves need no ownership callbacks because each one is only
		the closed native-enum discriminant selected by the compiler.
	**/
	static function fieldlessEnumTrace():Bool {
		final table = new KindTable();
		table.set("build", Draft);
		if (table.getOr("build", Complete) != Draft || table.get("missing") != null)
			return false;

		table.set("build", Ready);
		if (table.getOr("build", Complete) != Ready)
			return false;

		table.set("release", Complete);
		return table.remove("build")
			&& !table.remove("build")
			&& table.get("build") == null
			&& table.getOr("release", Draft) == Complete;
	}

	/**
		Exercise managed records through construction, replacement, lookup, and removal.

		The Array inside each direct record keeps Haxe's shared Array identity.
		Mutating the source after insertion is therefore visible through a lookup,
		while the record itself remains a compact by-value compiler representation.
	**/
	static function managedRecordTrace():Bool {
		final table = new RecordTable();
		final first:StoredRecord = {score: 7, flags: [false]};
		table.set("hero", first);
		first.flags[0] = true;
		final loaded = table.get("hero");
		if (loaded == null || loaded.score != 7 || !loaded.flags[0])
			return false;

		final replacement:StoredRecord = {score: 11, flags: [false, true]};
		table.set("hero", replacement);
		final replaced = table.get("hero");
		if (replaced == null || replaced.score != 11 || replaced.flags[0] || !replaced.flags[1])
			return false;
		if (!table.remove("hero") || table.remove("hero") || table.get("hero") != null)
			return false;

		// The local owners remain valid after their former map slots are gone.
		return first.flags[0] && replacement.flags[1];
	}

	/** Build a runtime-owned nominal value so the fixture is not literal-only. */
	static function runtimeName():StoredName
		return new StoredName("run" + String.fromCharCode(116) + "ime");

	/** Build a runtime-owned key to exercise the borrowed key call boundary. */
	static function runtimeKey():String
		return "run-" + String.fromCharCode(107) + "ey";

	/** Leave the map as the sole owner after this helper's source local ends. */
	static function insertRuntimeName(table:NameTable, key:String):Void {
		final source = runtimeName();
		table.set(key, source);
	}

	/** Leave one live runtime-owned slot for final map destruction. */
	static function finalRuntimeSlot():Bool {
		final table = new NameTable();
		table.set("final", runtimeName());
		return table.get("final") != null;
	}

	/** Pin raw String and a second nominal String as intentional carrier families. */
	static function neighboringStringFamilies():Bool {
		final raw:Map<String, String> = [];
		raw.set("raw", "raw-" + String.fromCharCode(118) + "alue");
		raw.set(runtimeKey(), "key-" + String.fromCharCode(118) + "alue");
		final rawValue = raw.get("raw");
		final runtimeKeyValue = raw.get(runtimeKey());
		final tags:Map<String, StoredTag> = [];
		tags.set("tag", new StoredTag("tag-" + String.fromCharCode(118) + "alue"));
		final tag = tags.get("tag");
		return rawValue == "raw-value" && runtimeKeyValue == "key-value" && tag != null && tag.text() == "tag-value";
	}

	/** Exercise nominal String storage, copy, replacement, and cleanup. */
	static function nominalStringTrace():Bool {
		final table = new NameTable();
		table.set("literal", new StoredName("static"));
		final literal = table.get("literal");
		if (literal == null || literal.text() != "static" || table.get("missing") != null)
			return false;

		table.set("direct", runtimeName());
		final direct = table.get("direct");
		if (direct == null || direct.text() != "runtime")
			return false;

		final source = runtimeName();
		table.set("active", source);
		final loaded = table.get("active");
		if (loaded == null || loaded.text() != "runtime")
			return false;
		table.get("active");
		table.get("missing-ignored");

		table.set("same-owner", source);
		table.set("same-owner", source);
		final sameOwner = table.get("same-owner");
		if (sameOwner == null || sameOwner.text() != "runtime")
			return false;

		insertRuntimeName(table, "survivor");
		final survivor = table.get("survivor");
		if (survivor == null || !table.remove("survivor") || survivor.text() != "runtime")
			return false;

		table.set(runtimeKey(), runtimeName());
		final runtimeKeyValue = table.get(runtimeKey());
		if (runtimeKeyValue == null || runtimeKeyValue.text() != "runtime")
			return false;

		table.set("copy-source", runtimeName());
		table.set("copy-target", table.get("copy-source"));
		final copied = table.get("copy-target");
		if (copied == null || copied.text() != "runtime")
			return false;

		table.set("active", new StoredName("replacement"));
		final replacement = table.get("active");
		if (loaded.text() != "runtime" || replacement == null || replacement.text() != "replacement")
			return false;
		if (!table.remove("active") || table.remove("active") || table.get("active") != null || replacement.text() != "replacement")
			return false;
		for (index in 0...64)
			table.set("growth-" + index, runtimeName());
		table.set("clear", runtimeName());
		table.clear();
		if (table.get("literal") != null
			|| table.get("clear") != null
			|| table.get("growth-63") != null
			|| source.text() != "runtime")
			return false;
		table.set("scope-cleanup", runtimeName());
		return finalRuntimeSlot() && neighboringStringFamilies();
	}

	/**
		Create and return one mutable map.

		Returning the value proves that construction ownership can move across a
		function boundary without cloning or prematurely destroying the table.
	**/
	static function makeMap():Map<String, Bool> {
		final result:Map<String, Bool> = [];
		result.set("alpha", false);
		result.set("beta", true);
		return result;
	}

	/** Preserve alias identity across an ordinary parameter and return value. */
	static function identity(values:Map<String, Bool>):Map<String, Bool>
		return values;

	/** Mutate the caller's table through a borrowed parameter. */
	static function replace(values:Map<String, Bool>, key:String, value:Bool):Void
		values.set(key, value);

	/** Keep nullable lookup explicit so missing remains distinct from `false`. */
	static function lookup(values:Map<String, Bool>, key:String):Null<Bool>
		return values.get(key);

	/** Pass fresh empty and populated maps through static and instance borrows. */
	static function freshArgumentTrace():Bool {
		final borrower = new MapBorrower();
		return lookup([], runtimeKey()) == null && borrower.contains(makeMap(), "beta", true);
	}

	/**
		Return the absent value of the same nullable pointer carrier.

		The caller owns and later cleans up the result just like a present map;
		retain/release must therefore accept the absent `NULL` carrier as a no-op.
	**/
	static function absentMap():Null<Map<String, Bool>>
		return null;

	/**
		Run the complete bounded semantic trace.

		The final empty loop is a portable, output-free assertion: correct Eval
		and generated-C executions terminate, while any semantic mismatch remains
		observable to the test runner as a timeout.
	**/
	static function main():Void {
		final values = makeMap();
		final alias = values;
		final returned = identity(values);
		final absent = absentMap();
		final alphaBefore = lookup(values, "alpha");
		final missingBefore = lookup(values, "missing");

		replace(alias, "alpha", true);
		final removedBeta = returned.remove("beta");
		final removedBetaAgain = returned.remove("beta");
		values.set("gamma", false);
		values.set("", true);
		final gammaBeforeClear = alias.exists("gamma");
		final emptyBeforeClear = alias.exists("");
		alias.clear();

		while (!integerTrace() || !fieldlessEnumTrace() || !managedRecordTrace() || !nominalStringTrace() || !freshArgumentTrace() || alias != values
			|| absent != null || null != absent || values == null || alphaBefore == null || alphaBefore || missingBefore != null || !removedBeta
			|| removedBetaAgain || !gammaBeforeClear || !emptyBeforeClear || values.exists("alpha") || values.exists("gamma") || values.exists("")
			|| values.get("alpha") != null) {}
	}
}
