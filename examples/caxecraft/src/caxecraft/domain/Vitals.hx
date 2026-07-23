package caxecraft.domain;

/**
	Shared fixed-step health rules for players, companions, and non-player actors.

	The module is stateless: callers hold a `VitalsState` value and receive a new
	value after each operation. This keeps update order explicit and lets the same
	rules run in the native game, deterministic probes, and future scripted actors.
**/
/**
	Six health points are presented as three hearts with half-heart steps.

	In Haxe, `inline` asks the compiler to put a declaration's value or function
	body directly where it is used, during compilation. It is commonly used for:

	- small named constants, so code says `MAX_HEALTH` instead of repeating `6`;
	- tiny forwarding functions, getters, or constructors whose call adds no
	  useful runtime boundary;
	- abstract operators and conversions, where a strong Haxe type should cost no
	  more at runtime than its underlying value; and
	- short, frequently used calculations after measurement shows that removing a
	  call is worthwhile.

	It is not a general "make this fast" switch. Repeating a large body at many
	call sites can increase the program's size, and some shapes cannot sensibly be
	inlined, such as recursive calls. The compiler still type-checks an inlined
	use normally, and the program must remain correct even when performance is not
	improved by inlining.

	Here, `final` means game code cannot assign a new maximum later. Combining it
	with `inline` makes this a named Haxe compile-time constant: each use receives
	the integer value `6` instead of reading a runtime field. This is a good fit
	because the maximum is one small, permanent rule shared by several functions;
	it is not per-character state and has no independent runtime lifetime.

	For a C build, this is still Haxe inlining—not C's `inline` keyword. Haxe has
	already resolved `MAX_HEALTH` before haxe.c lowers this typed program. In the
	current generated-C evidence, each use contains the literal `6`; there is no C
	global, preprocessor macro, storage, initialization, or runtime lookup for the
	declaration. A C `inline` function would be different: it would still be a C
	function declaration and the C compiler would decide whether to expand calls.
**/
inline final MAX_HEALTH:Int = 6;

/** Number of 50 ms ticks during which a recent hit prevents another hit. */
inline final ATTACK_SAFE_TICKS:Int = 20;

/**
	Create full health with no active protected period.

	This demonstrates the function form of Haxe `inline`. Callers keep the helpful
	domain name `start()`, while Haxe substitutes this one-expression body at each
	legal call site. Arguments to an inline function are still ordinary typed Haxe
	expressions: their values and side effects must not be duplicated merely
	because the surrounding function was expanded.

	In this program's generated C there is no `Vitals_start` function or call.
	haxe.c receives the expanded `make(6, 0)` expression and emits a direct call to
	the shared validation helper. Keeping `make` as a real function is deliberate:
	it owns non-trivial bounds logic in one place, whereas `start` is only a tiny,
	descriptive preset. That difference is a practical guide for using `inline`:
	prefer it for small semantic aliases and proven zero-cost typed wrappers, not
	as a blanket annotation on substantial functions whose repeated bodies would
	make generated code larger and harder to inspect.
**/
inline function start():VitalsState
	return make(MAX_HEALTH, 0);

/** Restore a validated health value, as used by saves and deterministic pilots. */
inline function startAt(health:Int):VitalsState
	return make(health, 0);

/** Advance the protected period by one exact simulation tick. */
function step(state:VitalsState):VitalsState {
	if (isDefeated(state))
		return state;
	if (state.safeTicks > 0)
		return make(state.health, state.safeTicks - 1);
	return state;
}

/** Apply one explicit impact when the current protected period permits it. */
function applyAttack(state:VitalsState, attacked:Bool):VitalsState {
	if (!attacked || isDefeated(state) || state.safeTicks > 0)
		return state;
	return make(state.health - 1, ATTACK_SAFE_TICKS);
}

/** True when this character can no longer act without being revived. */
inline function isDefeated(state:VitalsState):Bool
	return state.health <= 0;

/** True when recovery cannot increase this character's health. */
inline function isFull(state:VitalsState):Bool
	return state.health >= MAX_HEALTH;

/** Add a positive amount without exceeding the three-heart maximum. */
function recover(state:VitalsState, amount:Int):VitalsState {
	if (amount <= 0 || isDefeated(state) || isFull(state))
		return state;
	return make(state.health + amount, state.safeTicks);
}

/** Start again with full health after the owning game rule permits revival. */
inline function revive(_state:VitalsState):VitalsState
	return start();

private function make(health:Int, safeTicks:Int):VitalsState {
	var boundedHealth = health;
	if (boundedHealth < 0)
		boundedHealth = 0;
	var boundedSafeTicks = safeTicks;
	if (boundedSafeTicks < 0)
		boundedSafeTicks = 0;
	return {health: boundedHealth, safeTicks: boundedSafeTicks};
}
