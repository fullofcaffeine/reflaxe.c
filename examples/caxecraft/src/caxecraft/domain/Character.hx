package caxecraft.domain;

import caxecraft.domain.Aquatics.adoptProfile as adoptAquaticProfile;
import caxecraft.domain.Aquatics.observe as observeAquatics;
import caxecraft.domain.Aquatics.start as startAquatics;
import caxecraft.domain.Aquatics.step as stepAquatics;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.Vitals.applyAttack as applyVitalsAttack;
import caxecraft.domain.Vitals.isDefeated as vitalsAreDefeated;
import caxecraft.domain.Vitals.revive as reviveVitals;
import caxecraft.domain.Vitals.startAt as startVitalsAt;
import caxecraft.domain.Vitals.step as stepVitals;

/**
	One committed character made from reusable simulation components.

	This record deliberately contains no input device, camera, sprite, dialogue,
	name, or player/NPC tag. Those are controller, presentation, and authored
	content concerns. The same body, aquatic state, capability profile, and vitals
	therefore work for Haxirio, an NPC, a companion, an enemy, or a test fixture.

	This is an immutable `typedef` record rather than a class because it is a
	committed snapshot: systems calculate a replacement, and `EntityStore` owns the
	one authoritative identity and lifetime. A mutable character object here would
	let systems change shared state out of tick order. Haxe.c lowers the admitted
	record to a direct C struct value with no separate object allocation.
**/
typedef Character = {
	final id:EntityId;
	final body:CharacterBody;
	final aquatic:AquaticState;
	final aquaticProfile:AquaticProfile;
	final vitals:VitalsState;
}

/** Build a live character from validated identity, placement, and profile. */
function start(id:EntityId, body:CharacterBody, aquaticProfile:AquaticProfile, health:Int):Character {
	return {
		id: id,
		body: body,
		aquatic: startAquatics(aquaticProfile),
		aquaticProfile: aquaticProfile,
		vitals: startVitalsAt(health)
	};
}

/**
	Create the invalid value held by an unoccupied `EntityStore`.

	It is storage initialization, not a playable default. `EntityStore.put`
	rejects it, and validated content supplies every live character fact.
**/
function empty():Character {
	final emptyProfile = Aquatics.profile(1, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 1, false, false);
	return {
		id: EntityId.invalid(),
		body: createBody(0.0, 0.0, 0.0),
		aquatic: startAquatics(emptyProfile),
		aquaticProfile: emptyProfile,
		vitals: startVitalsAt(0)
	};
}

/** True when this value can be committed as a live character. */
inline function isValid(character:Character):Bool
	return character.id.isValid();

/**
	Advance shared body, water, breath, and health rules by one exact tick.

	The damage policy is explicit because Creative mode intentionally moves and
	swims without advancing survival damage. Role names never enter this system.
**/
function step(cells:WorldCells, original:Character, intent:CharacterIntent, damagePolicy:CharacterDamagePolicy):CharacterStep {
	if (vitalsAreDefeated(original.vitals)) {
		return {
			character: original,
			immersion: observeAquatics(cells, original.body),
			drowningDamage: 0
		};
	}

	final aquaticStep = stepAquatics(cells, original.body, original.aquatic, intent, original.aquaticProfile);
	var vitals = original.vitals;
	if (damagePolicy == CharacterDamagePolicy.Survival) {
		vitals = stepVitals(vitals);
		vitals = applyVitalsAttack(vitals, aquaticStep.drowningDamage > 0);
	}
	return {
		character: {
			id: original.id,
			body: aquaticStep.body,
			aquatic: aquaticStep.aquatic,
			aquaticProfile: original.aquaticProfile,
			vitals: vitals
		},
		immersion: aquaticStep.immersion,
		drowningDamage: aquaticStep.drowningDamage
	};
}

/** Apply one shared attack decision without changing other components. */
function applyAttack(original:Character, attacked:Bool):Character {
	return withVitals(original, applyVitalsAttack(original.vitals, attacked));
}

/** Replace the aquatic capability without granting back spent breath. */
function adoptProfile(original:Character, replacement:AquaticProfile):Character {
	return {
		id: original.id,
		body: original.body,
		aquatic: adoptAquaticProfile(original.aquatic, original.aquaticProfile, replacement),
		aquaticProfile: replacement,
		vitals: original.vitals
	};
}

/** Revive at a validated body placement while retaining the current profile. */
function reviveAt(original:Character, body:CharacterBody):Character {
	return {
		id: original.id,
		body: body,
		aquatic: startAquatics(original.aquaticProfile),
		aquaticProfile: original.aquaticProfile,
		vitals: reviveVitals(original.vitals)
	};
}

/** Replace only health/safe-time after another shared mechanic resolves it. */
function withVitals(original:Character, vitals:VitalsState):Character {
	return {
		id: original.id,
		body: original.body,
		aquatic: original.aquatic,
		aquaticProfile: original.aquaticProfile,
		vitals: vitals
	};
}
