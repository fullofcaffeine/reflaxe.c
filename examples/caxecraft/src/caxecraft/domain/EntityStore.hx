package caxecraft.domain;

import caxecraft.domain.Character.empty as emptyCharacter;
import caxecraft.domain.Character.isValid as isValidCharacter;

/**
	Owns every committed character in one loaded simulation.

	`GameSession` inserts player and non-player characters by stable `EntityId`.
	The private Array preserves insertion order, which gives simulation, saves,
	and tests one deterministic order. Public observations are shallow Array
	copies: the Array container is independent, while each immutable `Character`
	record remains a safe value snapshot.

	The explicit capacity is a gameplay and validation limit, not a C array
	layout promise. Ordinary Haxe Array storage lets a level contain a variable
	number of characters; haxe.c selects and documents the managed C
	representation. If profiling later justifies a packed store, callers can keep
	using the same ID-based API.

	This is a class rather than a record plus unrelated helpers because it is the
	mutable commit point for character ownership. Its identity is inherited
	from the containing session lifetime; snapshots enter and leave as values, but
	callers cannot replace the store's invariants piecemeal.
**/
final class EntityStore {
	/** Maximum live characters admitted by the current bounded engine. */
	public static inline final MAX_CHARACTERS:Int = 64;

	final characters:Array<Character> = [];

	/** Create one empty store; validated level loading adds every live character. */
	public function new() {}

	/**
		Insert one new character at the end of deterministic iteration order.

		Invalid IDs, duplicate IDs, and capacity overflow leave the store unchanged.
		Replacing an existing snapshot is deliberately a separate operation so a
		loader cannot silently turn two authored placements into one character.
	**/
	public function put(character:Character):Bool {
		if (!isValidCharacter(character) || contains(character.id) || characters.length >= MAX_CHARACTERS)
			return false;
		characters.push(character);
		return true;
	}

	/** True when this store owns the requested live character. */
	public function contains(id:EntityId):Bool
		return indexOf(id) >= 0;

	/**
		Read one immutable value snapshot by stable identity.

		A missing ID returns the invalid empty value instead of exposing a nullable
		or another entity. The caller can test `Character.isValid`.
	**/
	public function read(id:EntityId):Character {
		final index = indexOf(id);
		return index < 0 ? emptyCharacter() : characters[index];
	}

	/** Commit a newer snapshot only when both stored and replacement IDs agree. */
	public function replace(id:EntityId, character:Character):Bool {
		final index = indexOf(id);
		if (index < 0 || character.id != id || !isValidCharacter(character))
			return false;
		characters[index] = character;
		return true;
	}

	/**
		Remove one non-retained character while preserving the remaining order.

		`Array.splice` shifts later snapshots left and returns removed values; this
		store discards that returned container immediately. Haxe.c must still honor
		the Array element ownership and cleanup contract—this is ordinary Haxe, not
		a hand-written C storage workaround.
	**/
	public function remove(id:EntityId):Bool {
		final index = indexOf(id);
		if (index < 0)
			return false;
		characters.splice(index, 1);
		return true;
	}

	/**
		Replace every non-retained character as one validated store transaction.

		The local character named by `retained` remains first. All replacements
		are checked and copied before commit begins, so duplicate, invalid, or
		over-capacity input leaves the previous store untouched. Commit mutates the
		private Array in place; no callback or observation can run midway through
		this synchronous operation.
	**/
	public function replaceOthers(retained:EntityId, replacements:Array<Character>):Bool {
		final retainedCharacter = read(retained);
		if (!isValidCharacter(retainedCharacter) || replacements.length + 1 > MAX_CHARACTERS)
			return false;
		final next:Array<Character> = [retainedCharacter];
		for (replacement in replacements) {
			if (!isValidCharacter(replacement) || replacement.id == retained)
				return false;
			for (accepted in next)
				if (accepted.id == replacement.id)
					return false;
			next.push(replacement);
		}
		while (characters.length > 0)
			characters.pop();
		for (character in next)
			characters.push(character);
		return true;
	}

	/** Number of live character snapshots currently owned by this store. */
	public inline function count():Int
		return characters.length;

	/**
		Return a copy-owned observation in deterministic insertion order.

		Changing the returned Array cannot add, remove, or replace store entries.
		`Character` itself is immutable, so its shallow element copy grants no
		mutation authority over committed simulation state.
	**/
	public function snapshots():Array<Character>
		return characters.copy();

	/** Find one stable ID without exposing the private Array or its indices. */
	function indexOf(id:EntityId):Int {
		if (!id.isValid())
			return -1;
		for (index in 0...characters.length)
			if (characters[index].id == id)
				return index;
		return -1;
	}
}
