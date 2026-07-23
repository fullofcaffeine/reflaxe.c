package caxecraft.domain;

/**
	Stable runtime identity for one character inside a loaded game session.

	The validated level loader owns the positive storage code. It must derive that
	code from authored identity rather than an actor's current array position, so
	movement or removal cannot silently rename an entity. Zero is reserved as the
	invalid sentinel used by empty engine storage; it is never an authored actor.

	A nominal `abstract` is used instead of a raw `Int` so unrelated counts and
	indices cannot become IDs by accident. A class would add reference identity and
	allocation without adding meaning: the scalar code already is the identity.
	Haxe.c keeps the direct C integer carrier and inlines these checked operations.
**/
abstract EntityId(Int) {
	private inline function new(storageCode:Int)
		this = storageCode;

	/** Construct an ID only after a loader or explicit fixture validated it. */
	public static inline function fromValidatedStorageCode(storageCode:Int):EntityId
		return new EntityId(storageCode);

	/** Internal empty-slot marker; authored content must never use it. */
	public static inline function invalid():EntityId
		return new EntityId(0);

	/** True when this value can identify a live session entity. */
	public inline function isValid():Bool
		return this > 0;

	/** Stable scalar spelling used by saves, traces, and generated C. */
	public inline function storageCode():Int
		return this;
}
