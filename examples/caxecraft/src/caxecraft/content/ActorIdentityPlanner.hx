package caxecraft.content;

import caxecraft.domain.EntityId;
import caxecraft.domain.EntityStore;
import caxecraft.scenario.ScenarioId;

/**
	Plans compact runtime identities for already validated authored actors.

	CaxeMap keeps readable text identities such as `guide.nia`, while the live
	`EntityStore` uses a positive integer `EntityId`. `actorEntityId` derives that
	integer from the identifier itself, so reordering actors or inserting an
	unrelated actor cannot rename an existing save-game identity.

	Hashing has a small but real collision risk: two different text IDs can produce
	the same integer. `planActorIdentities` therefore checks the complete candidate
	set before a loader mutates `GameSession`. It either returns every binding in
	input order or one exact rejection; callers must never publish a partial plan.
**/
/** One checked correspondence between authored content and live session storage. */
typedef ActorIdentityBinding = {
	/** Stable CaxeMap identity retained for saves, diagnostics, and later lookup. */
	final authoredId:ScenarioId;

	/** Compact positive identity used by `EntityStore` during this loaded level. */
	final entityId:EntityId;
}

/** Exact fail-closed reason an authored actor set has no safe runtime ID plan. */
enum ActorIdentityPlanError {
	/** The candidate repeats one CaxeMap identity that should name one actor. */
	DuplicateAuthoredId(id:ScenarioId);

	/** Two distinct text identities produced the same compact integer. */
	StorageCodeCollision(first:ScenarioId, second:ScenarioId, storageCode:Int);

	/** The validated candidate cannot fit in the current bounded character store. */
	CharacterCapacityExceeded(count:Int, maximum:Int);
}

/** Complete plan or one rejection; there is no partially usable result. */
enum ActorIdentityPlanResult {
	/** Every input ID has one checked binding, in the original authored order. */
	Planned(bindings:Array<ActorIdentityBinding>);

	/** No binding is safe to publish; the payload explains the first rejection. */
	Rejected(error:ActorIdentityPlanError);
}

/**
	Derive one positive runtime ID from a validated CaxeMap identifier.

	CAXEMAP 1 identifiers contain only lowercase ASCII letters, digits, `.`, `_`,
	and `-`. FNV-1a processes those exact one-byte character codes in order using
	Haxe's wrapping 32-bit `Int` arithmetic. Masking off the sign bit produces the
	positive C carrier; the otherwise-invalid zero result is deterministically
	remapped to one.

	This function alone does not prove uniqueness. A level loader must use
	`planActorIdentities`, which checks collisions across the whole candidate
	before any live session state changes. Haxe.c emits the same wrapping integer
	operations and a direct `int32_t` `EntityId`; neither allocation nor a runtime
	string registry is added to the stored actor.
**/
function actorEntityId(id:ScenarioId):EntityId {
	var hash = -2128831035;
	final text = id.text();
	for (index in 0...text.length) {
		hash = hash ^ text.charCodeAt(index);
		hash = hash * 16777619;
	}
	final positive = hash & 0x7fffffff;
	return EntityId.fromValidatedStorageCode(positive == 0 ? 1 : positive);
}

/**
	Build one all-or-nothing identity plan in deterministic candidate order.

	The store admits at most 64 characters, so the straightforward pairwise check
	is bounded to 2,016 comparisons. That is easier to audit than a second mutable
	map and is not a measured loading bottleneck. Revisit the representation only
	if a later, larger actor population changes the established capacity.
**/
function planActorIdentities(authoredIds:Array<ScenarioId>):ActorIdentityPlanResult {
	if (authoredIds.length > EntityStore.MAX_CHARACTERS)
		return Rejected(CharacterCapacityExceeded(authoredIds.length, EntityStore.MAX_CHARACTERS));

	final bindings:Array<ActorIdentityBinding> = [];
	for (authoredId in authoredIds) {
		final entityId = actorEntityId(authoredId);
		for (binding in bindings) {
			if (binding.authoredId.text() == authoredId.text())
				return Rejected(DuplicateAuthoredId(authoredId));
			if (binding.entityId == entityId)
				return Rejected(StorageCodeCollision(binding.authoredId, authoredId, entityId.storageCode()));
		}
		bindings.push({authoredId: authoredId, entityId: entityId});
	}
	return Planned(bindings);
}
