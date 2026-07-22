package caxecraft.scenario;

/**
	Compile-time game-content facts used by scenario validation.

	The file format names content; it never loads code. A game supplies this
	closed registry from ordinary typed Haxe before a candidate can become live.
**/
interface ScenarioContentRegistry {
	function supportsFeature(id:ContentId):Bool;
	function isAirBlock(id:ContentId):Bool;
	function hasBlock(id:ContentId):Bool;
	function hasFluid(id:ContentId):Bool;
	function hasItem(id:ContentId):Bool;
	function hasEntity(id:ContentId):Bool;
	function hasNpc(id:ContentId):Bool;
	function hasPrefab(id:ContentId):Bool;
	function hasStatefulObject(id:ContentId):Bool;
	function hasState(id:ContentId):Bool;
	function hasEffect(id:ContentId):Bool;
	function hasSignal(id:ContentId):Bool;
	function maximumItemQuantity(id:ContentId):Int;
}
