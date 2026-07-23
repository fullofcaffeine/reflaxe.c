package caxecraft.domain;

import caxecraft.domain.Character.empty as emptyCharacter;
import caxecraft.domain.Character.isValid as isValidCharacter;

/**
	Owns the one committed character admitted by the first architecture slice.

	This is intentionally a narrow storage boundary, not a pretend general entity
	collection. `GameSession` owns this object directly for its complete lifetime;
	haxe.c embeds it in the parent C struct without allocation. The one-character
	capacity remains a first-slice gameplay limit because fixed C arrays of
	aggregate records are not admitted yet. The later runtime-actor slice may
	replace the private storage after that general representation is earned;
	callers already use stable IDs and do not depend on a field layout.

	This is a class rather than a record plus unrelated helpers because it is the
	one mutable commit point for character ownership. Its identity is inherited
	from the containing session lifetime; snapshots enter and leave as values, but
	callers cannot replace the store's invariants piecemeal.
**/
final class EntityStore {
	var occupied:Bool;
	var current:Character;

	/** Create one empty store with no playable default character. */
	public function new() {
		occupied = false;
		current = emptyCharacter();
	}

	/** Insert the first character or replace the existing character with the same ID. */
	public function put(character:Character):Bool {
		if (!isValidCharacter(character))
			return false;
		if (occupied && current.id != character.id)
			return false;
		current = character;
		occupied = true;
		return true;
	}

	/** True when this store owns the requested live character. */
	public inline function contains(id:EntityId):Bool
		return occupied && current.id == id;

	/**
		Read a value snapshot after `contains` proves ownership.

		A mismatch returns the invalid empty value instead of exposing another
		entity. The caller can test `Character.isValid` without nullable or dynamic
		storage.
	**/
	public function read(id:EntityId):Character
		return contains(id) ? current : emptyCharacter();

	/** Commit a newer snapshot only when both stored and replacement IDs agree. */
	public function replace(id:EntityId, character:Character):Bool {
		if (!contains(id) || character.id != id)
			return false;
		current = character;
		return true;
	}
}
